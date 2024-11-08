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

#include "BVFVMultiPhasePressureTimeDerivative.h"

registerADMooseObject("BeaverApp", BVFVMultiPhasePressureTimeDerivative);

InputParameters
BVFVMultiPhasePressureTimeDerivative::validParams()
{
  InputParameters params = FVTimeKernel::validParams();
  params.addClassDescription("Kernel for the total pressure transient term for multi phase flow.");
  params.addRequiredCoupledVar("saturation_w", "The wetting phase saturation variable.");
  params.addRequiredCoupledVar("saturation_n", "The non-wetting phase saturation variable.");
  return params;
}

BVFVMultiPhasePressureTimeDerivative::BVFVMultiPhasePressureTimeDerivative(const InputParameters & parameters)
  : FVTimeKernel(parameters),
    _sw(adCoupledValue("saturation_w")),
    _sn(adCoupledValue("saturation_n")),
    _porosity(getADMaterialProperty<Real>("porosity")),
    _density_w(getADMaterialProperty<Real>("density_w")),
    _density_w_old(getMaterialPropertyOld<Real>("density_w")),
    _density_n(getADMaterialProperty<Real>("density_n")),
    _density_n_old(getMaterialPropertyOld<Real>("density_n"))
{
}

ADReal
BVFVMultiPhasePressureTimeDerivative::computeQpResidual()
{
  return _porosity[_qp] * _sw[_qp] / _density_w[_qp] * (_density_w[_qp] - _density_w_old[_qp]) /
             _dt +
         _porosity[_qp] * _sn[_qp] / _density_n[_qp] * (_density_n[_qp] - _density_n_old[_qp]) /
             _dt;
}