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
  params.addClassDescription(
      "Computes properties for multiphase fluid flow in a porous material.");
  return params;
}

BVMultiPhaseFlowMaterial::BVMultiPhaseFlowMaterial(const InputParameters & parameters)
  : Material(parameters),
    _permeability(getADMaterialProperty<Real>("permeability")),
    _kr_w(getADMaterialProperty<Real>("relative_permeability_w")),
    _kr_n(getADMaterialProperty<Real>("relative_permeability_n")),
    // _dkr_w(getADMaterialProperty<Real>("relative_permeability_w_derivative")),
    // _dkr_n(getADMaterialProperty<Real>("relative_permeability_n_derivative")),
    _viscosity_w(getADMaterialProperty<Real>("viscosity_w")),
    _viscosity_n(getADMaterialProperty<Real>("viscosity_n")),
    _lambda_w(declareADProperty<Real>("fluid_mobility_w")),
    _lambda_n(declareADProperty<Real>("fluid_mobility_n"))
    // _lambda(declareADProperty<Real>("fluid_mobility")),
    // _f_w(declareADProperty<Real>("fractional_flow_w")),
    // _df_w(declareADProperty<Real>("fractional_flow_w_derivative"))
{
}

void
BVMultiPhaseFlowMaterial::computeQpProperties()
{
  // Fluid mobilities
  _lambda_w[_qp] = _permeability[_qp] * _kr_w[_qp] / _viscosity_w[_qp];
  _lambda_n[_qp] = _permeability[_qp] * _kr_n[_qp] / _viscosity_n[_qp];
//   _lambda[_qp] = _lambda_w[_qp] + _lambda_n[_qp];

//   // Fractional flow
//   _f_w[_qp] = _lambda_w[_qp] / _lambda[_qp];

//   // Derivative
//   _df_w[_qp] = _viscosity_w[_qp] / _viscosity_n[_qp] * (_kr_n[_qp] * _dkr_w[_qp] - _kr_w[_qp] * _dkr_n[_qp]) / Utility::pow<2>(_kr_w[_qp] + _viscosity_w[_qp] / _viscosity_n[_qp] * _kr_n[_qp]);
}