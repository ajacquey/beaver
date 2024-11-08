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

class BVSinglePhaseFlowSoluteMaterial : public Material
{
public:
  static InputParameters validParams();
  BVSinglePhaseFlowSoluteMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const ADMaterialProperty<Real> & _porosity;
  const ADMaterialProperty<Real> & _dispersion;
  ADMaterialProperty<Real> & _lambda_c;
};
