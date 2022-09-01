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

#include "BVSinglePhaseDarcy.h"

registerMooseObject("BeaverApp", BVSinglePhaseDarcy);

InputParameters
BVSinglePhaseDarcy::validParams()
{
  InputParameters params = ADKernelGrad::validParams();
  params.addClassDescription("Kernel for the divergence of Darcy's velocity for single phase flow.");
  return params;
}

BVSinglePhaseDarcy::BVSinglePhaseDarcy(const InputParameters & parameters)
  : ADKernelGrad(parameters),
    _lambda(getADMaterialProperty<Real>("fluid_mobility"))
{
}

ADRealVectorValue
BVSinglePhaseDarcy::precomputeQpResidual()
{
  return _lambda[_qp] * _grad_u[_qp];
}