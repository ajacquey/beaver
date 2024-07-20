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

#include "BVCapillaryPressurePowerLaw.h"

registerMooseObject("BeaverApp", BVCapillaryPressurePowerLaw);

InputParameters
BVCapillaryPressurePowerLaw::validParams()
{
  InputParameters params = BVCapillaryPressureBase::validParams();
  params.addClassDescription("Class for computing the capillary pressure and its derivatives for two-phase flow as a power law function.");
  params.addRequiredRangeCheckedParam<Real>("exponent", "exponent>=1.0", "The exponent for the power law functions.");
  params.addRequiredRangeCheckedParam<Real>("reference_capillary_pressure", "reference_capillary_pressure>=0.0", "The reference capillary pressure.");
  return params;
}

BVCapillaryPressurePowerLaw::BVCapillaryPressurePowerLaw(const InputParameters & parameters)
  : BVCapillaryPressureBase(parameters),
    _n(getParam<Real>("exponent")),
    _pce(getParam<Real>("reference_capillary_pressure"))
{
}

void
BVCapillaryPressurePowerLaw::computeQpProperties()
{
  // Capillary pressure
  _pc[_qp] = _pce * std::pow(_sw[_qp], -_n);
  _dpc[_qp] = -_n * _pce * std::pow(_sw[_qp], -(_n + 1.0));
}