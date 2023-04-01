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

#include "BVBurgerModelUpdate.h"

registerMooseObject("BeaverApp", BVBurgerModelUpdate);

InputParameters
BVBurgerModelUpdate::validParams()
{
  InputParameters params = BVCreepUpdateBase::validParams();
  params.addClassDescription("Material for computing a Burger's model viscoelastic update.");
  params.addRequiredRangeCheckedParam<Real>("viscosity_maxwell", "viscosity_maxwell > 0.0", "The Maxwell viscosity.");
  params.addRequiredRangeCheckedParam<Real>("viscosity_kelvin", "viscosity_kelvin > 0.0", "The Kelvin viscosity.");
  params.addRequiredRangeCheckedParam<Real>(
      "shear_modulus_kelvin", "shear_modulus_kelvin > 0.0", "The Kelvin shear modulus.");
  return params;
}

BVBurgerModelUpdate::BVBurgerModelUpdate(const InputParameters & parameters)
  : BVCreepUpdateBase(parameters),
    _etaM0(getParam<Real>("viscosity_maxwell")),
    _etaK0(getParam<Real>("viscosity_kelvin")),
    _GK0(getParam<Real>("shear_modulus_kelvin")),
    // Internal variable for Kelvin creep strain
    _eqv_creep_strainK(declareADProperty<Real>(_base_name + "eqv_Kelvin_creep_strain")),
    _eqv_creep_strainK_old(getMaterialPropertyOld<Real>(_base_name + "eqv_Kelvin_creep_strain"))
{
}

void
BVBurgerModelUpdate::initQpStatefulProperties()
{
  _eqv_creep_strainK[_qp] = 0.0;
}

ADReal
BVBurgerModelUpdate::creepRate(const ADReal & eqv_stress)
{
  return creepRateMaxwell(eqv_stress) + creepRateKelvin(eqv_stress);
}

ADReal
BVBurgerModelUpdate::creepRateMaxwell(const ADReal & eqv_stress)
{
  return eqv_stress / (3.0 * viscosityMaxwell(eqv_stress));
}

ADReal
BVBurgerModelUpdate::creepRateKelvin(const ADReal & eqv_stress)
{
  return (eqv_stress / (3.0 * viscosityKelvin(eqv_stress)) -
          shearModulusKelvin(eqv_stress) / viscosityKelvin(eqv_stress) * _eqv_creep_strainK_old[_qp]) /
         (1.0 + shearModulusKelvin(eqv_stress) * _dt / viscosityKelvin(eqv_stress));
}

ADReal
BVBurgerModelUpdate::creepRateDerivative(const ADReal & eqv_stress)
{
  return creepRateMaxwellDerivative(eqv_stress) + creepRateKelvinDerivative(eqv_stress);
}

ADReal
BVBurgerModelUpdate::creepRateMaxwellDerivative(const ADReal & eqv_stress)
{
  return 1.0 / (3.0 * viscosityMaxwell(eqv_stress)) *
         (1.0 - eqv_stress / viscosityMaxwell(eqv_stress) * viscosityMaxwellDerivative(eqv_stress));
}

ADReal
BVBurgerModelUpdate::creepRateKelvinDerivative(const ADReal & eqv_stress)
{
  return (1.0 / 3.0 -
          (shearModulusKelvinDerivative(eqv_stress) -
           shearModulusKelvin(eqv_stress) / viscosityKelvin(eqv_stress) * viscosityKelvinDerivative(eqv_stress)) *
              (_eqv_creep_strainK_old[_qp] +
               (eqv_stress - shearModulusKelvin(eqv_stress) * _eqv_creep_strainK_old[_qp]) * _dt /
                   (viscosityKelvin(eqv_stress) *
                    (1.0 + shearModulusKelvin(eqv_stress) * _dt / viscosityKelvin(eqv_stress))))) /
         (viscosityKelvin(eqv_stress) * (1.0 + shearModulusKelvin(eqv_stress) * _dt / viscosityKelvin(eqv_stress)));
}

ADReal
BVBurgerModelUpdate::viscosityMaxwell(const ADReal & eqv_stress)
{
  return _etaM0;
}

ADReal
BVBurgerModelUpdate::viscosityKelvin(const ADReal & eqv_stress)
{
  return _etaK0;
}


ADReal
BVBurgerModelUpdate::viscosityMaxwellDerivative(const ADReal & eqv_stress)
{
  return 0.0;
}

ADReal
BVBurgerModelUpdate::viscosityKelvinDerivative(const ADReal & eqv_stress)
{
  return 0.0;
}

ADReal
BVBurgerModelUpdate::shearModulusKelvin(const ADReal & eqv_stress)
{
  return _GK0;
}

ADReal
BVBurgerModelUpdate::shearModulusKelvinDerivative(const ADReal & eqv_stress)
{
  return 0.0;
}

void
BVBurgerModelUpdate::preReturnMap()
{
  _eqv_creep_strainK[_qp] = _eqv_creep_strainK_old[_qp];
}

void
BVBurgerModelUpdate::postReturnMap(const ADReal & eqv_stress)
{
  _eqv_creep_strainK[_qp] = _eqv_creep_strainK_old[_qp] + creepRateKelvin(eqv_stress) * _dt;
}