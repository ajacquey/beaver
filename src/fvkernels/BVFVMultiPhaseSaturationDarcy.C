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

#include "BVFVMultiPhaseSaturationDarcy.h"

registerADMooseObject("BeaverApp", BVFVMultiPhaseSaturationDarcy);

InputParameters
BVFVMultiPhaseSaturationDarcy::validParams()
{
  InputParameters params = BVTwoPointFluxApproximationBase::validParams();
  params.addClassDescription(
    "Kernel for the divergence of Darcy's velocity for multi phase flow.");
  MooseEnum phase("wetting non_wetting single");
  params.addRequiredParam<MooseEnum>(
      "phase", phase, "The phase of the fluid properties (wetting, non_wetting or single).");
  params.addRequiredCoupledVar("total_pressure", "The total pressure variable.");
  return params;
}

BVFVMultiPhaseSaturationDarcy::BVFVMultiPhaseSaturationDarcy(const InputParameters & parameters)
  : BVTwoPointFluxApproximationBase(parameters),
    _phase(getParam<MooseEnum>("phase").getEnum<PhaseEnum>()),
    _p_var(dynamic_cast<const MooseVariableFV<Real> *>(getFieldVar("total_pressure", 0))),
    _lambda_w(getADMaterialProperty<Real>("fluid_mobility_w")),
    _lambda_w_neighbor(getNeighborADMaterialProperty<Real>("fluid_mobility_w")),
    _lambda_n(getADMaterialProperty<Real>("fluid_mobility_n")),
    _lambda_n_neighbor(getNeighborADMaterialProperty<Real>("fluid_mobility_n")),
    _dpc(getADMaterialProperty<Real>("capillary_pressure_derivative")),
    _dpc_neighbor(getNeighborADMaterialProperty<Real>("capillary_pressure_derivative"))
{
}

ADReal
BVFVMultiPhaseSaturationDarcy::computeQpResidual()
{
  // Total mobility
  ADReal lambda_T = _lambda_w[_qp] + _lambda_n[_qp];
  ADReal lambda_T_neighbor = _lambda_w_neighbor[_qp] + _lambda_n_neighbor[_qp];

  // Total velocity
  ADRealVectorValue u = diffusiveFlux(lambda_T, lambda_T_neighbor, (*_p_var));

  // Advective flux
  ADRealVectorValue u_adv = ADRealVectorValue();
  switch (_phase)
  {

    case PhaseEnum::WETTING:
      u_adv = advectiveFlux(_lambda_w[_qp] / (_lambda_w[_qp] + _lambda_n[_qp]), _lambda_w_neighbor[_qp] / (_lambda_w_neighbor[_qp] + _lambda_n_neighbor[_qp]), u);
      break;
    case PhaseEnum::NON_WETTING:
      u_adv = advectiveFlux(_lambda_n[_qp] / (_lambda_w[_qp] + _lambda_n[_qp]), _lambda_n_neighbor[_qp] / (_lambda_w_neighbor[_qp] + _lambda_n_neighbor[_qp]), u);
      break;
    default:
      mooseError("Unknow phase!");
  }

  // Non-linear diffusive flux
  ADReal D_diff = _lambda_w[_qp] * _lambda_n[_qp] / (_lambda_w[_qp] + _lambda_n[_qp]) * _dpc[_qp];
  ADReal D_diff_neighbor = _lambda_w_neighbor[_qp] * _lambda_n_neighbor[_qp] / (_lambda_w_neighbor[_qp] + _lambda_n_neighbor[_qp]) * _dpc_neighbor[_qp];
  // Diffusive flux
  ADRealVectorValue u_diff = diffusiveFlux(D_diff, D_diff_neighbor, _var);

  return (u_adv - u_diff) * (*_face_info).normal();
}