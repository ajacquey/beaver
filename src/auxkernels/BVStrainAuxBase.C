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

#include "BVStrainAuxBase.h"

InputParameters
BVStrainAuxBase::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Base class for outputting strain values.");
  return params;
}

BVStrainAuxBase::BVStrainAuxBase(const InputParameters & parameters)
  : AuxKernel(parameters),
    _strain_increment(getADMaterialProperty<RankTwoTensor>("strain_increment"))
{
}