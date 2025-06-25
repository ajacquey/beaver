# Modified Lemaitre creep model
# See the first three figures of Fig.8 in Azabou et al. (2021)
# Parameters
# Units: stress in MPa, time in days, strain in m / m
E = 22126
nu = 0.2
alpha = 0.5812 
A1 = 0.1854
n1 = 2.1012
A2 = 3.7009
n2 = 6.7562
A = 155.6582
n = 11.989
B = 0.01918
m = 2.2195
Tr = 289
Ar = 1725
P = 5 
z = 0.4523
Nz = 0.0241
nz = 1.2644
Mz = 0.024
mz = 1.028

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
  [temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 293.15
  []
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
  [eqv_creep_strain_R]
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
  [vol_strain]
    order = CONSTANT
    family = MONOMIAL
  []
  [vol_strain_neg]
    order = CONSTANT
    family = MONOMIAL
  []
[]
  
[AuxKernels]
  [temp_aux]
    type = ConstantAux
    variable = temp
    value = 293.15
    execute_on = 'TIMESTEP_END'
  []
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
  [eqv_creep_strain_R_aux]
    type = ADMaterialRealAux
    variable = eqv_creep_strain_R
    property = eqv_creep_strain_R
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
  [vol_strain_aux]
    type = BVVolStrainAux
    variable = vol_strain
    execute_on = 'TIMESTEP_END'
  []
  [vol_strain_neg]
    type = ParsedAux
    coupled_variables = vol_strain 
    expression =  '-vol_strain'  
    variable = vol_strain_neg
  []
[]
  
[Functions]
  [strain_rate]
  type = ParsedFunction
  symbol_names = 'e_dot L'
  symbol_values = '1.0e-05 130e-03'
  expression = '-e_dot*L*t*86400' #converted to per days
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
      function = ${P} 
    []
    [pressure_front]
      boundary = 'front'
      displacement_vars = 'disp_x disp_y disp_z'
      function = ${P} 
    []
  []
  [strain_rate]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = strain_rate
  []
[]
  
[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    young_modulus = ${E}
    poisson_ratio = ${nu}
    initial_stress = '-${P} -${P} -${P}'
    inelastic_models = 'viscoelastic'
  []
  [viscoelastic]
    type = BVRTL2020ModelUpdate
    volumetric = true
    temperature = temp
    Tr = ${Tr}
    Ar = ${Ar}
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
  [eqv_strain_R]
    type = ElementAverageValue
    variable = eqv_creep_strain_R
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
  [vol_strain]
    type = ElementAverageValue
    variable = vol_strain_neg   #used parsedaux kernel to turn the vol strain into negative 
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
  end_time =  0.8873456790123455  #200 
  num_steps = 800
  #dt = 0.001
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