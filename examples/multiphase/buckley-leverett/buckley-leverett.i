[Mesh]
  type = GeneratedMesh
  dim = 3
  xmin = 0
  xmax = 1
  nx = 200
  ymin = -0.01
  ymax = 0.01
  zmin = -0.01
  zmax = 0.01
  ny = 1
  nz = 1
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
  # [sn]
  #   order = CONSTANT
  #   family = MONOMIAL
  #   fv = true
  #   initial_condition = 0.99
  # []
[]

[FVKernels]
  # Total pressure
  [darcy_p]
    # type = FVDiffusion
    type = BVFVMultiPhasePressureDarcy
    variable = p
    # coeff = 1
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
  # # Non-wetting phase
  # [time_nw]
  #   type = BVFVMultiPhaseSaturationTimeDerivative
  #   variable = sn
  #   phase = 'non_wetting'
  # []
  # [darcy_nw]
  #   type = BVFVMultiPhaseSaturationDarcy
  #   variable = sn
  #   total_pressure = p
  #   phase = 'non_wetting'
  # []
[]

[AuxVariables]
  [u]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [u_aux]
    type = BVDarcyVelocityAux
    variable = u
    fluid_pressure = p
    component = x
    execute_on = 'TIMESTEP_END'
  []
[]

[FVBCs]
  [leftinflux_pw]
    type = FVNeumannBC
    variable = p
    value = 1.0
    boundary = 'left'
  []
  [rightoutflux_pw]
    type = FVDirichletBC
    variable = p
    value = 0.0
    boundary = 'right'
  []
  [leftinflux_sw]
    # type = FVDirichletBC
    type = FVNeumannBC
    variable = sw
    # value = 0.99
    value = 1.0
    boundary = 'left'
  []
  # [right_sw]
  #   # type = FVDirichletBC
  #   type = FVNeumannBC
  #   variable = sw
  #   # value = 0.01
  #   value = -1
  #   boundary = 'right'
  # []
  # [leftinflux_sn]
  #   type = FVDirichletBC
  #   variable = sn
  #   value = 0.05
  #   boundary = 'left'
  # []
  # [right_sn]
  #   type = FVDirichletBC
  #   variable = sn
  #   value = 0.95
  #   boundary = 'right'
  # []
[]

[Materials]
  [fluid_flow_mat]
    type = BVMultiPhaseFlowMaterial
    output_properties = 'fluid_mobility fractional_flow_w diffusivity_saturation'
    outputs = exodus
  []
  [porosity]
    type = BVConstantPorosity
    porosity = 1.0
  []
  [permeability]
    type = BVConstantPermeability
    permeability = 1.0
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
  [capillary_pressure]
    type = BVCapillaryPressurePowerLaw
    saturation_w = sw
    exponent = 2
    reference_capillary_pressure = 0.0
    output_properties = 'capillary_pressure'
    outputs = exodus
  []
  [rel_perm]
    type = BVRelativePermeabilityPowerLaw
    saturation_w = sw
    exponent = 2
    output_properties = 'relative_permeability_w relative_permeability_n'
    outputs = exodus
  []
[]

[FluidProperties]
  [simple_fluid_w]
    type = SimpleFluidProperties
    density0 = 1.0
    viscosity = 1.0
  []
  [simple_fluid_nw]
    type = SimpleFluidProperties
    density0 = 1.0
    viscosity = 1.0
  []
[]

[VectorPostprocessors]
  [line_sw]
    type = LineValueSampler
    variable = sw
    start_point = '0 0 0'
    end_point = '1 0 0'
    num_points = 200
    sort_by = 'x'
    outputs = 'csv'
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

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  scheme = 'bdf2'
  start_time = 0
  end_time = 1.0
  num_steps = 100
  timestep_tolerance = 1.0e-05
  automatic_scaling = true
  residual_and_jacobian_together = true
[]

[Outputs]
  print_linear_residuals = false
  perf_graph = true
  execute_on = 'INITIAL TIMESTEP_END'
  exodus = true
  [csv]
    type = CSV
    sync_only = true
    sync_times = '0.25 0.5 0.75'
  []
[]
