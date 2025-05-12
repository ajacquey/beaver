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

#include "BVFaultSlipRateAux.h"

registerMooseObject("BeaverApp", BVFaultSlipRateAux);

InputParameters
BVFaultSlipRateAux::validParams()
{
  InputParameters params = BVFaultDisplacementAuxBase::validParams();
  params.addClassDescription("Calculates the slip rate acting on the fault.");
  return params;
}

BVFaultSlipRateAux::BVFaultSlipRateAux(const InputParameters & parameters)
  : BVFaultDisplacementAuxBase(parameters)
{
}

Real
BVFaultSlipRateAux::computeValue()
{
  ADRealVectorValue shear_disp_disc_incr =
      _displacement_jump_incr[_qp] - (_displacement_jump_incr[_qp] * _normals[_qp]) * _normals[_qp];

  return MetaPhysicL::raw_value(shear_disp_disc_incr.norm()) / _dt;
}