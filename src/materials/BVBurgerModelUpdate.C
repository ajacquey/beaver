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

#include "BVBurgerModelUpdate.h"

registerMooseObject("BeaverApp", BVBurgerModelUpdate);

InputParameters
BVBurgerModelUpdate::validParams()
{
  InputParameters params = BVTwoCreepUpdateBase::validParams();
  params.addClassDescription("Material for computing a Burger's model viscoelastic update.");
  params.addRequiredRangeCheckedParam<Real>("viscosity_maxwell", "viscosity_maxwell > 0.0", "The Maxwell viscosity.");
  params.addRequiredRangeCheckedParam<Real>("viscosity_kelvin", "viscosity_kelvin > 0.0", "The Kelvin viscosity.");
  params.addRequiredRangeCheckedParam<Real>(
      "shear_modulus_kelvin", "shear_modulus_kelvin > 0.0", "The Kelvin shear modulus.");
  return params;
}

BVBurgerModelUpdate::BVBurgerModelUpdate(const InputParameters & parameters)
  : BVTwoCreepUpdateBase(parameters),
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
BVBurgerModelUpdate::creepRate(const std::vector<ADReal> & eqv_strain_incr, const unsigned int i)
{
  if (i == 0) // Maxwell
    return creepRateMaxwell(eqv_strain_incr);
  else if (i == 1) // Kelvin
    return creepRateKelvin(eqv_strain_incr);
  else
    throw MooseException("BVBurgerModelUpdate: error, unknow creep model called in `creepRate`!");
}

ADReal
BVBurgerModelUpdate::creepRateMaxwell(const std::vector<ADReal> & eqv_strain_incr)
{

  return (_eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1])) /
         (3.0 * viscosityMaxwell(eqv_strain_incr));
}

ADReal
BVBurgerModelUpdate::creepRateKelvin(const std::vector<ADReal> & eqv_strain_incr)
{
  return (_eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1]) -
          3.0 * shearModulusKelvin(eqv_strain_incr) * kelvinCreepStrain(eqv_strain_incr)) /
         (3.0 * viscosityKelvin(eqv_strain_incr));
}

ADReal
BVBurgerModelUpdate::creepRateDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                         const unsigned int i,
                                         const unsigned int j)
{
  if (i == 0) // Maxwell
    return creepRateMaxwellDerivative(eqv_strain_incr, j);
  else if (i == 1) // Kelvin
    return creepRateKelvinDerivative(eqv_strain_incr, j);
  else
    throw MooseException(
        "BVBurgerModelUpdate: error, unknow creep model called in `creepRateDerivative`!");
}

ADReal
BVBurgerModelUpdate::creepRateMaxwellDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                                const unsigned int j)
{
  return -_G / viscosityMaxwell(eqv_strain_incr) -
         (_eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1])) *
             viscosityMaxwellDerivative(eqv_strain_incr, j) / viscosityMaxwell(eqv_strain_incr);
}

ADReal
BVBurgerModelUpdate::creepRateKelvinDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                               const unsigned int j)
{
  if (j == 0) // Kelvin wrt Maxwell
    return -(_G + shearModulusKelvinDerivative(eqv_strain_incr, 0) *
                      kelvinCreepStrain(eqv_strain_incr)) /
               viscosityKelvin(eqv_strain_incr) -
           (_eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1])) *
               viscosityKelvinDerivative(eqv_strain_incr, 0) / viscosityKelvin(eqv_strain_incr);
  else if (j == 1) // Kelvin wrt Kelvin
    return -(_G + shearModulusKelvin(eqv_strain_incr) +
             shearModulusKelvinDerivative(eqv_strain_incr, 0) *
                 kelvinCreepStrain(eqv_strain_incr)) /
               viscosityKelvin(eqv_strain_incr) -
           (_eqv_stress_tr - 3.0 * _G * (eqv_strain_incr[0] + eqv_strain_incr[1])) *
               viscosityKelvinDerivative(eqv_strain_incr, 0) / viscosityKelvin(eqv_strain_incr);
  else
    throw MooseException(
        "BVBurgerModelUpdate: error, unknow creep model called in `creepRateDerivative`!");
}

ADReal
BVBurgerModelUpdate::viscosityMaxwell(const std::vector<ADReal> & /*eqv_strain_incr*/)
{
  return _etaM0;
}

ADReal
BVBurgerModelUpdate::viscosityKelvin(const std::vector<ADReal> & /*eqv_strain_incr*/)
{
  return _etaK0;
}

ADReal
BVBurgerModelUpdate::viscosityMaxwellDerivative(const std::vector<ADReal> & /*eqv_strain_incr*/,
                                                const unsigned int /*j*/)
{
  return 0.0;
}

ADReal
BVBurgerModelUpdate::viscosityKelvinDerivative(const std::vector<ADReal> & /*eqv_strain_incr*/,
                                               const unsigned int /*j*/)
{
  return 0.0;
}

ADReal
BVBurgerModelUpdate::shearModulusKelvin(const std::vector<ADReal> & /*eqv_strain_incr*/)
{
  return _GK0;
}

ADReal
BVBurgerModelUpdate::shearModulusKelvinDerivative(const std::vector<ADReal> & /*eqv_strain_incr*/,
                                                  const unsigned int /*j*/)
{
  return 0.0;
}

ADReal
BVBurgerModelUpdate::kelvinCreepStrain(const std::vector<ADReal> & eqv_strain_incr)
{
  return _eqv_creep_strainK_old[_qp] + eqv_strain_incr[1];
}

void
BVBurgerModelUpdate::preReturnMap()
{
  _eqv_creep_strainK[_qp] = _eqv_creep_strainK_old[_qp];
}

void
BVBurgerModelUpdate::postReturnMap(const std::vector<ADReal> & eqv_strain_incr)
{
  _eqv_creep_strainK[_qp] = kelvinCreepStrain(eqv_strain_incr);
}