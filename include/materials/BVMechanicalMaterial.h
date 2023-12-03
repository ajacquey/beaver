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

#pragma once

#include "Material.h"
#include "BVInelasticUpdateBase.h"

class BVMechanicalMaterial : public Material
{
public:
  static InputParameters validParams();
  BVMechanicalMaterial(const InputParameters & parameters);
  void initialSetup() override;
  void elasticModuliInputCheck();
  void displacementIntegrityCheck();
  void initializeInelasticModels();
  void initializeInitialStress();

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;
  virtual void computeQpStrainIncrement();
  virtual void computeQpSmallStrain(const ADRankTwoTensor & grad_tensor,
                                    const RankTwoTensor & grad_tensor_old);
  virtual void computeQpFiniteStrain(const ADRankTwoTensor & grad_tensor,
                                     const RankTwoTensor & grad_tensor_old);
  virtual void computeQpElasticityTensor();
  virtual void computeQpStress();
  virtual ADRankTwoTensor spinRotation(const ADRankTwoTensor & tensor);

  // Coupled variables
  const unsigned int _ndisp;
  std::vector<const ADVariableGradient *> _grad_disp;
  std::vector<const VariableGradient *> _grad_disp_old;

  // Strain parameters
  const unsigned int _strain_model;

  // Elastic parameters
  Real _bulk_modulus;
  Real _shear_modulus;

  // Strain properties
  ADMaterialProperty<RankTwoTensor> & _strain_increment;
  ADMaterialProperty<RankTwoTensor> & _spin_increment;

  // Elastic property
  ADMaterialProperty<Real> & _K;

  // Stress properties
  ADMaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _stress_old;

  // Elasticity tensor
  RankFourTensor _Cijkl;

  // Inelastic models
  unsigned int _num_inelastic;
  bool _has_inelastic;
  std::vector<BVInelasticUpdateBase *> _inelastic_models;

  // Initial stress
  unsigned int _num_ini_stress;
  std::vector<const Function *> _initial_stress;
};