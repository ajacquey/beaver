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

#include "BVVolStrainRateAux.h"

registerMooseObject("BeaverApp", BVVolStrainRateAux);

InputParameters
BVVolStrainRateAux::validParams()
{
  InputParameters params = BVStrainAuxBase::validParams();
  params.addClassDescription("Class for outputting the volumetric strain rate.");
  return params;
}

BVVolStrainRateAux::BVVolStrainRateAux(const InputParameters & parameters)
  : BVStrainAuxBase(parameters)
{
}

Real
BVVolStrainRateAux::computeValue()
{
  return MetaPhysicL::raw_value(_strain_increment[_qp].trace()) / _dt;
}