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

#include "BVEqvStrainAux.h"

registerMooseObject("BeaverApp", BVEqvStrainAux);

InputParameters
BVEqvStrainAux::validParams()
{
  InputParameters params = BVStrainAuxBase::validParams();
  params.addClassDescription("Class for outputting the equivalent strain.");
  return params;
}

BVEqvStrainAux::BVEqvStrainAux(const InputParameters & parameters)
  : BVStrainAuxBase(parameters), _u_old(uOld())
{
}

Real
BVEqvStrainAux::computeValue()
{
  ADRankTwoTensor strain_incr_dev = _strain_increment[_qp].deviatoric();
  return _u_old[_qp] + std::sqrt(2.0 / 3.0) * MetaPhysicL::raw_value(strain_incr_dev.L2norm());
}