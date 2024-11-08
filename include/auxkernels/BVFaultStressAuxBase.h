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

#include "BVStressAuxBase.h"

class BVFaultStressAuxBase : public BVStressAuxBase
{
public:
  static InputParameters validParams();
  BVFaultStressAuxBase(const InputParameters & parameters);

protected:
  virtual RealVectorValue computeNormalVector();
  virtual ADRealVectorValue computeFaultTraction();

  const MooseArray<Point> & _normals;
  RealVectorValue _normal;
};