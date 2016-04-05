#This simulation is non-dimensionalized

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100 #500
  ny = 100 #500
  nz = 0
  xmin = 0 #200
  xmax = 1000 #500 #300 #500
  ymin = 0 #200
  ymax = 1000 #500 #300 #500
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  #elem_type = QUAD9
  #elem_type = HEX8
[]

[MeshModifiers]
  [./AddExtraNodeset]
    boundary = 100
    coord = '1000 1000'
    type = AddExtraNodeset
  [../]
[]

[Variables]
  [./concentration]
    order = FIRST
    family = LAGRANGE
  [../]

  [./mu]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./PSSCIC_c]
     #type = ConstantIC
     type = SmoothCircleIC
     variable = concentration
     #min = 0.099
     #max = 0.101
     int_width = 3
     invalue = 0.6
     outvalue = 0.1
     radius = 7
     x1 = 140
     y1 = 100
     #value = 0.1
  [../]

  [./PSSCIC_n]
      type = SmoothCircleIC
      #type = ConstantIC
      variable = n
      #value = 0.001
      #min = 0
      #max = 0.001
      int_width = 3
      invalue = 1
      outvalue = 0
      radius = 7
      x1 = 140
      y1 = 100
  [../]

  [./t_init]
    type = ConstantIC
    variable = temperature
    value = 600
  [../]
[]

