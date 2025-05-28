# Input file for purely Lemaitre creep model based on RTL2020 model (Azabou et al. (2021)). 
# Note: Parameter A is set to zero to ensure the Munson-Dawson component is zero.
# Also, volumetric part is set to false and parameter z is set to zero.  
# Parameters
# Units: stress in MPa, time in days, strain in m / m

E = 6000
nu = 0.44
alpha = 0.1 
A2 = 1
n2 = 14.8
Ar = 1
Tr = 289
# parameters for Munson-Dawson part
A1 = 1
n1 = 1
A = 0.0    #0.001
n = 10
B = 0.0
m = 2.2195
P = 1
# parameters for volumetric part
z = 0.0
Nz = 0.0241
nz = 1.2644
Mz = 0.024
mz = 1.028

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
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
  [temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 289
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
  [strain_yy]
    order = CONSTANT
    family = MONOMIAL
  []
  [eqv_creep_strain_L]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [temp_aux]
    type = ConstantAux
    variable = temp
    value = 289
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
  [strain_yy_aux]
    type = BVStrainComponentAux
    variable = strain_yy
    index_i = y
    index_j = y
    execute_on = 'TIMESTEP_END'
  []
  [eqv_creep_strain_L_aux]
    type = ADMaterialRealAux
    variable = eqv_creep_strain_L
    property = eqv_creep_strain_L
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
    young_modulus = ${E}
    poisson_ratio = ${nu}
    initial_stress = '-${P} ${P} 0.0'
    inelastic_models = 'viscoelastic'
  []
  [viscoelastic]
    type = BVRTL2020ModelUpdate
    volumetric = false
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
  [eqv_strain_L]
    type = ElementAverageValue
    variable = eqv_creep_strain_L
    outputs = csv
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  start_time = 0.0
  end_time = 1 #10.0
  dt = 0.01
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
