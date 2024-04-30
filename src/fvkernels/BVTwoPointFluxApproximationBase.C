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

#include "BVTwoPointFluxApproximationBase.h"

InputParameters
BVTwoPointFluxApproximationBase::validParams()
{
  InputParameters params = FVFluxKernel::validParams();
  params.set<unsigned short>("ghost_layers") = 2;
  return params;
}

BVTwoPointFluxApproximationBase::BVTwoPointFluxApproximationBase(const InputParameters & params)
  : FVFluxKernel(params)
{
}

ADReal
BVTwoPointFluxApproximationBase::transmissibility(const ADReal & coeff_elem,
                                                  const ADReal & coeff_neighbor) const
{
  ADReal coeff;
  // If we are on internal faces, we interpolate the diffusivity as usual
  if (_var.isInternalFace(*_face_info))
  {
    // If the diffusivities are zero, then we can early return 0 (and avoid warnings if we
    // have a harmonic interpolation)
    if (!coeff_elem.value() && !coeff_neighbor.value())
      return 0;

    interpolate(Moose::FV::InterpMethod::HarmonicAverage,
                coeff,
                coeff_elem,
                coeff_neighbor,
                *_face_info,
                true);
  }
  // Else we just use the boundary values (which depend on how the diffusion
  // coefficient is constructed)
  else
    coeff = coeff_elem;

  return coeff;
}

ADRealVectorValue
BVTwoPointFluxApproximationBase::diffusiveFlux(const ADReal & mobility_elem,
                                               const ADReal & mobility_neighbor,
                                               const MooseVariableFV<Real> & fv_var) const
{
  auto T = transmissibility(mobility_elem, mobility_neighbor);

  return -T * fv_var.adGradSln(*_face_info,
                               determineState(),
                               _var.faceInterpolationMethod() ==
                                   Moose::FV::InterpMethod::SkewCorrectedAverage);
}

ADRealVectorValue
BVTwoPointFluxApproximationBase::advectiveFluxVariable(const ADRealVectorValue & vel) const
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
BVTwoPointFluxApproximationBase::advectiveFluxMaterial(const ADReal & mat_elem,
                                                       const ADReal & mat_neighbor,
                                                       const ADRealVectorValue & vel) const
{
  // If we are on internal faces, we interpolate the advected material as usual
  ADReal mat_adv;
  if (_var.isInternalFace(*_face_info))
    interpolate(
        Moose::FV::InterpMethod::Upwind, mat_adv, mat_elem, mat_neighbor, vel, *_face_info, true);
  else
    mat_adv = mat_elem;

  return mat_adv * vel;
}
