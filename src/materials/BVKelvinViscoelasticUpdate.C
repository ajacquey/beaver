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

#include "BVKelvinViscoelasticUpdate.h"

registerMooseObject("BeaverApp", BVKelvinViscoelasticUpdate);

InputParameters
BVKelvinViscoelasticUpdate::validParams()
{
  InputParameters params = BVCreepUpdateBase::validParams();
  params.addClassDescription("Material for computing a linear Maxwell viscoelastic update.");
  params.addRequiredRangeCheckedParam<Real>("viscosity", "viscosity > 0.0", "The viscosity.");
  params.addRequiredRangeCheckedParam<Real>(
      "shear_modulus", "shear_modulus > 0.0", "The Kelin shear modulus.");
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
BVKelvinViscoelasticUpdate::creepRate(const ADReal & eqv_stress)
{
  return (eqv_stress / (3.0 * viscosity(eqv_stress)) -
          shearModulus(eqv_stress) / viscosity(eqv_stress) * _eqv_creep_strain_old[_qp]) /
         (1.0 + shearModulus(eqv_stress) * _dt / viscosity(eqv_stress));
}

ADReal
BVKelvinViscoelasticUpdate::creepRateDerivative(const ADReal & eqv_stress)
{
  return (1.0 / 3.0 -
          (shearModulusDerivative(eqv_stress) -
           shearModulus(eqv_stress) / viscosity(eqv_stress) * viscosityDerivative(eqv_stress)) *
              (_eqv_creep_strain_old[_qp] +
               (eqv_stress - shearModulus(eqv_stress) * _eqv_creep_strain_old[_qp]) * _dt /
                   (viscosity(eqv_stress) *
                    (1.0 + shearModulus(eqv_stress) * _dt / viscosity(eqv_stress))))) /
         (viscosity(eqv_stress) * (1.0 + shearModulus(eqv_stress) * _dt / viscosity(eqv_stress)));
}

ADReal
BVKelvinViscoelasticUpdate::viscosity(const ADReal & eqv_stress)
{
  return _eta0;
}

ADReal
BVKelvinViscoelasticUpdate::viscosityDerivative(const ADReal & eqv_stress)
{
  return 0.0;
}

ADReal
BVKelvinViscoelasticUpdate::shearModulus(const ADReal & eqv_stress)
{
  return _G0;
}

ADReal
BVKelvinViscoelasticUpdate::shearModulusDerivative(const ADReal & eqv_stress)
{
  return 0.0;
}

void
BVKelvinViscoelasticUpdate::preReturnMap()
{
  _eqv_creep_strain[_qp] = _eqv_creep_strain_old[_qp];
}

void
BVKelvinViscoelasticUpdate::postReturnMap(const ADReal & gamma)
{
  _eqv_creep_strain[_qp] = _eqv_creep_strain_old[_qp] + gamma * _dt;
}