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
                             "updates with Two flow directions (or rules). This class inherits "
                             "from BVTwoCreepUpdateBase.");
  params.addRequiredParam<bool>("volumetric",
                                "Whether to perform deviatoric and volumetric"
                                "calculations or only deviatoric calculations");
  return params;
}

BVDeviatoricVolumetricUpdateBase::BVDeviatoricVolumetricUpdateBase(
    const InputParameters & parameters)
  : BVTwoCreepUpdateBase(parameters),
    _volumetric(parameters.get<bool>("volumetric"))
{
}

std::vector<ADReal>
BVDeviatoricVolumetricUpdateBase::returnMap()
{
  // Deviatoric update
  std::vector<ADReal> creep_strain_incr = BVTwoCreepUpdateBase::returnMap();

  // Volumetric update
  if (_volumetric)
  {
    // Initialize volumetric strain incr
    ADReal vol_strain_incr = 0.0;

    // Initial residual
    ADReal res_ini = residualVol(vol_strain_incr);

    ADReal res = res_ini;
    ADReal jac = jacobianVol(vol_strain_incr);

    // Newton loop
    for (unsigned int iter = 0; iter < _max_its; ++iter)
    {
      vol_strain_incr -= res / jac;

      res = residualVol(vol_strain_incr);
      jac = jacobianVol(vol_strain_incr);

      // Convergence check
      if ((std::abs(res) <= _abs_tol) || (std::abs(res / res_ini) <= _rel_tol))
      {
        creep_strain_incr.push_back(vol_strain_incr);
        return creep_strain_incr;
      }
    }
    throw MooseException("BVDeviatoricVolumetricUpdateBase: maximum number of iterations exceeded in volumetric 'returnMap'!");
  }
  else
  {
    return creep_strain_incr;
  }
}

ADReal
BVDeviatoricVolumetricUpdateBase::residualVol(const ADReal & vol_strain_incr)
{
  return creepRateVol(vol_strain_incr) * _dt - vol_strain_incr;
}

ADReal
BVDeviatoricVolumetricUpdateBase::jacobianVol(const ADReal & vol_strain_incr)
{
  return creepRateVolDerivative(vol_strain_incr) * _dt - 1.0;
}

ADReal
BVDeviatoricVolumetricUpdateBase::creepRateVol(const ADReal & vol_strain_incr)
{
  return 0.0;
}

ADReal
BVDeviatoricVolumetricUpdateBase::creepRateVolDerivative(const ADReal & vol_strain_incr)
{
  return 0.0;
}

ADRankTwoTensor
BVDeviatoricVolumetricUpdateBase::reformPlasticStrainTensor(
    const std::vector<ADReal> & creep_strain_incr)
{
  ADRankTwoTensor res = BVTwoCreepUpdateBase::reformPlasticStrainTensor(creep_strain_incr);

  // Volumetric part
  if (_volumetric)
    res.addIa(-creep_strain_incr[2] / 3.0);

  return res;
}

