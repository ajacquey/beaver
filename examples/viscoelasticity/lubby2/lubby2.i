[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 20
  ny = 20
  nz = 2
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 0.1
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
  [stress_x]
    type = BVStressDivergence
    component = x
    variable = disp_x
  []
  [stress_y]
    type = BVStressDivergence
    component = y
    variable = disp_y
  []
  [stress_z]
    type = BVStressDivergence
    component = z
    variable = disp_z
  []
[]

[AuxVariables]
  [eqv_stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [eqv_strain]
    order = CONSTANT
    family = MONOMIAL
  []
  [eqv_strain_rate]
    order = CONSTANT
    family = MONOMIAL
  []
  [Kelvin_creep_strain]
    order = CONSTANT
    family = MONOMIAL
  []
  [strain_yy]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [eqv_stress_aux]
    type = BVMisesStressAux
    variable = eqv_stress
    execute_on = 'TIMESTEP_END'
  []
  [eqv_strain_aux]
    type = BVEqvStrainAux
    variable = eqv_strain
    execute_on = 'TIMESTEP_END'
  []
  [eqv_strain_rate_aux]
    type = BVEqvStrainRateAux
    variable = eqv_strain_rate
    execute_on = 'TIMESTEP_END'
  []
  [Kelvin_creep_strain_aux]
    type = ADMaterialRealAux
    variable = Kelvin_creep_strain
    property = eqv_Kelvin_creep_strain
    execute_on = 'TIMESTEP_END'
  []
  [strain_yy_aux]
    type = BVStrainComponentAux
    variable = strain_yy
    index_i = y
    index_j = y
    execute_on = 'TIMESTEP_END'
  []
[]

[BCs]
  [no_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0.0
  []
  [no_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0.0
  []
  [no_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'front back'
    value = 0.0
  []
  [BVPressure]
    [pressure_right]
      boundary = 'right'
      displacement_vars = 'disp_x disp_y disp_z'
      value = 1.0
    []
    [pressure_top]
      boundary = 'top'
      displacement_vars = 'disp_x disp_y disp_z'
      value = -1.0
    []
  []
[]

[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    bulk_modulus = 1.0
    shear_modulus = 2.0
    initial_stress = '-1.0 1.0 0.0'
    inelastic_models = 'viscoelastic'
  []
  [viscoelastic]
    type = BVLubby2ModelUpdate
    viscosity_maxwell = 10.0
    viscosity_kelvin = 1.0
    shear_modulus_kelvin = 1.0
    sigma_0 = 1.0
    m_1 = 0.327
    m_2 = 0.267
    m_G = 0.254
  []
[]

[Preconditioning]
  active = 'hypre'
  [hypre]
    type = SMP
    full = true
    petsc_options = '-snes_ksp_ew -snes_converged_reason -ksp_converged_reason  -ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-pc_type -pc_hypre_type
                           -snes_atol -snes_rtol -snes_stol -snes_max_it -snes_linesearch_type'
    petsc_options_value = 'hypre boomeramg
                           1.0e-10 1.0e-12 0 20
                           basic'
  []
[]

[Postprocessors]
  [e]
    type = ElementAverageValue
    variable = eqv_strain
    outputs = csv
  []
  [e_rate]
    type = ElementAverageValue
    variable = eqv_strain_rate
    outputs = csv
  []
  [Kelvin_creep]
    type = ElementAverageValue
    variable = Kelvin_creep_strain
    outputs = csv
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  start_time = 0.0
  end_time = 10.0
  dt = 0.05
[]

[Outputs]
  perf_graph = true
  execute_on = 'TIMESTEP_END'
  print_linear_residuals = false
  exodus = true
  [csv]
    type = CSV
  []
[]
