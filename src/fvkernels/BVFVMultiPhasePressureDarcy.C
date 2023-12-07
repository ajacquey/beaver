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

#include "BVFVMultiPhasePressureDarcy.h"

registerADMooseObject("BeaverApp", BVFVMultiPhasePressureDarcy);

InputParameters
BVFVMultiPhasePressureDarcy::validParams()
{
  InputParameters params = BVTwoPointFluxApproximationBase::validParams();
  params.addClassDescription(
    "Kernel for the divergence of the total velocity for multi phase flow.");
  return params;
}

BVFVMultiPhasePressureDarcy::BVFVMultiPhasePressureDarcy(const InputParameters & parameters)
  : BVTwoPointFluxApproximationBase(parameters),
    _lambda_w(getADMaterialProperty<Real>("fluid_mobility_w")),
    _lambda_w_neighbor(getNeighborADMaterialProperty<Real>("fluid_mobility_w")),
    _lambda_n(getADMaterialProperty<Real>("fluid_mobility_n")),
    _lambda_n_neighbor(getNeighborADMaterialProperty<Real>("fluid_mobility_n"))
{
}

ADReal
BVFVMultiPhasePressureDarcy::computeQpResidual()
{
  // Total mobility
  ADReal lambda_T = _lambda_w[_qp] + _lambda_n[_qp];
  ADReal lambda_T_neighbor = _lambda_w_neighbor[_qp] + _lambda_n_neighbor[_qp];

  // Total velocity
  ADRealVectorValue u = diffusiveFlux(lambda_T, lambda_T_neighbor, _var);

  return u * (*_face_info).normal();
}