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

#include "Material.h"

class BVRelativePermeabilityBase : public Material
{
public:
  static InputParameters validParams();
  BVRelativePermeabilityBase(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() = 0;
  
  const ADVariableValue & _sw;
  ADMaterialProperty<Real> & _kr_w;
  ADMaterialProperty<Real> & _kr_n;
  // ADMaterialProperty<Real> & _dkr_w;
  // ADMaterialProperty<Real> & _dkr_n;
};