[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 3
  ny = 3
  nz = 3
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
  [stress_zz]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [stress_zz_aux]
    type = BVStressComponentAux
    variable = stress_zz
    index_i = 2
    index_j = 2
    execute_on = 'TIMESTEP_END'
  []
[]

[BCs]
  [symmy]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
  [symmx]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [symmz]
    type = DirichletBC
    variable = disp_z
    boundary = back
    value = 0
  []
  [tdisp]
    type = DirichletBC
    variable = disp_z
    boundary = front
    value = 0.1
  []
[]

[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    bulk_modulus = 8.0e+09
    shear_modulus = 3.5e+09
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  dt = 0.05
  solve_type = 'NEWTON'
  petsc_options_iname = -pc_hypre_type
  petsc_options_value = boomeramg
  dtmin = 0.05
  num_steps = 1
[]

[Outputs]
  exodus = true
[]
