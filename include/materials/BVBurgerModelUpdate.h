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

#include "BVTwoCreepUpdateBase.h"

class BVBurgerModelUpdate : public BVTwoCreepUpdateBase
{
public:
  static InputParameters validParams();
  BVBurgerModelUpdate(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual ADReal creepRate(const std::vector<ADReal> & eqv_strain_incr,
                           const unsigned int i) override;
  virtual ADReal creepRateMaxwell(const std::vector<ADReal> & eqv_strain_incr);
  virtual ADReal creepRateKelvin(const std::vector<ADReal> & eqv_strain_incr);
  virtual ADReal creepRateDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                     const unsigned int i,
                                     const unsigned int j) override;
  virtual ADReal creepRateMaxwellDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                            const unsigned int /*j*/);
  virtual ADReal creepRateKelvinDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                           const unsigned int j);
  virtual ADReal viscosityMaxwell(const std::vector<ADReal> & eqv_strain_incr);
  virtual ADReal viscosityKelvin(const std::vector<ADReal> & eqv_strain_incr);
  virtual ADReal viscosityMaxwellDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                            const unsigned int j);
  virtual ADReal viscosityKelvinDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                           const unsigned int j);
  virtual ADReal shearModulusKelvin(const std::vector<ADReal> & eqv_strain_incr);
  virtual ADReal shearModulusKelvinDerivative(const std::vector<ADReal> & eqv_strain_incr,
                                              const unsigned int j);
  virtual ADReal kelvinCreepStrain(const std::vector<ADReal> & eqv_strain_incr);
  virtual void preReturnMap() override;
  virtual void postReturnMap(const std::vector<ADReal> & eqv_strain_incr) override;

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
