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

#include "BVFrictionUpdateBase.h"

class BVConstantFrictionUpdate : public BVFrictionUpdateBase
{
public:
  static InputParameters validParams();
  BVConstantFrictionUpdate(const InputParameters & parameters);

protected:
  virtual ADReal frictionalStrength(const ADReal & delta_dot) override;
  virtual ADReal frictionalStrengthDeriv(const ADReal & delta_dot) override;

  // Friction parameters
  const Real _f;
};