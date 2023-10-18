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

#include "BVLubby2ModelUpdate.h"

registerMooseObject("BeaverApp", BVLubby2ModelUpdate);

InputParameters
BVLubby2ModelUpdate::validParams()
{
  InputParameters params = BVBurgerModelUpdate::validParams();
  params.addClassDescription("Material for computing a Lubby 2's model viscoelastic update.");
  params.addRangeCheckedParam<Real>("sigma_0", 1.0, "sigma_0 > 0.0", "The reference stress.");
  params.addRangeCheckedParam<Real>("m_1", 0.0, "m_1 >= 0.0", "The Maxwell stress-dependence parameter.");
  params.addRangeCheckedParam<Real>("m_2", 0.0, "m_2 >= 0.0", "The Kelvin viscosity stress-dependence parameter.");
  params.addRangeCheckedParam<Real>("m_G", 0.0, "m_G >= 0.0", "The Kelvin elastic modulus stress-dependence parameter.");
  return params;
}

BVLubby2ModelUpdate::BVLubby2ModelUpdate(const InputParameters & parameters)
  : BVBurgerModelUpdate(parameters),
    _s0(getParam<Real>("sigma_0")),
    _m1(getParam<Real>("m_1")),
    _m2(getParam<Real>("m_2")),
    _mG(getParam<Real>("m_G"))
{
}

ADReal
BVLubby2ModelUpdate::viscosityMaxwell(const ADReal & eqv_stress)
{
  return _etaM0 * exp(-_m1 * eqv_stress / _s0);
}

ADReal
BVLubby2ModelUpdate::viscosityKelvin(const ADReal & eqv_stress)
{
  return _etaK0 * exp(-_m2 * eqv_stress / _s0);
}


ADReal
BVLubby2ModelUpdate::viscosityMaxwellDerivative(const ADReal & eqv_stress)
{
  return -_m1 /_s0 * viscosityMaxwell(eqv_stress);
}

ADReal
BVLubby2ModelUpdate::viscosityKelvinDerivative(const ADReal & eqv_stress)
{
  return -_m2 /_s0 * viscosityKelvin(eqv_stress);
}

ADReal
BVLubby2ModelUpdate::shearModulusKelvin(const ADReal & eqv_stress)
{
  return _GK0 * exp(-_mG * eqv_stress / _s0);
}

ADReal
BVLubby2ModelUpdate::shearModulusKelvinDerivative(const ADReal & eqv_stress)
{
  return -_mG / _s0 * shearModulusKelvin(eqv_stress);
}