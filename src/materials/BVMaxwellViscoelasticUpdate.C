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
BVMaxwellViscoelasticUpdate::creepRate(const ADReal & eqv_stress)
{
  return eqv_stress / (3.0 * viscosity(eqv_stress));
}

ADReal
BVMaxwellViscoelasticUpdate::creepRateDerivative(const ADReal & eqv_stress)
{
  return 1.0 / (3.0 * viscosity(eqv_stress)) *
         (1.0 - eqv_stress / viscosity(eqv_stress) * viscosityDerivative(eqv_stress));
}

ADReal
BVMaxwellViscoelasticUpdate::viscosity(const ADReal & eqv_stress)
{
  return _eta0;
}

ADReal
BVMaxwellViscoelasticUpdate::viscosityDerivative(const ADReal & eqv_stress)
{
  return 0.0;
}