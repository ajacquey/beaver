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

#include "BVFVSinglePhaseSoluteDarcy.h"

registerADMooseObject("BeaverApp", BVFVSinglePhaseSoluteDarcy);

InputParameters
BVFVSinglePhaseSoluteDarcy::validParams()
{
  InputParameters params = BVFVFluxKernelBase::validParams();
  params.addClassDescription(
      "Kernel for the divergence of Darcy's velocity for single pahse flow and solute transport.");
  params.addRequiredCoupledVar("fluid_pressure", "The fluid pressure variable.");
  return params;
}

BVFVSinglePhaseSoluteDarcy::BVFVSinglePhaseSoluteDarcy(const InputParameters & parameters)
  : BVFVFluxKernelBase(parameters),
    _p_var(dynamic_cast<const MooseVariableFV<Real> *>(getFieldVar("fluid_pressure", 0))),
    _lambda(getADMaterialProperty<Real>("fluid_mobility")),
    _lambda_neighbor(getNeighborADMaterialProperty<Real>("fluid_mobility")),
    _lambda_c(getADMaterialProperty<Real>("solute_mobility")),
    _lambda_c_neighbor(getNeighborADMaterialProperty<Real>("solute_mobility"))
{
}

ADReal
BVFVSinglePhaseSoluteDarcy::computeQpResidual()
{

  // Darcy velocity
  ADRealVectorValue u = diffusiveFlux(_lambda[_qp], _lambda_neighbor[_qp], (*_p_var));

  // Advective flux
  ADRealVectorValue u_adv = advectiveFluxVariable(u);

  // Diffusive flux
  ADRealVectorValue u_diff = diffusiveFlux(_lambda_c[_qp], _lambda_c_neighbor[_qp], _var);

  return (u_adv - u_diff) * (*_face_info).normal();
}
