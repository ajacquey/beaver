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

class BVPoroMechanicsMaterial : public Material
{
public:
  static InputParameters validParams();
  BVPoroMechanicsMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  // Poroelastic parameters
  const Real _biot_coefficient;

  // Strain increment
  const ADMaterialProperty<RankTwoTensor> & _strain_increment;

  // Bulk modulus
  const ADMaterialProperty<Real> & _K;

  // Porosity
  const ADMaterialProperty<Real> & _porosity;

  // Poroelastic properties
  ADMaterialProperty<Real> & _biot;
  ADMaterialProperty<Real> & _porous_storage;
  ADMaterialProperty<Real> & _poromech;
};