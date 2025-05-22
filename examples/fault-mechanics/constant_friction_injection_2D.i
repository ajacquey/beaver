# Fault stress parameter
f = 1.0
sigma = 1.0
T = 0.4
tau = ${fparse sigma - T}

[Mesh]
  [file]
    type = FileMeshGenerator
    file = mesh-2D-injection.msh
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
  [pressure]
    order = FIRST
    family = LAGRANGE
  []
  [vel_x]
    order = CONSTANT
    family = MONOMIAL
  []
  [vel_y]
    order = CONSTANT
    family = MONOMIAL
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
  [pressure_aux]
    type = FunctionAux
    function = pressure_fct
    variable = pressure
    boundary = 'interface'
    execute_on = 'TIMESTEP_BEGIN TIMESTEP_END'
  []
  [vel_x_aux]
    type = TimeDerivativeAux
    variable = vel_x
    functor = disp_x
    execute_on = 'TIMESTEP_END'
  []
  [vel_y_aux]
    type = TimeDerivativeAux
    variable = vel_y
    functor = disp_y
    execute_on = 'TIMESTEP_END'
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
    expression = 'if(t<=0,0.0,1.0-erf(abs(x)/sqrt(t)))' 
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
    young_modulus = 1.0e+02
    poisson_ratio = 0.25
    initial_stress = '-${sigma} -${sigma} -${sigma} 0.0 0.0 +${tau}'
  []
  [interface]
    type = BVMechanicalInterfaceMaterial
    boundary = 'interface'
    displacements = 'disp_x disp_y'
    normal_stiffness = 1.0e+05
    tangent_stiffness = 4.0e+04
    friction_model = 'constant_friction'
  []
  [constant_friction]
    type = BVConstantFrictionUpdate
    friction = ${f}
  []
[]

[Postprocessors]
  [slip_max]
    type = ElementExtremeValue
    variable = slip
    outputs = 'max'
    execute_on = 'TIMESTEP_END'
  []
  [slip_rate_max]
    type = ElementExtremeValue
    variable = slip_rate
    outputs = 'max'
    execute_on = 'TIMESTEP_END'
  []
[]

[VectorPostprocessors]
  [fault_pp]
    type = SideValueSampler
    boundary = 'interface'
    variable = 'pressure normal_stress shear_stress slip slip_rate'
    sort_by = 'x'
    outputs = 'fault'
    execute_on = 'TIMESTEP_END'
  []
[]

[Preconditioning]
  [hypre]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type -snes_linesearch_type'
    petsc_options_value = 'hypre boomeramg l2'
  []
[]

# [Preconditioning]
#   [precond]
#     type = SMP
#     full = true
#     petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -snes_linesearch_type'
#     petsc_options_value = 'bcgs bjacobi 1e-08 1e-10 100 basic'
#   []
# []

# [Preconditioning]
#   [precond]
#     type = SMP
#     full = true
#     petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -snes_linesearch_type'
#     petsc_options_value = 'bcgs bjacobi 1e-10 1e-10 100 bt'
#   []
# []

# [Preconditioning]
#   active = 'asm_ilu'
#   [asm_ilu]
#     type = SMP
#     petsc_options = '-snes_ksp_ew'
#     petsc_options_iname = '-ksp_type
#                            -pc_type
#                            -sub_pc_type
#                            -snes_type -snes_atol -snes_rtol -snes_max_it -snes_linesearch_type
#                            -ksp_gmres_restart'
#     petsc_options_value = 'gmres
#                            asm
#                            ilu
#                            newtonls 1.0e-08 1e-05 100 l2
#                            201'
#   []
#   [asm_lu]
#     type = SMP
#     full = true
#     petsc_options = '-snes_ksp_ew -snes_converged_reason -ksp_converged_reason'
#     petsc_options_iname = '-pc_type -sub_pc_type -pc_factor_mat_solver_type
#                            -sub_pc_factor_shift_type -pc_factor_shift_type
#                            -mat_mffd_err -pc_factor_shift_amount
#                            -snes_atol -snes_rtol -snes_stol -snes_max_it
#                            -snes_linesearch_type
#                            -ksp_type -ksp_max_it'
#     petsc_options_value = 'asm lu superlu_dist
#                            NONZERO NONZERO
#                            1e-5 1e-10
#                            1.0e-08 1.0e-03 0 20
#                            basic
#                            gmres 200'
#   []
#   [lu]
#     type = SMP
#     full = true
#     petsc_options = '-snes_ksp_ew -ksp_diagonal_scale -ksp_diagonal_scale_fix'
#     petsc_options_iname = '-snes_type
#                            -snes_linesearch_type -snes_linesearch_maxstep -sneslinesearch_minlambda
#                            -snes_atol -snes_rtol -snes_max_it
#                            -pc_type -pc_factor_mat_solver_package -pc_factor_shift_type -pc_factor_shift_amount
#                            -ksp_type -ksp_atol -ksp_rtol -ksp_stol -ksp_max_it'
#     petsc_options_value = 'newtonls
#                            basic 2e12 1e-3
#                            1e-08 1e-08 100
#                            lu mumps NONZERO 1e-12
#                            bcgs 1e-12 1e-15 1e-8 500'
#   []
#   [mg_cheb]
#     type = SMP
#     full = true
#     petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
#     # '-snes_ksp_ew
#     #                  -snes_converged_reason
#     #                  -ksp_converged_reason'
#                      # -ksp_diagonal_scale -ksp_diagonal_scale_fix'
#                      # -snes_view
#     petsc_options_iname = '-snes_linesearch_type -snes_linesearch_maxstep -sneslinesearch_minlambda
#                            -snes_atol -snes_rtol -snes_max_it
#                            -ksp_type -ksp_rtol -ksp_max_it
#                            -pc_type
#                            -pc_mg_type
#                            -pc_mg_galerkin
#                            -pc_mg_levels_ksp_type -pc_pc_mg_levels_ksp_max_it -pc_mg_levels_ksp_norm_type
#                            -pc_mg_levels_ksp_chebyshev_esteig -pc_mg_levels_esteig_ksp_norm_type
#                            -pc_mg_levels_pc_type
#                            -pc_mg_coarse_ksp_type -pc_mg_coarse_ksp_max_it -pc_mg_coarse_ksp_rtol
#                            -pc_mg_coarse_pc_type -pc_mg_coarse_pc_asm_overlap -pc_mg_coarse_sub_pc_type'
#     petsc_options_value = 'basic 2e12 1.0e-3
#                            1.0e-08 1.0e-08 200
#                            bcgs 1e-3 200
#                            mg
#                            full
#                            mat
#                            chebyshev 10 NONE
#                            0,0.2,0,1.1 NONE
#                            jacobi
#                            bcgs 25 1.0e-4
#                            asm 2 ilu'
#   []
# []

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  automatic_scaling = true
  # compute_scaling_once = false
  start_time = 0.0
  end_time = 1.0
  num_steps = 500
[]

[Outputs]
  print_linear_residuals = false
  perf_graph = true
  execute_on = 'TIMESTEP_END'
  [out]
    type = Exodus
    file_base = 'outputs/constant_friction_injection_2D_T_${T}'
  []
  [max]
    type = CSV
    file_base = 'outputs/constant_friction_injection_2D_T_${T}'
  []
  [fault]
    type = CSV
    file_base = 'outputs/constant_friction_injection_2D_T_${T}/constant_friction_injection_2D_T_${T}'
    time_data = true
  []
[]
