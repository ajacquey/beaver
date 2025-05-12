/******************************************************************************/
/*                            This file is part of                            */
/*                       BEAVER, a MOOSE-based application                    */
/*       Multiphase Flow Poromechanics for Induced Seismicity Problems        */
/*                                                                            */
/*                  Copyright (C) 2024 by Antoine B. Jacquey                  */
/*                           Polytechnique Montréal                           */
/*                                                                            */
/*            Licensed under GNU Lesser General Public License v2.1           */
/*                       please see LICENSE for details                       */
/*                 or http://www.gnu.org/licenses/lgpl.html                   */
/******************************************************************************/

#include "BVMechanicalInterfaceMaterial.h"
#include "RotationMatrix.h"

registerMooseObject("BeaverApp", BVMechanicalInterfaceMaterial);

InputParameters
BVMechanicalInterfaceMaterial::validParams()
{
  InputParameters params = InterfaceMaterial::validParams();
  params.addClassDescription("Material for computing the displacement jump and the traction across "
                             "a cohesive zone interface.");
  // Coupled variables
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system.");
  // Elastic stiffness parameters
  params.addRequiredRangeCheckedParam<Real>(
      "normal_stiffness", "normal_stiffness > 0.0", "The stiffness in the normal direction.");
  params.addRequiredRangeCheckedParam<Real>(
      "tangent_stiffness", "tangent_stiffness > 0.0", "The stifness in the tangent direction.");
  // Friction model
  params.addParam<MaterialName>(
      "friction_model",
      "The material object to use to calculate slip and opening of the interface.");
  // Strain and stress update need to be done on the undisplaced mesh
  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

BVMechanicalInterfaceMaterial::BVMechanicalInterfaceMaterial(const InputParameters & parameters)
  : InterfaceMaterial(parameters),
    // Coupled variables
    _ndisp(coupledComponents("displacements")),
    _disp(3),
    _disp_old(3),
    _disp_neighbor(3),
    _disp_old_neighbor(3),
    // Elastic stiffness parameters
    _K(std::vector<Real>{getParam<Real>("normal_stiffness"),
                         getParam<Real>("tangent_stiffness"),
                         getParam<Real>("tangent_stiffness")}),
    // Displacement jump properties
    _displacement_jump_incr_global(
        declareADProperty<RealVectorValue>("displacement_jump_increment_global")),
    _interface_displacement_jump_incr(
        declareADProperty<RealVectorValue>("interface_displacement_jump_increment")),
    _interface_displacement_jump(declareADProperty<RealVectorValue>("interface_displacement_jump")),
    _interface_displacement_jump_old(
        getMaterialPropertyOld<RealVectorValue>("interface_displacement_jump")),
    _interface_total_rotation(declareProperty<RankTwoTensor>("interface_total_rotation")),
    // Traction properties
    _interface_traction(declareADProperty<RealVectorValue>("interface_traction")),
    _interface_traction_old(getMaterialPropertyOld<RealVectorValue>("interface_traction")),
    _traction_global(declareADProperty<RealVectorValue>("traction_global"))
{
}

void
BVMechanicalInterfaceMaterial::initialSetup()
{
  displacementIntegrityCheck();

  initializeFrictionModel();

  // Check if we need displacement increment (slip increment or slip rate)
  // Fetch coupled variables
  for (unsigned int i = 0; i < _ndisp; ++i)
  {
    _disp[i] = &adCoupledValue("displacements", i);
    _disp_neighbor[i] = &adCoupledNeighborValue("displacements", i);
    if (_fe_problem.isTransient())
    {
      _disp_old[i] = &coupledValueOld("displacements", i);
      _disp_old_neighbor[i] = &coupledNeighborValueOld("displacements", i);
    }
  }
  // Set unused dimensions to zero
  for (unsigned int i = _ndisp; i < 3; i++)
  {
    _disp[i] = &_ad_zero;
    _disp_neighbor[i] = &_ad_zero;
    _disp_old[i] = &_zero;
    _disp_old_neighbor[i] = &_zero;
  }
}

void
BVMechanicalInterfaceMaterial::displacementIntegrityCheck()
{
  // Checking for consistency between mesh size and length of the provided displacements vector
  if (_ndisp != _mesh.dimension())
    paramError(
        "displacements",
        "The number of variables supplied in 'displacements' must match the mesh dimension.");
}

void
BVMechanicalInterfaceMaterial::initializeFrictionModel()
{
  _has_friction = isParamValid("friction_model");
  if (_has_friction)
  {
    const MaterialName model_name = getParam<MaterialName>("friction_model");
    _friction_model = dynamic_cast<BVFrictionUpdateBase *>(&this->getMaterialByName(model_name));
  }
}

void
BVMechanicalInterfaceMaterial::initQpStatefulProperties()
{
  // Requried to promote _interface_displacement_jump to stateful in case we need slip rate or slip
  // increment
  _interface_displacement_jump[_qp] = RealVectorValue();
  _interface_traction[_qp] = RealVectorValue();
}

void
BVMechanicalInterfaceMaterial::computeQpProperties()
{
  // Computes the global and local displacement jump
  computeQpDisplacementJump();
  computeQpRotationMatrices();
  computeQpLocalDisplacementJump();
  // Computes the local and global traction
  computeQpLocalTraction();
  computeQpGlobalTraction();
}

void
BVMechanicalInterfaceMaterial::computeQpDisplacementJump()
{
  // Computing the displacement jump
  for (unsigned int i = 0; i < _ndisp; i++)
    _displacement_jump_incr_global[_qp](i) =
        ((*_disp_neighbor[i])[_qp] - (*_disp_old_neighbor[i])[_qp]) -
        ((*_disp[i])[_qp] - (*_disp_old[i])[_qp]);
  for (unsigned int i = _ndisp; i < 3; i++)
    _displacement_jump_incr_global[_qp](i) = 0.0;
}

void
BVMechanicalInterfaceMaterial::computeQpRotationMatrices()
{
  _interface_total_rotation[_qp] = computeReferenceRotation(_normals[_qp], _mesh.dimension());
}

void
BVMechanicalInterfaceMaterial::computeQpLocalDisplacementJump()
{
  _interface_displacement_jump_incr[_qp] =
      _interface_total_rotation[_qp].transpose() * _displacement_jump_incr_global[_qp];

  // Incremental displacement
  _interface_displacement_jump[_qp] =
      _interface_displacement_jump_old[_qp] + _interface_displacement_jump_incr[_qp];
}

void
BVMechanicalInterfaceMaterial::computeQpLocalTraction()
{
  ADRealVectorValue interface_traction_incr = _K * _interface_displacement_jump_incr[_qp];
  _interface_traction[_qp] = _interface_traction_old[_qp] + interface_traction_incr;

  // Correct here for slip
  if (_has_friction)
  {
    _friction_model->setQp(_qp);
    _friction_model->frictionUpdate(_interface_traction[_qp], _K);
  }
}

void
BVMechanicalInterfaceMaterial::computeQpGlobalTraction()
{
  _traction_global[_qp] = _interface_total_rotation[_qp] * _interface_traction[_qp];
}

RankTwoTensor
BVMechanicalInterfaceMaterial::computeReferenceRotation(const RealVectorValue & normal,
                                                        const unsigned int mesh_dimension)
{
  RankTwoTensor rot;
  switch (mesh_dimension)
  {
    case 3:
      rot = RotationMatrix::rotVec1ToVec2(RealVectorValue(1, 0, 0), normal);
      break;
    case 2:
      rot = RotationMatrix::rotVec2DToX(normal).transpose();
      break;
    case 1:
      rot = RankTwoTensor::Identity();
      break;
    default:
      mooseError("computeReferenceRotation: mesh_dimension value should be 1, 2 or, 3. You "
                 "provided " +
                 std::to_string(mesh_dimension));
  }
  return rot;
}