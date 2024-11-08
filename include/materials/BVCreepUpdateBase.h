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

class BVCreepUpdateBase : public BVInelasticUpdateBase
{
public:
  static InputParameters validParams();
  BVCreepUpdateBase(const InputParameters & parameters);
  virtual void inelasticUpdate(ADRankTwoTensor & stress, const RankFourTensor & Cijkl) override;

protected:
  virtual ADReal returnMap();
  virtual ADReal residual(const ADReal & creep_strain_incr);
  virtual ADReal jacobian(const ADReal & creep_strain_incr);
  virtual ADRankTwoTensor reformPlasticStrainTensor(const ADReal & creep_strain_incr);
  virtual ADReal creepRate(const ADReal & creep_strain_incr) = 0;
  virtual ADReal creepRateDerivative(const ADReal & creep_strain_incr) = 0;
  virtual void preReturnMap();
  virtual void postReturnMap(const ADReal & creep_strain_incr);

  // Name used as a prefix for all material properties related to this creep model
  const std::string _base_name;

  // Creep strain increment
  ADMaterialProperty<RankTwoTensor> & _creep_strain_incr;

  // Trial stress tensor and scalar effective stress
  ADRankTwoTensor _stress_tr;
  ADReal _eqv_stress_tr;

  // Shear modulus
  Real _G;
};