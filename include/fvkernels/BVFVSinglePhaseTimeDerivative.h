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

#include "FVTimeKernel.h"

class BVFVSinglePhaseTimeDerivative : public FVTimeKernel
{
public:
  static InputParameters validParams();
  BVFVSinglePhaseTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADMaterialProperty<Real> & _porosity;
  const ADMaterialProperty<Real> & _density;
  const MaterialProperty<Real> & _density_old;
  const bool _coupled_mech;
  const ADMaterialProperty<Real> * _porous_storage;
  const ADMaterialProperty<Real> * _poromech;
};