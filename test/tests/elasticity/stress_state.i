[Mesh]
  [mesh]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = -1
    xmax = 1
    ymin = -0.5
    ymax = 0.5
    nx = 20
    ny = 10
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
[]

[AuxVariables]
  # Mechanical ouputs
  [pressure]
    order = CONSTANT
    family = MONOMIAL
  []
  [dev_stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [normal_stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [shear_stress]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  # Mechanical outputs
  [pressure_aux]
    type = BVPressureAux
    variable = pressure
    execute_on = 'TIMESTEP_END'
  []
  [dev_stress_aux]
    type = BVMisesStressAux
    variable = dev_stress
    execute_on = 'TIMESTEP_END'
  []
  [normal_stress_aux]
    type = BVFaultNormalStressAux
    variable = normal_stress
    normal = '0 1 0'
    execute_on = 'TIMESTEP_END'
  []
  [shear_stress_aux]
    type = BVFaultShearStressAux
    variable = shear_stress
    normal = '0 1 0'
    execute_on = 'TIMESTEP_END'
  []
[]

[BCs]
  [BVPressure]
    [pressure]
      boundary = 'top bottom left right'
      displacement_vars = 'disp_x disp_y'
      value = 2.0
    []
  []
  [BVShearTraction]
    [shear_stress_bottom_top]
      boundary = 'bottom top'
      displacement_vars = 'disp_x disp_y'
      value = 1.0
    []
    [shear_stress_left_right]
      boundary = 'left right'
      displacement_vars = 'disp_x disp_y'
      value = -1.0
    []
  []
[]

[Materials]
  [mech_elastic]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y'
    bulk_modulus = 33.33e+03
    shear_modulus = 20.0e+03
    initial_stress = '-2.0 -2.0 -2.0 0.0 0.0 1.0'
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
