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

protected:
  virtual std::vector<ADReal> returnMap() override;
  virtual void preReturnMapVol(const std::vector<ADReal> & creep_strain_incr);
  virtual ADReal residualVol(const ADReal & vol_strain_incr);
  virtual ADReal jacobianVol(const ADReal & vol_strain_incr);
  virtual ADReal creepRateVol(const ADReal & vol_strain_incr);
  virtual ADReal creepRateVolDerivative(const ADReal & vol_strain_incr);
  virtual ADRankTwoTensor reformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr) override;
  
  bool _volumetric;
};