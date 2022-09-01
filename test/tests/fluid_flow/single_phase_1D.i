[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 1
  nz = 1
  xmin = 0
  xmax = 100
  ymin = 0
  ymax = 10
  zmin = 0
  zmax = 10
[]

[Variables]
  [pf]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [darcy]
    type = BVSinglePhaseDarcy
    variable = pf
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
  [p0_left]
    type = DirichletBC
    variable = pf
    boundary = left
    value = 0
  []
  [p1_right]
    type = DirichletBC
    variable = pf
    boundary = right
    value = 1.0e+06
  []
[]

[Materials]
  [fluid_flow_mat]
    type = BVSinglePhaseFlowMaterial
  []
  [permeability]
    type = BVConstantPermeability
    permeability = 1.0e-15
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
      viscosity = 1.0e-03
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
  type = Steady
  solve_type = 'NEWTON'
  automatic_scaling = true
[]

[Outputs]
  exodus = true
[]
