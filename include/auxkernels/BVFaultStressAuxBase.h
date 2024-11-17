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

#pragma once

#include "AuxKernel.h"

class BVFaultStressAuxBase : public AuxKernel
{
public:
  static InputParameters validParams();
  BVFaultStressAuxBase(const InputParameters & parameters);

protected:
  const MooseArray<Point> & _normals;
  const ADMaterialProperty<RealVectorValue> & _traction;
};