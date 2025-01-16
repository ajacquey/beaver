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

#include "BVDeviatoricVolumetricUpdateBase.h"
#include "BVElasticityTensorTools.h"

InputParameters
BVDeviatoricVolumetricUpdateBase::validParams()
{
  InputParameters params = BVTwoCreepUpdateBase::validParams();
  params.addClassDescription("Base material for computing multiple creep stress" 
  "updates with Two flow directions (or rules). This class inherits from BVTwoCreepUpdateBase.");
  params.addParam<std::string>(
      "base_name",
      "Optional parameter that defines a prefix for all material "
      "properties related to this stress update model. This allows for "
      "multiple models of the same type to be used without naming conflicts.");
  params.addRequiredParam<Real>("num_cm", "number of creep models");
  params.addParam<bool>("dev_vol",
                      true,
                    "Whether to perform deviatoric and volumetric calculations or only deviatoric calculations");
  return params;
}

BVDeviatoricVolumetricUpdateBase::BVDeviatoricVolumetricUpdateBase(const InputParameters & parameters)
  : BVTwoCreepUpdateBase(parameters),
    _identity_two(RankTwoTensor::initIdentity),
    _num_cm(getParam<Real>("num_cm")),
    _dev_vol(parameters.get<bool>("dev_vol"))
//    _creep_strain_incr(declareADProperty<RankTwoTensor>(_base_name + "creep_strain_increment"))
{
}

void
BVDeviatoricVolumetricUpdateBase::inelasticUpdate(ADRankTwoTensor & stress, const RankFourTensor & Cijkl)
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
  _mean_stress_tr = _avg_stress_tr / 3.0;    // my modification: mean stress
  // Shear and bulk modulus
  _G = BVElasticityTensorTools::getIsotropicShearModulus(Cijkl);
  _K = BVElasticityTensorTools::getIsotropicBulkModulus(Cijkl);

  // Initialize creep strain increment
  _creep_strain_incr[_qp].zero();

  // Pre return map calculations (model specific)
  preReturnMap();

  // initialize the equivalent creep strains
  std::vector<ADReal> eqv_strain_incr(_num_cm, 0.0);
  ADReal Vol_eqv_strain_incr = 0.0;
  // Viscoelastic update
  DVreturnMap(eqv_strain_incr, Vol_eqv_strain_incr);

  _creep_strain_incr[_qp] = DVreformPlasticStrainTensor(eqv_strain_incr, Vol_eqv_strain_incr);
  stress -= 2.0 * _G * _creep_strain_incr[_qp];
  postReturnMap(eqv_strain_incr);
}

void
BVDeviatoricVolumetricUpdateBase::nrStep(const std::vector<ADReal> & res,
                             const std::vector<std::vector<ADReal>> & jac,
                             std::vector<ADReal> & creep_strain_incr)
{
  std::vector<ADReal> dx(_num_cm, 0.0);

  // Check determinant
  ADReal det = jac[0][0] * jac[1][1] - jac[0][1] * jac[1][0];

  if (det == 0.0)
    throw MooseException("BVDeviatoricVolumetricUpdateBase: matrix is singular in 'returnMap!");

  creep_strain_incr[0] -= (jac[1][1] * res[0] - jac[0][1] * res[1]) / det;
  creep_strain_incr[1] -= (jac[0][0] * res[1] - jac[1][0] * res[0]) / det;

  return;
}

ADReal
BVDeviatoricVolumetricUpdateBase::norm(const std::vector<ADReal> & vec)
{
  ADReal res = 0.0;
  for (const auto & r : vec)
    res += r * r;

  return std::sqrt(res);
}

std::vector<ADReal>
BVDeviatoricVolumetricUpdateBase::residual(const std::vector<ADReal> & creep_strain_incr)
{
  std::vector<ADReal> res(_num_cm);
  for (unsigned int i = 0; i < _num_cm; ++i)
    res[i] = creepRate(creep_strain_incr, i) * _dt - creep_strain_incr[i];

  return res;
}

std::vector<std::vector<ADReal>>
BVDeviatoricVolumetricUpdateBase::jacobian(const std::vector<ADReal> & creep_strain_incr)
{
  std::vector<std::vector<ADReal>> jac(_num_cm, std::vector<ADReal>(_num_cm));

  for (unsigned int i = 0; i < _num_cm; ++i)
    for (unsigned int j = 0; j < _num_cm; ++j)
      jac[i][j] = creepRateDerivative(creep_strain_incr, i, j) * _dt - ((i == j) ? 1.0 : 0.0);

  return jac;
}

void
BVDeviatoricVolumetricUpdateBase::preReturnMap()
{
}

void
BVDeviatoricVolumetricUpdateBase::postReturnMap(const std::vector<ADReal> & /*creep_strain_incr*/)
{
}


