[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1000
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
    initial_condition = 0.001
  []
  # [sn]
  #   order = CONSTANT
  #   family = MONOMIAL
  #   fv = true
  #   initial_condition = 0.999
  # []
[]

[FVKernels]
  # Total pressure
  [time_p]
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

# [DiracKernels]
#   [sw]
#     type = ConstantPointSource
#     point = '0 0 0'
#     variable = sw
#     value = 1.0
#   []
# []

[FVBCs]
  [leftinflux_pw]
    type = FVDirichletBC
    variable = p
    value = 1.0
    boundary = 'left'
  []
  [right_pw]
    type = FVDirichletBC
    variable = p
    value = 0.0
    boundary = 'right'
  []
  [leftinflux_sw]
    type = FVDirichletBC
    variable = sw
    value = 1.0
    boundary = 'left'
  []
  [right_sw]
    type = FVDirichletBC
    variable = sw
    value = 0.001
    boundary = 'right'
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
    exponent = 2.0
    reference_capillary_pressure = 1.0e-05
    output_properties = 'capillary_pressure'
    outputs = exodus
  []
  [rel_perm]
    type = BVRelativePermeabilityPowerLaw
    saturation_w = sw
    exponent = 2.0
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
    viscosity = 0.5
  []
[]

[Preconditioning]
  [hypre]
    type = SMP
    full = true
    petsc_options = '-snes_ksp_ew -snes_converged_reason -ksp_converged_reason'
    petsc_options_iname = '-pc_type -pc_hypre_type
                           -snes_atol -snes_max_it -snes_linesearch_type'
    petsc_options_value = 'hypre boomeramg
                           1.0e-04 1000 basic'
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  start_time = 0
  end_time = 0.3
  num_steps = 50
  timestep_tolerance = 1.0e-05
  automatic_scaling = true
[]

[Outputs]
  print_linear_residuals = false
  perf_graph = true
  execute_on = 'INITIAL TIMESTEP_END'
  exodus = true
[]