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

#include "BVPermeabilityBase.h"

InputParameters
BVPermeabilityBase::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Base class for computing the permeability of a porous material for "
                             "single and multi phase flow.");
  return params;
}

BVPermeabilityBase::BVPermeabilityBase(const InputParameters & parameters)
  : Material(parameters), _permeability(declareADProperty<Real>("permeability"))
{
}