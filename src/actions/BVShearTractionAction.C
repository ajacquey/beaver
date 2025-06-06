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

#include "BVShearTractionAction.h"
#include "Factory.h"
#include "FEProblem.h"
#include "Conversion.h"

registerMooseAction("BeaverApp", BVShearTractionAction, "add_bc");

InputParameters
BVShearTractionAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription("Set up shear traction boundary conditions.");
  params.addRequiredParam<std::vector<BoundaryName>>(
      "boundary",
      "The list of boundary IDs from the mesh where the shear traction will be applied.");
  params.addRequiredParam<std::vector<NonlinearVariableName>>(
      "displacement_vars",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addParam<Real>("value", 1.0, "Value of the shear traction applied.");
  params.addParam<FunctionName>("function", "Function giving the shear traction applied.");
  return params;
}

BVShearTractionAction::BVShearTractionAction(const InputParameters & params) : Action(params) {}

void
BVShearTractionAction::act()
{
  const std::string kernel_name = "BVShearTractionBC";

  std::vector<NonlinearVariableName> displacements =
      getParam<std::vector<NonlinearVariableName>>("displacement_vars");

  // Create pressure BCs
  for (unsigned int i = 0; i < displacements.size(); ++i)
  {
    // Create unique kernel name for each of the components
    std::string unique_kernel_name = kernel_name + "_" + _name + "_" + Moose::stringify(i);

    InputParameters params = _factory.getValidParams(kernel_name);
    params.applyParameters(parameters());
    params.set<bool>("use_displaced_mesh") = true;
    params.set<unsigned int>("component") = i;
    params.set<NonlinearVariableName>("variable") = displacements[i];

    _problem->addBoundaryCondition(kernel_name, unique_kernel_name, params);
  }
}