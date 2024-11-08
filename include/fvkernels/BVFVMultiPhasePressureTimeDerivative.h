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

#include "FVTimeKernel.h"

class BVFVMultiPhasePressureTimeDerivative : public FVTimeKernel
{
public:
  static InputParameters validParams();
  BVFVMultiPhasePressureTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADVariableValue & _sw;
  const ADVariableValue & _sn;
  const ADMaterialProperty<Real> & _porosity;
  const ADMaterialProperty<Real> & _density_w;
  const MaterialProperty<Real> & _density_w_old;
  const ADMaterialProperty<Real> & _density_n;
  const MaterialProperty<Real> & _density_n_old;
};