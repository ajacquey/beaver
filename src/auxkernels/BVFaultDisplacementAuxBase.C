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

#include "BVFaultDisplacementAuxBase.h"
#include "Assembly.h"

InputParameters
BVFaultDisplacementAuxBase::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Base class for outputting fault displacement discontinuity.");
  return params;
}

BVFaultDisplacementAuxBase::BVFaultDisplacementAuxBase(const InputParameters & parameters)
  : AuxKernel(parameters),
    _normals(_assembly.normals()),
    _displacement_jump_incr(
        getADMaterialProperty<RealVectorValue>("displacement_jump_increment_global"))
{
  if (!_bnd)
    mooseError("You need to provide a boundary for this AuxKernel!\n");
}