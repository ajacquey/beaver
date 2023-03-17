# Terzaghi's problem of consolodation of a soil layer
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 2.2 Terzaghi's problem
# 
# Time dimension is t = Cv * t / h^2
# Space dimension is z = z / h

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 1
  ny = 10
  xmin = -1.0
  xmax = 1.0
  ymin = 0
  ymax = 1
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
    boundary = 'left right'
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
    displacements = 'disp_x disp_y'
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
    petsc_options_value = 'bcgs bjacobi 1E-14 1E-10 100 basic'
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
    first_dt = 0.0001
    log_dt = 0.044
  []
[]

[Outputs]
  perf_graph = true
  execute_on = 'TIMESTEP_END'
  print_linear_residuals = false
  exodus = true
[]
