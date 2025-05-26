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

#include "BVMechanicalInterface.h"

registerADMooseObject("BeaverApp", BVMechanicalInterface);

InputParameters
BVMechanicalInterface::validParams()
{
  InputParameters params = ADInterfaceKernel::validParams();
  params.addClassDescription("Kernel for stress equilibrium at an inferface.");
  params.addCoupledVar("fluid_pressure", 0, "The fluid pressure variable.");
  params.set<bool>("use_displaced_mesh") = false;
  MooseEnum component("x y z");
  params.addRequiredParam<MooseEnum>("component",
                                     component,
                                     "The direction "
                                     "the variable this kernel acts in.");
  return params;
}

BVMechanicalInterface::BVMechanicalInterface(const InputParameters & parameters)
  : ADInterfaceKernel(parameters),
    _pf(adCoupledValue("fluid_pressure")),
    _component(getParam<MooseEnum>("component")),
    _traction_global(getADMaterialProperty<RealVectorValue>("traction_global"))
{
}

ADReal
BVMechanicalInterface::computeQpResidual(Moose::DGResidualType type)
{
  ADReal r = _traction_global[_qp](_component) - _pf[_qp] * _normals[_qp](_component);

  switch (type)
  {
    // [test_secondary-test_primary]*T where T represents the traction.
    case Moose::Element:
      r *= -_test[_i][_qp];
      break;
    case Moose::Neighbor:
      r *= _test_neighbor[_i][_qp];
      break;
  }

  return r;
}