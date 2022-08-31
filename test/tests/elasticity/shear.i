[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 8
  ny = 8
  nz = 2
  xmin = 0
  xmax = 10
  ymin = 0
  ymax = 10
  zmin = 0
  zmax = 2.5
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
    component = 0
    variable = disp_x
  []
  [stress_y]
    type = BVStressDivergence
    component = 1
    variable = disp_y
  []
  [stress_z]
    type = BVStressDivergence
    component = 2
    variable = disp_z
  []
[]

[AuxVariables]
  [strain_xy]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_xy]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [strain_xy_aux]
    type = BVStrainComponentAux
    variable = strain_xy
    index_i = 0
    index_j = 1
    execute_on = 'TIMESTEP_END'
  []
  [stress_xy_aux]
    type = BVStressComponentAux
    variable = stress_xy
    index_i = 0
    index_j = 1
    execute_on = 'TIMESTEP_END'
  []
[]

[Functions]
  [disp_y_func]
    type = ParsedFunction
    value = 'm*t*x'
    vars = 'm'
    vals = '-0.1'
  []
[]

[BCs]
  [no_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'left right bottom top front back'
    value = 0.0
  []
  [no_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'left right bottom top front back'
    value = 0.0
  []
  [disp_y_plate]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'left right bottom top front back'
    function = disp_y_func
  []
[]

[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    young_modulus = 10.0e+09
    poisson_ratio = 0.25
  []
[]

[Preconditioning]
  [hypre]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type'
    petsc_options_value = 'hypre boomeramg'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  start_time = 0.0
  end_time = 2.0
  dt = 1.0
[]

[Outputs]
  exodus = true
[]
