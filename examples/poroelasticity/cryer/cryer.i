# Cryer's problem
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 3.3 Cryer's problem
# 
# Time dimension is t = Cv * t / a^2
# Space dimension is x = x / a

[Mesh]
  type = FileMesh
  file = 'mesh.msh'
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

# [AuxVariables]
#   [ur]
#     order = FIRST
#     family = LAGRANGE
#   []
# []

# [AuxKernels]
#   [ur_aux]
#     type = FunctionAux
#     variable = ur
#     function = disp_r_fct
#     execute_on = 'TIMESTEP_END'
#   []
# []

# [Functions]
#   [disp_r_fct]
#     type = ParsedFunction
#     value = 'disp_x^2+disp_y^2+disp_z^2'
#   []
# []

[BCs]
  [confine_x]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = 'left'
  []
  [confine_y]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'back'
  []
  [confine_z]
    type = DirichletBC
    variable = disp_z
    value = 0
    boundary = 'bottom'
  []
  [BVPressure]
    [out_pressure]
      boundary = 'out'
      displacement_vars = 'disp_x disp_y disp_z'
      value = 1.0
    []
  []
  [out_drained]
    type = DirichletBC
    variable = pf
    value = 0
    boundary = 'out'
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

[Modules]
  [FluidProperties]
    [simple_fluid]
      type = SimpleFluidProperties
      density0 = 1.0
      viscosity = 3.636363636363636 # scaled by effective time
      bulk_modulus = 8.0
    []
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
                           newtonls 1e-10 1e-10 200 basic
                           201'
  []
[]

[Postprocessors]
  # [U]
  #   type = SideAverageValue
  #   boundary = top
  #   outputs = 'csv_t'
  #   variable = disp_y
  # []
  [Pc]
    type = PointValue
    outputs = 'csv_t'
    point = '0.0 0.0 0.0'
    variable = pf
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
  # [csv_p]
  #   type = CSV
  #   sync_only = true
  #   sync_times = '0.01 0.1 0.5 1.0 2.0'
  # []
  [csv_t]
    type = CSV
  []
[]
