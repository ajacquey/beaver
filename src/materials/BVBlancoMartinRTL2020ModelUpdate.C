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

#include "BVBlancoMartinRTL2020ModelUpdate.h"

registerMooseObject("BeaverApp", BVBlancoMartinRTL2020ModelUpdate);

InputParameters
BVBlancoMartinRTL2020ModelUpdate::validParams()
{
  InputParameters params = BVDeviatoricVolumetricUpdateBase::validParams();
  params.addClassDescription(
      "Material for computing a RTL2020 creep update. See Azabou et al. (2021), Rock salt "
      "behavior: From laboratory experiments to pertinent long-term predictions.");
  params.addRequiredRangeCheckedParam<Real>("alpha", "0.0 < alpha & alpha < 1.0", "The alpha parameter.");
  params.addRequiredRangeCheckedParam<Real>("kr1", "kr1 > 0.0", "The kr1 parameter.");
  params.addRequiredRangeCheckedParam<Real>("kr2", "kr2 > 0.0", "The kr2 parameter.");
  params.addRequiredRangeCheckedParam<Real>("beta1", "beta1 > 0.0", "The beta1 parameter.");
  params.addRequiredRangeCheckedParam<Real>("beta2", "beta2 > 0.0", "The beta2 parameter.");
  params.addRequiredRangeCheckedParam<Real>("A1", "A1 > 0.0", "The A1 parameter.");
  params.addRequiredRangeCheckedParam<Real>("n1", "n1 > 0.0", "The n1 parameter.");
  params.addRequiredRangeCheckedParam<Real>("A", "A >= 0.0", "The A parameter.");
  params.addRequiredRangeCheckedParam<Real>("B", "B >= 0.0", "The B parameter.");
  params.addRequiredRangeCheckedParam<Real>("m", "m > 1.0", "The m parameter.");
  params.addRequiredRangeCheckedParam<Real>("n", "n > 1.0", "The n parameter.");
  params.addRequiredParam<Real>("z","number of creep models");
  params.addRequiredParam<Real>("Nz", "number of creep models");
  params.addRequiredParam<Real>("nz","number of creep models");
  params.addRequiredParam<Real>("Mz",  "number of creep models");
  params.addRequiredParam<Real>("mz", "number of creep models");
  return params;
}

BVBlancoMartinRTL2020ModelUpdate::BVBlancoMartinRTL2020ModelUpdate(const InputParameters & parameters)
  : BVDeviatoricVolumetricUpdateBase(parameters),
    // Modified Lemaitre creep strain rate parameters
    _alpha(getParam<Real>("alpha")),
    _kr1(getParam<Real>("kr1")),
    _kr2(getParam<Real>("kr2")),
    _beta1(getParam<Real>("beta1")),
    _beta2(getParam<Real>("beta2")),
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
    _eqv_creep_strain_R_old(getMaterialPropertyOld<Real>(_base_name + "eqv_creep_strain_R")),
    // Volumetric creep strain rate parameters
    _z(getParam<Real>("z")),
    _Nz(getParam<Real>("Nz")),
    _nz(getParam<Real>("nz")),
    _Mz(getParam<Real>("Mz")),
    _mz(getParam<Real>("mz"))

{
}

