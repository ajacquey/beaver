/******************************************************************************/
/*                            This file is part of                            */
/*                       BEAVER, a MOOSE-based application                    */
/*       Multiphase Flow Poromechanics for Induced Seismicity Problems        */
/*                                                                            */
/*                  Copyright (C) 2022 by Antoine B. Jacquey                  */
/*                  Tufts University / Polytechnique Montreal                 */
/*                                                                            */
/*            Licensed under GNU Lesser General Public License v2.1           */
/*                       please see LICENSE for details                       */
/*                 or http://www.gnu.org/licenses/lgpl.html                   */
/******************************************************************************/

#include "BVMechanicalMaterial.h"
#include "Function.h"

registerMooseObject("BeaverApp", BVMechanicalMaterial);

InputParameters
BVMechanicalMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Material for computing the deformation of a porous material.");
  // Coupled variables
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system.");
  // Strain parameters
  MooseEnum strain_model("small=0 finite=1", "small");
  params.addParam<MooseEnum>(
      "strain_model", strain_model, "The model to use to calculate the strain rate tensor.");
  // Elastic moduli parameters
  params.addRangeCheckedParam<Real>(
      "bulk_modulus", "bulk_modulus > 0.0", "The bulk modulus of the material.");
  params.addRangeCheckedParam<Real>(
      "shear_modulus", "shear_modulus > 0.0", "The shear modulus of the material.");
  params.addRangeCheckedParam<Real>(
      "young_modulus", "young_modulus > 0.0", "The Young's modulus of the material.");
  params.addRangeCheckedParam<Real>(
      "poisson_ratio", "poisson_ratio > 0.0", "The Poisson's ratio of the material.");
  // Initial stress
  params.addParam<std::vector<FunctionName>>(
      "initial_stress", "The initial stress principal components (negative in compression).");
  // Inelastic models
  params.addParam<std::vector<MaterialName>>(
      "inelastic_models",
      "The material objects to use to calculate stress and inelastic strains. "
      "Note: specify creep models first and plasticity models second.");
  // Strain and stress update need to be done on the undisplaced mesh
  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

BVMechanicalMaterial::BVMechanicalMaterial(const InputParameters & parameters)
  : Material(parameters),
    // Coupled variables
    _ndisp(coupledComponents("displacements")),
    _grad_disp(3),
    _grad_disp_old(3),
    // Strain parameters
    _strain_model(getParam<MooseEnum>("strain_model")),
    // Initial stress
    _initial_stress_fct(getParam<std::vector<FunctionName>>("initial_stress")),
    _num_ini_stress(_initial_stress_fct.size()),
    // Inelastic models
    _has_inelastic(isParamValid("inelastic_models")),
    _num_inelastic(getParam<std::vector<MaterialName>>("inelastic_models").size()),
    // Strain properties
    _strain_increment(declareADProperty<RankTwoTensor>("strain_increment")),
    _spin_increment(declareADProperty<RankTwoTensor>("spin_increment")),
    // Elastic property
    _K(declareADProperty<Real>("bulk_modulus")),
    // Stress properties
    _stress(declareADProperty<RankTwoTensor>("stress")),
    _stress_old(getMaterialPropertyOld<RankTwoTensor>("stress"))
{
  if (getParam<bool>("use_displaced_mesh"))
    paramError("use_displaced_mesh",
               "The strain and stress calculator needs to run on the undisplaced mesh.");

  if (_num_ini_stress != 3 && _num_ini_stress != 0 && _num_ini_stress != 6)
    paramError("initial_stress", "You need to provide 3 or 6 components for the initial stress.");

  _initial_stress.resize(_num_ini_stress);

  for (unsigned int i = 0; i < _num_ini_stress; i++)
    _initial_stress[i] = &getFunctionByName(_initial_stress_fct[i]);
}

void
BVMechanicalMaterial::initialSetup()
{
  elasticModuliInputCheck();

  displacementIntegrityCheck();

  initializeInelasticModels();

  // Fetch coupled variables and gradients
  for (unsigned int i = 0; i < _ndisp; ++i)
  {
    _grad_disp[i] = &adCoupledGradient("displacements", i);
    if (_fe_problem.isTransient())
      _grad_disp_old[i] = &coupledGradientOld("displacements", i);
    else
      _grad_disp_old[i] = &_grad_zero;
  }

  // Set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
  {
    _grad_disp[i] = &_ad_grad_zero;
    _grad_disp_old[i] = &_grad_zero;
  }
}

