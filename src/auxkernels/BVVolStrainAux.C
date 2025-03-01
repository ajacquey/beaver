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

#include "BVVolStrainAux.h"

registerMooseObject("BeaverApp", BVVolStrainAux);

InputParameters
BVVolStrainAux::validParams()
{
  InputParameters params = BVStrainAuxBase::validParams();
  params.addClassDescription("Class for outputting the volumetric strain.");
  return params;
}

BVVolStrainAux::BVVolStrainAux(const InputParameters & parameters)
  : BVStrainAuxBase(parameters), _u_old(uOld())
{
}

Real
BVVolStrainAux::computeValue()
{
  return _u_old[_qp] + MetaPhysicL::raw_value(_strain_increment[_qp].trace());
}