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

#include "BVFVSinglePhaseSoluteTimeDerivative.h"

registerADMooseObject("BeaverApp", BVFVSinglePhaseSoluteTimeDerivative);

InputParameters
BVFVSinglePhaseSoluteTimeDerivative::validParams()
{
  InputParameters params = FVTimeKernel::validParams();
  params.addClassDescription(
      "Kernel for the transient term for single phase flow solute transport.");
  return params;
}

BVFVSinglePhaseSoluteTimeDerivative::BVFVSinglePhaseSoluteTimeDerivative(
    const InputParameters & parameters)
  : FVTimeKernel(parameters), _porosity(getADMaterialProperty<Real>("porosity"))
{
}

ADReal
BVFVSinglePhaseSoluteTimeDerivative::computeQpResidual()
{
  return _porosity[_qp] * _u_dot[_qp];
}
