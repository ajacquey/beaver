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

#include "BVFaultStressAuxBase.h"
#include "Assembly.h"

InputParameters
BVFaultStressAuxBase::validParams()
{
  InputParameters params = BVStressAuxBase::validParams();
  params.addClassDescription("Base class for outputting fault stress.");
  params.addParam<RealVectorValue>("normal", "The vector normal to the fault plane.");
  return params;
}

BVFaultStressAuxBase::BVFaultStressAuxBase(const InputParameters & parameters)
  : BVStressAuxBase(parameters), _normals(_assembly.normals())
{
  if (isParamValid("normal") && (!_bnd))
  {
    _normal = getParam<RealVectorValue>("normal");
    if (_normal.norm() != 1.0)
      _normal /= _normal.norm();
  }
  else if (!isParamValid("normal") && (!_bnd))
    mooseError("You need to provide either a boundary or a normal vector for this AuxKernel!\n");
}

RealVectorValue
BVFaultStressAuxBase::computeNormalVector()
{
  if (!_bnd)
    return _normal;
  else
    return _normals[_qp];
}

ADRealVectorValue
BVFaultStressAuxBase::computeFaultTraction()
{
  RealVectorValue normal = computeNormalVector();

  return _stress[_qp] * normal;
}