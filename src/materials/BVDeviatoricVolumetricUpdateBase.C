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
#include "RankTwoTensor.h"

InputParameters
BVDeviatoricVolumetricUpdateBase::validParams()
{
  InputParameters params = BVTwoCreepUpdateBase::validParams();
  params.addClassDescription("Base material for computing multiple creep stress" 
  "updates with Two flow directions (or rules). This class inherits from BVTwoCreepUpdateBase.");
  params.addRequiredParam<bool>("volumetric", "Whether to perform deviatoric and volumetric" 
  "calculations or only deviatoric calculations");
  return params;
}

BVDeviatoricVolumetricUpdateBase::BVDeviatoricVolumetricUpdateBase(const InputParameters & parameters)
  : BVTwoCreepUpdateBase(parameters),
    _volumetric(parameters.get<bool>("volumetric")),
    _vcreep_strain_incr(declareADProperty<RankTwoTensor>(_base_name + "vcreep_strain_increment"))
{
}

void
BVDeviatoricVolumetricUpdateBase::inelasticUpdate(ADRankTwoTensor & stress, const RankFourTensor & Cijkl)
{
  if (_volumetric)
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
      _avg_stress_tr = -_stress_tr.trace() / 3.0;
      // Shear and bulk modulus
      _G = BVElasticityTensorTools::getIsotropicShearModulus(Cijkl);
      _K = BVElasticityTensorTools::getIsotropicBulkModulus(Cijkl);

        // Initialize creep strain increments
        _creep_strain_incr[_qp].zero();
        _vcreep_strain_incr[_qp].zero();

        // Pre return map calculations (model specific)
        preReturnMap();

        // Viscoelastic update
        std::vector<ADReal> creep_strain_incr = BVTwoCreepUpdateBase::returnMap();
        std::vector<ADReal> vcreep_strain_incr = returnMap();

        // Concatenate the two vectors
        std::vector<ADReal> total_creep_strain_inc;
        total_creep_strain_inc.reserve(creep_strain_incr.size() + vcreep_strain_incr.size()); // Reserve memory
        total_creep_strain_inc.insert(total_creep_strain_inc.end(), creep_strain_incr.begin(), creep_strain_incr.end()); // Add first vector
        total_creep_strain_inc.insert(total_creep_strain_inc.end(), vcreep_strain_incr.begin(), vcreep_strain_incr.end()); // Add second vector

        _creep_strain_incr[_qp] = BVTwoCreepUpdateBase::reformPlasticStrainTensor(creep_strain_incr);
        _vcreep_strain_incr[_qp] = reformPlasticStrainTensor(vcreep_strain_incr);
        stress -= ((2.0 * _G * _creep_strain_incr[_qp]) + (_K * _vcreep_strain_incr[_qp]));
        postReturnMap(total_creep_strain_inc);
      }
    else
      {
        BVTwoCreepUpdateBase::inelasticUpdate(stress, Cijkl);
      }
}

std::vector<ADReal>
BVDeviatoricVolumetricUpdateBase::returnMap()
{ 
  if (_volumetric)
    {
        std::vector<ADReal> creep_strain_incr(_num_cm, 0.0); // initialize deviatoric creep strain increment
        std::vector<ADReal> vcreep_strain_incr (1.0, 0.0); // initialize volumetric creep strain increment

        // Initial volumetric residual
        std::vector<ADReal> vres_ini = vresidual(creep_strain_incr,vcreep_strain_incr); // initialize volumetric residual
        std::vector<ADReal> vres = vres_ini;
        std::vector<std::vector<ADReal>> vjac = vjacobian(creep_strain_incr,vcreep_strain_incr); // initialize volumetric Jacobian

        // Newton loop
    for (unsigned int iter = 0; iter < _max_its; ++iter)
      {
        vcreep_strain_incr[0] -= vres[0] / vjac[0][0]; // Newton-Raphson step

          vres = vresidual(creep_strain_incr, vcreep_strain_incr);
          vjac = vjacobian(creep_strain_incr, vcreep_strain_incr);

          // Convergence check
          if ((norm(vres) <= _abs_tol) || (norm(vres) / norm(vres_ini) <= _rel_tol))

          return vcreep_strain_incr; // Ensure vcreep_strain_incr is updated correctly
      }
        // If maximum iterations are exceeded without convergence, throw an exception
  throw MooseException("BVDeviatoricVolumetricUpdateBase: maximum number of iterations exceeded in the deviatoric-volumetric 'returnMap'!");
    }
  else
    {
      return BVTwoCreepUpdateBase::returnMap();
    }
}

std::vector<ADReal>
BVDeviatoricVolumetricUpdateBase::vresidual(const std::vector<ADReal> & creep_strain_incr, const std::vector<ADReal> & vcreep_strain_incr)
{
  std::vector<ADReal> res(vcreep_strain_incr.size());
  for (unsigned int i = 0; i < vcreep_strain_incr.size(); ++i)
    res[i] = vcreepRate(creep_strain_incr, vcreep_strain_incr) * _dt - vcreep_strain_incr[i];
  return res;
}

std::vector<std::vector<ADReal>>
BVDeviatoricVolumetricUpdateBase::vjacobian(const std::vector<ADReal> & creep_strain_incr, const std::vector<ADReal> & vcreep_strain_incr)
{
  std::vector<std::vector<ADReal>> jac(vcreep_strain_incr.size(), std::vector<ADReal>(vcreep_strain_incr.size()));

  for (unsigned int i = 0; i < vcreep_strain_incr.size(); ++i)
    for (unsigned int j = 0; j < vcreep_strain_incr.size(); ++j)
      jac[i][j] = vcreepRateDerivative(creep_strain_incr,vcreep_strain_incr) * _dt - ((i == j) ? 1.0 : 0.0);

  return jac;
}

ADRankTwoTensor
BVDeviatoricVolumetricUpdateBase::reformPlasticStrainTensor(const std::vector<ADReal> & creep_strain_incr)
{
  if (_volumetric)
    {
    ADRankTwoTensor res;
    res.zero();
    ADRankTwoTensor flow_dir2 = RankTwoTensor::Identity();
    
      for (unsigned int i = 0; i < creep_strain_incr.size(); ++i)
        res += (1.0 / std::sqrt(3)) * creep_strain_incr[i] * _dt * flow_dir2;
        
      return res;  
    }
  else
    {
      return BVTwoCreepUpdateBase::reformPlasticStrainTensor(creep_strain_incr);
    }
}
