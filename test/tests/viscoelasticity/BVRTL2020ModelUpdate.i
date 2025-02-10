# Modified Lemaitre creep model
# See Blanco-Martin et al. (2023)
# Parameters
# Units: stress in MPa, time in days, strain in m / m
E = 28567
nu = 0.30
alpha = 0.2601
A1 = 0.0181
n1 = 1.162
A2 = 0.3986
n2 = 9.6768
A = 0.01
n = 13.5
B = 0.0
m = 2.0
P = 10.0
Q = 5.0
Nz = 0.0241
nz = 1.2644
Mz = 0.024
mz = 1.028
z = 0.4523

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
    property = eqv_creep_strain_L
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
  
[Functions]
  [loading2]
    type = ParsedFunction 
    expression = 'if(t<=20,5.5,if(t<=55,6,if(t<=76,7,if(t<=97,10,if(t<=118,15,if(t<=139,20,25))))))' 
  []
  [salt3loading]
    type = ParsedFunction 
    expression = 'if(t<=15,14.4,if(t<=29,20,24.4))' 
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
      function = salt3loading #loading2
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
  #[viscoelastic]
  #  type = BVModifiedLemaitreModelUpdate 
  #  alpha = ${alpha}
  #  kr1 = ${kr1}
  #  beta1 = ${beta1} 
  #  kr2 = ${kr2}
  #  beta2 = ${beta2}
  #[]
  #[viscoelastic]
  #  type = BVBlancoMartinModelUpdate 
  #  alpha = ${alpha}
  #  kr1 = ${kr1}
  #  beta1 = ${beta1} 
  #  kr2 = ${kr2}
  #  beta2 = ${beta2}
  #  A1 = ${A1}
  #  n1 = ${n1}
  #  A = ${A}
  #  n = ${n}
  #  B = 0.0
  #  m = ${n}
  #[]
  [viscoelastic]
    type = BVRTL2020ModelUpdate
   volumetric = true
    alpha = ${alpha}
    A1 = ${A1}
    n1 = ${n1}
    A2 = ${A2}
    n2 = ${n2}
    A = ${A}
    n = ${n}
    B = ${B}
    m = ${m}
    Nz = ${Nz}
    nz = ${nz}
    Mz = ${Mz}
    mz = ${mz}
    z = ${z}
  []
[]

[Postprocessors]
   [eqv_strain_rate]
    type = ElementAverageValue
    variable = eqv_strain_rate
    outputs = csv
  []
  [eqv_strain]
    type = ElementAverageValue
    variable = eqv_strain
    outputs = csv
  []
  [eqv_strain_L]
    type = ElementAverageValue
    variable = eqv_creep_strain_L
    outputs = csv
  []
  [strain_zz]
    type = ElementAverageValue
    variable = strain_yy
    outputs = csv
  []
  [stress_zz]
    type = ElementAverageValue
    variable = stress_yy
    outputs = csv
  []
  [q]
    type = ElementAverageValue
    variable = eqv_stress
    outputs = csv
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
  end_time = 50 # 50 days
  dt = 0.02
  timestep_tolerance = 1.0e-10
[]
  
[Outputs]
  perf_graph = true
  exodus = true
  [csv]
  type = CSV
  execute_on = 'timestep_end'
  []
[]