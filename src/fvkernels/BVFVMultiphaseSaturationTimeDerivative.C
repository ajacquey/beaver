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

#include "BVFVMultiPhaseSaturationTimeDerivative.h"

registerADMooseObject("BeaverApp", BVFVMultiPhaseSaturationTimeDerivative);

InputParameters
BVFVMultiPhaseSaturationTimeDerivative::validParams()
{
  InputParameters params = FVTimeKernel::validParams();
  params.addClassDescription("Kernel for the saturation transient term for multi phase flow.");
  MooseEnum phase("wetting non_wetting single");
  params.addRequiredParam<MooseEnum>(
      "phase", phase, "The phase of the fluid properties (wetting, non_wetting or single).");
  return params;
}

BVFVMultiPhaseSaturationTimeDerivative::BVFVMultiPhaseSaturationTimeDerivative(const InputParameters & parameters)
  : FVTimeKernel(parameters),
    _phase(getParam<MooseEnum>("phase").getEnum<PhaseEnum>()),
    _ext(phase_ext()),
    _porosity(getADMaterialProperty<Real>("porosity")),
    _density(getADMaterialProperty<Real>("density" + _ext)),
    _density_old(getMaterialPropertyOld<Real>("density" + _ext))
{
}

std::string
BVFVMultiPhaseSaturationTimeDerivative::phase_ext()
{
  switch (_phase)
  {
    case PhaseEnum::WETTING:
      return "_w";
    case PhaseEnum::NON_WETTING:
      return "_n";
    default:
      mooseError("Unknow phase!");
  }
}
ADReal
BVFVMultiPhaseSaturationTimeDerivative::computeQpResidual()
{
  return _porosity[_qp] * _u[_qp] / _density[_qp] * (_density[_qp] - _density_old[_qp]) / _dt +
         _porosity[_qp] * _u_dot[_qp];
}