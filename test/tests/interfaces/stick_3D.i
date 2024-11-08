[Mesh]
  [file]
    type = FileMeshGenerator
    file = mesh-3D.msh
  []
  [interface_gen]
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
  [traction_z]
    type = BVMechanicalInterface
    boundary = 'interface'
    component = Z
    variable = disp_z
    neighbor_var = disp_z
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
    [all]
      variable = 'disp_x disp_y disp_z'
      auto_direction = 'x'
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
  [no_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'bottom top'
    value = 0.0
  []
[]

[Materials]
  [elasticity]
    type = BVMechanicalMaterial
    displacements = 'disp_x disp_y disp_z'
    young_modulus = 10.0e+09
    poisson_ratio = 0.25
  []
  [interface]
    type = BVMechanicalInterfaceMaterial
    boundary = 'interface'
    displacements = 'disp_x disp_y disp_z'
    normal_stiffness = 1.0e+12
    tangent_stiffness = 1.0e+12
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
