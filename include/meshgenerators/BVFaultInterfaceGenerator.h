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

#pragma once

#include "MeshGenerator.h"

class BVFaultInterfaceGenerator : public MeshGenerator
{
public:
  static InputParameters validParams();
  BVFaultInterfaceGenerator(const InputParameters & parameters);
  std::unique_ptr<MeshBase> generate() override;

protected:
  std::map<boundary_id_type, std::set<dof_id_type>>
  getSidesetNodes(const std::vector<BndElement *> & bnd_elems,
                  const std::set<boundary_id_type> & mesh_sideset_ids);
  std::map<boundary_id_type, std::set<std::vector<dof_id_type>>>
  splitNodesOnInterface(const std::map<boundary_id_type, std::set<dof_id_type>> & bnd_node_ids,
                        std::unique_ptr<MeshBase> & mesh);
  void stitchNodesToElems(
      const std::map<boundary_id_type, std::set<std::vector<dof_id_type>>> & split_nodes_map,
      std::map<dof_id_type, std::vector<dof_id_type>> & node_to_elem_map,
      const std::vector<BndElement *> & bnd_elems,
      std::unique_ptr<MeshBase> & mesh);

  std::unique_ptr<MeshBase> & _input;
  const std::vector<BoundaryName> _sideset_names;
};
