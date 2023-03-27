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

#pragma once

#include "Material.h"
#include "SinglePhaseFluidProperties.h"

class BVFluidProperties : public Material
{
public:
  static InputParameters validParams();
  BVFluidProperties(const InputParameters & parameters);
  //   std::string phase_ext();

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  const ADVariableValue & _pf;
  const ADVariableValue & _temp;

  //   const enum class PhaseEnum { WETTING, NON_WETTING, SINGLE } _phase;
  //   const std::string _ext;

  const SinglePhaseFluidProperties & _fp;

  ADMaterialProperty<Real> & _density;
  ADMaterialProperty<Real> & _viscosity;
};