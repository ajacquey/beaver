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

#include "BVTwoPointFluxApproximationBase.h"

class BVFVMultiPhaseSaturationDarcy : public BVTwoPointFluxApproximationBase
{
public:
  static InputParameters validParams();
  BVFVMultiPhaseSaturationDarcy(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const enum class PhaseEnum { WETTING, NON_WETTING, SINGLE } _phase;

  const MooseVariableFV<Real> * _p_var;
  const ADMaterialProperty<Real> & _lambda;
  const ADMaterialProperty<Real> & _lambda_neighbor;
  const ADMaterialProperty<Real> & _f_w;
  const ADMaterialProperty<Real> & _f_w_neighbor;
  const ADMaterialProperty<Real> & _f_n;
  const ADMaterialProperty<Real> & _f_n_neighbor;
  const ADMaterialProperty<Real> & _D;
  const ADMaterialProperty<Real> & _D_neighbor;
};
