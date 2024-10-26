/******************************************************************************/
/*                            This file is part of                            */
/*                       BEAVER, a MOOSE-based application                    */
/*       Multiphase Flow Poromechanics for Induced Seismicity Problems        */
/*                                                                            */
/*                  Copyright (C) 2024 by Antoine B. Jacquey                  */
/*                  Tufts University / Polytechnique Montreal                 */
/*                                                                            */
/*            Licensed under GNU Lesser General Public License v2.1           */
/*                       please see LICENSE for details                       */
/*                 or http://www.gnu.org/licenses/lgpl.html                   */
/******************************************************************************/

#include "BVFrictionUpdateBase.h"

InputParameters
BVFrictionUpdateBase::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Base material for computing a fault friction slip update.");
  // Newton-Raphson loop parameters
  params.addRangeCheckedParam<Real>("abs_tolerance",
                                    1.0e-10,
                                    "abs_tolerance > 0.0",
                                    "The absolute tolerance for the iterative update.");
  params.addRangeCheckedParam<Real>("rel_tolerance",
                                    1.0e-10,
                                    "rel_tolerance > 0.0",
                                    "The relative tolerance for the iterative update.");
  params.addRangeCheckedParam<unsigned int>(
      "max_iterations",
      200,
      "max_iterations >= 1",
      "The maximum number of iterations for the iterative update");
  // Material is called
  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");
  return params;
}

BVFrictionUpdateBase::BVFrictionUpdateBase(const InputParameters & parameters)
  : Material(parameters),
    _abs_tol(getParam<Real>("abs_tolerance")),
    _rel_tol(getParam<Real>("rel_tolerance")),
    _max_its(getParam<unsigned int>("max_iterations"))
{
}

void
BVFrictionUpdateBase::setQp(unsigned int qp)
{
  _qp = qp;
}

void
BVFrictionUpdateBase::frictionUpdate(ADRealVectorValue & traction, const RankTwoTensor & K)
{
  // Trial traction
  _traction_tr = ADRealVectorValue(0.0, traction(1), traction(2));
  // Trial shear and normal stress
  _sigma_tr = traction(0);
  _tau_tr = _traction_tr.norm();

  // Elastic shear stiffness
  _ks = K(1, 1);

  // Pre return map calculations (model specific)
  preReturnMap();

  if ((_tau_tr - frictionalStrength(0.0)) <= 0.0) // No slip
    return;

  // mooseError(_traction_tr(0).value(), "\n", _traction_tr(1).value(), "\n",
  // _traction_tr(2).value()); mooseError("Normal stress: ", _sigma_tr.value(), "\nShear stress: ",
  // _tau_tr.value()); Friction update to compute slip rate
  ADReal delta_dot = returnMap();

  // Update quantities
  ADRealVectorValue slip_incr = reformSlipVector(delta_dot);
  traction -= K * slip_incr;
  postReturnMap(delta_dot);
}

ADReal
BVFrictionUpdateBase::returnMap()
{
  // Initialize scalar slip rate
  ADReal delta_dot = 0.0;

  // Initial residual
  ADReal res_ini = residual(delta_dot);

  ADReal res = res_ini;
  ADReal jac = jacobian(delta_dot);

  // Newton loop
  for (unsigned int iter = 0; iter < _max_its; ++iter)
  {
    delta_dot -= res / jac;

    res = residual(delta_dot);
    jac = jacobian(delta_dot);

    // Convergence check
    if ((std::abs(res) <= _abs_tol) || (std::abs(res / res_ini) <= _rel_tol))
      return delta_dot;
  }
  throw MooseException(
      "BVFrictionUpdateBase: maximum number of iterations exceeded in 'returnMap'!");
}

ADReal
BVFrictionUpdateBase::residual(const ADReal & delta_dot)
{
  return _tau_tr - _ks * delta_dot * _dt - frictionalStrength(delta_dot);
}

ADReal
BVFrictionUpdateBase::jacobian(const ADReal & delta_dot)
{
  return -_ks * _dt - frictionalStrengthDeriv(delta_dot);
}

ADRealVectorValue
BVFrictionUpdateBase::reformSlipVector(const ADReal & delta_dot)
{
  return delta_dot * _dt * _traction_tr / _tau_tr;
}

void
BVFrictionUpdateBase::preReturnMap()
{
}

void
BVFrictionUpdateBase::postReturnMap(const ADReal & delta_dot)
{
}