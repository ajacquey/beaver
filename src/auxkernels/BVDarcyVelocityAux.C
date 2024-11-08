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

#include "BVDarcyVelocityAux.h"
#include "Assembly.h"

registerMooseObject("BeaverApp", BVDarcyVelocityAux);

InputParameters
BVDarcyVelocityAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Class for outputting components of the Darcy's velocity.");
  MooseEnum component("x y z normal");
  params.addRequiredParam<MooseEnum>(
      "component", component, "The desired component of the Darcy velocity.");
  params.addRequiredCoupledVar("fluid_pressure", "The fluid pressure variable.");
  return params;
}

BVDarcyVelocityAux::BVDarcyVelocityAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _use_normal(getParam<MooseEnum>("component") == "normal"),
    _component(getParam<MooseEnum>("component")),
    _normals(_assembly.normals()),
    _grad_pf(adCoupledGradient("fluid_pressure")),
    _lambda(getADMaterialProperty<Real>("fluid_mobility"))
{
  if (_use_normal && !isParamValid("boundary"))
    paramError("boundary", "A boundary must be provided if using the normal component!");
}

Real
BVDarcyVelocityAux::computeValue()
{
  const Real grad_pf = _use_normal ? MetaPhysicL::raw_value(_grad_pf[_qp] * _normals[_qp])
                                   : MetaPhysicL::raw_value(_grad_pf[_qp](_component));

  return -MetaPhysicL::raw_value(_lambda[_qp]) * grad_pf;
}