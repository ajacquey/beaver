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

#include "BVCreepUpdateBase.h"

class BVModifiedLemaitreModelUpdate : public BVCreepUpdateBase
{
public:
  static InputParameters validParams();
  BVModifiedLemaitreModelUpdate(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual ADReal creepRateR(const ADReal & eqv_strain_incr);
  virtual ADReal creepRate(const ADReal & eqv_strain_incr) override;
  virtual ADReal creepRateRDerivative(const ADReal & eqv_strain_incr);
  virtual ADReal creepRateDerivative(const ADReal & eqv_strain_incr) override;
  virtual ADReal lemaitreCreepStrain(const ADReal & eqv_strain_incr);
  virtual void preReturnMap() override;
  virtual void postReturnMap(const ADReal & eqv_strain_incr) override;

  // Lemaitre creep strain rate parameters
  const Real _alpha;
  const Real _kr1;
  const Real _kr2;
  const Real _beta1;
  const Real _beta2;

  // Internal variable for creep strain
  ADMaterialProperty<Real> & _eqv_creep_strain;
  const MaterialProperty<Real> & _eqv_creep_strain_old;
};