void
BVDeviatoricVolumetricUpdateBase::DVreturnMap(std::vector<ADReal>& creep_strain_incr_out, 
                                              ADReal & Vcreep_strain_incr_out)
{
    // Initialize scalar deviatoric creep strain increment
    std::vector<ADReal> creep_strain_incr(_num_cm, 0.0);

    // Initial residual
    std::vector<ADReal> res_ini = residual(creep_strain_incr); // initialize residual
    std::vector<ADReal> res = res_ini;

    std::vector<std::vector<ADReal>> jac = jacobian(creep_strain_incr); // initialize Jacobian

    if (_dev_vol)
    {
        // Initialize scalar volumetric creep strain increment
        ADReal Vcreep_strain_incr = 0.0; 

        // Initial volumetric residual
        ADReal Vres_ini = Vresidual(creep_strain_incr, Vcreep_strain_incr); // initialize volumetric residual
        ADReal Vres = Vres_ini;

        // Initial volumetric jacobian
        ADReal Vjac = Vjacobian(creep_strain_incr,Vcreep_strain_incr); // initialize volumetric Jacobian

        // Newton loop
        for (unsigned int iter = 0; iter < _max_its; ++iter)
        {
            // Call to nrStep to update the scalar deviatoric strain increment
            nrStep(res, jac, creep_strain_incr);

            // Recalculate residuals and Jacobians for the deviatoric part
            res = residual(creep_strain_incr);
            jac = jacobian(creep_strain_incr);

            // compute and update the scalar volumetric strain increment
            Vcreep_strain_incr -= Vres / Vjac;  
            Vres = Vresidual(creep_strain_incr, Vcreep_strain_incr);
            Vjac = Vjacobian(creep_strain_incr, Vcreep_strain_incr);
            
            // Convergence check
            bool D_converge = (norm(res) <= _abs_tol) || (norm(res) / norm(res_ini) <= _rel_tol);
            bool V_converge = (abs(Vres) <= _abs_tol) || (abs(Vres) / Vres_ini <= _rel_tol);
            bool converge = D_converge && V_converge;

            // Check if convergence was achieved
            if (converge) 
                {
                    // Assign results to output parameters
                    creep_strain_incr_out = creep_strain_incr;
                    Vcreep_strain_incr_out = Vcreep_strain_incr; // Ensure Vcreep_strain_incr is returned
                  return;  // Exit after successfully converging
                }
        }
        // If maximum iterations are exceeded without convergence, throw an exception
        throw MooseException("BVDeviatoricVolumetricUpdateBase: maximum number of iterations exceeded in the deviatoric-volumetric 'returnMap'!");
    }
    else // Implement only the deviatoric part for now
    {  
        // Newton loop for deviatoric part
        for (unsigned int iter = 0; iter < _max_its; ++iter)
        {
            // Call to nrStep which updates strain increments
            nrStep(res, jac, creep_strain_incr); // Assuming nrStep works for deviatoric part without volumetric parameters

            // Recalculate residuals and Jacobians
            res = residual(creep_strain_incr);
            jac = jacobian(creep_strain_incr);

            // Convergence check
            if ((norm(res) <= _abs_tol) || (norm(res) / norm(res_ini) <= _rel_tol)) {
                // Assign results to output parameters
                creep_strain_incr_out = creep_strain_incr; 
                return; // Exit after successfully converging
            }
        }

        // If maximum iterations are exceeded without convergence, throw an exception
        throw MooseException("BVDeviatoricVolumetricUpdateBase: maximum number of iterations exceeded in deviatoric 'returnMap'!");
    }
}

ADReal
BVDeviatoricVolumetricUpdateBase::Vresidual(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr)
{
   return VcreepRate(creep_strain_incr, Vcreep_strain_incr) * _dt - Vcreep_strain_incr;
}

ADReal
BVDeviatoricVolumetricUpdateBase::Vjacobian(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr)
{
   return  VcreepRateDerivative(creep_strain_incr,Vcreep_strain_incr) * _dt - 1.0;
}

ADRankTwoTensor
BVDeviatoricVolumetricUpdateBase::DVreformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr, const ADReal & Vcreep_strain_incr)
{
  ADRankTwoTensor res;
  ADRankTwoTensor Dres;
  ADRankTwoTensor Vres;
  res.zero();
  Dres.zero();
  Vres.zero();

  ADRankTwoTensor flow_dir1 =
      (_eqv_stress_tr != 0.0) ? _stress_tr.deviatoric() / _eqv_stress_tr : ADRankTwoTensor();

  ADRankTwoTensor flow_dir2 =  _identity_two;
  
  for (unsigned int i = 0; i < creep_strain_incr.size(); ++i)
    Dres += 1.5 * creep_strain_incr[i] * flow_dir1;

  Vres = ((1.0 / std::sqrt(3)) * Vcreep_strain_incr * _dt * flow_dir2);

  res = Dres + Vres;
  return res;
}
