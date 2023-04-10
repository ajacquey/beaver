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

#include "BVInelasticUpdateBase.h"

class BVCreepUpdateBase : public BVInelasticUpdateBase
{
public:
  static InputParameters validParams();
  BVCreepUpdateBase(const InputParameters & parameters);
  virtual void inelasticUpdate(ADRankTwoTensor & stress, const RankFourTensor & Cijkl) override;

protected:
  virtual ADReal returnMap();
  virtual ADReal residual(const ADReal & eqv_stress);
  virtual ADReal jacobian(const ADReal & eqv_stress);
  virtual ADRankTwoTensor reformPlasticStrainTensor(const ADReal & eqv_stress);
  virtual ADReal creepRate(const ADReal & eqv_stress) = 0;
  virtual ADReal creepRateDerivative(const ADReal & eqv_stress) = 0;
  virtual void preReturnMap();
  virtual void postReturnMap(const ADReal & eqv_stress);

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