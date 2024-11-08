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

#include "BVFaultInterfaceGenerator.h"
#include "BndElement.h"
#include "CastUniquePointer.h"

#include "libmesh/mesh_modification.h"
#include "libmesh/distributed_mesh.h"
#include "libmesh/elem.h"
#include "libmesh/fe_base.h"
#include "libmesh/quadrature_gauss.h"

#include <typeinfo>

registerMooseObject("BeaverApp", BVFaultInterfaceGenerator);

InputParameters
BVFaultInterfaceGenerator::validParams()
{
  InputParameters params = MeshGenerator::validParams();

  params.addRequiredParam<MeshGeneratorName>("input", "The mesh we want to modify.");
  params.addClassDescription("Generate a fault interface by duplicating the nodes.");
  params.addRequiredParam<std::vector<BoundaryName>>(
      "sidesets", "The name of the sidesets to transform to lower dimensional elements.");
  return params;
}

BVFaultInterfaceGenerator::BVFaultInterfaceGenerator(const InputParameters & parameters)
  : MeshGenerator(parameters),
    _input(getMesh("input")),
    _sideset_names(getParam<std::vector<BoundaryName>>("sidesets"))
{
  if (typeid(_input).name() == typeid(std::unique_ptr<DistributedMesh>).name())
    mooseError("BVFaultInterfaceGenerator only works with ReplicatedMesh");
}

std::unique_ptr<MeshBase>
BVFaultInterfaceGenerator::generate()
{
  std::unique_ptr<MeshBase> mesh = std::move(_input);

  // Initialize the node to element map
  std::map<dof_id_type, std::vector<dof_id_type>> node_to_elem_map;
  for (const auto & elem : mesh->active_element_ptr_range())
    for (unsigned int n = 0; n < elem->n_nodes(); n++)
      node_to_elem_map[elem->node_id(n)].push_back(elem->id());

  // Get IDs of all boundaries for which we have a fault interface
  auto & boundary_info = mesh->get_boundary_info();
  std::set<boundary_id_type> mesh_sideset_ids;
  for (auto & sideset_name : _sideset_names)
    mesh_sideset_ids.insert(boundary_info.get_id_by_name(sideset_name));

  // Equivalent to MooseMesh::buildBndElemList()
  auto bc_tuples = boundary_info.build_active_side_list();
  int n = bc_tuples.size();
  std::vector<BndElement *> bnd_elems;
  std::map<boundary_id_type, std::set<dof_id_type>> bnd_elem_ids;
  bnd_elems.reserve(n);
  for (const auto & t : bc_tuples)
  {
    auto elem_id = std::get<0>(t);
    auto side_id = std::get<1>(t);
    auto bc_id = std::get<2>(t);

    bnd_elems.push_back(new BndElement(mesh->elem_ptr(elem_id), side_id, bc_id));
    bnd_elem_ids[bc_id].insert(elem_id);
  }

  // Get boundary node ids
  std::map<boundary_id_type, std::set<dof_id_type>> bnd_node_ids =
      getSidesetNodes(bnd_elems, mesh_sideset_ids);

  // Split nodes on fault interface
  std::map<boundary_id_type, std::set<std::vector<dof_id_type>>> split_nodes_map =
      splitNodesOnInterface(bnd_node_ids, mesh);

  // Stich nodes to elems
  stitchNodesToElems(split_nodes_map, node_to_elem_map, bnd_elems, mesh);

  return dynamic_pointer_cast<MeshBase>(mesh);
}

std::map<boundary_id_type, std::set<dof_id_type>>
BVFaultInterfaceGenerator::getSidesetNodes(const std::vector<BndElement *> & bnd_elems,
                                           const std::set<boundary_id_type> & mesh_sideset_ids)
{
  // Get a list of node ids to be split
  std::map<boundary_id_type, std::set<dof_id_type>> bnd_node_ids;

  // First get the list of nodes on sideset
  for (auto itss = mesh_sideset_ids.begin(); itss != mesh_sideset_ids.end(); ++itss)
  {
    std::map<dof_id_type, unsigned int> node_ids_cnt;
    for (auto it = bnd_elems.begin(); it != bnd_elems.end(); ++it)
      if ((*itss) == (*it)->_bnd_id)
      {
        Elem * elem = (*it)->_elem;
        auto s = (*it)->_side;

        // build neighbor element (this element should also be in the bnd_elems list)
        Elem * neighbor = elem->neighbor_ptr(s);

        // This is just to be sure that we duplicate the sideset only on one side
        if (elem->id() < neighbor->id())
          continue;

        // get the nodes on the side
        std::vector<unsigned int> nodes_on_side = elem->nodes_on_side(s);

        for (unsigned int nos = 0; nos < nodes_on_side.size(); nos++)
        {
          Node * node = elem->node_ptr(nodes_on_side[nos]);
          if (bnd_node_ids[(*itss)].count(node->id()) > 0)
          {
            unsigned int count = node_ids_cnt[node->id()];
            node_ids_cnt[node->id()] = count + 1;
          }
          else
            node_ids_cnt[node->id()] = 1;
          bnd_node_ids[(*itss)].insert(node->id());
        }
      }

    // // Remove nodes at the end
    // for (auto it = node_ids_cnt.begin(); it != node_ids_cnt.end(); ++it)
    //   if ((*it).second < 2)
    //     bnd_node_ids[(*itss)].erase((*it).first);
  }

  return bnd_node_ids;
}

