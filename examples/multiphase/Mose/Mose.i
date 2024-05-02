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
  [p]
    order = CONSTANT
    family = MONOMIAL
    fv = true
  []
  [sw]
    order = CONSTANT
    family = MONOMIAL
    fv = true
    initial_condition = 0.01
  []
  [sn]
    order = CONSTANT
    family = MONOMIAL
    fv = true
    initial_condition = 0.99
  []
[]

[FVKernels]
  # Total pressure
  [darcy_p]
    type = BVFVMultiPhasePressureDarcy
    variable = p
  []
  # Wetting phase
  [time_w]
    type = BVFVMultiPhaseSaturationTimeDerivative
    variable = sw
    phase = 'wetting'
  []
  [darcy_w]
    type = BVFVMultiPhaseSaturationDarcy
    variable = sw
    total_pressure = p
    phase = 'wetting'
  []
  # Non-wetting phase
  [time_nw]
    type = BVFVMultiPhaseSaturationTimeDerivative
    variable = sn
    phase = 'non_wetting'
  []
  [darcy_nw]
    type = BVFVMultiPhaseSaturationDarcy
    variable = sn
    total_pressure = p
    phase = 'non_wetting'
  []
[]

[FVBCs]
  [topinflux]
    type = FVDirichletBC
    variable = p
    value = 100
    boundary = 'top'
  []
  [bottondrained]
    type = FVDirichletBC
    variable = p
    value = 0
    boundary = 'bottom'
  []
  [topinflux_sw]
    type = FVDirichletBC
    variable = sw
    value = 1.0
    boundary = 'top'
  []
  [bottom_sw]
    type = FVDirichletBC
    variable = sw
    value = 0.01
    boundary = 'bottom'
  []
  [topinflux_sn]
    type = FVDirichletBC
    variable = sn
    value = 0.0
    boundary = 'top'
  []
  [bottom_sn]
    type = FVDirichletBC
    variable = sn
    value = 0.99
    boundary = 'bottom'
  []
[]

[Materials]
  [fluid_flow_mat]
    type = BVMultiPhaseFlowMaterial
  []
  [porosity]
    type = BVConstantPorosity
    porosity = 1.0
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
  [fluid_properties_w]
    type = BVFluidProperties
    fluid_pressure = p
    temperature = 273
    fp = simple_fluid_w
    phase = 'wetting'
  []
  [fluid_properties_nw]
    type = BVFluidProperties
    fluid_pressure = p
    temperature = 273
    fp = simple_fluid_nw
    phase = 'non_wetting'
  []
  [rel_perm]
    type = BVRelativePermeabilityPowerLaw
    saturation_w = sw
    exponent = 2.0
  []
  [capillary_pressure]
    type = BVCapillaryPressurePowerLaw
    saturation_w = sw
    exponent = 2.0
    reference_capillary_pressure = 1.0e-04
  []
[]

[FluidProperties]
  [simple_fluid_w]
    type = SimpleFluidProperties
    density0 = 1.0
    viscosity = 0.5
  []
  [simple_fluid_nw]
    type = SimpleFluidProperties
    density0 = 1.0
    viscosity = 1.0
  []
[]

[Preconditioning]
  active = 'superlu'
  [hypre]
    type = SMP
    full = true
    petsc_options = '-snes_ksp_ew -snes_converged_reason -ksp_converged_reason'
    petsc_options_iname = '-pc_type -pc_hypre_type
                           -pc_hypre_boomeramg_strong_threshold -pc_hypre_boomeramg_agg_nl -pc_hypre_boomeramg_agg_num_paths -pc_hypre_boomeramg_max_levels
                           -pc_hypre_boomeramg_coarsen_type -pc_hypre_boomeramg_interp_type
                           -pc_hypre_boomeramg_P_max -pc_hypre_boomeramg_truncfacto -snes_atol'
    petsc_options_value = 'hypre boomeramg
                           0.7 4 5 25
                           HMIS ext+i
                           2 0.3 1.0e-14'
  []
  [superlu]
    type = SMP
    full = true
    petsc_options = '-snes_ksp_ew -snes_converged_reason -ksp_converged_reason'# -ksp_gmres_modifiedgramschmidt -ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-snes_type
                           -snes_atol -snes_rtol -snes_max_it
                           -pc_type -pc_factor_mat_solver_package'
    petsc_options_value = 'newtonls
                           1e-10 1e-08 50
                           lu superlu_dist'
  []
[]

# [Preconditioning]
#   [hypre]
#     type = SMP
#     full = true
#     petsc_options = '-snes_ksp_ew -snes_converged_reason -ksp_converged_reason'
#     petsc_options_iname = '-pc_type -pc_hypre_type
#                            -snes_atol -snes_max_it -snes_linesearch_type'
#     petsc_options_value = 'hypre boomeramg
#                            1.0e-10 1000 basic'
#   []
# []

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  start_time = 0
  end_time = 500
  num_steps = 500
  timestep_tolerance = 1.0e-05
  automatic_scaling = true
  residual_and_jacobian_together = true
[]

[Outputs]
  print_linear_residuals = false
  perf_graph = true
  execute_on = 'INITIAL TIMESTEP_END'
  exodus = true
[]
