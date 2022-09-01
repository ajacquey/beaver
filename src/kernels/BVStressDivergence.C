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

#include "BVStressDivergence.h"

registerMooseObject("BeaverApp", BVStressDivergence);

InputParameters
BVStressDivergence::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription("Kernel for stress divergence in the momentum balance.");
  params.addCoupledVar("fluid_pressure", 0, "The fluid pressure variable.");
  params.set<bool>("use_displaced_mesh") = false;
  MooseEnum component("x y z");
  params.addRequiredParam<MooseEnum>("component",
                                     component,
                                     "The direction "
                                     "the variable this kernel acts in.");
  //   params.addRangeCheckedParam<Real>(
  //       "density", 0.0, "density >= 0.0", "The density of the material.");
  //   params.addParam<RealVectorValue>("gravity", RealVectorValue(), "The gravity vector.");
  return params;
}

BVStressDivergence::BVStressDivergence(const InputParameters & parameters)
  : ADKernel(parameters),
    _pf(coupledValue("fluid_pressure")),
    _component(getParam<MooseEnum>("component")),
    // _rho(getParam<Real>("density")),
    // _gravity(getParam<RealVectorValue>("gravity")),
    _stress(getADMaterialProperty<RankTwoTensor>("stress"))
// _biot(getMaterialProperty<Real>("biot_coefficient"))
{
}

ADReal
BVStressDivergence::computeQpResidual()
{
  //   RealVectorValue grav_term = -_rho * _gravity;

  ADRealVectorValue stress_row = _stress[_qp].row(_component);
  //   stress_row(_component) -= _biot[_qp] * _pf[_qp];

  return stress_row * _grad_test[_i][_qp]; // + grav_term(_component) * _test[_i][_qp];
}