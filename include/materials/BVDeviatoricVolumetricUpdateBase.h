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

#include "BVTwoCreepUpdateBase.h"

class BVDeviatoricVolumetricUpdateBase : public BVTwoCreepUpdateBase
{
public:
  static InputParameters validParams();
  BVDeviatoricVolumetricUpdateBase(const InputParameters & parameters);
  virtual void inelasticUpdate(ADRankTwoTensor & stress, const RankFourTensor & Cijkl) override;

protected:
// newly added methods
  virtual void dvreturnMap(std::vector<ADReal>& creep_strain_incr_out, 
                           ADReal & Vcreep_strain_incr_out);
  virtual ADReal vresidual(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr);
  virtual ADReal vjacobian(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr);
  virtual ADRankTwoTensor dvreformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr);
  virtual ADReal vcreepRate(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr) = 0;
  virtual ADReal vcreepRateDerivative(const std::vector<ADReal> & creep_strain_incr,const ADReal & Vcreep_strain_incr) = 0;

  const unsigned int _num_cm;
  const RankTwoTensor _identity_two;
  ADReal _mean_stress_tr; // mean stress to compute the equivalent volumetric creep strain
  bool _dev_vol;
  
};