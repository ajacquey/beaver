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
    [stress_zz]
      order = CONSTANT
      family = MONOMIAL
    []
  []
  
  [AuxKernels]
    [stress_zz_aux]
      type = BVStressComponentAux
      variable = stress_zz
      index_i = z
      index_j = z
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
    dt = 0.05
    solve_type = 'NEWTON'
    dtmin = 0.05
    num_steps = 1
  []
  
  [Outputs]
    exodus = true
  []
  