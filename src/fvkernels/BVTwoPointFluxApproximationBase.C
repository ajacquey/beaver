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
BVTwoPointFluxApproximationBase::transmissibility(const ADReal & mobility,
                                                  const ADReal & mobility_neighbor) const
{
  ADReal T;
  // If we are on internal faces, we interpolate the mobility as usual
  if (_var.isInternalFace(*_face_info))
  {
    // If the mobilities are zero, then we can early return 0 (and avoid warnings if we
    // have a harmonic interpolation)
    if (!mobility.value() && !mobility_neighbor.value())
      return 0;

    interpolate(Moose::FV::InterpMethod::HarmonicAverage, T, mobility, mobility_neighbor, *_face_info, true);
  }
  // Else we just use the boundary values (which depend on how the diffusion
  // coefficient is constructed)
  else
    T = mobility;

  return T;
}

ADRealVectorValue
BVTwoPointFluxApproximationBase::diffusiveFlux(const ADReal & mobility,
                                               const ADReal & mobility_neighbor,
                                               const MooseVariableFV<Real> & fv_var) const
{
  auto T = transmissibility(mobility, mobility_neighbor);

  return -T * fv_var.adGradSln(*_face_info, determineState(), _var.faceInterpolationMethod() == Moose::FV::InterpMethod::SkewCorrectedAverage);
}

ADRealVectorValue
BVTwoPointFluxApproximationBase::advectiveFlux(const ADReal & qty,
                                               const ADReal & qty_neighbor,
                                               const ADRealVectorValue & vel) const
{
  // if (v_interface >= 0.0)
  //   return u;
  // else
  //   return u_neighbor;
  ADReal qty_adv = 0.0;
  interpolate(Moose::FV::InterpMethod::Upwind, qty_adv, qty, qty_neighbor, vel, *_face_info, true);

  return qty_adv * vel;
}