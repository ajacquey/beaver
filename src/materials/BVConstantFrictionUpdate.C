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

#include "BVConstantFrictionUpdate.h"

registerMooseObject("BeaverApp", BVConstantFrictionUpdate);

InputParameters
BVConstantFrictionUpdate::validParams()
{
  InputParameters params = BVFrictionUpdateBase::validParams();
  params.addClassDescription("Fault slip update with a constant Mohr-Coulomb friction.");
  params.addRequiredRangeCheckedParam<Real>(
      "friction", "friction > 0.0", "The Mohr-Coulomb friction coefficient.");
  return params;
}

BVConstantFrictionUpdate::BVConstantFrictionUpdate(const InputParameters & parameters)
  : BVFrictionUpdateBase(parameters), _f(getParam<Real>("friction"))
{
}

ADReal
BVConstantFrictionUpdate::frictionalStrength(const ADReal & /*delta_dot*/)
{
  return _f * _sigma_tr;
}

ADReal
BVConstantFrictionUpdate::frictionalStrengthDeriv(const ADReal & /*delta_dot*/)
{
  return 0.0;
}