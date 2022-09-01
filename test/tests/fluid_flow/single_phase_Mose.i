[Mesh]
  [mesh_gen]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = -50
    xmax = 50
    ymin = -50
    ymax = 50
    nx = 50
    ny = 50
  []
  [barrier1]
    type = SubdomainBoundingBoxGenerator
    input = 'mesh_gen'
    block_id = 1
    bottom_left = '-50 6 0'
    top_right = '30 10 0'
  []
  [barrier2]
    type = SubdomainBoundingBoxGenerator
    input = 'barrier1'
    block_id = 1
    bottom_left = '-30 -10 0'
    top_right = '50 -6 0'
  []
[]

[Variables]
  [pf]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [darcy]
    type = BVSinglePhaseDarcy
    variable = pf
  []
[]

[AuxVariables]
  [vf_x]
    order = CONSTANT
    family = MONOMIAL
  []
  [vf_y]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [vf_x_aux]
    type = BVDarcyVelocityAux
    variable = vf_x
    fluid_pressure = pf
    component = x
    execute_on = 'TIMESTEP_END'
  []
  [vf_y_aux]
    type = BVDarcyVelocityAux
    variable = vf_y
    fluid_pressure = pf
    component = y
    execute_on = 'TIMESTEP_END'
  []
[]

[BCs]
  [topinflux]
    type = DirichletBC
    variable = pf
    value = 1
    boundary = 'top'
  []
  [bottomdrained]
    type = DirichletBC
    variable = pf
    value = 0
    boundary = 'bottom'
  []
[]

[Materials]
  [single_phase]
    type = BVSinglePhaseFlowMaterial
  []
  [permeability0]
    type = BVConstantPermeability
    block = 0
    permeability = 1.0
  []
  [permeability1]
    type = BVConstantPermeability
    block = 1
    permeability = 1.0e-05
  []
  [fluid]
    type = BVFluidProperties
    fluid_pressure = pf
    temperature = 273
    fp = simple_fluid
  []
[]

[Modules]
  [FluidProperties]
    [simple_fluid]
      type = SimpleFluidProperties
      density0 = 1.0
      viscosity = 1.0
    []
  []
[]

# [Preconditioning]
#   [hypre]
#     type = SMP
#     full = true
#     petsc_options = '-snes_ksp_ew -snes_converged_reason -ksp_converged_reason'
#     petsc_options_iname = '-pc_type -pc_hypre_type
#                            -pc_hypre_boomeramg_strong_threshold -pc_hypre_boomeramg_agg_nl -pc_hypre_boomeramg_agg_num_paths -pc_hypre_boomeramg_max_levels
#                            -pc_hypre_boomeramg_coarsen_type -pc_hypre_boomeramg_interp_type
#                            -pc_hypre_boomeramg_P_max -pc_hypre_boomeramg_truncfacto -snes_atol'
#     petsc_options_value = 'hypre boomeramg
#                            0.7 4 5 25
#                            HMIS ext+i
#                            2 0.3 1.0e-14'
#   []
# []
[Preconditioning]
  [hypre]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type'
    petsc_options_value = 'hypre boomeramg'
  []
[]

[Executioner]
  type = Steady
  solve_type = 'NEWTON'
  automatic_scaling = true
[]

[Outputs]
  execute_on = 'TIMESTEP_END'
  exodus = true
[]
