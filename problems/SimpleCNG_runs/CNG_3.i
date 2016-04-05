# this input file is to test the concurrent nucleation and growth with mesh adaptivity and the updated
# MeshSolutionModify executioner (svn rev. 16195)

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30 #480
  ny = 30 #480
  nz = 0
  xmin = 0
  xmax = 153.6 #0.3*512
  ymin = 0
  ymax = 153.6
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 4
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
     type = ConstantIC
     value = 0.0562
    #type = RandomIC
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
     #type = RandomIC
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
   # type = CHBulkPolyCoupled
    type = CHBulkCoupled
    variable = concentration
    mob_name = M
   # n_OP_variables = 1
   # OP_variable_names = 'n1'
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
  #  type = ACBulkPolyCoupled
    type = ACBulkCoupled
    variable = n1
    mob_name = L
    coupled_CH_var = concentration
   # n_OP_vars = 1
   # OP_var_names = 'n1
   # OP_number = 1
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
    execute_on = timestep_end#_begin
  [../]

  [./NucleationRate]
    type = AuxNucleationRate
    variable = elemental_NucleationRate
    OP_var_names = 'n1'
    n_OP_vars = 1
    coupled_aux_var = elemental_Supersaturation

    Beta_star = 1e-4
    Kb = 1
    Z = 1e-3
    gamma = 3
    linear_density = 50
    scale_factor = 1

    execute_on = timestep_end#_begin
  [../]

  [./NucleationProbability]
    type = AuxNucleationProbability
    variable = elemental_NucleationProbability
    coupled_aux_var = elemental_NucleationRate
    coupled_variables = 'n1'
    n_OP_vars = 1
    execute_on = timestep_end#_begin
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
#    variable = n1
    coupled_aux_vars = 'elemental_NucleationProbability'
    n_coupled_aux = 1
    dwell_time = 0.1
    num_orientations = 1
    execute_on = timestep_end#_begin
    random_seed = 300
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
  [./ElementIntegral_n1]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n1
  [../]

  [./ElementIntegral_c]
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

  [./VolumeFraction]
    type = NodalVolumeFraction
    output = both
    bubble_volume_file = testCNG_HT_3_SizeDistribution.csv
    Avrami_file = testCNG_HT_3_Avrami.csv
    threshold = 0.75
    variable = n1
    mesh_volume = Volume
    equil_fraction = 0.67369140625
  [../]

  [./Volume]
    type = VolumePostprocessor
    execute_on = initial
    output = file
  [../]
[]

[Executioner]
  type = MeshSolutionModify
  scheme = 'bdf2'

  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 0.01
    percent_change = 0.01
  [../]

 # num_steps = 10
 # dt = 0.01
  dtmin = 0.001
  dtmax = 0.1

  start_time = 0.0
  end_time = 50

#  abort_on_solve_fail = true
  adapt_nucleus = 4
  adapt_cycles = 1

  use_nucleation_userobject = true
  nucleation_userobject = NLUO

  l_max_its = 50
  petsc_options = '-snes_mf_operator' #-ksp_monitor'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Adaptivity]
  marker = combo
  [./Markers]
    [./NM]
      type = NucleationMarker
      nucleation_userobject = NLUO
      max_h_level = 4
    [../]

    [./EFMHM_1]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 4
      indicator = GJI_1
    [../]

    [./EFMHM_2]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 4
      indicator = GJI_2
    [../]

    [./combo]
      type = ComboMarker
      markers = 'NM EFMHM_1 EFMHM_2'
    [../]
  [../]

  [./Indicators]
    [./GJI_1]
     type = GradientJumpIndicator
      variable = n1
    [../]

    [./GJI_2]
     type = GradientJumpIndicator
     variable = concentration
    [../]
  [../]
[]


[Output]
  file_base = testCNG_HT_3
  interval = 50
  exodus = true
  perf_log = true
  postprocessor_csv = true
[]
