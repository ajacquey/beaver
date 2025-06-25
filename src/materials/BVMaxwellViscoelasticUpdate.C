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

#include "BVMaxwellViscoelasticUpdate.h"

registerMooseObject("BeaverApp", BVMaxwellViscoelasticUpdate);

InputParameters
BVMaxwellViscoelasticUpdate::validParams()
{
  InputParameters params = BVCreepUpdateBase::validParams();
  params.addClassDescription("Material for computing a linear Maxwell viscoelastic update.");
  params.addRequiredRangeCheckedParam<Real>("viscosity", "viscosity > 0.0", "The viscosity.");
  return params;
}

BVMaxwellViscoelasticUpdate::BVMaxwellViscoelasticUpdate(const InputParameters & parameters)
  : BVCreepUpdateBase(parameters), _eta0(getParam<Real>("viscosity"))
{
}

ADReal
BVMaxwellViscoelasticUpdate::creepRate(const ADReal & eqv_strain_incr)
{
  return (_eqv_stress_tr - 3.0 * _G * eqv_strain_incr) / (3.0 * _eta0);
}

ADReal
BVMaxwellViscoelasticUpdate::creepRateDerivative(const ADReal & /*eqv_strain_incr*/)
{
  return - _G / _eta0;
}