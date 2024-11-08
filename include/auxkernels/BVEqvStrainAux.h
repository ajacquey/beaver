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

#include "BVStrainAuxBase.h"

class BVEqvStrainAux : public BVStrainAuxBase
{
public:
  static InputParameters validParams();
  BVEqvStrainAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  const VariableValue & _u_old;
};