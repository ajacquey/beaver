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

class BVMaxwellViscoelasticUpdate : public BVCreepUpdateBase
{
public:
  static InputParameters validParams();
  BVMaxwellViscoelasticUpdate(const InputParameters & parameters);

protected:
  virtual ADReal creepRate(const ADReal & eqv_strain_incr) override;
  virtual ADReal creepRateDerivative(const ADReal & eqv_strain_incr) override;

  // Reference viscosity
  const Real _eta0;
};
