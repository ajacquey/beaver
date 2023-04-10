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

#pragma once

#include "BVCreepUpdateBase.h"

class BVBurgerModelUpdate : public BVCreepUpdateBase
{
public:
  static InputParameters validParams();
  BVBurgerModelUpdate(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual ADReal creepRate(const ADReal & eqv_stress) override;
  virtual ADReal creepRateMaxwell(const ADReal & eqv_stress);
  virtual ADReal creepRateKelvin(const ADReal & eqv_stress);
  virtual ADReal creepRateDerivative(const ADReal & eqv_stress) override;
  virtual ADReal creepRateMaxwellDerivative(const ADReal & eqv_stress);
  virtual ADReal creepRateKelvinDerivative(const ADReal & eqv_stress);
  virtual ADReal viscosityMaxwell(const ADReal & eqv_stress);
  virtual ADReal viscosityKelvin(const ADReal & eqv_stress);
  virtual ADReal viscosityMaxwellDerivative(const ADReal & eqv_stress);
  virtual ADReal viscosityKelvinDerivative(const ADReal & eqv_stress);
  virtual ADReal shearModulusKelvin(const ADReal & eqv_stress);
  virtual ADReal shearModulusKelvinDerivative(const ADReal & eqv_stress);
  virtual void preReturnMap() override;
  virtual void postReturnMap(const ADReal & eqv_stress) override;

  // Maxwell viscosity
  const Real _etaM0;
  // Kelvin viscosity
  const Real _etaK0;
  // Kelvin shear modulus
  const Real _GK0;

  // Internal variable for Kelvin creep strain
  ADMaterialProperty<Real> & _eqv_creep_strainK;
  const MaterialProperty<Real> & _eqv_creep_strainK_old;
};
