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

#include "BVPressureAux.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("BeaverApp", BVPressureAux);

InputParameters
BVPressureAux::validParams()
{
  InputParameters params = BVStressAuxBase::validParams();
  params.addClassDescription("Class for outputting the pressure or mean stress.");
  return params;
}

BVPressureAux::BVPressureAux(const InputParameters & parameters)
  : BVStressAuxBase(parameters)
{
}

Real
BVPressureAux::computeValue()
{
  return -MetaPhysicL::raw_value(_stress[_qp].trace()) / 3.0;
}