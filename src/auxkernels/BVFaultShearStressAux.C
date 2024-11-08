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

#include "BVFaultShearStressAux.h"

registerMooseObject("BeaverApp", BVFaultShearStressAux);

InputParameters
BVFaultShearStressAux::validParams()
{
  InputParameters params = BVFaultStressAuxBase::validParams();
  params.addClassDescription("Calculates the shear stress acting on the fault.");
  return params;
}

BVFaultShearStressAux::BVFaultShearStressAux(const InputParameters & parameters)
  : BVFaultStressAuxBase(parameters)
{
}

Real
BVFaultShearStressAux::computeValue()
{
  RealVectorValue normal = computeNormalVector();
  ADRealVectorValue fault_traction = computeFaultTraction();
  ADRealVectorValue shear_traction = fault_traction - (fault_traction * normal) * normal;
  return MetaPhysicL::raw_value(shear_traction.norm());
}