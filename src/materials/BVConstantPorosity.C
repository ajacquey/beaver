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

#include "BVConstantPorosity.h"

registerMooseObject("BeaverApp", BVConstantPorosity);

InputParameters
BVConstantPorosity::validParams()
{
  InputParameters params = BVPorosityBase::validParams();
  params.addClassDescription("Computes a constant porosity value for the porous medium.");
  params.addRequiredRangeCheckedParam<Real>(
      "porosity", "porosity >= 0 & porosity <= 1", "The porosity of the porous medium.");
  return params;
}

BVConstantPorosity::BVConstantPorosity(const InputParameters & parameters)
  : BVPorosityBase(parameters), _porosity0(getParam<Real>("porosity"))
{
}

void
BVConstantPorosity::initQpStatefulProperties()
{
  _porosity[_qp] = _porosity0;
}

void
BVConstantPorosity::computeQpProperties()
{
  _porosity[_qp] = _porosity0;
}