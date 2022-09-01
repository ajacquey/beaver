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

#include "BVSinglePhaseTimeDerivative.h"

registerMooseObject("BeaverApp", BVSinglePhaseTimeDerivative);

InputParameters
BVSinglePhaseTimeDerivative::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription("Kernel for the transient term for single phase flow.");
  return params;
}

BVSinglePhaseTimeDerivative::BVSinglePhaseTimeDerivative(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _porosity(getADMaterialProperty<Real>("porosity")),
    _density(getADMaterialProperty<Real>("density")),
    _density_old(getMaterialPropertyOld<Real>("density"))
{
}

ADReal
BVSinglePhaseTimeDerivative::precomputeQpResidual()
{
  return _porosity[_qp] / _density[_qp] * (_density[_qp] - _density_old[_qp]) / _dt;
}