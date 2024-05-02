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

#include "BVDispersionBase.h"

InputParameters
BVDispersionBase::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Base class for computing the dispersion of a porous material for "
                             "single phase flow with solute transport.");
  return params;
}

BVDispersionBase::BVDispersionBase(const InputParameters & parameters)
  : Material(parameters), _dispersion(declareADProperty<Real>("dispersion"))
{
}