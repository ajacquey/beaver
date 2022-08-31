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

#include "BVStressAuxBase.h"

class BVStressComponentAux : public BVStressAuxBase
{
public:
  static InputParameters validParams();
  BVStressComponentAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();
  
  const unsigned int _i;
  const unsigned int _j;
};