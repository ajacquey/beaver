/******************************************************************************/
/*                            This file is part of                            */
/*                       BEAVER, a MOOSE-based application                    */
/*       Multiphase Flow Poromechanics for Induced Seismicity Problems        */
/*                                                                            */
/*                  Copyright (C) 2024 by Antoine B. Jacquey                  */
/*                  Tufts University / Polytechnique Montreal                 */
/*                                                                            */
/*            Licensed under GNU Lesser General Public License v2.1           */
/*                       please see LICENSE for details                       */
/*                 or http://www.gnu.org/licenses/lgpl.html                   */
/******************************************************************************/

#pragma once

#include "InterfaceMaterial.h"
#include "BVFrictionUpdateBase.h"

class BVMechanicalInterfaceMaterial : public InterfaceMaterial
{
public:
  static InputParameters validParams();
  BVMechanicalInterfaceMaterial(const InputParameters & parameters);
  void initialSetup() override;
  void displacementIntegrityCheck();
  void initializeFrictionModel();

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;
  virtual void computeQpDisplacementJump();
  virtual void computeQpRotationMatrices();
  virtual void computeQpLocalDisplacementJump();
  virtual void computeQpLocalTraction();
  virtual void computeQpGlobalTraction();
  virtual RankTwoTensor computeReferenceRotation(const RealVectorValue & normal,
                                                 const unsigned int mesh_dimension);

  // Coupled variables
  const unsigned int _ndisp;
  std::vector<const ADVariableValue *> _disp;
  std::vector<const VariableValue *> _disp_old;
  std::vector<const ADVariableValue *> _disp_neighbor;
  std::vector<const VariableValue *> _disp_old_neighbor;

  // Elastic stiffness parameters
  const RankTwoTensor _K;

  // Displacement jump properties
  ADMaterialProperty<RealVectorValue> & _displacement_jump_incr_global;
  ADMaterialProperty<RealVectorValue> & _interface_displacement_jump_incr;
  ADMaterialProperty<RealVectorValue> & _interface_displacement_jump;
  const MaterialProperty<RealVectorValue> & _interface_displacement_jump_old;
  MaterialProperty<RankTwoTensor> & _interface_total_rotation;

  // Traction properties
  ADMaterialProperty<RealVectorValue> & _interface_traction;
  const MaterialProperty<RealVectorValue> & _interface_traction_old;
  ADMaterialProperty<RealVectorValue> & _traction_global;

  // Friction model
  bool _has_friction;
  BVFrictionUpdateBase * _friction_model;
};
