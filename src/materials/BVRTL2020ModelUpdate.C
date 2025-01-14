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

#include "BVRTL2020ModelUpdate.h"

registerMooseObject("BeaverApp", BVRTL2020ModelUpdate);

InputParameters
BVRTL2020ModelUpdate::validParams()
{
  InputParameters params = BVTwoCreepUpdateBase::validParams();
  params.addClassDescription(
      "Material for computing a RTL2020 creep update. See Azabou et al. (2021), Rock salt "
      "behavior: From laboratory experiments to pertinent long-term predictions.");
  // Lemaitre creep strain rate parameters
  params.addRequiredRangeCheckedParam<Real>("alpha", "0.0 < alpha & alpha < 1.0", "The alpha parameter.");
  params.addRequiredRangeCheckedParam<Real>("A2", "A2 > 0.0", "The A2 parameter.");
  params.addRequiredRangeCheckedParam<Real>("n2", "n2 > 0.0", "The n2 parameter.");
  // Munson-Dawson creep strain rate parameters
  params.addRequiredRangeCheckedParam<Real>("A1", "A1 > 0.0", "The A1 parameter.");
  params.addRequiredRangeCheckedParam<Real>("n1", "n1 > 0.0", "The n1 parameter.");
  params.addRequiredRangeCheckedParam<Real>("A", "A >= 0.0", "The A parameter.");
  params.addRequiredRangeCheckedParam<Real>("B", "B >= 0.0", "The B parameter.");
  params.addRequiredRangeCheckedParam<Real>("m", "m > 1.0", "The m parameter.");
  params.addRequiredRangeCheckedParam<Real>("n", "n > 1.0", "The n parameter.");
  return params;
}

BVRTL2020ModelUpdate::BVRTL2020ModelUpdate(const InputParameters & parameters)
  : BVTwoCreepUpdateBase(parameters),
    // Lemaitre creep strain rate parameters
    _alpha(getParam<Real>("alpha")),
    _A2(getParam<Real>("A2")),
    _n2(getParam<Real>("n2")),
    // Munson-Dawson creep strain rate parameters
    _A1(getParam<Real>("A1")),
    _n1(getParam<Real>("n1")),
    _A(getParam<Real>("A")),
    _B(getParam<Real>("B")),
    _m(getParam<Real>("m")),
    _n(getParam<Real>("n")),
    // Internal variable for Lemaitre and Munson-Dawson creep strain
    _eqv_creep_strain_L(declareADProperty<Real>(_base_name + "eqv_creep_strain_L")),
    _eqv_creep_strain_L_old(getMaterialPropertyOld<Real>(_base_name + "eqv_creep_strain_L")),
    _eqv_creep_strain_R(declareADProperty<Real>(_base_name + "eqv_creep_strain_R")),
    _eqv_creep_strain_R_old(getMaterialPropertyOld<Real>(_base_name + "eqv_creep_strain_R"))
{
}

void
BVRTL2020ModelUpdate::initQpStatefulProperties()
{
  _eqv_creep_strain_L[_qp] = 0.0;
  _eqv_creep_strain_R[_qp] = 0.0;
}

ADReal
BVRTL2020ModelUpdate::creepRate(const std::vector<ADReal> & eqv_strain_incr, const unsigned int i)
{
  if (i == 0) // Lemaitre
    return creepRateLemaitre(eqv_strain_incr);
  else if (i == 1) // Munson-Dawson
    return creepRateMunsonDawson(eqv_strain_incr);
  else
    throw MooseException("BVRTL2020ModelUpdate: error, unknow creep model called in `creepRate`!");
}

ADReal
BVRTL2020ModelUpdate::creepRateR(const std::vector<ADReal> & eqv_strain_incr)
{
  ADReal q = _eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1]);

  if (q == 0.0)
    return 0.0;
  else
    return 1.0e-06 * std::pow(q / _A2, _n2);
}

ADReal
BVRTL2020ModelUpdate::creepRateLemaitre(const std::vector<ADReal> & eqv_strain_incr)
{
  ADReal gamma_l = 1.0e+06 * lemaitreCreepStrain(eqv_strain_incr);

  if (gamma_l == 0.0)
    return _alpha * creepRateR(eqv_strain_incr);
  else
    return _alpha * creepRateR(eqv_strain_incr) * std::pow(gamma_l, 1.0 - 1.0 / _alpha);
}

ADReal
BVRTL2020ModelUpdate::creepRateMunsonDawson(const std::vector<ADReal> & eqv_strain_incr)
{
  ADReal q = _eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1]);
  ADReal saturation_strain = (q != 0.0) ? std::pow(q / _A1, _n1) : 1.0e+06;

  ADReal gamma_ms = 1.0e+06 * munsondawsonCreepStrain(eqv_strain_incr);

  if (gamma_ms < saturation_strain)
    return _A * std::pow(1.0 - gamma_ms / saturation_strain, _n) *
           creepRateR(eqv_strain_incr);
  else
    return -_B * std::pow(gamma_ms / saturation_strain - 1.0, _m) *
           creepRateR(eqv_strain_incr);
}

