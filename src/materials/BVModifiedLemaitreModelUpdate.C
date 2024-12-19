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

#include "BVModifiedLemaitreModelUpdate.h"

registerMooseObject("BeaverApp", BVModifiedLemaitreModelUpdate);

InputParameters
BVModifiedLemaitreModelUpdate::validParams()
{
  InputParameters params = BVCreepUpdateBase::validParams();
  params.addClassDescription(
      "Material for computing a modified Lemaitre creep update (see Blanco-Martin 2024).");
  params.addRequiredRangeCheckedParam<Real>("alpha", "0.0 < alpha & alpha < 1.0", "The alpha parameter.");
  params.addRequiredRangeCheckedParam<Real>("kr1", "kr1 > 0.0", "The kr1 parameter.");
  params.addRequiredRangeCheckedParam<Real>("kr2", "kr2 > 0.0", "The kr2 parameter.");
  params.addRequiredRangeCheckedParam<Real>("beta1", "beta1 > 0.0", "The beta1 parameter.");
  params.addRequiredRangeCheckedParam<Real>("beta2", "beta2 > 0.0", "The beta2 parameter.");
  return params;
}

BVModifiedLemaitreModelUpdate::BVModifiedLemaitreModelUpdate(const InputParameters & parameters)
  : BVCreepUpdateBase(parameters),
    // Modified Lemaitre creep strain rate parameters
    _alpha(getParam<Real>("alpha")),
    _kr1(getParam<Real>("kr1")),
    _kr2(getParam<Real>("kr2")),
    _beta1(getParam<Real>("beta1")),
    _beta2(getParam<Real>("beta2")),
    // Internal variable for creep strain
    _eqv_creep_strain(declareADProperty<Real>(_base_name + "eqv_creep_strain")),
    _eqv_creep_strain_old(getMaterialPropertyOld<Real>(_base_name + "eqv_creep_strain"))
{
}

void
BVModifiedLemaitreModelUpdate::initQpStatefulProperties()
{
  _eqv_creep_strain[_qp] = 0.0;
}

ADReal
BVModifiedLemaitreModelUpdate::creepRateR(const ADReal & eqv_strain_incr)
{
  ADReal q = _eqv_stress_tr - 3.0 * _G * eqv_strain_incr;

  if (q == 0.0)
    return 0.0;
  else
    return 1.0e-06 * std::pow(std::pow(q / _kr1, _beta1) + std::pow(q / _kr2, _beta2), 1.0 / _alpha);
}

ADReal
BVModifiedLemaitreModelUpdate::creepRate(const ADReal & eqv_strain_incr)
{
  ADReal gamma_l = 1.0e+06 * lemaitreCreepStrain(eqv_strain_incr);

  if (gamma_l == 0.0)
    return _alpha * creepRateR(eqv_strain_incr);
  else
    return _alpha * creepRateR(eqv_strain_incr) * std::pow(gamma_l, 1.0 - 1.0 / _alpha);
}

ADReal
BVModifiedLemaitreModelUpdate::creepRateRDerivative(const ADReal & eqv_strain_incr)
{
  ADReal q = _eqv_stress_tr - 3.0 * _G * eqv_strain_incr;

  if (q == 0.0)
    return 1.0;
  else
    return -1.0e-06 * 3.0 * _G / _alpha *
         std::pow(std::pow(q / _kr1, _beta1) + std::pow(q / _kr2, _beta2), 1.0 / _alpha - 1.0) *
         (_beta1 / _kr1 * std::pow(q / _kr1, _beta1 - 1.0) +
          _beta2 / _kr2 * std::pow(q / _kr2, _beta2 - 1.0));
}

ADReal
BVModifiedLemaitreModelUpdate::creepRateDerivative(const ADReal & eqv_strain_incr)
{
  ADReal gamma_l = 1.0e+06 * lemaitreCreepStrain(eqv_strain_incr);

  if (gamma_l == 0.0)
    return _alpha * creepRateRDerivative(eqv_strain_incr);
  else
    return std::pow(gamma_l, -1.0 / _alpha) *
           (_alpha * gamma_l * creepRateRDerivative(eqv_strain_incr) +
            1.0e+06 * (_alpha - 1.0) * creepRateR(eqv_strain_incr));
}

ADReal
BVModifiedLemaitreModelUpdate::lemaitreCreepStrain(const ADReal & eqv_strain_incr)
{
  return _eqv_creep_strain_old[_qp] + eqv_strain_incr;
}

void
BVModifiedLemaitreModelUpdate::preReturnMap()
{
  _eqv_creep_strain[_qp] = _eqv_creep_strain_old[_qp];
}

void
BVModifiedLemaitreModelUpdate::postReturnMap(const ADReal & eqv_strain_incr)
{
  _eqv_creep_strain[_qp] = lemaitreCreepStrain(eqv_strain_incr);
}