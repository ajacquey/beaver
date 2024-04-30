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
  params.addClassDescription("Kernel for the divergence of Darcy's velocity for multi phase flow.");
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
    _lambda(getADMaterialProperty<Real>("fluid_mobility")),
    _lambda_neighbor(getNeighborADMaterialProperty<Real>("fluid_mobility")),
    _f_w(getADMaterialProperty<Real>("fractional_flow_w")),
    _f_w_neighbor(getNeighborADMaterialProperty<Real>("fractional_flow_w")),
    _f_n(getADMaterialProperty<Real>("fractional_flow_n")),
    _f_n_neighbor(getNeighborADMaterialProperty<Real>("fractional_flow_n")),
    _D(getADMaterialProperty<Real>("diffusivity_saturation")),
    _D_neighbor(getNeighborADMaterialProperty<Real>("diffusivity_saturation"))
{
}

ADReal
BVFVMultiPhaseSaturationDarcy::computeQpResidual()
{
  // Total velocity
  ADRealVectorValue u = diffusiveFlux(_lambda[_qp], _lambda_neighbor[_qp], (*_p_var));

  // Advective flux
  ADRealVectorValue u_adv = ADRealVectorValue();
  switch (_phase)
  {
    case PhaseEnum::WETTING:
      u_adv = advectiveFluxMaterial(_f_w[_qp], _f_w_neighbor[_qp], u);
      break;
    case PhaseEnum::NON_WETTING:
      u_adv = advectiveFluxMaterial(_f_n[_qp], _f_n_neighbor[_qp], u);
      break;
    default:
      mooseError("Unknow phase!");
  }

  // Diffusive flux
  ADRealVectorValue u_diff = diffusiveFlux(_D[_qp], _D_neighbor[_qp], _var);

  return (u_adv - u_diff) * (*_face_info).normal();
}
