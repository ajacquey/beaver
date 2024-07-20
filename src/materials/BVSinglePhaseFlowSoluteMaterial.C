/******************************************************************************/
/*                            This file is part of                            */
/*                       BEAVER, a MOOSE-based application                    */
/*       Multiphase Flow Poromechanics for Induced Seismicity Problems        */
/*                                                                            */
/*                  Copyright (C) 2020 by Antoine B. Jacquey                  */
/*                    Massachusetts Institute of Technology                   */
/*                                                                            */
/*            Licensed under GNU Lesser General Public License v2.1           */
/*                       please see LICENSE for details                       */
/*                 or http://www.gnu.org/licenses/lgpl.html                   */
/******************************************************************************/

#include "BVSinglePhaseFlowSoluteMaterial.h"

registerMooseObject("BeaverApp", BVSinglePhaseFlowSoluteMaterial);

InputParameters
BVSinglePhaseFlowSoluteMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "Computes properties for single phase fluid flow and solute transport in a porous material.");
  return params;
}

BVSinglePhaseFlowSoluteMaterial::BVSinglePhaseFlowSoluteMaterial(const InputParameters & parameters)
  : Material(parameters),
    _porosity(getADMaterialProperty<Real>("porosity")),
    _dispersion(getADMaterialProperty<Real>("dispersion")),
    _lambda_c(declareADProperty<Real>("solute_mobility"))
{
}

void
BVSinglePhaseFlowSoluteMaterial::computeQpProperties()
{
  // Solute mobility
  _lambda_c[_qp] = _porosity[_qp] * _dispersion[_qp];
}
