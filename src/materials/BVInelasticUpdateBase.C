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

#include "BVInelasticUpdateBase.h"

InputParameters
BVInelasticUpdateBase::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Base material for computing an inelastic stress update.");
  // Newton-Raphson loop parameters
  params.addRangeCheckedParam<Real>("abs_tolerance",
                                    1.0e-10,
                                    "abs_tolerance > 0.0",
                                    "The absolute tolerance for the iterative update.");
  params.addRangeCheckedParam<Real>("rel_tolerance",
                                    1.0e-10,
                                    "rel_tolerance > 0.0",
                                    "The relative tolerance for the iterative update.");
  params.addRangeCheckedParam<unsigned int>(
      "max_iterations",
      200,
      "max_iterations >= 1",
      "The maximum number of iterations for the iterative update");
  // Material is called
  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");
  return params;
}

BVInelasticUpdateBase::BVInelasticUpdateBase(const InputParameters & parameters)
  : Material(parameters),
    _abs_tol(getParam<Real>("abs_tolerance")),
    _rel_tol(getParam<Real>("rel_tolerance")),
    _max_its(getParam<unsigned int>("max_iterations"))
{
}

void
BVInelasticUpdateBase::setQp(unsigned int qp)
{
  _qp = qp;
}

void
BVInelasticUpdateBase::inelasticUpdate(ADRankTwoTensor & stress,
                                       const RankFourTensor & Cijkl,
                                       ADRankTwoTensor & strain_increment)
{
}