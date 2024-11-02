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

#include "BVTwoCreepUpdateBase.h"
#include "BVElasticityTensorTools.h"

InputParameters
BVTwoCreepUpdateBase::validParams()
{
  InputParameters params = BVInelasticUpdateBase::validParams();
  params.addClassDescription("Base material for computing multiple creep stress updates.");
  params.addParam<std::string>(
      "base_name",
      "Optional parameter that defines a prefix for all material "
      "properties related to this stress update model. This allows for "
      "multiple models of the same type to be used without naming conflicts.");
  return params;
}

BVTwoCreepUpdateBase::BVTwoCreepUpdateBase(const InputParameters & parameters)
  : BVInelasticUpdateBase(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _num_cm(2),
    _creep_strain_incr(declareADProperty<RankTwoTensor>(_base_name + "creep_strain_increment"))
{
}

void
BVTwoCreepUpdateBase::inelasticUpdate(ADRankTwoTensor & stress, const RankFourTensor & Cijkl)
{
  // Here we do an iterative update with a single variable being the scalar effective stress
  // We are trying to find the zero of the function F which is defined as:
  // $F\left(\sigma_{e}\right) = \sigma_{e}^{tr} - \sigme_{e} - 3 * \eta * \dot{gamma}$
  // $\dot{\gamma}$: scalar creep strain rate
  // $\sigma_{e}$: scalar effective stress
  // $\eta$: the viscosity
  // flow rule: $\dot{\gamma} = \frac{\sigma_{e}}{3 \eta}$

  // Trial stress
  _stress_tr = stress;
  // Trial effective stress
  _eqv_stress_tr = std::sqrt(1.5) * _stress_tr.deviatoric().L2norm();
  _avg_stress_tr = _stress_tr.trace();
  // Shear and bulk modulus
  _G = BVElasticityTensorTools::getIsotropicShearModulus(Cijkl);
  _K = BVElasticityTensorTools::getIsotropicBulkModulus(Cijkl);

  // Initialize creep strain increment
  _creep_strain_incr[_qp].zero();

  // Pre return map calculations (model specific)
  preReturnMap();

  // Viscoelastic update
  std::vector<ADReal> creep_strain_incr = returnMap();

  // Update quantities
  _creep_strain_incr[_qp] = reformPlasticStrainTensor(creep_strain_incr);
  stress -= 2.0 * _G * _creep_strain_incr[_qp];
  postReturnMap(creep_strain_incr);
}

std::vector<ADReal>
BVTwoCreepUpdateBase::returnMap()
{
  // Initialize scalar creep strain incr
  std::vector<ADReal> creep_strain_incr(_num_cm, 0.0);

  // Initial residual
  std::vector<ADReal> res_ini = residual(creep_strain_incr);

  std::vector<ADReal> res = res_ini;
  std::vector<std::vector<ADReal>> jac = jacobian(creep_strain_incr);

  // Newton loop
  for (unsigned int iter = 0; iter < _max_its; ++iter)
  {
    nrStep(res, jac, creep_strain_incr);

    res = residual(creep_strain_incr);
    jac = jacobian(creep_strain_incr);

    // Convergence check
    if ((norm(res) <= _abs_tol) || (norm(res) / norm(res_ini) <= _rel_tol))
      return creep_strain_incr;
  }
  throw MooseException(
      "BVTwoCreepUpdateBase: maximum number of iterations exceeded in 'returnMap'!");
}

void
BVTwoCreepUpdateBase::nrStep(const std::vector<ADReal> & res,
                             const std::vector<std::vector<ADReal>> & jac,
                             std::vector<ADReal> & creep_strain_incr)
{
  std::vector<ADReal> dx(_num_cm, 0.0);

  // Check determinant
  ADReal det = jac[0][0] * jac[1][1] - jac[0][1] * jac[1][0];

  if (det == 0.0)
    throw MooseException("BVTwoCreepUpdateBase: matrix is singular in 'returnMap!");

  creep_strain_incr[0] -= (jac[1][1] * res[0] - jac[0][1] * res[1]) / det;
  creep_strain_incr[1] -= (jac[0][0] * res[1] - jac[1][0] * res[0]) / det;

  return;
}

ADReal
BVTwoCreepUpdateBase::norm(const std::vector<ADReal> & vec)
{
  ADReal res = 0.0;
  for (const auto & r : vec)
    res += r * r;

  return std::sqrt(res);
}

std::vector<ADReal>
BVTwoCreepUpdateBase::residual(const std::vector<ADReal> & creep_strain_incr)
{
  std::vector<ADReal> res(_num_cm);
  for (unsigned int i = 0; i < _num_cm; ++i)
    res[i] = creepRate(creep_strain_incr, i) * _dt - creep_strain_incr[i];

  return res;
}

std::vector<std::vector<ADReal>>
BVTwoCreepUpdateBase::jacobian(const std::vector<ADReal> & creep_strain_incr)
{
  std::vector<std::vector<ADReal>> jac(_num_cm, std::vector<ADReal>(_num_cm));

  for (unsigned int i = 0; i < _num_cm; ++i)
    for (unsigned int j = 0; j < _num_cm; ++j)
      jac[i][j] = creepRateDerivative(creep_strain_incr, i, j) * _dt - ((i == j) ? 1.0 : 0.0);

  return jac;
}

ADRankTwoTensor
BVTwoCreepUpdateBase::reformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr)
{
  ADRankTwoTensor res;
  res.zero();

  ADRankTwoTensor flow_dir =
      (_eqv_stress_tr != 0.0) ? _stress_tr.deviatoric() / _eqv_stress_tr : ADRankTwoTensor();

  for (unsigned int i = 0; i < _num_cm; ++i)
    res += 1.5 * creep_strain_incr[i] * flow_dir;

  return res;
}

void
BVTwoCreepUpdateBase::preReturnMap()
{
}

void
BVTwoCreepUpdateBase::postReturnMap(const std::vector<ADReal> & /*creep_strain_incr*/)
{
}