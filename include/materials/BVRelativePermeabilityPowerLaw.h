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

#include "BVRelativePermeabilityBase.h"

class BVRelativePermeabilityPowerLaw : public BVRelativePermeabilityBase
{
public:
  static InputParameters validParams();
  BVRelativePermeabilityPowerLaw(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  
  const Real _n;
};