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

#include "FVTimeKernel.h"

class BVFVMultiPhaseSaturationTimeDerivative : public FVTimeKernel
{
public:
  static InputParameters validParams();
  BVFVMultiPhaseSaturationTimeDerivative(const InputParameters & parameters);
  std::string phase_ext();

protected:
  virtual ADReal computeQpResidual() override;

  const enum class PhaseEnum { WETTING, NON_WETTING, SINGLE } _phase;
  const std::string _ext;

  const ADMaterialProperty<Real> & _porosity;
  const ADMaterialProperty<Real> & _density;
  const MaterialProperty<Real> & _density_old;
};