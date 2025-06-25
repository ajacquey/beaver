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

#include "BVFaultStressAuxBase.h"
#include "Assembly.h"

InputParameters
BVFaultStressAuxBase::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Base class for outputting fault stress.");
  params.addParam<RealVectorValue>("normal", "The vector normal to the fault plane.");
  return params;
}

BVFaultStressAuxBase::BVFaultStressAuxBase(const InputParameters & parameters)
  : AuxKernel(parameters),
    _normals(_assembly.normals()),
    _traction(getADMaterialProperty<RealVectorValue>("traction_global"))
{
  if (!_bnd)
    mooseError("You need to provide a boundary for this AuxKernel!\n");
}