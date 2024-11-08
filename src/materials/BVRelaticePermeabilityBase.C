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

#include "BVRelativePermeabilityBase.h"

InputParameters
BVRelativePermeabilityBase::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Base class for computing relative permeabilities and their derivatives of a porous material for two-phase flow.");
  params.addRequiredCoupledVar("saturation_w", "The wetting phase saturation variable.");
  return params;
}

BVRelativePermeabilityBase::BVRelativePermeabilityBase(const InputParameters & parameters)
  : Material(parameters),
    _sw(adCoupledValue("saturation_w")),
    _kr_w(declareADProperty<Real>("relative_permeability_w")),
    _kr_n(declareADProperty<Real>("relative_permeability_n"))
    // _dkr_w(declareADProperty<Real>("relative_permeability_w_derivative")),
    // _dkr_n(declareADProperty<Real>("relative_permeability_n_derivative"))
{
}