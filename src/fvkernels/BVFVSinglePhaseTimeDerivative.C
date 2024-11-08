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

#include "BVFVSinglePhaseTimeDerivative.h"

registerADMooseObject("BeaverApp", BVFVSinglePhaseTimeDerivative);

InputParameters
BVFVSinglePhaseTimeDerivative::validParams()
{
  InputParameters params = FVTimeKernel::validParams();
  params.addClassDescription("Kernel for the transient term for single phase flow.");
  return params;
}

BVFVSinglePhaseTimeDerivative::BVFVSinglePhaseTimeDerivative(const InputParameters & parameters)
  : FVTimeKernel(parameters),
    _porosity(getADMaterialProperty<Real>("porosity")),
    _density(getADMaterialProperty<Real>("density")),
    _density_old(getMaterialPropertyOld<Real>("density")),
    _coupled_mech(hasADMaterialProperty<Real>("poromechanics")),
    _porous_storage(_coupled_mech ? &getADMaterialProperty<Real>("porous_storage") : nullptr),
    _poromech(_coupled_mech ? &getADMaterialProperty<Real>("poromechanics") : nullptr)
{
}

ADReal
BVFVSinglePhaseTimeDerivative::computeQpResidual()
{
  // Fluid compressibility
  ADReal res = _porosity[_qp] / _density[_qp] * (_density[_qp] - _density_old[_qp]) / _dt;

  // Add poromechanics
  if (_coupled_mech)
    res += (*_porous_storage)[_qp] * _u_dot[_qp] + (*_poromech)[_qp];

  return res;
}