ADReal
BVRTL2020ModelUpdate::creepRateDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                          const unsigned int i,
                                          const unsigned int j)
{
  if (i == 0) // Lemaitre
    return creepRateLemaitreDerivative(eqv_strain_incr, j);
  else if (i == 1) // Munson-Dawson
    return creepRateMunsonDawsonDerivative(eqv_strain_incr, j);
  else
    throw MooseException(
        "BVRTL2020ModelUpdate: error, unknow creep model called in `creepRateDerivative`!");
}

ADReal
BVRTL2020ModelUpdate::creepRateRDerivative(const std::vector<ADReal> & eqv_strain_incr)
{
  ADReal q = _eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1]);

  if (q == 0.0)
    return 1.0;
  else
    return -1.0e-06 * 3.0 * _G * _n2 / _A2 * std::pow(q / _A2, _n2 - 1.0);
}

ADReal
BVRTL2020ModelUpdate::creepRateLemaitreDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                                  const unsigned int j)
{
  ADReal gamma_l = 1.0e+06 * lemaitreCreepStrain(eqv_strain_incr);

  if (j == 0) // Lemaitre wrt Lemaitre
    if (gamma_l == 0.0)
      return _alpha * creepRateRDerivative(eqv_strain_incr);
    else
      return std::pow(gamma_l, -1.0 / _alpha) *
             (_alpha * gamma_l * creepRateRDerivative(eqv_strain_incr) +
              1.0e+06 * (_alpha - 1.0) * creepRateR(eqv_strain_incr));

  else if (j == 1) // Lemaitre wrt Munson-Dawson
    if (gamma_l == 0.0)
      return _alpha * creepRateRDerivative(eqv_strain_incr);
    else
      return _alpha * creepRateRDerivative(eqv_strain_incr) * std::pow(gamma_l, 1.0 - 1.0 / _alpha);

  else
    throw MooseException(
        "BVRTL2020ModelUpdate: error, unknow creep model called in `creepRateDerivative`!");
}

ADReal
BVRTL2020ModelUpdate::creepRateMunsonDawsonDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                                      const unsigned int j)
{
  ADReal q = _eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1]);
  ADReal saturation_strain = (q != 0.0) ? std::pow(q / _A1, _n1) : 1.0e+06;

  ADReal gamma_ms = 1.0e+06 * munsondawsonCreepStrain(eqv_strain_incr);

  if (j == 0) // Munson-Dawson wrt Lemaitre
    if (gamma_ms < saturation_strain)
      return _A * std::pow(1.0 - gamma_ms / saturation_strain, _n) *
             creepRateRDerivative(eqv_strain_incr);
    else
      return -_B * std::pow(gamma_ms / saturation_strain - 1.0, _m) *
             creepRateRDerivative(eqv_strain_incr);
             
  else if (j == 1) // Munson-Dawson wrt Munson-Dawson
    if (gamma_ms < saturation_strain)
      return _A * std::pow(1.0 - gamma_ms / saturation_strain, _n - 1.0) *
             ((1.0 - gamma_ms / saturation_strain) * creepRateRDerivative(eqv_strain_incr) -
              1.0e+06 * _n / saturation_strain * creepRateR(eqv_strain_incr));
    else
      return -_B * std::pow(gamma_ms / saturation_strain - 1.0, _m - 1.0) *
             ((gamma_ms / saturation_strain - 1.0) * creepRateRDerivative(eqv_strain_incr) +
              1.0e+06 * _m / saturation_strain * creepRateR(eqv_strain_incr));
  
  else
    throw MooseException(
        "BVRTL2020ModelUpdate: error, unknow creep model called in `creepRateDerivative`!");
}

ADReal
BVRTL2020ModelUpdate::lemaitreCreepStrain(const std::vector<ADReal> & eqv_strain_incr)
{
  return _eqv_creep_strain_L_old[_qp] + eqv_strain_incr[0];
}

ADReal
BVRTL2020ModelUpdate::munsondawsonCreepStrain(const std::vector<ADReal> & eqv_strain_incr)
{
  return _eqv_creep_strain_R_old[_qp] + eqv_strain_incr[1];
}

void
BVRTL2020ModelUpdate::preReturnMap()
{
  _eqv_creep_strain_L[_qp] = _eqv_creep_strain_L_old[_qp];
  _eqv_creep_strain_R[_qp] = _eqv_creep_strain_R_old[_qp];
}

void
BVRTL2020ModelUpdate::postReturnMap(const std::vector<ADReal> & eqv_strain_incr)
{
  _eqv_creep_strain_L[_qp] = lemaitreCreepStrain(eqv_strain_incr);
  _eqv_creep_strain_R[_qp] = munsondawsonCreepStrain(eqv_strain_incr);
}