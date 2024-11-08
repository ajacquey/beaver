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
  []
  [traction_y]
    type = BVMechanicalInterface
    boundary = 'interface'
    component = y
    variable = disp_y
    neighbor_var = disp_y
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
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
  []
  [normal_stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [shear_stress]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [strain_xy_aux]
    type = BVStrainComponentAux
    variable = strain_xy
    index_i = x
    index_j = y
    execute_on = 'TIMESTEP_END'
  []
  [stress_xy_aux]
    type = BVStressComponentAux
    variable = stress_xy
    index_i = x
    index_j = y
    execute_on = 'TIMESTEP_END'
  []
  [stress_yy_aux]
    type = BVStressComponentAux
    variable = stress_yy
    index_i = y
    index_j = y
    execute_on = 'TIMESTEP_END'
  []
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
[]

[Functions]
  [disp_x_func]
    type = ParsedFunction
    expression = 'm*t*y'
    symbol_names = 'm'
    symbol_values = '0.1'
  []
[]

[BCs]
  [Periodic]
    [left_right]
      auto_direction = x
      primary = 'left'
      secondary = 'right'
      variable = 'disp_x disp_y'
    []
  []
  [no_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom top'
    value = 0.0
  []
  [disp_x_plate]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'bottom top'
    function = disp_x_func
  []
[]

[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y'
    young_modulus = 10.0
    poisson_ratio = 0.25
    initial_stress = '0.0 1.6 0.0'
  []
  [interface]
    type = BVMechanicalInterfaceMaterial
    boundary = 'interface'
    displacements = 'disp_x disp_y'
    normal_stiffness = 1.0e+03
    tangent_stiffness = 1.0e+03
    friction_model = 'constant_friction'
  []
  [constant_friction]
    type = BVConstantFrictionUpdate
    friction = 0.5
  []
[]

# [Preconditioning]
#   [hypre]
#     type = SMP
#     full = true
#     petsc_options_iname = '-pc_type -pc_hypre_type'
#     petsc_options_value = 'hypre boomeramg'
#   []
# []

[Preconditioning]
  [asm]
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
                           newtonls 1e-12 1e-08 100 basic
                           201'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  start_time = 0.0
  end_time = 4.0
  dt = 1.0
[]

[Outputs]
  exodus = true
[]
