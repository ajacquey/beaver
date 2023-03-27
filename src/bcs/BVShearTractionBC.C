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

#include "BVShearTractionBC.h"
#include "Function.h"

registerMooseObject("BeaverApp", BVShearTractionBC);

InputParameters
BVShearTractionBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params.addClassDescription("Applies a shear traction vector on a given boundary.");
  params.addRequiredParam<unsigned int>("component", "The component for the shear traction.");
  params.addParam<Real>("value", 0.0, "Value of the shear traction applied.");
  params.addParam<FunctionName>("function", "The function that describes the shear traction.");
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}

BVShearTractionBC::BVShearTractionBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    _component(getParam<unsigned int>("component")),
    _value(getParam<Real>("value")),
    _function(isParamValid("function") ? &getFunction("function") : NULL)
{
  if (_component > 2)
    paramError("component", "Invalid component given.\n");
}

Real
BVShearTractionBC::computeQpResidual()
{
  // Compute tangent (only works in 2D)
  RealVectorValue tangent = RealVectorValue(-_normals[_qp](1), _normals[_qp](0), 0.0);

  Real value = _value;

  if (_function)
    value = _function->value(_t, _q_point[_qp]);

  return value * (tangent(_component) * _test[_i][_qp]);
}