# Terzaghi's problem of consolodation of a soil layer
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 2.2 Terzaghi's problem
# 
# Time dimension is t = Cv * t / h^2
# Space dimension is z = z / h

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 5
  ny = 100
  nz = 5
  xmin = -0.05
  xmax = 0.05
  ymin = 0
  ymax = 1
  zmin = -0.05
  zmax = 0.05
[]

[Variables]
  [pf]
    order = FIRST
    family = LAGRANGE
  []
  [disp_x]
    order = FIRST
    family = LAGRANGE
  []
  [disp_y]
    order = FIRST
    family = LAGRANGE
  []
  [disp_z]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [fluid_time_derivative]
    type = BVSinglePhaseTimeDerivative
    variable = pf
  []
  [darcy]
    type = BVSinglePhaseDarcy
    variable = pf
  []
  [stress_x]
    type = BVStressDivergence
    component = x
    variable = disp_x
    fluid_pressure = pf
  []
  [stress_y]
    type = BVStressDivergence
    component = y
    variable = disp_y
    fluid_pressure = pf
  []
  [stress_z]
    type = BVStressDivergence
    component = z
    variable = disp_z
    fluid_pressure = pf
  []
[]

[BCs]
  [confine_x]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = 'left right'
  []
  [confine_z]
    type = DirichletBC
    variable = disp_z
    value = 0
    boundary = 'front back'
  []
  [base_fixed]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'bottom'
  []
  [top_load]
    type = NeumannBC
    variable = disp_y
    value = -1
    boundary = 'top'
  []
  [topdrained]
    type = DirichletBC
    variable = pf
    value = 0
    boundary = 'top'
  []
[]

[Materials]
  [mechanical]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    bulk_modulus = 4
    shear_modulus = 3
  []
  [fluid_flow_mat]
    type = BVSinglePhaseFlowMaterial
  []
  [single_phase_poromech]
    type = BVPoroMechanicsMaterial
    biot_coefficient = 0.6
  []
  [permeability]
    type = BVConstantPermeability
    permeability = 1.5e-02 # scaled by effective space
  []
  [porosity]
    type = BVConstantPorosity
    porosity = 0.1
  []
  [fluid]
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
    viscosity = 0.13953488372093023 # scaled by effective time
    bulk_modulus = 8.0
  []
[]

[Preconditioning]
  [precond]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -snes_linesearch_type'
    petsc_options_value = 'fgmres ilu 1e-08 1e-10 100 basic'
  []
[]

[Postprocessors]
  [U]
    type = PointValue
    outputs = 'csv_u'
    point = '0 1 0'
    variable = disp_y
  []
[]

[VectorPostprocessors]
  [line_pf]
    type = LineValueSampler
    variable = pf
    start_point = '0.0 0.0 0.0'
    end_point = '0.0 1.0 0.0'
    num_points = 100
    sort_by = 'y'
    outputs = 'csv_p'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  automatic_scaling = true
  start_time = 0
  end_time = 10
  [TimeStepper]
    type = LogConstantDT
    first_dt = 0.0001
    log_dt = 0.022
  []
[]

[Outputs]
  perf_graph = true
  execute_on = 'TIMESTEP_END'
  print_linear_residuals = false
  exodus = true
  [csv_p]
    type = CSV
    sync_only = true
    sync_times = '0.001 0.005 0.01 0.05 0.1 0.2 0.5 1.0'
  []
  [csv_u]
    type = CSV
  []
[]
