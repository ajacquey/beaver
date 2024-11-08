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

#include "BVFVMultiPhasePressureDarcy.h"

registerADMooseObject("BeaverApp", BVFVMultiPhasePressureDarcy);

InputParameters
BVFVMultiPhasePressureDarcy::validParams()
{
  InputParameters params = BVFVFluxKernelBase::validParams();
  params.addClassDescription(
      "Kernel for the divergence of the total velocity for multi phase flow.");
  params.set<unsigned short>("ghost_layers") = 2;
  return params;
}

BVFVMultiPhasePressureDarcy::BVFVMultiPhasePressureDarcy(const InputParameters & parameters)
  : BVFVFluxKernelBase(parameters),
    _lambda(getADMaterialProperty<Real>("fluid_mobility")),
    _lambda_neighbor(getNeighborADMaterialProperty<Real>("fluid_mobility"))
{
  if ((_var.faceInterpolationMethod() == Moose::FV::InterpMethod::SkewCorrectedAverage) &&
      (_tid == 0))
    adjustRMGhostLayers(std::max((unsigned short)(3), _pars.get<unsigned short>("ghost_layers")));
}

ADReal
BVFVMultiPhasePressureDarcy::computeQpResidual()
{
  // Diffusive flux
  ADRealVectorValue u = diffusiveFlux(_lambda[_qp], _lambda_neighbor[_qp], _var);

  return u * (*_face_info).normal();
}
