[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 400
  xmin = 0
  xmax = 1
[]

[Variables]
  [pf]
    order = CONSTANT
    family = MONOMIAL
    fv = true
  []
  [c]
    order = CONSTANT
    family = MONOMIAL
    fv = true
  []
[]

[ICs]
  [pf_ic]
    type = FunctionIC
    variable = pf
    function = '1-x'
  []
[]

[FVKernels]
  # [time_derivative_p]
  #   type = BVFVSinglePhaseTimeDerivative
  #   variable = pf
  # []
  [darcy_p]
    type = BVFVSinglePhaseDarcy
    variable = pf
  []
  [time_derivative_c]
    type = BVFVSinglePhaseSoluteTimeDerivative
    variable = c
  []
  [darcy_c]
    type = BVFVSinglePhaseSoluteDarcy
    variable = c
    fluid_pressure = pf
  []
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
    fluid_pressure = pf
    component = x
    execute_on = 'TIMESTEP_END'
  []
[]

[FVBCs]
  [p_left]
    type = FVDirichletBC
    variable = pf
    boundary = 'left'
    value = 1
  []
  [p_right]
    type = FVDirichletBC
    variable = pf
    boundary = 'right'
    value = 0
  []
  [c_left]
    type = FVDirichletBC
    variable = c
    boundary = 'left'
    value = 1
  []
  [c_right]
    type = FVDirichletBC
    variable = c
    boundary = 'right'
    value = 0
  []
[]

[Materials]
  [fluid_flow_mat]
    type = BVSinglePhaseFlowMaterial
  []
  [permeability]
    type = BVConstantPermeability
    permeability = 1.0
  []
  [porosity]
    type = BVConstantPorosity
    porosity = 1.0
  []
  [solute_mat]
    type = BVSinglePhaseFlowSoluteMaterial
  []
  [dispersion]
    type = BVConstantDispersion
    dispersion = 1.0e-03
  []
  [fluid_properties]
    type = BVFluidProperties
    fluid_pressure = pf
    temperature = 273
    fp = simple_fluid
  []
[]

[FluidProperties]
  [simple_fluid]
    type = SimpleFluidProperties
    density0 = 1.0
    viscosity = 1.0
  []
[]

[VectorPostprocessors]
  [line_c]
    type= LineValueSampler
    num_points = 400
    start_point = '0 0 0'
    end_point = '1 0 0'
    sort_by = x
    variable = c
    execute_on = 'TIMESTEP_END'
    outputs = csv
  []
[]

[Preconditioning]
  [hypre]
    type = SMP
    full = true
    petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-snes_linesearch_type -pc_type -pc_hypre_type'
    petsc_options_value = 'bt hypre boomeramg'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  # scheme = 'BDF2'
  automatic_scaling = true
  residual_and_jacobian_together = true
  start_time = 0
  end_time = 0.6
  num_steps = 600
[]

[Outputs]
  print_linear_residuals = false
  perf_graph = true
  file_base = 'outputs/advection-diffusion'
  exodus = true
  [csv]
    type = CSV
    sync_only = true
    sync_times = '0.2 0.4 0.6'
  []
[]
