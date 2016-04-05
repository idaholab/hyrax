# this input file is to test the concurrent nucleation and growth with mesh adaptivity

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  nz = 0
  xmin = 0
  xmax = 153.6 #0.3*512
  ymin = 0
  ymax = 153.6
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 1#4 # 80 elements, dx=0.96...
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
      [./InitialCondition]
      type = ConstantIC
      value = 0.0562
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      invalue = 1.6
      outvalue = 0.0
      radius = 1.8
      x1 = 20
      y1 = 20
     [../]
  [../]
[]

[AuxVariables]
  [./elemental_Supersaturation]
    order = CONSTANT
    family = MONOMIAL
  [../]

 [./elemental_NucleationProbability]
    order = CONSTANT
    family = MONOMIAL
  [../]

 [./elemental_NucleationRate]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./dcdt]
    type = TimeDerivative
    variable = concentration
  [../]

  [./dn1dt]
    type = TimeDerivative
    variable = n1
  [../]

  [./CHSolid]
    type = CHBulkCoupled
    variable = concentration
    mob_name = M
    coupled_OP_var = n1
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
  [../]

  [./ACSolidn1]
    type = ACBulkCoupled
    variable = n1
    mob_name = L
    coupled_CH_var = concentration
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]
[]

[AuxKernels]
  [./Supersaturation]
    type = AuxSupersaturation
    variable = elemental_Supersaturation
    coupled_var = concentration
    functional_c1 = 0.006
    execute_on = timestep_begin
  [../]

  [./NucleationRate]
    type = AuxNucleationRate
    variable = elemental_NucleationRate
    OP_var_names = 'n1'
    n_OP_vars = 1
    coupled_aux_var = elemental_Supersaturation
    Beta_star = 0.0005
    linear_density = 50
    Z = 0.001
    Kn2 = 0.033
    execute_on = timestep_begin
  [../]

  [./NucleationProbability]
    type = AuxNucleationProbability
    variable = elemental_NucleationProbability
    coupled_aux_var = elemental_NucleationRate
    coupled_variables = 'n1'
    n_OP_vars = 1
    execute_on = timestep_begin
  [../]
[]

#[BCs]
#  [./c_BC]
#    type = NeumannBC
#    variable = concentration
#    boundary = '0 1 2 3'
#    value = 0.0
#  [../]

#  [./n1_BC]
#    type = NeumannBC
#    variable = n1
#    boundary = '0 1 2 3'
#    value = 0.0
#  [../]
#[]

[Materials]
  [./constant]
    type = PFMobilityLandau
    block = 0
    mob_CH = 0.4
    mob_AC = 0.4
    kappa_CH = 1.5
    kappa_AC = 1.5
    A1 = 18.5
    A2 = -8.5
    A3 = 11.5
    A4 = 4.5
    C1 = 0.006
    C2 = 0.59
  [../]
[]

[UserObjects]
  [./NLUO]
    type = NucleationLocationUserObject
    variable = n1
    coupled_aux = elemental_NucleationProbability
    dwell_time = 0.1
    num_orientations = 1
   # execute_on = timestep_begin
  [../]

  [./NISM]
    type = NucleusIntroductionSolutionModifier
    nucleation_userobject = NLUO
    variables = 'n1'
    num_vars = 1
    seed_value = 1.6
    radius = 1.8
    int_width = 0.9
    execute_on = custom
  [../]
[]

[Postprocessors]
  [./ElementIntegralVariablePostprocessor_n1]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n1
  [../]

  [./ElementIntegralVariablePostprocessor_c]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = concentration
  [../]

  [./NodalMaxValue_n1]
    output = file
    type = NodalMaxValue
    variable = n1
  [../]

  [./NodalMaxValue_c]
    output = file
    type = NodalMaxValue
    variable = concentration
  [../]

  [./ndofs]
    type = NumDOFs
  [../]
[]

[Executioner]
  type = MeshSolutionModify
  scheme = 'crank-nicolson'
 # num_steps = 10
  dt = 0.01
  start_time = 0.0
  end_time = 2
  abort_on_solve_fail = true
  adapt_cycles = 5
  adapt_nucleus = 7
  nucleation_userobject = NLUO
#  max_h_level = 2


  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'

[]

[Adaptivity]
   marker = combo
 [./Markers]
    [./NM]
      type = NucleationMarker
      nucleation_userobject = NLUO
      max_h_level = 5
    [../]
    [./EFMHM]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 5
      indicator = GJI
    [../]
    [./combo]
      type = ComboMarker
      markers = 'NM EFMHM'
    [../]
 [../]

 [./Indicators]
   [./GJI]
    type = GradientJumpIndicator
    variable = n1
  [../]
 [../]
[]

[Output]
  file_base = testCNG_h-adapt_bug1
  interval = 1
  exodus = true
  perf_log = true
  postprocessor_csv = true
[]

