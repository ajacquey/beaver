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

#include "BVRelativePermeabilityPowerLaw.h"

registerMooseObject("BeaverApp", BVRelativePermeabilityPowerLaw);

InputParameters
BVRelativePermeabilityPowerLaw::validParams()
{
  InputParameters params = BVRelativePermeabilityBase::validParams();
  params.addClassDescription("Class for computing relative permeabilities and their derivatives of a porous material for two-phase flow as power law functions.");
  params.addRequiredRangeCheckedParam<Real>("exponent", "exponent>=1.0", "The exponent for the power law functions.");
  return params;
}

BVRelativePermeabilityPowerLaw::BVRelativePermeabilityPowerLaw(const InputParameters & parameters)
  : BVRelativePermeabilityBase(parameters),
    _n(getParam<Real>("exponent"))
{
}

void
BVRelativePermeabilityPowerLaw::computeQpProperties()
{
  // Wetting phase
  _kr_w[_qp] = std::pow(_sw[_qp], _n);
  // _dkr_w[_qp] = _n * std::pow(_sw[_qp], _n - 1.0);
  // Non-wetting phase
  _kr_n[_qp] = std::pow(1.0 - _sw[_qp], _n);
  // _dkr_n[_qp] = -_n * std::pow(1.0 - _sw[_qp], _n - 1.0);
}