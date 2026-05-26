[Mesh]
  [mesh]
    type = GeneratedMeshGenerator
    dim = 3
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    zmin = -0.5
    zmax = 0.5
    nx = 3
    ny = 3
    nz = 3
  []
[]

[Variables]
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
  [mech_x]
    type = BVStressDivergence
    variable = disp_x
    component = x
  []
  [mech_y]
    type = BVStressDivergence
    variable = disp_y
    component = y
  []
  [mech_z]
    type = BVStressDivergence
    variable = disp_z
    component = z
  []
[]

[AuxVariables]
  [temp]
    order = FIRST
    family = LAGRANGE
  []
  # Mechanical ouputs
  [pressure]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [temp_aux]
    type = ConstantAux
    variable = temp
    value = 100
    execute_on = 'TIMESTEP_BEGIN'
  []
  # Mechanical outputs
  [pressure_aux]
    type = BVPressureAux
    variable = pressure
    execute_on = 'TIMESTEP_END'
  []
[]

[BCs]
  [x_fixed]
    type = DirichletBC
    variable = disp_x
    boundary = 'left right'
    value = 0
  []
  [y_fixed]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom top'
    value = 0
  []
  [z_fixed]
    type = DirichletBC
    variable = disp_z
    boundary = 'front back'
    value = 0
  []
[]

[Materials]
  [mech_elastic]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    temperature = temp
    bulk_modulus = 1.0e+04
    shear_modulus = 2.0e+04
    thermal_expansion_coefficient = 1.0e-04
  []
[]

[Preconditioning]
  active = 'hypre'
  [hypre]
    type = SMP
    full = true
    petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-pc_type -pc_hypre_type
                           -snes_atol -snes_rtol -snes_stol -snes_max_it -snes_linesearch_type'
    petsc_options_value = 'hypre boomeramg
                           1.0e-12 1.0e-12 0 20
                           basic'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  automatic_scaling = true
  start_time = 0.0
  end_time = 1.0
  dt = 1.0
[]

[Outputs]
  execute_on = 'TIMESTEP_END'
  print_linear_residuals = false
  perf_graph = true
  exodus = true
[]