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

#include "BVFVFluxKernelBase.h"

InputParameters
BVFVFluxKernelBase::validParams()
{
  InputParameters params = FVFluxKernel::validParams();
  params.set<unsigned short>("ghost_layers") = 2;
  return params;
}

BVFVFluxKernelBase::BVFVFluxKernelBase(const InputParameters & params) : FVFluxKernel(params) {}

ADReal
BVFVFluxKernelBase::transmissibility(const ADReal & coeff_elem, const ADReal & coeff_neighbor) const
{
  ADReal coeff;

  // If we are on the boundary, we just use the boundary values (which depend on how the diffusion
  // coefficient is constructed)
  if (onBoundary(*_face_info))
    coeff = coeff_elem;
  // If we are on internal faces, we interpolate the diffusivity as usual
  else
  {
    // If the diffusivities are zero, then we can early return 0 (and avoid warnings if we
    // have a harmonic interpolation)
    if (!coeff_elem.value() && !coeff_neighbor.value())
      return 0;

    interpolate(
        Moose::FV::InterpMethod::Average, coeff, coeff_elem, coeff_neighbor, *_face_info, true);
  }

  return coeff;
}

ADRealVectorValue
BVFVFluxKernelBase::diffusiveFlux(const ADReal & mobility_elem,
                                  const ADReal & mobility_neighbor,
                                  const MooseVariableFV<Real> & fv_var) const
{
  auto T = transmissibility(mobility_elem, mobility_neighbor);

  return -T * fv_var.adGradSln(*_face_info,
                               determineState(),
                               fv_var.faceInterpolationMethod() ==
                                   Moose::FV::InterpMethod::SkewCorrectedAverage);
}

ADRealVectorValue
BVFVFluxKernelBase::advectiveFluxVariable(const ADRealVectorValue & vel) const
{
  const bool elem_is_upwind = vel * (*_face_info).normal() >= 0;
  const auto face =
      makeFace(*_face_info,
               Moose::FV::limiterType(Moose::FV::InterpMethod::Upwind),
               elem_is_upwind,
               _var.faceInterpolationMethod() == Moose::FV::InterpMethod::SkewCorrectedAverage);
  ADReal u_interface = _var(face, determineState());

  return vel * u_interface;
}

ADRealVectorValue
BVFVFluxKernelBase::advectiveFluxMaterial(const ADReal & mat_elem,
                                          const ADReal & mat_neighbor,
                                          const ADRealVectorValue & vel) const
{
  // If we are on internal faces, we interpolate the advected material as usual
  ADReal mat_adv;
  if (onBoundary(*_face_info))
    mat_adv = mat_elem;
  else
    interpolate(Moose::FV::InterpMethod::Upwind,
                mat_adv,
                mat_elem,
                mat_neighbor,
                vel,
                *_face_info,
                _var.faceInterpolationMethod() == Moose::FV::InterpMethod::SkewCorrectedAverage);

  return mat_adv * vel;
}
