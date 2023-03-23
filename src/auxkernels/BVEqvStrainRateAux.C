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

#include "BVEqvStrainRateAux.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("BeaverApp", BVEqvStrainRateAux);

InputParameters
BVEqvStrainRateAux::validParams()
{
  InputParameters params = BVStrainAuxBase::validParams();
  params.addClassDescription("Class for outputting the equivalent strain rate.");
  return params;
}

BVEqvStrainRateAux::BVEqvStrainRateAux(const InputParameters & parameters)
  : BVStrainAuxBase(parameters)
{
}

Real
BVEqvStrainRateAux::computeValue()
{
  ADRankTwoTensor strain_rate_dev = _strain_increment[_qp].deviatoric() / _dt;
  return std::sqrt(2.0 / 3.0) * MetaPhysicL::raw_value(strain_rate_dev.L2norm());
}