[AuxVariables]
  [./temperature]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elem_ChemElastic]
    order = CONSTANT
    family = MONOMIAL
  [../]

 [./elem_NucleationRate]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elem_NucleationProbability]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Galpha]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Gdelta]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./BulkEnergy]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Grad_C_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Grad_N_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elastic_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./s11_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./s12_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./s22_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./e11_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./e12_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./e22_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[TensorMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[AuxKernels]
  [./auxtemp]
    type = AuxTemperature
    variable = temperature
    temp_in_K = 600
  [../]

 [./ChemElastic]
    type = AuxCalphadEnergy
    variable = elem_ChemElastic
    concentration = concentration
    OP_var_names = 'n'
    OP_number = 1
    n_OP_vars = 1
    precip_conserved = 0.6
    precip_nonconserved = 1
    execute_on = timestep_end
  [../]

 [./NucleationRate]
    type = AuxFullNucleationRate
    variable = elem_NucleationRate
    coupled_bulk_energy_change = elem_ChemElastic

    T = temperature
    X = concentration

    gamma = 13.25E-2
    scale_factor = 0.17816
    linear_density = 3.755E9
    jump_distance = 2.04E-10

    execute_on = timestep_end
  [../]

 [./AuxNucleationProbability]
    type = AuxNucleationProbability
    variable = elem_NucleationProbability
    coupled_aux_var = elem_NucleationRate
    coupled_variables = 'n'
    n_OP_vars = 1
    execute_on = timestep_end
    OP_threshold = 1E-4
 [../]

  [./auxGalpha]
    type = MaterialRealAux
    variable = Galpha
    property = G_AB1CD1
  [../]

  [./auxGdelta]
    type = MaterialRealAux
    variable = Gdelta
    property = G_AB1CD2
  [../]

  [./AuxBulkEnergy]
    type = AuxBulkEnergyCalphad
    variable = BulkEnergy
    conserved_variable = concentration
    OP_variable_names = 'n'
    n_OP_variables = 1
    scaling_factor = 2.49410145E-9
    execute_on = timestep_end
  [../]

  [./AuxGrad_C_energy]
    type = AuxGradientEnergy
    variable = Grad_C_energy
    field_variable = concentration
    kappa_name = kappa_c
    execute_on = timestep_end
  [../]

  [./AuxGrad_N_energy]
    type = AuxGradientEnergy
    variable = Grad_N_energy
    field_variable = n
    kappa_name = kappa_n
    execute_on = timestep_end
  [../]

  [./AuxElasticEnergy]
    type = AuxElasticEnergy
    variable = elastic_energy
    execute_on = timestep_end
  [../]

  [./matl_s11]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    variable = s11_aux
  [../]

 [./matl_s12]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 2
    variable = s12_aux
  [../]

  [./matl_s22]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 2
    index_j = 2
    variable = s22_aux
  [../]

  [./matl_e11]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 1
    variable = e11_aux
  [../]

 [./matl_e12]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 2
    variable = e12_aux
  [../]

  [./matl_e22]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 2
    index_j = 2
    variable = e22_aux
  [../]
[]



[Preconditioning]
 active = 'SMP_TempAux'
  [./SMP]
   type = SMP
   off_diag_row = 'concentration concentration concentration mu mu mu n n n temperature temperature temperature'
   off_diag_column = 'mu n temperature concentration n temperature concentration mu temperature concentration mu n'
  [../]

  [./SMP_TempAux]
   type = SMP
   off_diag_row = 'concentration concentration  mu  mu n n'
   off_diag_column = 'mu n  concentration n  concentration mu'
  [../]
[]

[Kernels]
  [./dcdt]
    type = CoupledTimeDerivative
    variable = mu
    v = concentration
  [../]

  [./mu_residual]
    type = SplitCoupledCHWRes
    variable = mu
    mob_name = M
    c = concentration
   # n = n
    n_OP_vars = 1
    OP_var_names = 'n'
    T = temperature
  [../]

  [./conc_residual]
    type = CHCoupledCalphadSplit
    variable = concentration
    kappa_name = kappa_c
    n_OP_vars = 1
    OP_var_names = 'n'
    w = mu
   # n = n
    T = temperature
    #Well_height = 1
    scaling_factor = 2.49410145E-9
  [../]

  [./dndt]
    type = TimeDerivative
    variable = n
  [../]

  [./ACSolidn]
    type = ACCoupledCalphad
    variable = n
    mob_name = L
    #coupled_CH_var = concentration
    n_OP_vars = 1
    OP_var_names = 'n'
    OP_number = 1
    w = mu
    T = temperature
    c = concentration
    scaling_factor = 2.49410145E-9
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n
    mob_name = L
    #w = mu
    #T = temperature
    #c = concentration
    kappa_name = kappa_n
  [../]

 [./ACTransform]
    type = ACPrecipMatrixElasticity
    variable = n
    OP_number = 1
  [../]

  [./CHTransform]
    type = CHPrecipMatrixElasticity
    variable = concentration
    kappa_name = kappa_c
    w = mu
    n_OP_vars = 1
    OP_var_names = 'n'
    T = temperature
  [../]
[]

[Materials]
  [./calphad]
    type = ZrHCalphadDiffusivity
    block = 0

    n_OP_variables = 1
    OP_variable_names = 'n'
    concentration = concentration

    H_Zr_D0 = 7.00e-7    #m^2/s
    H_ZrH2_D0 = 1.53e-7  # m^2/s
    H_Zr_Q0 =  4.456e4   #J/mol
    H_ZrH2_Q0 = 5.885E4  #J/mol

    #ALWAYS have mobility_AC = 1 for nondimensionalization (L*)!!
    #we pick L = 1E-3, and so L* = 1
    mobility_AC = 1E0
    #to scale mobility, M* = M/L*l^2
    CH_mobility_scaling = 1E-21
    #mobility_CH = 2E-10
    #mobility_CH = 1E3

    #interface energies are scaled
    kappa_CH = 1.9484
    kappa_AC = 19.484

    #well height and molar volume remain unscaled.
    well_height = 60000
    molar_volume = 1.4E-5

    thermal_diffusivity = 1
    coupled_temperature = temperature
  [../]

  [./alphaZr]
   type = CalphadAB1CD1Material
   block = 0

   pure_endpoint_low_coeffs = '-7827.595
                                 125.64905
                                 -24.1618
                                  -0.00437791
                               34971.0' #HCP_Zr
  #this is fucked...

 pure_endpoint_high_coeffs = '2589675
                                 -34719.21
                                   5126.9713
                                     -3.250187
                             -208070050'  #H2_gas
   mixture_coeffs = '-45965
                         41.6
                          0'  #FCC_ZrH
   L0_coeffs = '0 0'
   L1_coeffs = '0 0'


    coupled_temperature = temperature
    coupled_concentration = concentration
  [../]

  [./deltaZrH2]
   type = CalphadAB1CD2Material
   block = 0
   pure_endpoint_low_coeffs = '-227.595
                               124.74905
                               -24.1618
                                -0.00437791
                             34971' #FCC_Zr
    pure_endpoint_high_coeffs = '2589675
                                 -34719.21
                                   5126.9713
                                     -3.250187
                             -208070050'  #H2_gas
   mixture_coeffs =  '-170490
                          208.2
                           -9.47' #FCC_ZrH2'
   L0_coeffs = '14385 -6.0'
   L1_coeffs = '-106445 87.3'


    coupled_temperature = temperature
    coupled_concentration = concentration

    pure_EP1_phase1_coeffs = '-7827.595
                                 125.64905
                                 -24.1618
                                  -0.00437791
                               34971.0' #HCP_Zr
  [../]

  [./Zr_system]
    type = PrecipitateMatrixMisfitMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    #reading         C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66
    C_ijkl =        '155E9 65E9  66E9  155E9 66E9  173E9 36E9  36E9  44E9'
    C_precipitate = '108E9 139E9 139E9 108E9 139E9 108E9 51E9  51E9  51E9'
    #C_precipitate = '155E9 65E9  66E9  155E9 66E9  173E9 36E9  36E9  44E9'
    #reading          S_11    S_22   S_33   S_23 S_13 S_12
    #e_precipitate = '0.00551 0.0564 0.0570 0.0  0.0  0.0'
    e_matrix       = '0.0329  0.0329 0.0542 0.0  0.0  0.0'
    #e_matrix = '0 0 0 0 0 0'
    n_variants = 1
    variable_names = 'n'
    all_21 = false
    scaling_factor = 2.49410145E-9
    solute_name = concentration

    #THIS HAS TEMPERATURE DEPENDENCE
    temperature = temperature
    e_precipitate = '0.03888 0.03888 0.06646 0 0 0'
    misfit_temperature_coeffs = '2.315E-5 2.315E-5 1.9348E-5 0 0 0'

    #e_precipitate = '0.0 0.0 0.0 0 0 0'
    #misfit_temperature_coeffs = '0 0 0 0 0 0'

    percent_matrix_misfit = 1
    percent_precip_misfit = 0.5
  [../]
[]

[BCs]
  [./Stress_dispx]
    type = StressBC
    variable = disp_x
    component = 0
   # boundary_stress = '0.2494 0.2494 0 0 0 0'
    boundary_stress = '0 0 0 0 0 0'
    boundary = '0 1 2 3'
  [../]

  [./Stress_dispy]
    type = StressBC
    variable = disp_y
    component = 1
   # boundary_stress = '0.2494 0.2494 0 0 0 0'
    boundary_stress = '0 0 0 0 0 0'
    boundary = '0 1 2 3'
  [../]

 [./pin_nodex]
    type = DirichletBC
    variable = disp_x
    value = 0.0
    boundary = '100'
  [../]

 [./pin_nodey]
    type = DirichletBC
    variable = disp_y
    value = 0.0
    boundary = '100'
  [../]
[]

[UserObjects]
  [./NLUO]
    type = NucleationLocationUserObject
    coupled_aux_vars = 'elem_NucleationProbability'
    n_coupled_aux = 1
    dwell_time = 0.3
    num_orientations = 1
    execute_on = timestep_end
    boundary_width = 50
    random_seed = 3000
  [../]

  [./NISM]
    type = NucleusIntroductionSolutionModifier
    nucleation_userobject = NLUO
    variables = 'n'
    num_vars = 1
    seed_value = 1
    radius = 7
    int_width = 3
    execute_on = custom
  [../]
[]

[Postprocessors]
  [./BulkEnergy]
    type = ElementIntegralVariablePostprocessor
    variable = BulkEnergy
    output = both
  [../]

  [./GradC_energy]
    type = ElementIntegralVariablePostprocessor
    variable = Grad_C_energy
    output = file
  [../]

 [./GradN_energy]
    type = ElementIntegralVariablePostprocessor
    variable = Grad_N_energy
    output = file
  [../]

 [./Elastic_energy]
    type = ElementIntegralVariablePostprocessor
    variable = elastic_energy
    output = both
  [../]

  [./Maxs11]
    type = NodalExtremeValue
    variable = s11_aux
    value_type = max
    output = file
  [../]

  [./Maxs12]
    type = NodalExtremeValue
    variable = s12_aux
    value_type = max
    output = file
  [../]

  [./Maxs22]
    type = NodalExtremeValue
    variable = s22_aux
    value_type = max
    output = file
  [../]

  [./Mins11]
    type = NodalExtremeValue
    variable = s11_aux
    value_type = min
    output = file
  [../]

  [./Mins12]
    type = NodalExtremeValue
    variable = s12_aux
    value_type = min
    output = file
  [../]

  [./Mins22]
    type = NodalExtremeValue
    variable = s22_aux
    value_type = min
    output = file
  [../]

  [./Maxe11]
    type = NodalExtremeValue
    variable = e11_aux
    value_type = max
    output = file
  [../]

  [./Maxe12]
    type = NodalExtremeValue
    variable = e12_aux
    value_type = max
    output = file
  [../]

  [./Maxe22]
    type = NodalExtremeValue
    variable = e22_aux
    value_type = max
    output = file
  [../]

  [./Mine11]
    type = NodalExtremeValue
    variable = e11_aux
    value_type = min
    output = file
  [../]

  [./Mine12]
    type = NodalExtremeValue
    variable = e12_aux
    value_type = min
    output = file
  [../]

  [./Mine22]
    type = NodalExtremeValue
    variable = e22_aux
    value_type = min
    output = file
  [../]

  [./VolumeFraction]
    type = NodalVolumeFraction
    output = both
    bubble_volume_file = CalphadNewElasNewDiff_50percent_SizeDistribution.csv
    Avrami_file = CalphadNewElasNewDiff_50percent_Avrami.csv
    threshold = 0.5
    variable = n
    mesh_volume = Volume
    equil_fraction =0.157
  [../]

  [./Volume]
    type = VolumePostprocessor
    execute_on = initial
    output = file
  [../]
[]


[Adaptivity]
  marker = combo
  initial_steps = 2

  [./Markers]
 [./NM]
      type = NucleationMarker
      nucleation_userobject = NLUO
      max_h_level = 3
    [../]
    [./EFMHM_1]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 3
      indicator = GJI_1
    [../]
    [./EFMHM_2]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 3
      indicator = GJI_2
    [../]
    [./EFMHM_3]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 3
      indicator = GJI_3
    [../]
    [./EFMHM_4]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 3
      indicator = GJI_4
    [../]

     [./combo]
       type = ComboMarker
       markers = 'NM EFMHM_1 EFMHM_2 EFMHM_3 EFMHM_4'
     [../]
  [../]

  [./Indicators]
    [./GJI_1]
     type = GradientJumpIndicator
      variable = n
    [../]
    [./GJI_2]
     type = GradientJumpIndicator
      variable = concentration
    [../]
    [./GJI_3]
     type = GradientJumpIndicator
      variable = disp_x
    [../]
    [./GJI_4]
     type = GradientJumpIndicator
      variable = disp_y
    [../]
  [../]
[]


[Executioner]
  type = MeshSolutionModify #Transient
  scheme = 'BDF2'

  [./TimeStepper]
    #type = SolutionTimeAdaptiveDT
    type = ConstantDT
    dt = 1E-2
    #percent_change = 0.1
  [../]

  adapt_nucleus = 2
  adapt_cycles = 1

  use_nucleation_userobject = true
  nucleation_userobject = NLUO

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  #petsc_options_iname = '-pc_type -ksp_gmres_restart'
  #petsc_options_value = 'lu 50'
  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_gmres_restart'
  petsc_options_value = ' asm      30              lu           250'

  l_max_its = 100
  l_tol = 1.0e-4

  nl_rel_tol = 1.0e-5
  nl_max_its = 40

  start_time = 0
  num_steps = 1000
  dtmax = 1E0
  dtmin = 1E-3
[]

[Output]
  file_base = Calphad_CNG_0mpa_new_solute_misfit
  interval = 2
#  linear_residuals = true
  exodus = true
  perf_log = true
  all_var_norms = true
#  checkpoint = 1
  num_checkpoint_files = 2

  postprocessor_csv = true
[]
