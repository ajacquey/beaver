
# Fault stress parameter
f = 0.5
sigma = 1.0
Delta_p = 0.4
alpha = 0.04
T = 0.4
tau = ${fparse f * sigma * (1.0 - T * Delta_p / sigma)}

[Mesh]
  [file]
    type = FileMeshGenerator
    file = mesh-2D.msh
  []
  [fault_gen]
    type = BVFaultInterfaceGenerator
    input = file
    sidesets = 'interface'
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
[]

[InterfaceKernels]
  [traction_x]
    type = BVMechanicalInterface
    boundary = 'interface'
    component = x
    variable = disp_x
    neighbor_var = disp_x
    fluid_pressure = pressure
  []
  [traction_y]
    type = BVMechanicalInterface
    boundary = 'interface'
    component = y
    variable = disp_y
    neighbor_var = disp_y
    fluid_pressure = pressure
  []
[]

[AuxVariables]
  [normal_stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [shear_stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [pressure]
    order = FIRST
    family = LAGRANGE
  []
  [slip_rate]
    order = CONSTANT
    family = MONOMIAL
  []
  [slip]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [normal_stress_aux]
    type = BVFaultNormalStressAux
    variable = normal_stress
    boundary = 'interface'
    execute_on = 'TIMESTEP_END'
  []
  [shear_stress_aux]
    type = BVFaultShearStressAux
    variable = shear_stress
    boundary = 'interface'
    execute_on = 'TIMESTEP_END'
  []
  [pressure_aux]
    type = FunctionAux
    function = pressure_fct
    variable = pressure
    boundary = 'interface'
    execute_on = 'TIMESTEP_BEGIN TIMESTEP_END'
  []
  [slip_rate_aux]
    type = BVFaultSlipRateAux
    variable = slip_rate
    boundary = 'interface'
    execute_on = 'TIMESTEP_END'
  []
  [slip_aux]
    type = BVFaultSlipAux
    variable = slip
    boundary = 'interface'
    execute_on = 'TIMESTEP_END'
  []
[]

[Functions]
  [pressure_fct]
    type = ParsedFunction
    expression = 'if(t<=0,0.0,${Delta_p}*(1.0-erf(abs(x)/sqrt(${alpha}*t))))' 
  []
[]

[BCs]
  [BVPressure]
    [pressure]
      boundary = 'bottom top left right'
      displacement_vars = 'disp_x disp_y'
      value = ${sigma}
    []
  []
  [BVShearTraction]
    [shear_stress_bottom_top]
      boundary = 'bottom top'
      displacement_vars = 'disp_x disp_y'
      value = +${tau}
    []
    [shear_stress_left_right]
      boundary = 'left right'
      displacement_vars = 'disp_x disp_y'
      value = -${tau}
    []
  []
  [no_disp_x]
    type = DirichletBC
    boundary = 'no_disp_x'
    value = 0.0
    variable = 'disp_x'
  []
  [no_disp_y]
    type = DirichletBC
    boundary = 'no_disp_y'
    value = 0.0
    variable = 'disp_y'
  []
[]

[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y'
    shear_modulus = 6.667e+02
    bulk_modulus = 6.667e+02
    initial_stress = '-${sigma} -${sigma} -${sigma} 0.0 0.0 +${tau}'
  []
  [interface]
    type = BVMechanicalInterfaceMaterial
    boundary = 'interface'
    displacements = 'disp_x disp_y'
    normal_stiffness = 6.667e+05
    tangent_stiffness = 6.667e+05
    friction_model = 'constant_friction'
  []
  [constant_friction]
    type = BVConstantFrictionUpdate
    friction = ${f}
  []
[]

[Preconditioning]
  active = 'asm_ilu'
  [asm_ilu]
    type = SMP
    petsc_options = '-snes_ksp_ew'
    petsc_options_iname = '-ksp_type -ksp_rtol -ksp_max_it
                           -pc_type
                           -sub_pc_type
                           -snes_type -snes_atol -snes_rtol -snes_max_it -snes_linesearch_type
                           -ksp_gmres_restart'
    petsc_options_value = 'fgmres 1e-10 100
                           asm
                           ilu
                           newtonls 1.0e-08 1e-08 100 l2
                           201'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  automatic_scaling = true
  start_time = 0.0
  end_time = 2
  num_steps = 5
[]

[Outputs]
  execute_on = 'TIMESTEP_END'
  exodus = true
[]
