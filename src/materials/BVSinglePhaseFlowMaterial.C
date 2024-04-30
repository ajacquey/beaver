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

#include "BVSinglePhaseFlowMaterial.h"

registerMooseObject("BeaverApp", BVSinglePhaseFlowMaterial);

InputParameters
BVSinglePhaseFlowMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "Computes properties for single phase fluid flow in a porous material.");
  return params;
}

BVSinglePhaseFlowMaterial::BVSinglePhaseFlowMaterial(const InputParameters & parameters)
  : Material(parameters),
    _permeability(getADMaterialProperty<Real>("permeability")),
    _viscosity(getADMaterialProperty<Real>("viscosity")),
    _lambda(declareADProperty<Real>("fluid_mobility"))
{
}

void
BVSinglePhaseFlowMaterial::computeQpProperties()
{
  // Fluid mobility
  _lambda[_qp] = _permeability[_qp] / _viscosity[_qp];
}
