/******************************************************************************/
/*                            This file is part of                            */
/*                       BEAVER, a MOOSE-based application                    */
/*       Multiphase Flow Poromechanics for Induced Seismicity Problems        */
/*                                                                            */
/*                  Copyright (C) 2024 by Antoine B. Jacquey                  */
/*                  Tufts University / Polytechnique Montreal                 */
/*                                                                            */
/*            Licensed under GNU Lesser General Public License v2.1           */
/*                       please see LICENSE for details                       */
/*                 or http://www.gnu.org/licenses/lgpl.html                   */
/******************************************************************************/

#pragma once

#include "Material.h"

class BVFrictionUpdateBase : public Material
{
public:
  static InputParameters validParams();
  BVFrictionUpdateBase(const InputParameters & parameters);
  void setQp(unsigned int qp);
  virtual void frictionUpdate(ADRealVectorValue & traction, const RankTwoTensor & K);
  void resetQpProperties() final {}
  void resetProperties() final {}

protected:
  virtual ADReal frictionalStrength(const ADReal & delta_dot) = 0;
  virtual ADReal frictionalStrengthDeriv(const ADReal & delta_dot) = 0;
  virtual ADReal returnMap();
  virtual ADReal residual(const ADReal & delta_dot);
  virtual ADReal jacobian(const ADReal & delta_dot);
  virtual ADRealVectorValue reformSlipVector(const ADReal & delta_dot);
  virtual void preReturnMap();
  virtual void postReturnMap(const ADReal & delta_dot);

  // Return map parameters
  const Real _abs_tol;
  const Real _rel_tol;
  unsigned int _max_its;

  // Friction update parameters
  ADRealVectorValue _traction_tr;
  ADReal _sigma_tr;
  ADReal _tau_tr;
  Real _kn;
  Real _ks;
};