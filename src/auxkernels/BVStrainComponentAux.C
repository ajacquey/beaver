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

#include "BVStrainComponentAux.h"

registerMooseObject("BeaverApp", BVStrainComponentAux);

InputParameters
BVStrainComponentAux::validParams()
{
  InputParameters params = BVStrainAuxBase::validParams();
  params.addClassDescription("Class for outputting components of the strain tensor.");
  MooseEnum component("x y z");
  params.addRequiredParam<MooseEnum>(
      "index_i", component, "The index i of ij for the strain tensor.");
  params.addRequiredParam<MooseEnum>(
      "index_j", component, "The index j of ij for the strain tensor.");
  return params;
}

BVStrainComponentAux::BVStrainComponentAux(const InputParameters & parameters)
  : BVStrainAuxBase(parameters),
    _u_old(uOld()),
    _i(getParam<MooseEnum>("index_i")),
    _j(getParam<MooseEnum>("index_j"))
{
}

Real
BVStrainComponentAux::computeValue()
{
  return _u_old[_qp] + MetaPhysicL::raw_value(_strain_increment[_qp](_i, _j));
}