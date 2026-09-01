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

#include "BVFaultNormalStressAux.h"

registerMooseObject("BeaverApp", BVFaultNormalStressAux);

InputParameters
BVFaultNormalStressAux::validParams()
{
  InputParameters params = BVFaultStressAuxBase::validParams();
  params.addClassDescription("Calculates the effective normal stress.");
  return params;
}

BVFaultNormalStressAux::BVFaultNormalStressAux(const InputParameters & parameters)
  : BVFaultStressAuxBase(parameters)
{
}

Real
BVFaultNormalStressAux::computeValue()
{
  return -MetaPhysicL::raw_value(_traction[_qp] * _normals[_qp]);
}