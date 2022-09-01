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

#include "BVPorosityBase.h"

InputParameters
BVPorosityBase::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Base class for computing the porosity of a porous material for "
                             "single and multi phase flow.");
  return params;
}

BVPorosityBase::BVPorosityBase(const InputParameters & parameters)
  : Material(parameters), _porosity(declareADProperty<Real>("porosity"))
{
}