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

#include "BVConstantPermeability.h"

registerMooseObject("BeaverApp", BVConstantPermeability);

InputParameters
BVConstantPermeability::validParams()
{
  InputParameters params = BVPermeabilityBase::validParams();
  params.addClassDescription("Computes a constant permeability value for the porous medium.");
  params.addRequiredRangeCheckedParam<Real>(
      "permeability", "permeability>0", "The permeability of the porous medium.");
  return params;
}

BVConstantPermeability::BVConstantPermeability(const InputParameters & parameters)
  : BVPermeabilityBase(parameters), _permeability0(getParam<Real>("permeability"))
{
}

void
BVConstantPermeability::computeQpProperties()
{
  _permeability[_qp] = _permeability0;
}