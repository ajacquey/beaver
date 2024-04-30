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

#include "BVMultiPhaseFlowMaterial.h"

registerMooseObject("BeaverApp", BVMultiPhaseFlowMaterial);

InputParameters
BVMultiPhaseFlowMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Computes properties for multiphase fluid flow in a porous material.");
  return params;
}

BVMultiPhaseFlowMaterial::BVMultiPhaseFlowMaterial(const InputParameters & parameters)
  : Material(parameters),
    _permeability(getADMaterialProperty<Real>("permeability")),
    _kr_w(getADMaterialProperty<Real>("relative_permeability_w")),
    _kr_n(getADMaterialProperty<Real>("relative_permeability_n")),
    _viscosity_w(getADMaterialProperty<Real>("viscosity_w")),
    _viscosity_n(getADMaterialProperty<Real>("viscosity_n")),
    _dpc(getADMaterialProperty<Real>("capillary_pressure_derivative")),
    _lambda(declareADProperty<Real>("fluid_mobility")),
    _f_w(declareADProperty<Real>("fractional_flow_w")),
    _f_n(declareADProperty<Real>("fractional_flow_n")),
    _D(declareADProperty<Real>("diffusivity_saturation"))
{
}

void
BVMultiPhaseFlowMaterial::computeQpProperties()
{
  // Fluid mobilities
  ADReal lambda_w = _permeability[_qp] * _kr_w[_qp] / _viscosity_w[_qp];
  ADReal lambda_n = _permeability[_qp] * _kr_n[_qp] / _viscosity_n[_qp];
  _lambda[_qp] = lambda_w + lambda_n;

  // Fractional flow
  _f_w[_qp] = lambda_w / _lambda[_qp];
  _f_n[_qp] = lambda_n / _lambda[_qp];

  // Non-linear diffusivity
  _D[_qp] = lambda_w * lambda_n / _lambda[_qp] * _dpc[_qp];
}