void
BVBlancoMartinRTL2020ModelUpdate::initQpStatefulProperties()
{
  _eqv_creep_strain_L[_qp] = 0.0;
  _eqv_creep_strain_R[_qp] = 0.0;
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::creepRate(const std::vector<ADReal> & eqv_strain_incr, const unsigned int i)
{
  if (i == 0) // Lemaitre
    return creepRateLemaitre(eqv_strain_incr);
  else if (i == 1) // Munson-Dawson
    return creepRateMunsonDawson(eqv_strain_incr);
  else
    throw MooseException("BVBlancoMartinRTL2020ModelUpdate: error, unknow creep model called in `creepRate`!");
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::creepRateR(const std::vector<ADReal> & eqv_strain_incr)
{
  ADReal q = _eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1]);

  if (q == 0.0)
    return 0.0;
  else
    return 1.0e-06 * std::pow(std::pow(q / _kr1, _beta1) + std::pow(q / _kr2, _beta2), 1.0 / _alpha);
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::creepRateLemaitre(const std::vector<ADReal> & eqv_strain_incr)
{
  ADReal gamma_l = 1.0e+06 * lemaitreCreepStrain(eqv_strain_incr);

  if (gamma_l == 0.0)
    return _alpha * creepRateR(eqv_strain_incr);
  else
    return _alpha * creepRateR(eqv_strain_incr) * std::pow(gamma_l, 1.0 - 1.0 / _alpha);
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::creepRateMunsonDawson(const std::vector<ADReal> & eqv_strain_incr)
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
BVBlancoMartinRTL2020ModelUpdate::creepRateDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                          const unsigned int i,
                                          const unsigned int j)
{
  if (i == 0) // Lemaitre
    return creepRateLemaitreDerivative(eqv_strain_incr, j);
  else if (i == 1) // Munson-Dawson
    return creepRateMunsonDawsonDerivative(eqv_strain_incr, j);
  else
    throw MooseException(
        "BVBlancoMartinRTL2020ModelUpdate: error, unknow creep model called in `creepRateDerivative`!");
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::creepRateRDerivative(const std::vector<ADReal> & eqv_strain_incr)
{
  ADReal q = _eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1]);

  if (q == 0.0)
    return 1.0;
  else
    return -1.0e-06 * 3.0 * _G / _alpha *
         std::pow(std::pow(q / _kr1, _beta1) + std::pow(q / _kr2, _beta2), 1.0 / _alpha - 1.0) *
         (_beta1 / _kr1 * std::pow(q / _kr1, _beta1 - 1.0) +
          _beta2 / _kr2 * std::pow(q / _kr2, _beta2 - 1.0));
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::creepRateLemaitreDerivative(const std::vector<ADReal> & eqv_strain_incr,
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
        "BVBlancoMartinRTL2020ModelUpdate: error, unknow creep model called in `creepRateDerivative`!");
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::creepRateMunsonDawsonDerivative(const std::vector<ADReal> & eqv_strain_incr,
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
        "BVBlancoMartinRTL2020ModelUpdate: error, unknow creep model called in `creepRateDerivative`!");
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::lemaitreCreepStrain(const std::vector<ADReal> & eqv_strain_incr)
{
  return _eqv_creep_strain_L_old[_qp] + eqv_strain_incr[0];
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::munsondawsonCreepStrain(const std::vector<ADReal> & eqv_strain_incr)
{
  return _eqv_creep_strain_R_old[_qp] + eqv_strain_incr[1];
}

void
BVBlancoMartinRTL2020ModelUpdate::preReturnMap()
{
  _eqv_creep_strain_L[_qp] = _eqv_creep_strain_L_old[_qp];
  _eqv_creep_strain_R[_qp] = _eqv_creep_strain_R_old[_qp];
}

void
BVBlancoMartinRTL2020ModelUpdate::postReturnMap(const std::vector<ADReal> & eqv_strain_incr)
{
  _eqv_creep_strain_L[_qp] = lemaitreCreepStrain(eqv_strain_incr);
  _eqv_creep_strain_R[_qp] = munsondawsonCreepStrain(eqv_strain_incr);
}

ADReal
BVBlancoMartinRTL2020ModelUpdate::VcreepRate(const std::vector<ADReal> & creep_strain_incr, 
                                             const ADReal & Vcreep_strain_incr)
{   
  ADReal p = _mean_stress_tr - (_K * Vcreep_strain_incr);
      if (p == 0.0)
    return 0.0; // No contribution since p is zero

  // Calculate numerator and denomenator for proportionality constant k
  ADReal gamma_vp = 1.0e+06 * (lemaitreCreepStrain(creep_strain_incr) + munsondawsonCreepStrain(creep_strain_incr));
  ADReal numer = std::pow((p / _Nz) > 0.0 ? (p / _Nz) : 0.0, _nz) - gamma_vp;
  ADReal denom = std::pow((p / _Mz) > 0.0 ? (p / _mz) : 0.0, _nz) + gamma_vp;

  // Safeguard against division by zero
  if (denom != 0.0) 
    {
      ADReal k = _z * (numer) / denom; // proportionality constant
      ADReal res = 0.0; // Initialize res to zero before aggregation
      for (unsigned int i = 0; i < creep_strain_incr.size(); ++i) 
        {
            res += k * creep_strain_incr[i];
        }
      return res;
    } 
  else // Handle the case where denom is zero 
    {
     return 0.0; // No contribution since k is not defined
    }
}

ADReal BVBlancoMartinRTL2020ModelUpdate::VcreepRateDerivative(const std::vector<ADReal> & creep_strain_incr, 
                                                              const ADReal & Vcreep_strain_incr)
{
    ADReal p = _mean_stress_tr - (_K * Vcreep_strain_incr);
      if (p == 0.0)
    return 0.0; // No contribution since p is zero

    ADReal gamma_vp = 1.0e+06 * (lemaitreCreepStrain(creep_strain_incr) + munsondawsonCreepStrain(creep_strain_incr));
    ADReal numer = std::pow((p / _Nz) > 0.0 ? (p / _Nz) : 0.0, _nz) - gamma_vp;
    ADReal denom = std::pow((p / _Mz) > 0.0 ? (p / _mz) : 0.0, _nz) + gamma_vp;

    // Derivative contributions
    ADReal dp_dVcreep_strain_incr = -_K; // derivative of p with respect to Vcreep_strain_incr

    // Computing the derivatives of numerator and denomenator
    ADReal dnumer_dVcreep_strain_incr = _nz * std::pow((p / _Nz) > 0.0 ? (p / _Nz) : 0.0, _nz - 1) * (dp_dVcreep_strain_incr / _Nz);
    ADReal ddenom_dVcreep_strain_incr = _nz * std::pow((p / _Mz) > 0.0 ? (p / _Mz) : 0.0, _nz - 1) * (dp_dVcreep_strain_incr / _Mz);

    // Safeguard situation for denominator
    if (denom != 0.0) 
    {
        // Using the quotient rule to differentiate k
        ADReal k = _z * (numer) / denom;
        ADReal dk_dVcreep_strain_incr = _z * (ddenom_dVcreep_strain_incr * numer - dnumer_dVcreep_strain_incr * denom) / (denom * denom);
        
        // Initialize res to zero before aggregation
        ADReal res = 0; 
        for (unsigned int i = 0; i < creep_strain_incr.size(); ++i) 
        {
            res += dk_dVcreep_strain_incr * creep_strain_incr[i]; // Use the derivative of k
        }
        return res;
    } 
    else 
    {
        // If denom is zero, then k is set to a default value (0.0 in this case)
        return 0.0; // No contribution since k is not defined
    }
}

