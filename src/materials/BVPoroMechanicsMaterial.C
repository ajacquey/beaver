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

#include "BVPoroMechanicsMaterial.h"

registerMooseObject("BeaverApp", BVPoroMechanicsMaterial);

InputParameters
BVPoroMechanicsMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Material for computing the poroelastic properties of a deformable porous medium.");
  // Poroelastic parameters
  params.addRangeCheckedParam<Real>(
      "biot_coefficient", 1.0, "biot_coefficient > 0 & biot_coefficient <= 1.0", "The Biot's poroelastic coefficient.");
  return params;
}

BVPoroMechanicsMaterial::BVPoroMechanicsMaterial(const InputParameters & parameters)
  : Material(parameters),
    // Poroelastic parameters
    _biot_coefficient(getParam<Real>("biot_coefficient")),
    // Strain increment
    _strain_increment(getADMaterialProperty<RankTwoTensor>("strain_increment")),
    // Bulk modulus
    _K(getADMaterialProperty<Real>("bulk_modulus")),
    // Porosity
    _porosity(getADMaterialProperty<Real>("porosity")),
    // Poroelastic properties
    _biot(declareADProperty<Real>("biot_coefficient")),
    _porous_storage(declareADProperty<Real>("porous_storage")),
    _poromech(declareADProperty<Real>("poromechanics"))
{
}

void
BVPoroMechanicsMaterial::computeQpProperties()
{
  // Solid compressibility
  ADReal Cs = (1.0 - _biot_coefficient) / _K[_qp];

  // Biot's coefficient
  _biot[_qp] = _biot_coefficient;

  // Porous storage
  _porous_storage[_qp] = (_biot[_qp] - _porosity[_qp]) * Cs;

  // Poromechanics
  _poromech[_qp] = _biot[_qp] * _strain_increment[_qp].trace() / _dt;
}
