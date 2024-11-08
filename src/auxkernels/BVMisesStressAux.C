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

#include "BVMisesStressAux.h"

registerMooseObject("BeaverApp", BVMisesStressAux);

InputParameters
BVMisesStressAux::validParams()
{
  InputParameters params = BVStressAuxBase::validParams();
  params.addClassDescription("Class for outputting the Von Mises stress.");
  return params;
}

BVMisesStressAux::BVMisesStressAux(const InputParameters & parameters) : BVStressAuxBase(parameters)
{
}

Real
BVMisesStressAux::computeValue()
{
  ADRankTwoTensor stress_dev = _stress[_qp].deviatoric();
  return std::sqrt(1.5) * MetaPhysicL::raw_value(stress_dev.L2norm());
}