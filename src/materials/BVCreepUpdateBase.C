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

#include "BVCreepUpdateBase.h"
#include "BVElasticityTensorTools.h"

InputParameters
BVCreepUpdateBase::validParams()
{
  InputParameters params = BVInelasticUpdateBase::validParams();
  params.addClassDescription("Base material for computing a creep stress update.");
  params.addParam<std::string>(
      "base_name",
      "Optional parameter that defines a prefix for all material "
      "properties related to this stress update model. This allows for "
      "multiple models of the same type to be used without naming conflicts.");
  return params;
}

BVCreepUpdateBase::BVCreepUpdateBase(const InputParameters & parameters)
  : BVInelasticUpdateBase(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _creep_strain_incr(declareADProperty<RankTwoTensor>(_base_name + "creep_strain_increment"))
{
}

void
BVCreepUpdateBase::inelasticUpdate(ADRankTwoTensor & stress, const RankFourTensor & Cijkl)
{
  // Here we do an iterative update with a single variable being the scalar creep strain increment
  // We are trying to find the zero of the function F which is defined as:
  // $F\left(\Delta \\gamma\right) = \dot{gamma} \Delta t - \Delta \gamma$
  // $\dot{\gamma}$: scalar creep strain rate

  // Trial stress
  _stress_tr = stress;
  // Trial effective stress
  _eqv_stress_tr = std::sqrt(1.5) * _stress_tr.deviatoric().L2norm();
  // Shear modulus
  _G = BVElasticityTensorTools::getIsotropicShearModulus(Cijkl);

  // Initialize creep strain increment
  _creep_strain_incr[_qp].zero();

  // Pre return map calculations (model specific)
  preReturnMap();

  // Viscoelastic update
  ADReal eqv_strain_incr = returnMap();

  // Update quantities
  _creep_strain_incr[_qp] = reformPlasticStrainTensor(eqv_strain_incr);
  stress -= 2.0 * _G * _creep_strain_incr[_qp];
  postReturnMap(eqv_strain_incr);
}

ADReal
BVCreepUpdateBase::returnMap()
{
  // Initialize scalar effective strain incr
  ADReal eqv_strain_incr = 0.0;

  // Initial residual
  ADReal res_ini = residual(eqv_strain_incr);

  ADReal res = res_ini;
  ADReal jac = jacobian(eqv_strain_incr);

  // Newton loop
  for (unsigned int iter = 0; iter < _max_its; ++iter)
  {
    eqv_strain_incr -= res / jac;

    res = residual(eqv_strain_incr);
    jac = jacobian(eqv_strain_incr);

    // Convergence check
    if ((std::abs(res) <= _abs_tol) || (std::abs(res / res_ini) <= _rel_tol))
      return eqv_strain_incr;
  }
  throw MooseException("BVCreepUpdateBase: maximum number of iterations exceeded in 'returnMap'!");
}

ADReal
BVCreepUpdateBase::residual(const ADReal & eqv_strain_incr)
{
  return creepRate(eqv_strain_incr) * _dt - eqv_strain_incr;
}

ADReal
BVCreepUpdateBase::jacobian(const ADReal & eqv_strain_incr)
{
  return creepRateDerivative(eqv_strain_incr) * _dt - 1.0;
}

ADRankTwoTensor
BVCreepUpdateBase::reformPlasticStrainTensor(const ADReal & eqv_strain_incr)
{
  ADRankTwoTensor flow_dir =
      (_eqv_stress_tr != 0.0) ? _stress_tr.deviatoric() / _eqv_stress_tr : ADRankTwoTensor();

  return 1.5 * creepRate(eqv_strain_incr) * _dt * flow_dir;
}

void
BVCreepUpdateBase::preReturnMap()
{
}

void
BVCreepUpdateBase::postReturnMap(const ADReal & /*eqv_strain_incr*/)
{
}