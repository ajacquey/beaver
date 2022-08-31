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

#include "BVStrainComponentAux.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("BeaverApp", BVStrainComponentAux);

InputParameters
BVStrainComponentAux::validParams()
{
  InputParameters params = BVStrainAuxBase::validParams();
  params.addClassDescription("Class for outputting components of the strain tensor.");
  params.addRequiredRangeCheckedParam<unsigned int>(
      "index_i",
      "index_i >= 0 & index_i <= 2",
      "The index i of ij for the strain tensor (0, 1, 2)");
  params.addRequiredRangeCheckedParam<unsigned int>(
      "index_j",
      "index_j >= 0 & index_j <= 2",
      "The index j of ij for the strain tensor (0, 1, 2)");
  return params;
}

BVStrainComponentAux::BVStrainComponentAux(const InputParameters & parameters)
  : BVStrainAuxBase(parameters),
    _u_old(uOld()),
    _i(getParam<unsigned int>("index_i")),
    _j(getParam<unsigned int>("index_j"))
{
}

Real
BVStrainComponentAux::computeValue()
{
  return _u_old[_qp] + MetaPhysicL::raw_value(_strain_increment[_qp](_i, _j));
}