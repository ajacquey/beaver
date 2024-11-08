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

#pragma once

#include "BVBurgerModelUpdate.h"

class BVLubby2ModelUpdate : public BVBurgerModelUpdate
{
public:
  static InputParameters validParams();
  BVLubby2ModelUpdate(const InputParameters & parameters);

protected:
  virtual ADReal viscosityMaxwell(const std::vector<ADReal> & eqv_strain_incr) override;
  virtual ADReal viscosityKelvin(const std::vector<ADReal> & eqv_strain_incr) override;
  virtual ADReal viscosityMaxwellDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                            const unsigned int j) override;
  virtual ADReal viscosityKelvinDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                           const unsigned int j) override;
  virtual ADReal shearModulusKelvin(const std::vector<ADReal> & eqv_strain_incr) override;
  virtual ADReal shearModulusKelvinDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                              const unsigned int j) override;

  // Reference stress
  Real _s0;
  // Stress-dependence parameters
  Real _m1;
  Real _m2;
  Real _mG;
};
