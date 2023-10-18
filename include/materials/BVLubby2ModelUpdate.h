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

#include "BVBurgerModelUpdate.h"

class BVLubby2ModelUpdate : public BVBurgerModelUpdate
{
public:
  static InputParameters validParams();
  BVLubby2ModelUpdate(const InputParameters & parameters);

protected:
  virtual ADReal viscosityMaxwell(const ADReal & eqv_stress) override;
  virtual ADReal viscosityKelvin(const ADReal & eqv_stress) override;
  virtual ADReal viscosityMaxwellDerivative(const ADReal & eqv_stress) override;
  virtual ADReal viscosityKelvinDerivative(const ADReal & eqv_stress) override;
  virtual ADReal shearModulusKelvin(const ADReal & eqv_stress) override;
  virtual ADReal shearModulusKelvinDerivative(const ADReal & eqv_stress) override;

  // Reference stress
  Real _s0;
  // Stress-dependence parameters
  Real _m1;
  Real _m2;
  Real _mG;
};
