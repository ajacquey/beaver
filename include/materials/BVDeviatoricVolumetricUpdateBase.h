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
  virtual std::vector<ADReal> returnMap() override;
  virtual std::vector<ADReal> vresidual(const std::vector<ADReal> & creep_strain_incr, 
                                        const std::vector<ADReal> & vcreep_strain_incr);
  virtual std::vector<std::vector<ADReal>> vjacobian(const std::vector<ADReal> & creep_strain_incr, 
                                                     const std::vector<ADReal> & vcreep_strain_incr);
  virtual ADReal vcreepRate(const std::vector<ADReal> & creep_strain_incr, 
                            const std::vector<ADReal> & vcreep_strain_incr) = 0;
  virtual ADReal vcreepRateDerivative(const std::vector<ADReal> & creep_strain_incr, 
                                      const std::vector<ADReal> & vcreep_strain_incr) = 0;
  virtual ADRankTwoTensor reformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr) override;

  bool _volumetric;
// volumetric creep strain increment
  ADMaterialProperty<RankTwoTensor> & _vcreep_strain_incr;
  
};