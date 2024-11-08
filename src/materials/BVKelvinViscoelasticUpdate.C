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

#include "BVKelvinViscoelasticUpdate.h"

registerMooseObject("BeaverApp", BVKelvinViscoelasticUpdate);

InputParameters
BVKelvinViscoelasticUpdate::validParams()
{
  InputParameters params = BVCreepUpdateBase::validParams();
  params.addClassDescription("Material for computing a linear Maxwell viscoelastic update.");
  params.addRequiredRangeCheckedParam<Real>("viscosity", "viscosity > 0.0", "The viscosity.");
  params.addRequiredRangeCheckedParam<Real>(
      "shear_modulus", "shear_modulus > 0.0", "The Kelvin shear modulus.");
  return params;
}

BVKelvinViscoelasticUpdate::BVKelvinViscoelasticUpdate(const InputParameters & parameters)
  : BVCreepUpdateBase(parameters),
    _eta0(getParam<Real>("viscosity")),
    _G0(getParam<Real>("shear_modulus")),
    // Internal variable for creep strain
    _eqv_creep_strain(declareADProperty<Real>(_base_name + "eqv_creep_strain")),
    _eqv_creep_strain_old(getMaterialPropertyOld<Real>(_base_name + "eqv_creep_strain"))
{
}

void
BVKelvinViscoelasticUpdate::initQpStatefulProperties()
{
  _eqv_creep_strain[_qp] = 0.0;
}

ADReal
BVKelvinViscoelasticUpdate::creepRate(const ADReal & eqv_strain_incr)
{
  return (_eqv_stress_tr - 3.0 * _G * eqv_strain_incr - 3.0 * _G0 * kelvinCreepStrain(eqv_strain_incr)) / (3.0 * _eta0);
}

ADReal
BVKelvinViscoelasticUpdate::creepRateDerivative(const ADReal & eqv_strain_incr)
{
  return -(_G + _G0 * kelvinCreepStrainDerivative(eqv_strain_incr)) / _eta0;
}

ADReal
BVKelvinViscoelasticUpdate::kelvinCreepStrain(const ADReal & eqv_strain_incr)
{
  return _eqv_creep_strain_old[_qp] + eqv_strain_incr;
}

ADReal
BVKelvinViscoelasticUpdate::kelvinCreepStrainDerivative(const ADReal & /*eqv_strain_incr*/)
{
  return 1.0;
}

void
BVKelvinViscoelasticUpdate::preReturnMap()
{
  _eqv_creep_strain[_qp] = _eqv_creep_strain_old[_qp];
}

void
BVKelvinViscoelasticUpdate::postReturnMap(const ADReal & eqv_strain_incr)
{
  _eqv_creep_strain[_qp] = kelvinCreepStrain(eqv_strain_incr);
}