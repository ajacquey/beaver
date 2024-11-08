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

#pragma once

#include "FVFluxKernel.h"

class BVFVFluxKernelBase : public FVFluxKernel
{
public:
  static InputParameters validParams();
  BVFVFluxKernelBase(const InputParameters & params);

protected:
  virtual ADReal transmissibility(const ADReal & coeff_elem, const ADReal & coeff_neighbor) const;
  virtual ADRealVectorValue diffusiveFlux(const ADReal & mobility_elem,
                                          const ADReal & mobility_neighbor,
                                          const MooseVariableFV<Real> & fv_var) const;
  virtual ADRealVectorValue advectiveFluxVariable(const ADRealVectorValue & vel) const;
  virtual ADRealVectorValue advectiveFluxMaterial(const ADReal & qty_elem,
                                                  const ADReal & qty_neighbor,
                                                  const ADRealVectorValue & vel) const;
};
