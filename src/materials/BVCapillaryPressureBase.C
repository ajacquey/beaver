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

#include "BVCapillaryPressureBase.h"

InputParameters
BVCapillaryPressureBase::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Base class for computing the capillary pressure and its derivatives for two-phase flow.");
  params.addRequiredCoupledVar("saturation_w", "The wetting phase saturation variable.");
  return params;
}

BVCapillaryPressureBase::BVCapillaryPressureBase(const InputParameters & parameters)
  : Material(parameters),
    _sw(adCoupledValue("saturation_w")),
    _pc(declareADProperty<Real>("capillary_pressure")),
    _dpc(declareADProperty<Real>("capillary_pressure_derivative"))
{
}