void
BVMechanicalMaterial::elasticModuliInputCheck()
{
  if (isParamValid("bulk_modulus") && isParamValid("shear_modulus"))
  {
    _bulk_modulus = getParam<Real>("bulk_modulus");
    _shear_modulus = getParam<Real>("shear_modulus");
  }
  else if (isParamValid("young_modulus") && isParamValid("poisson_ratio"))
  {
    Real E = getParam<Real>("young_modulus");
    Real nu = getParam<Real>("poisson_ratio");

    _bulk_modulus = E / (3.0 * (1.0 - 2.0 * nu));
    _shear_modulus = E / (2.0 * (1.0 + nu));
  }
  else if (isParamValid("bulk_modulus") && isParamValid("poisson_ratio"))
  {
    Real nu = getParam<Real>("poisson_ratio");

    _bulk_modulus = getParam<Real>("bulk_modulus");
    _shear_modulus = 3.0 * _bulk_modulus * (1.0 - 2.0 * nu) / (2.0 * (1.0 + nu));
  }
  else
    paramError("bulk_modulus",
               "Please provide 'bulk_modulus' and 'shear_modulus' OR 'young_modulus' and "
               "'poisson_ratio' OR 'bulk_modulus' and 'poisson_ratio' as elastic parameters!");
}

void
BVMechanicalMaterial::displacementIntegrityCheck()
{
  // Checking for consistency between mesh size and length of the provided displacements vector
  if (_ndisp != _mesh.dimension())
    paramError(
        "displacements",
        "The number of variables supplied in 'displacements' must match the mesh dimension.");
}

void
BVMechanicalMaterial::initializeInelasticModels()
{
  if (_has_inelastic)
  {
    std::vector<MaterialName> models = getParam<std::vector<MaterialName>>("inelastic_models");

    for (unsigned int i = 0; i < _num_inelastic; ++i)
    {
      BVInelasticUpdateBase * rrr =
          dynamic_cast<BVInelasticUpdateBase *>(&this->getMaterialByName(models[i]));

      if (rrr)
        _inelastic_models.push_back(rrr);
      else
        mooseError("Model " + models[i] + " is not compatible with BVMechanicalMaterial!");
    }
  }
}

void
BVMechanicalMaterial::initQpStatefulProperties()
{
  _stress[_qp].zero();
  RankTwoTensor init_stress_tensor = RankTwoTensor();
  if (_num_ini_stress > 0)
  {
    std::vector<Real> init_stress(_num_ini_stress, 0.0);
    for (unsigned int i = 0; i < _num_ini_stress; i++)
      init_stress[i] = (*_initial_stress[i]).value(_t, _q_point[_qp]);
    init_stress_tensor.fillFromInputVector(init_stress);
  }
  _stress[_qp] += init_stress_tensor;
}

void
BVMechanicalMaterial::computeQpProperties()
{
  computeQpStrainIncrement();
  computeQpElasticityTensor();
  computeQpStress();
}

void
BVMechanicalMaterial::computeQpStrainIncrement()
{
  ADRankTwoTensor grad_tensor = ADRankTwoTensor::initializeFromRows(
      (*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  RankTwoTensor grad_tensor_old = RankTwoTensor::initializeFromRows(
      (*_grad_disp_old[0])[_qp], (*_grad_disp_old[1])[_qp], (*_grad_disp_old[2])[_qp]);

  switch (_strain_model)
  {
    case 0: // SMALL STRAIN
      computeQpSmallStrain(grad_tensor, grad_tensor_old);
      break;
    case 1: // FINITE STRAIN
      computeQpFiniteStrain(grad_tensor, grad_tensor_old);
      break;
    default:
      paramError("strain_model", "Unknown strain model. Specify 'small' or 'finite'!");
  }
}

void
BVMechanicalMaterial::computeQpSmallStrain(const ADRankTwoTensor & grad_tensor,
                                           const RankTwoTensor & grad_tensor_old)
{
  ADRankTwoTensor A = grad_tensor - grad_tensor_old;

  _strain_increment[_qp] = 0.5 * (A + A.transpose());
  _spin_increment[_qp] = 0.5 * (A - A.transpose());
}

void
BVMechanicalMaterial::computeQpFiniteStrain(const ADRankTwoTensor & grad_tensor,
                                            const RankTwoTensor & grad_tensor_old)
{
  ADRankTwoTensor F = grad_tensor;
  RankTwoTensor F_old = grad_tensor_old;
  F.addIa(1.0);
  F_old.addIa(1.0);

  // Increment gradient
  ADRankTwoTensor L = -F_old * F.inverse();
  L.addIa(1.0);

  _strain_increment[_qp] = 0.5 * (L + L.transpose());
  _spin_increment[_qp] = 0.5 * (L - L.transpose());
}

void
BVMechanicalMaterial::computeQpElasticityTensor()
{
  // Elasticity tensor
  _Cijkl.fillGeneralIsotropic(_bulk_modulus - 2.0 / 3.0 * _shear_modulus, _shear_modulus, 0.0);

  // Bulk modulus
  _K[_qp] = _bulk_modulus;
}

void
BVMechanicalMaterial::computeQpStress()
{
  _stress[_qp] = spinRotation(_stress_old[_qp]) + _Cijkl * _strain_increment[_qp];
}

ADRankTwoTensor
BVMechanicalMaterial::spinRotation(const ADRankTwoTensor & tensor)
{
  return tensor + _spin_increment[_qp] * tensor.deviatoric() -
         tensor.deviatoric() * _spin_increment[_qp];
}