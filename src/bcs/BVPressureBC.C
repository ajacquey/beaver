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

#include "BVPressureBC.h"
#include "Function.h"

registerMooseObject("BeaverApp", BVPressureBC);

InputParameters
BVPressureBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params.addClassDescription("Applies a pressure on a given boundary in a given direction.");
  params.addRequiredParam<unsigned int>("component", "The component for the pressure.");
  params.addParam<Real>("value", 0.0, "Value of the pressure applied.");
  params.addParam<FunctionName>("function", "The function that describes the pressure.");
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}

BVPressureBC::BVPressureBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    _component(getParam<unsigned int>("component")),
    _value(getParam<Real>("value")),
    _function(isParamValid("function") ? &getFunction("function") : NULL)
{
  if (_component > 2)
    mooseError("Invalid component given for ", name(), ": ", _component, ".\n");
}

Real
BVPressureBC::computeQpResidual()
{
  Real value = _value;

  if (_function)
    value = _function->value(_t, _q_point[_qp]);

  return value * (_normals[_qp](_component) * _test[_i][_qp]);
}