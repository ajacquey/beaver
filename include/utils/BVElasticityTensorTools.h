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

namespace BVElasticityTensorTools
{

/**
 * Get the shear modulus for an isotropic elasticity tensor
 * param elasticity_tensor the tensor (must be isotropic, but not checked for efficiency)
 */
template <typename T>
T
getIsotropicShearModulus(const RankFourTensorTempl<T> & elasticity_tensor)
{
  return elasticity_tensor(0, 1, 0, 1);
}

/**
 * Get the bulk modulus for an isotropic elasticity tensor
 * param elasticity_tensor the tensor (must be isotropic, but not checked for efficiency)
 */
template <typename T>
T
getIsotropicBulkModulus(const RankFourTensorTempl<T> & elasticity_tensor)
{
  const T shear_modulus = getIsotropicShearModulus(elasticity_tensor);
  // dilatational modulus is defined as lambda plus two mu
  const T dilatational_modulus = elasticity_tensor(0, 0, 0, 0);
  const T lambda = dilatational_modulus - 2.0 * shear_modulus;
  const T bulk_modulus = lambda + 2.0 * shear_modulus / 3.0;
  return bulk_modulus;
}

/**
 * Get the Young's modulus for an isotropic elasticity tensor
 * param elasticity_tensor the tensor (must be isotropic, but not checked for efficiency)
 */
template <typename T>
T
getIsotropicYoungsModulus(const RankFourTensorTempl<T> & elasticity_tensor)
{
  const T shear_modulus = getIsotropicShearModulus(elasticity_tensor);
  // dilatational modulus is defined as lambda plus two mu
  const T dilatational_modulus = elasticity_tensor(0, 0, 0, 0);
  const T lambda = dilatational_modulus - 2.0 * shear_modulus;
  const T youngs_modulus =
      shear_modulus * (3.0 * lambda + 2.0 * shear_modulus) / (lambda + shear_modulus);
  return youngs_modulus;
}

/**
 * Get the Poisson's modulus for an isotropic elasticity tensor
 * param elasticity_tensor the tensor (must be isotropic, but not checked for efficiency)
 */
template <typename T>
T
getIsotropicPoissonsRatio(const RankFourTensorTempl<T> & elasticity_tensor)
{
  const T poissons_ratio = elasticity_tensor(1, 1, 0, 0) /
                           (elasticity_tensor(1, 1, 1, 1) + elasticity_tensor(1, 1, 0, 0));
  return poissons_ratio;
}

}