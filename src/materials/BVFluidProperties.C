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

#include "BVFluidProperties.h"

registerMooseObject("BeaverApp", BVFluidProperties);

InputParameters
BVFluidProperties::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "Computes fluid properties (density and viscosity) based on fluid_properties module.");
  params.addRequiredCoupledVar("fluid_pressure", "The fluid pressure (Pa)");
  params.addRequiredCoupledVar("temperature", "The temperature (K)");
  params.addRequiredParam<UserObjectName>("fp",
                                          "The name of the user object for fluid properties.");
  //   MooseEnum phase("wetting non_wetting single", "single");
  //   params.addParam<MooseEnum>(
  //       "phase", phase, "The phase of the fluid properties (wetting, non_wetting or single).");
  return params;
}

BVFluidProperties::BVFluidProperties(const InputParameters & parameters)
  : Material(parameters),
    _pf(adCoupledValue("fluid_pressure")),
    _temp(adCoupledValue("temperature")),
    // _phase(getParam<MooseEnum>("phase").getEnum<PhaseEnum>()),
    // _ext(phase_ext()),
    _fp(getUserObject<SinglePhaseFluidProperties>("fp")),
    // _density(declareADProperty<Real>("density" + _ext)),
    // _viscosity(declareADProperty<Real>("viscosity" + _ext))
    _density(declareADProperty<Real>("density")),
    _viscosity(declareADProperty<Real>("viscosity"))
{
}

// std::string
// BVFluidProperties::phase_ext()
// {
//   switch (_phase)
//   {
//     case PhaseEnum::WETTING:
//       return "_w";
//     case PhaseEnum::NON_WETTING:
//       return "_n";
//     case PhaseEnum::SINGLE:
//       return "";
//     default:
//       mooseError("Unknow phase!");
//   }
// }

void
BVFluidProperties::initQpStatefulProperties()
{
  _density[_qp] = _fp.rho_from_p_T(_pf[_qp], _temp[_qp]);
  _viscosity[_qp] = _fp.mu_from_p_T(_pf[_qp], _temp[_qp]);
}

void
BVFluidProperties::computeQpProperties()
{
  _density[_qp] = _fp.rho_from_p_T(_pf[_qp], _temp[_qp]);
  _viscosity[_qp] = _fp.mu_from_p_T(_pf[_qp], _temp[_qp]);
}