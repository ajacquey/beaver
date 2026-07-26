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

#pragma once

#include "BVInelasticUpdateBase.h"

class BVRTL2020ModelExplicitUpdate : public BVInelasticUpdateBase
{
public:
  static InputParameters validParams();
  BVRTL2020ModelExplicitUpdate(const InputParameters & parameters);
  virtual void inelasticUpdate(ADRankTwoTensor & stress,
                               const RankTwoTensor & stress_old,
                               const RankFourTensor & Cijkl) override;

protected:
  virtual void initQpStatefulProperties() override;
  virtual ADReal creepRateR();
  virtual ADReal creepRateLemaitre();
  virtual ADReal creepRateMunsonDawson();
  virtual ADReal creepRateVol();
  virtual void preUpdate();
  virtual void postUpdate(const std::vector<ADReal> & creep_strain_incr);
  virtual void preUpdateVol(const std::vector<ADReal> & creep_strain_incr);
  virtual void postUpdateVol(const std::vector<ADReal> & creep_strain_incr);
  virtual ADRankTwoTensor reformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr);

  // Name used as a prefix for all material properties related to this creep model
  const std::string _base_name;

  // Creep strain increment
  ADMaterialProperty<RankTwoTensor> & _creep_strain_incr;

  // Trial stress tensor and scalar effective stress
  ADRankTwoTensor _stress_tr;
  ADReal _eqv_stress_tr;
  ADReal _avg_stress_tr;

  // Old stress tensor invariants
  Real _eqv_stress_old;
  Real _avg_stress_old;
  // Shear and bulk modulus
  Real _G;
  Real _K;

  bool _volumetric;

  // Temperature coupling
  const ADVariableValue * const _temp;
  const Real _temp_ref;
  const Real _Ar;
  ADReal _exponential;

  // Lemaitre creep strain rate parameters
  const Real _alpha;
  const Real _A2;
  const Real _n2;

  // Munson-Dawson creep strain rate parameters
  const Real _A1;
  const Real _n1;
  const Real _A;
  const Real _B;
  const Real _m;
  const Real _n;

  // Volumetric creep strain rate parameters
  const Real _z;
  const Real _Nz;
  const Real _nz;
  const Real _Mz;
  const Real _mz;

  // Internal variable for Lemaitre and Munson-Dawson creep strain
  ADMaterialProperty<Real> & _eqv_creep_strain_L;
  const MaterialProperty<Real> & _eqv_creep_strain_L_old;
  ADMaterialProperty<Real> & _eqv_creep_strain_R;
  const MaterialProperty<Real> & _eqv_creep_strain_R_old;

  // Internal variable for volumetric creep strain
  ADMaterialProperty<Real> & _vol_creep_strain;
  const MaterialProperty<Real> & _vol_creep_strain_old;

  // Scalar equivalent creep strain
  ADReal _gamma_vp;
  ADReal _gamma_dot_vp;
};
