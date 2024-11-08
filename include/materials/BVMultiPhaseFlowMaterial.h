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

#include "Material.h"

class BVMultiPhaseFlowMaterial : public Material
{
public:
  static InputParameters validParams();
  BVMultiPhaseFlowMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const ADMaterialProperty<Real> & _permeability;
  const ADMaterialProperty<Real> & _kr_w;
  const ADMaterialProperty<Real> & _kr_n;
  const ADMaterialProperty<Real> & _viscosity_w;
  const ADMaterialProperty<Real> & _viscosity_n;
  const ADMaterialProperty<Real> & _dpc;

  ADMaterialProperty<Real> & _lambda;
  ADMaterialProperty<Real> & _f_w;
  ADMaterialProperty<Real> & _f_n;
  ADMaterialProperty<Real> & _D;
};
