# Modified Lemaitre creep model
# See Blanco-Martin et al. (2024)
# Parameters
# Units: stress in MPa, time in days, strain in m / m
E = 12000
nu = 0.3
alpha = 0.326
kr1 = 0.7
beta1 = 2.922
kr2 = 0.009
beta2 = 0.867
P = 5.0
Q = 5.0
Q1 = 5.5

[Mesh]
  type = GeneratedMesh
  dim = 3
  # nx = 5
  # ny = 10
  # nz = 5
  nx = 1
  ny = 1
  nz = 1
  xmin = 0
  xmax = 65e-03
  ymin = 0
  ymax = 130e-03
  zmin = 0
  zmax = 65e-03
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
  [eqv_creep_strain_L]
    order = CONSTANT
    family = MONOMIAL
  []
  [strain_yy]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_yy]
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
  [eqv_creep_strain_L_aux]
    type = ADMaterialRealAux
    variable = eqv_creep_strain_L
    property = eqv_creep_strain
    execute_on = 'TIMESTEP_END'
  []
  [strain_zz_aux]
    type = BVStrainComponentAux
    variable = strain_yy
    index_i = y
    index_j = y
    execute_on = 'TIMESTEP_END'
  []
  [stress_zz_aux]
    type = BVStressComponentAux
    variable = stress_yy
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
    boundary = 'back'
    value = 0.0
  []
  [BVPressure]
    [pressure_right]
      boundary = 'right'
      displacement_vars = 'disp_x disp_y disp_z'
      value = ${P}
    []
    [pressure_front]
      boundary = 'front'
      displacement_vars = 'disp_x disp_y disp_z'
      value = ${P}
    []
    [pressure_top]
      boundary = 'top'
      displacement_vars = 'disp_x disp_y disp_z'
      function = ${Q1}
    []
  []
[]
  
[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    young_modulus = ${E}
    poisson_ratio = ${nu}
    initial_stress = '-${P} -${Q} -${P}'
    inelastic_models = 'viscoelastic'
  []
  # [viscoelastic]
  #   type = BVBlancoMartinModelUpdate 
  #   alpha = ${alpha}
  #   kr1 = ${kr1}
  #   beta1 = ${beta1} 
  #   kr2 = ${kr2}
  #   beta2 = ${beta2}
  #   A1 = ${A1}
  #   n1 = ${n1}
  #   A = ${A}
  #   n = ${n}
  #   B = 0.0
  #   m = ${n}
  # []
  [viscoelastic]
    type = BVModifiedLemaitreModelUpdate 
    alpha = ${alpha}
    kr1 = ${kr1}
    beta1 = ${beta1} 
    kr2 = ${kr2}
    beta2 = ${beta2}
  []
[]

[Preconditioning]
  active = 'hypre'
  [hypre]
    type = SMP
    full = true
    petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-pc_type -pc_hypre_type
                           -snes_atol -snes_rtol -snes_stol -snes_max_it
                           -snes_linesearch_type'
    petsc_options_value = 'hypre boomeramg
                           1.0e-10 1.0e-12 0 20
                           basic'
  []
  [superlu]
    type = SMP
    full = true
    petsc_options = '-snes_ksp_ew -snes_converged_reason -ksp_converged_reason -ksp_gmres_modifiedgramschmidt -ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-snes_type
                           -snes_atol -snes_rtol -snes_max_it
                           -pc_type -pc_factor_mat_solver_package
                           -snes_linesearch_type'
    petsc_options_value = 'newtonls
                           1e-10 1e-12 50
                           lu superlu_dist
                           l2'
  []
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
                           newtonls 1e-10 1e-10 120 basic
                           201'
  []
[]
  
[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  automatic_scaling = true
  start_time = 0.0
  end_time = 1
  dt = 0.02
  timestep_tolerance = 1.0e-10
[]
  
[Outputs]
  perf_graph = true
  exodus = true
[]