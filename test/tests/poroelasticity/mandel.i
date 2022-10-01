# Mandel's problem
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 3.2 Mandel's problem
# 
# Time dimension is t = Cv * t / h^2
# Space dimension is x = x / a

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 1.0
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
  [sides_drained]
    type = DirichletBC
    variable = pf
    value = 0
    boundary = 'right'
  []
[]

[Materials]
  [mechanical]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y'
    bulk_modulus = 1
    shear_modulus = 0.6
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
      viscosity = 3.821656050955414 # scaled by effective time
      bulk_modulus = 8.0
    []
  []
[]

[Preconditioning]
  [asm]
    type = SMP
    petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-ksp_type -ksp_rtol -ksp_max_it
                           -pc_type
                           -sub_pc_type
                           -snes_type -snes_atol -snes_rtol -snes_max_it
                           -ksp_gmres_restart'
    petsc_options_value = 'fgmres 1e-10 100
                           asm
                           ilu
                           newtonls 1e-12 1e-08 100
                           201'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  automatic_scaling = true
  start_time = 0
  end_time = 0.01
  [TimeStepper]
    type = LogConstantDT
    first_dt = 0.001
    log_dt = 0.044
  []
[]

[Outputs]
  perf_graph = true
  execute_on = 'TIMESTEP_END'
  print_linear_residuals = false
  exodus = true
[]
