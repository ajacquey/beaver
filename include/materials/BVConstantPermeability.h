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

#include "BVPermeabilityBase.h"

class BVConstantPermeability : public BVPermeabilityBase
{
public:
  static InputParameters validParams();
  BVConstantPermeability(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const Real _permeability0;
};