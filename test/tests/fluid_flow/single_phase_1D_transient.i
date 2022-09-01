[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 200
  ny = 1
  nz = 1
  xmin = -50
  xmax = 50
  ymin = 0
  ymax = 5
  zmin = 0
  zmax = 5
[]

[Variables]
  [pf]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [fluid_time_derivative]
    type = BVSinglePhaseTimeDerivative
    variable = pf
  []
  [darcy]
    type = BVSinglePhaseDarcy
    variable = pf
  []
[]

[Functions]
  [pf_func]
    type = ParsedFunction
    value = 'p1*t'
    vars = 'p1'
    vals = '23.14814815'
  []
[]

[AuxVariables]
  [vf_x]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [vf_x_aux]
    type = BVDarcyVelocityAux
    variable = vf_x
    fluid_pressure = pf
    component = x
    execute_on = 'TIMESTEP_END'
  []
[]

[BCs]
  [p_left]
    type = FunctionDirichletBC
    variable = pf
    boundary = left
    function = pf_func
  []
  [p_right]
    type = FunctionDirichletBC
    variable = pf
    boundary = right
    function = pf_func
  []
[]

[Materials]
  [fluid_flow_mat]
    type = BVSinglePhaseFlowMaterial
  []
  [permeability]
    type = BVConstantPermeability
    permeability = 1.0e-14
  []
  [porosity]
    type = BVConstantPorosity
    porosity = 1.0
  []
  [fluid_properties]
    type = BVFluidProperties
    fluid_pressure = pf
    temperature = 273
    fp = simple_fluid
  []
[]

[Modules]
  [FluidProperties]
    [simple_fluid]
      type = SimpleFluidProperties
      density0 = 1.0
      viscosity = 1.728e-03
      bulk_modulus = 4.0e+09
    []
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
  automatic_scaling = true
  start_time = 0.0
  end_time = 43200
  num_steps = 10
[]

[Outputs]
  interval = 5
  exodus = true
[]
