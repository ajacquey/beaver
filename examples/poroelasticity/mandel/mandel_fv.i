# Mandel's problem
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 3.2 Mandel's problem
# Cheng and Detournay (1988), A direct boundary element method for plane strain poroelasticity
# 
# Time dimension is t = Cv * t / a^2
# Space dimension is x = x / a

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 20
  ny = 20
  nz = 20
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 1.0
  zmin = 0.0
  zmax = 1.0
[]

[Variables]
  [pf]
    type = MooseVariableFVReal
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

[FVKernels]
  [fluid_time_derivative]
    type = BVFVSinglePhaseTimeDerivative
    variable = pf
  []
  [darcy]
    type = BVFVSinglePhaseDarcy
    variable = pf
  []
[]

[BCs]
  [confine_x]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = 'left'
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
  [confine_z]
    type = DirichletBC
    variable = disp_z
    value = 0
    boundary = 'front back'
  []
[]

[FVBCs]
  [sides_drained]
    type = FVDirichletBC
    variable = pf
    value = 0
    boundary = 'right'
  []
[]

[Materials]
  [mechanical]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    bulk_modulus = 1
    poisson_ratio = 0.25
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
    permeability = 1.5 # scaled by effective space
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
    viscosity = 3.636363636363636 # scaled by effective time
    bulk_modulus = 8.0
  []
[]

[Preconditioning]
  [asm]
    type = SMP
    petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-ksp_type
                           -pc_type
                           -sub_pc_type
                           -snes_type -snes_atol -snes_rtol -snes_max_it -snes_linesearch_type
                           -ksp_gmres_restart'
    petsc_options_value = 'fgmres
                           asm
                           ilu
                           newtonls 1e-08 1e-10 120 basic
                           201'
  []
[]

[Postprocessors]
  [U]
    type = SideAverageValue
    boundary = top
    outputs = 'csv_t'
    variable = disp_y
  []
  [P1]
    type = PointValue
    outputs = 'csv_t'
    point = '0.0 0.0 0.0'
    variable = pf
  []
  [P2]
    type = PointValue
    outputs = 'csv_t'
    point = '0.25 0.0 0.0'
    variable = pf
  []
  [P3]
    type = PointValue
    outputs = 'csv_t'
    point = '0.5 0.0 0.0'
    variable = pf
  []
  [P4]
    type = PointValue
    outputs = 'csv_t'
    point = '0.75 0.0 0.0'
    variable = pf
  []
[]

[VectorPostprocessors]
  [line_pf]
    type = LineValueSampler
    variable = pf
    start_point = '0.0 0.0 0.0'
    end_point = '1.0 0.0 0.0'
    num_points = 20
    sort_by = 'x'
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
    first_dt = 0.001
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
    sync_times = '0.01 0.1 0.5 1.0 2.0'
  []
  [csv_t]
    type = CSV
  []
[]
