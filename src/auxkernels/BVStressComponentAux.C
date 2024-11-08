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

#include "BVStressComponentAux.h"

registerMooseObject("BeaverApp", BVStressComponentAux);

InputParameters
BVStressComponentAux::validParams()
{
  InputParameters params = BVStressAuxBase::validParams();
  params.addClassDescription("Class for outputting components of the stress tensor.");
  MooseEnum component("x y z");
  params.addRequiredParam<MooseEnum>(
      "index_i", component, "The index i of ij for the stress tensor.");
  params.addRequiredParam<MooseEnum>(
      "index_j", component, "The index j of ij for the stress tensor.");
  return params;
}

BVStressComponentAux::BVStressComponentAux(const InputParameters & parameters)
  : BVStressAuxBase(parameters),
    _i(getParam<MooseEnum>("index_i")),
    _j(getParam<MooseEnum>("index_j"))
{
}

Real
BVStressComponentAux::computeValue()
{
  return MetaPhysicL::raw_value(_stress[_qp](_i, _j));
}