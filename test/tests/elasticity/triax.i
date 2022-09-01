[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 4
    ny = 4
    nz = 8
    xmin = -0.25
    xmax = 0.25
    ymin = -0.25
    ymax = 0.25
    zmin = 0
    zmax = 1
  []
  [no_ux]
    type = ExtraNodesetGenerator
    input = gen
    coord = '0.0 -0.25 0.0; 0.0 0.25 0.0'
    new_boundary = 'no_disp_x'
  []
  [no_uy]
    type = ExtraNodesetGenerator
    input = no_ux
    coord = '-0.25 0.0 0.0; 0.25 0.0 0.0'
    new_boundary = 'no_disp_y'
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
  [strain_zz]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_zz]
    order = CONSTANT
    family = MONOMIAL
  []
  [pressure]
    order = CONSTANT
    family = MONOMIAL
  []
  [dev_stress]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [strain_zz_aux]
    type = BVStrainComponentAux
    variable = strain_zz
    index_i = z
    index_j = z
    execute_on = 'TIMESTEP_END'
  []
  [stress_zz_aux]
    type = BVStressComponentAux
    variable = stress_zz
    index_i = z
    index_j = z
    execute_on = 'TIMESTEP_END'
  []
  [pressure_aux]
    type = BVPressureAux
    variable = pressure
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [dev_stress_aux]
    type = BVMisesStressAux
    variable = dev_stress
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[BCs]
  [BVPressure]
    [pressure_conf]
      boundary = 'left right bottom top front'
      displacement_vars = 'disp_x disp_y'
      value = 10.0e+06
    []
  []
  [no_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'no_disp_x'
    value = 0.0
  []
  [no_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'no_disp_y'
    value = 0.0
  []
  [no_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'back'
    value = 0.0
  []
  [axial_load]
    type = FunctionDirichletBC
    variable = disp_z
    boundary = 'front'
    function = '-1.0e-02*t'
  []
[]

[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    young_modulus = 10.0e+09
    poisson_ratio = 0.25
    initial_stress = '-10.0e+06 -10.0e+06 -10.0e+06'
  []
[]

[Preconditioning]
  [hypre]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type -snes_atol'
    petsc_options_value = 'hypre boomeramg 1.0e-12'
  []
[]

[Executioner]
  type = Transient
  automatic_scaling = true
  solve_type = 'NEWTON'
  start_time = 0.0
  end_time = 2.0
  dt = 1.0
[]

[Outputs]
  exodus = true
[]