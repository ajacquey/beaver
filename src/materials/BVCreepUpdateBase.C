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
BVCreepUpdateBase::inelasticUpdate(ADRankTwoTensor & stress,
                                   const RankFourTensor & Cijkl,
                                   ADRankTwoTensor & strain_increment)
{
  // Here we do an iterative update with a single variable being the scalar effective stress
  // We are trying to find the zero of the function F which is defined as:
  // $F\left(\sigma_{e}\right) = \sigma_{e}^{tr} - \sigme_{e} - 3 * \eta * \dot{gamma}$
  // $\dot{\gamma}$: scalar creep strain rate
  // $\sigma_{e}$: scalar effective stress
  // $\eta$: the viscosity
  // flow rule: $\dot{\gamma} = \frac{\sigma_{e}}{\eta}$

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
  ADReal gamma = returnMap();

  // Update quantities
  _creep_strain_incr[_qp] = reformPlasticStrainTensor(gamma);
  strain_increment -= _creep_strain_incr[_qp];
  stress -= 2.0 * _G * _creep_strain_incr[_qp];
  postReturnMap(gamma);
}

ADReal
BVCreepUpdateBase::returnMap()
{
  // Initialize scalar effective stress
  ADReal eqv_stress = _eqv_stress_tr;

  // Initial residual
  ADReal res_ini = residual(eqv_stress);

  ADReal res = res_ini;
  ADReal jac = jacobian(eqv_stress);

  // Newton loop
  for (unsigned int iter = 0; iter < _max_its; ++iter)
  {
    eqv_stress -= res / jac;

    res = residual(eqv_stress);
    jac = jacobian(eqv_stress);

    // Convergence check
    if ((std::abs(res) <= _abs_tol) || (std::abs(res / res_ini) <= _rel_tol))
      return creepRate(eqv_stress);
  }
  throw MooseException("BVCreepUpdateBase: maximum number of iterations exceeded in 'returnMap'!");
}

ADReal
BVCreepUpdateBase::residual(const ADReal & eqv_stress)
{
  return eqv_stress - _eqv_stress_tr + 3.0 * _G * creepRate(eqv_stress) * _dt;
}

ADReal
BVCreepUpdateBase::jacobian(const ADReal & eqv_stress)
{
  return 1.0 + 3.0 * _G * creepRateDerivative(eqv_stress) * _dt;
}

ADRankTwoTensor
BVCreepUpdateBase::reformPlasticStrainTensor(const ADReal & gamma)
{
  ADRankTwoTensor flow_dir =
      (_eqv_stress_tr != 0.0) ? _stress_tr.deviatoric() / _eqv_stress_tr : ADRankTwoTensor();

  return 1.5 * gamma * _dt * flow_dir;
}

void 
BVCreepUpdateBase::preReturnMap()
{

}

void
BVCreepUpdateBase::postReturnMap(const ADReal & /*gamma*/)
{

}