std::map<boundary_id_type, std::set<std::vector<dof_id_type>>>
BVFaultInterfaceGenerator::splitNodesOnInterface(
    const std::map<boundary_id_type, std::set<dof_id_type>> & bnd_node_ids,
    std::unique_ptr<MeshBase> & mesh)
{
  // Loop over boundary nodes and split them.
  // Returns the map from old id to new id
  std::map<boundary_id_type, std::set<std::vector<dof_id_type>>> split_nodes_map;
  for (auto it = bnd_node_ids.begin(); it != bnd_node_ids.end(); ++it)
  {
    const boundary_id_type bnd_id = (*it).first;
    const std::set<dof_id_type> node_ids = (*it).second;

    for (auto itn = node_ids.begin(); itn != node_ids.end(); ++itn)
    {
      // Duplicate node and save new id
      Node * node = mesh->node_ptr((*itn));
      Node * new_node = nullptr;
      new_node = Node::build(*node, mesh->n_nodes()).release();
      new_node->processor_id() = node->processor_id();
      mesh->add_node(new_node);

      std::vector<dof_id_type> id_map(2);
      id_map[0] = (*itn);
      id_map[1] = new_node->id();
      split_nodes_map[bnd_id].insert(id_map);
    }
  }

  return split_nodes_map;
}

void
BVFaultInterfaceGenerator::stitchNodesToElems(
    const std::map<boundary_id_type, std::set<std::vector<dof_id_type>>> & split_nodes_map,
    std::map<dof_id_type, std::vector<dof_id_type>> & node_to_elem_map,
    const std::vector<BndElement *> & bnd_elems,
    std::unique_ptr<MeshBase> & mesh)
{
  RealVectorValue normal = RealVectorValue(0.0, 0.0, 0.0);
  for (auto it = split_nodes_map.begin(); it != split_nodes_map.end(); ++it)
  {
    const boundary_id_type bnd_id = (*it).first;
    const std::set<std::vector<dof_id_type>> node_ids = (*it).second;
    for (auto itn = node_ids.begin(); itn != node_ids.end(); ++itn)
    {
      // Get the elem ids which has this node
      dof_id_type node_ref_id = (*itn)[0];
      std::vector<dof_id_type> elem_ids = node_to_elem_map[node_ref_id];

      // First we treat the elements which have a side on the boundary
      std::set<dof_id_type> elem_ids_bnd;
      for (unsigned int i = 0; i < elem_ids.size(); ++i)
      {
        for (auto itelem = bnd_elems.begin(); itelem != bnd_elems.end(); ++itelem)
        {
          if ((*itelem)->_bnd_id == bnd_id)
          {
            Elem * elem = (*itelem)->_elem;
            if (elem->id() == elem_ids[i])
            {
              // Keep track of the elems we treat
              elem_ids_bnd.insert(elem_ids[i]);
              // Build neighbor on opposite side of sideset
              auto s = (*itelem)->_side;
              Elem * neighbor = elem->neighbor_ptr(s);

              // For first element, build normal
              if (normal.norm() == 0.0)
                normal = (elem->vertex_average() - neighbor->vertex_average()) /
                         (elem->vertex_average() - neighbor->vertex_average()).norm();

              // On "right" side of the sideset
              if (normal * (elem->vertex_average() - neighbor->vertex_average()) /
                      (elem->vertex_average() - neighbor->vertex_average()).norm() >
                  0.0)
              {
                unsigned int local_node_id = elem->local_node(node_ref_id);
                Node * new_node = mesh->node_ptr((*itn)[1]);
                elem->set_node(local_node_id) = new_node;
              }
            }
          }
        }
      }

      // Now the elems which are not in bnd_elems
      for (unsigned int i = 0; i < elem_ids.size(); ++i)
      {
        if (elem_ids_bnd.count(elem_ids[i]) == 0)
        {
          Elem * elem = mesh->elem_ptr(elem_ids[i]);
          Node * new_node = mesh->node_ptr((*itn)[1]);
          unsigned int local_node_id = elem->local_node(node_ref_id);
          Point new_point = elem->point(local_node_id);
          if (normal * (elem->vertex_average() - new_point) /
                  (elem->vertex_average() - new_point).norm() >
              0.0)
          {
            unsigned int local_node_id = elem->local_node(node_ref_id);
            elem->set_node(local_node_id) = new_node;
          }
        }
      }
    }
  }
}