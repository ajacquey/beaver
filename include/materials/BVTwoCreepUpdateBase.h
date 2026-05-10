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

class BVTwoCreepUpdateBase : public BVInelasticUpdateBase
{
public:
  static InputParameters validParams();
  BVTwoCreepUpdateBase(const InputParameters & parameters);
  virtual void inelasticUpdate(ADRankTwoTensor & stress, const RankFourTensor & Cijkl) override;

protected:
  virtual std::vector<ADReal> returnMap();
  virtual void nrStep(const std::vector<ADReal> & res,
                      const std::vector<std::vector<ADReal>> & jac,
                      std::vector<ADReal> & creep_strain_incr);
  virtual ADReal norm(const std::vector<ADReal> & vec);
  virtual std::vector<ADReal> residual(const std::vector<ADReal> & creep_strain_incr);
  virtual std::vector<std::vector<ADReal>> jacobian(const std::vector<ADReal> & creep_strain_incr);
  virtual ADRankTwoTensor reformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr);
  virtual ADReal creepRate(const std::vector<ADReal> & creep_strain_incr, const unsigned int i) = 0;
  virtual ADReal creepRateDerivative(const std::vector<ADReal> & creep_strain_incr,
                                     const unsigned int i,
                                     const unsigned int j) = 0;
  virtual void preReturnMap();
  virtual void postReturnMap(const std::vector<ADReal> & creep_strain_incr);

  // Name used as a prefix for all material properties related to this creep model
  const std::string _base_name;

  // Creep strain increment
  ADMaterialProperty<RankTwoTensor> & _creep_strain_incr;

  // Trial stress tensor and scalar effective stress
  ADRankTwoTensor _stress_tr;
  ADReal _eqv_stress_tr;
  ADReal _avg_stress_tr;

  // Shear and bulk modulus
  Real _G;
  Real _K;
};