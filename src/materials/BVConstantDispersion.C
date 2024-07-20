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

#include "BVConstantDispersion.h"

registerMooseObject("BeaverApp", BVConstantDispersion);

InputParameters
BVConstantDispersion::validParams()
{
  InputParameters params = BVDispersionBase::validParams();
  params.addClassDescription("Computes a constant dispersion value for the porous medium.");
  params.addRequiredRangeCheckedParam<Real>(
      "dispersion", "dispersion>=0", "The effective diffusion coefficient.");
  return params;
}

BVConstantDispersion::BVConstantDispersion(const InputParameters & parameters)
  : BVDispersionBase(parameters), _dispersion0(getParam<Real>("dispersion"))
{
}

void
BVConstantDispersion::computeQpProperties()
{
  _dispersion[_qp] = _dispersion0;
}