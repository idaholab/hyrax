[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 15 #240
  ny = 15 #240
  nz = 0
  xmin = 0
  xmax = 76.8 #153.6
  ymin = 0
  ymax = 76.8 #153.6
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[MeshModifiers]
  [./fixednodes]
    type = AddExtraNodeset
    boundary = 100
    coord = '25.6 25.6 25.6 30.72'
  [../]
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
      [./InitialCondition]
      # type = PolySpecifiedSmoothCircleIC
      # invalue = 0.6
      # outvalue = 0.1
      # radius = 1.8
      # int_width = 0.9
      # x_positions = '20 40'
      # y_positions = '20 52'
      # z_positions = '0 0'
      # n_seeds = 2
       type = RandomIC
      min = -0.3
      max = -0.31
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = PolySpecifiedSmoothCircleIC
      invalue = 1.6
      outvalue = 0
      radius = 1.8
      int_width = 0.9
      x_positions = '20'
      y_positions = '20'
      z_positions = '0'
      n_seeds = 1
      #type = RandomIC
      #min = 0.005
      #max = 0.01
    [../]
  [../]
#  [./n2]
#    order = FIRST
#    family = LAGRANGE
#     [./InitialCondition]
#       type = RandomIC
#       min = 0.005
#       max = 0.01
#     [../]
#  [../]
#  [./n3]
#    order = FIRST
#    family = LAGRANGE
#     [./InitialCondition]
#       type = RandomIC
#       min = 0.005
#       max = 0.01
#     [../]
#  [../]

  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./elem_ChemElastic_n1]
    order = CONSTANT
    family = MONOMIAL
  [../]
#  [./elem_ChemElastic_n2]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./elem_ChemElastic_n3]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]

 [./elem_NucleationRate_n1]
    order = CONSTANT
    family = MONOMIAL
  [../]
# [./elem_NucleationRate_n2]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
# [./elem_NucleationRate_n3]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]

 [./elem_NucleationProbability_n1]
    order = CONSTANT
    family = MONOMIAL
  [../]
# [./elem_NucleationProbability_n2]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
# [./elem_NucleationProbability_n3]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
[]

[TensorMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
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
#  [./dn2dt]
#    type = TimeDerivative
#    variable = n2
#  [../]
#  [./dn3dt]
#    type = TimeDerivative
#    variable = n3
#  [../]

  [./CHSolid]
    type = CHBulkCoupled
    variable = concentration
    mob_name = M
    coupled_OP_var = n1
  #  n_OP_variables = 1 #3
   # OP_variable_names = 'n1' # n2 n3'
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
    #n_OP_vars = 1 #3
    #OP_var_names = 'n1' #n2 n3'
    #OP_number = 1
  [../]

 # [./ACSolidn2]
 #   type = ACBulkPolyCoupled
 #   variable = n2
 #   mob_name = L
 #   coupled_CH_var = concentration
 #   n_OP_vars = 3
 #   OP_var_names = 'n1 n2 n3'
 #   OP_number = 2
 # [../]
 # [./ACSolidn3]
 #   type = ACBulkPolyCoupled
#    variable = n3
#    mob_name = L
#    coupled_CH_var = concentration
#    n_OP_vars = 3
#    OP_var_names = 'n1 n2 n3'
#    OP_number = 3
#  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]
#  [./ACInterfacen2]
#    type = ACInterface
#    variable = n2
#    mob_name = L
#    kappa_name = kappa_n
#  [../]
#  [./ACInterfacen3]
#    type = ACInterface
#    variable = n3
#    mob_name = L
#    kappa_name = kappa_n
#  [../]

  [./ACTransformn1]
    type = ACTransformElasticDF
    variable = n1
    OP_number = 1
    OP_var_names = 'n1' # n2 n3'
    n_OP_vars = 1 # 3
  [../]
 # [./ACTransformn2]
 #   type = ACTransformElasticDF
 #   variable = n2
 #   OP_number = 2
 #   OP_var_names = 'n1 n2 n3'
 #   n_OP_vars = 3
 # [../]
 # [./ACTransformn3]
 #   type = ACTransformElasticDF
 #   variable = n3
 #   OP_number = 3
 #   OP_var_names = 'n1 n2 n3'
 #   n_OP_vars = 3
 # [../]
[]

[AuxKernels]
  [./ChemElastic_n1]
    type = AuxChemElastic
    variable = elem_ChemElastic_n1
    coupled_conserved_var = concentration
    coupled_nonconserved_var = n1
    nonconserved_var_number = 1
    precip_conserved = 0.6
    precip_nonconserved = 1.6
    execute_on = timestep_end
  [../]

 [./NucleationRate_n1]
    type = AuxNucleationRate
    variable = elem_NucleationRate_n1
    coupled_aux_var = elem_ChemElastic_n1

    gamma = 0.01
    scale_factor = 0.5
    Kb = 1

    Z = 1e-8
    Beta_star = 1e-5
    linear_density = 50
    OP_var_names = 'n1' # n2 n3'
    n_OP_vars = 1 #3
    execute_on = timestep_end
  [../]

 [./AuxNucleationProbability_n1]
    type = AuxNucleationProbability
    variable = elem_NucleationProbability_n1
    coupled_aux_var = elem_NucleationRate_n1
    coupled_variables = 'n1' # n2 n3'
    n_OP_vars = 1 #3
    execute_on = timestep_end
 [../]

 # [./ChemElastic_n2]
 #   type = AuxChemElastic
 #   variable = elem_ChemElastic_n2
 #   coupled_conserved_var = concentration
 #   coupled_nonconserved_var = n2
  #  nonconserved_var_number = 2
  #  precip_conserved = 0.6
  #  precip_nonconserved = 1.6
  #  execute_on = timestep_end
  #[../]

 #[./NucleationRate_n2]
 #   type = AuxNucleationRate
 #   variable = elem_NucleationRate_n2
 #   coupled_aux_var = elem_ChemElastic_n2

#    gamma = 2
#    scale_factor = 1
#    Kb = 1

#    Z = 0.0001
#    Beta_star = .001
#    linear_density = 50
#    OP_var_names = 'n1 n2 n3'
#    n_OP_vars = 3
#    execute_on = timestep_end
#  [../]

 #[./AuxNucleationProbability_n2]
#    type = AuxNucleationProbability
#    variable = elem_NucleationProbability_n2
#    coupled_aux_var = elem_NucleationRate_n2
#    coupled_variables = 'n1 n2 n3'
#    n_OP_vars = 3
#    execute_on = timestep_end
# [../]

#  [./ChemElastic_n3]
#    type = AuxChemElastic
#    variable = elem_ChemElastic_n3
#    coupled_conserved_var = concentration
#    coupled_nonconserved_var = n3
#    nonconserved_var_number = 3
#    precip_conserved = 0.6
#    precip_nonconserved = 1.6
#    execute_on = timestep_end
#  [../]

# [./NucleationRate_n3]
#    type = AuxNucleationRate
#    variable = elem_NucleationRate_n3
#    coupled_aux_var = elem_ChemElastic_n3

#    gamma = 2
#    scale_factor = 1
#    Kb = 1

#    Z = 0.0001
#    Beta_star = .001
#    linear_density = 50
#    OP_var_names = 'n1 n2 n3'
#    n_OP_vars = 3
#    execute_on = timestep_end
#  [../]

# [./AuxNucleationProbability_n3]
#    type = AuxNucleationProbability
#    variable = elem_NucleationProbability_n3
#    coupled_aux_var = elem_NucleationRate_n3
#    coupled_variables = 'n1 n2 n3'
#    n_OP_vars = 3
#    execute_on = timestep_end
# [../]
[]

[BCs]
# boundary_stress is supplied as a list, s11, s22, s33, s12, s13, s12 in GPa.
# right now it's specified as hydrostatic pressure.
  [./stress_dispx]
   type = StressBC
   variable = disp_x
   component = 0
   boundary_stress = '-0.1 -0.1 -0.1 0 0 0'
   boundary = '0 1 2 3'
  [../]

  [./stress_dispy]
   type = StressBC
   variable = disp_y
   component = 1
   boundary_stress = '-0.1 -0.1 -0.1 0 0 0'
   boundary = '0 1 2 3'
  [../]

  [./flux_c]
    type = NeumannBC
    variable = concentration
    value = 1e-5
    boundary = '0'
  [../]

  [./Dirichlet_dispx]
    type = DirichletBC
    variable = disp_x
    value = 0.0
    boundary = '100'
  [../]

  [./Dirichlet_dispy]
    type = DirichletBC
    variable = disp_y
    value = 0.0
    boundary = '100'
  [../]

# [./Dirichlet_dispx_overall]
#    type = DirichletBC
#    variable = disp_x
#    value = 0.0
#    boundary = '0 1 2 3'
#  [../]

#  [./Dirichlet_dispy_overall]
#    type = DirichletBC
#    variable = disp_y
#    value = 0.0
#    boundary = '0 1 2 3'
#  [../]
[]

[Materials]
  [./constant]
    type = PFMobilityLandau
    block = 0
    mob_CH = 4e-4
    mob_AC = 4e-4
    kappa_CH = 1.5
    kappa_AC = 1.5
    A1 = 18.5
    A2 = -8.5
    A3 = 11.5
    A4 = 4.5
    C1 = 0.006
    C2 = 0.59
  [../]

  [./test_material]
    type = LinearSingleCrystalPrecipitateMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
 #   disp_z = disp_z
    #reading C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66
    C_ijkl ='155.4 68.03 64.6 155.4  64.6 172.51 36.31 36.31 44.09'
    C_precipitate ='155.4 68.03 64.6 155.4  64.6 172.51 36.31 36.31 44.09'
    #reading        S_11   S_22  S_33 S_23 S_13 S_12
    e_precipitate = '0.00551  0.0564  0.0570  0.0  0.0  0.0'
    n_variants = 1 #3
    variable_names = 'n1' # n2 n3'
    all_21 = false
  [../]
[]

[Postprocessors]
  [./ElementIntegral_c]
    type = ElementIntegralVariablePostprocessor
    variable = concentration
    output = both
  [../]
#  [./NodalFloodCount]
#    type = NodalFloodCount
#    variable = 'n1 n2 n3'
#    output = file
#    threshold = 0.75
#    use_single_map = false
#  [../]

  [./volume_fraction]
    type = NodalVolumeFraction
    variable = 'n1' # n2 n3'
    use_single_map = true
    threshold = 0.75
    execute_on = timestep_end
    mesh_volume = Volume
    output = both
  [../]

  [./Volume]
    type = VolumePostprocessor
    execute_on = initial
  [../]
[]

[UserObjects]
  [./NLUO]
    type = NucleationLocationUserObject
    coupled_aux_vars = 'elem_NucleationProbability_n1' # elem_NucleationProbability_n2 elem_NucleationProbability_n3'
    n_coupled_aux = 1 #3
    dwell_time = 0.1
    num_orientations = 1 #3
    execute_on = timestep_end
    boundary_width = 3.6
  [../]

  [./NISM]
    type = NucleusIntroductionSolutionModifier
    nucleation_userobject = NLUO
    variables = 'n1' # n2 n3'
    num_vars = 1 #3
    seed_value = 1.6
    radius = 1.8
    int_width = 0.9
    execute_on = custom
  [../]
[]

[Executioner]
  type = MeshSolutionModify
  scheme = 'bdf2'

  dt = 10
  dtmin = 0.1
  #dtmax = 1000
  percent_change = 1

  #Not sure what needs to go here for the end of the simulation
  start_time = 0.0
  #end_time = 200

  abort_on_solve_fail = true
  adapt_nucleus = 5
  adapt_cycles = 1

  use_nucleation_userobject = true
  nucleation_userobject = NLUO


  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Adaptivity]
  marker = combo
  initial_steps = 4
  [./Markers]
    [./NM]
      type = NucleationMarker
      nucleation_userobject = NLUO
      max_h_level = 4
    [../]
    [./EFMHM]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 4
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
  linear_residuals = true
  file_base = HyraxCoupling
  interval = 1
  exodus = true
  perf_log = true
  postprocessor_csv = true
[]

