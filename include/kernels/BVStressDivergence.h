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

#include "ADKernel.h"

class BVStressDivergence : public ADKernel
{
public:
  static InputParameters validParams();
  BVStressDivergence(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const VariableValue & _pf;
  const unsigned int _component;
//   const Real _rho;
//   const RealVectorValue _gravity;

  const ADMaterialProperty<RankTwoTensor> & _stress;
//   const MaterialProperty<Real> & _biot;
};