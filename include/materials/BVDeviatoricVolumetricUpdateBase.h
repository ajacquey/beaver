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
//  virtual std::vector<ADReal> returnMap() override;
  virtual void nrStep(const std::vector<ADReal> & res,
                      const std::vector<std::vector<ADReal>> & jac,
                      std::vector<ADReal> & creep_strain_incr) override;
  virtual ADReal norm(const std::vector<ADReal> & vec) override;
  virtual std::vector<ADReal> residual(const std::vector<ADReal> & creep_strain_incr) override;
  virtual std::vector<std::vector<ADReal>> jacobian(const std::vector<ADReal> & creep_strain_incr) override;
  virtual ADReal creepRate(const std::vector<ADReal> & creep_strain_incr, const unsigned int i) override = 0;
  virtual ADReal creepRateDerivative(const std::vector<ADReal> & creep_strain_incr,
                                     const unsigned int i,
                                     const unsigned int j) override = 0;
  virtual void preReturnMap() override;
  virtual void postReturnMap(const std::vector<ADReal> & creep_strain_incr) override;

// newly added methods
  virtual void DVreturnMap(std::vector<ADReal>& creep_strain_incr_out, 
                           ADReal & Vcreep_strain_incr_out);
  virtual ADReal Vresidual(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr);
  virtual ADReal Vjacobian(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr);
  virtual ADRankTwoTensor DVreformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr);
  virtual ADReal VcreepRate(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr) = 0;
  virtual ADReal VcreepRateDerivative(const std::vector<ADReal> & creep_strain_incr,const ADReal & Vcreep_strain_incr) = 0;

  const unsigned int _num_cm;
  const RankTwoTensor _identity_two;
  ADReal _mean_stress_tr; // mean stress to compute the equivalent volumetric creep strain
  bool _dev_vol;
  
};