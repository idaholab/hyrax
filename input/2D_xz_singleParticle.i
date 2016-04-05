#This simulation is non-dimensionalized

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 25
  nz = 0
  xmin = 0
  xmax = 250
  ymin = 0
  ymax = 250
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  #uniform_refine = 2
[]

[MeshModifiers]
  [./AddExtraNodeset]
    #this is done to fix a node for mechanical computations
    new_boundary = 100
    coord = '0 0'
    type = AddExtraNodeset
  [../]
[]

[Variables]
  [./concentration]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = EllipsoidIC
      variable = concentration
      n_seeds = 1
      int_width = 1
      invalue = 0.6
      outvalue = 0.05
      x_positions = '125'
      y_positions = '125'
      z_positions = '0'
      coefficients = '8 7 8'
    [../]
  [../]

  [./mu]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = EllipsoidIC
      variable = n
      n_seeds = 1
      int_width = 1
      invalue = 1
      outvalue = 0
      x_positions = '125'
      y_positions = '125'
      z_positions = '0'
      coefficients = '8 7 8'
    [../]
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

[AuxVariables]
  [./temperature]
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

[AuxKernels]
  [./auxtemp]
    type = AuxTemperature
    variable = temperature
    temp_in_K = 600
  [../]

  [./matl_s11]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 0
    index_j = 0
    variable = s11_aux
  [../]

 [./matl_s12]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 0
    index_j = 1
    variable = s12_aux
  [../]

  [./matl_s22]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    variable = s22_aux
  [../]

  [./matl_e11]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 0
    index_j = 0
    variable = e11_aux
  [../]

 [./matl_e12]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 0
    index_j = 1
    variable = e12_aux
  [../]

  [./matl_e22]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 1
    variable = e22_aux
  [../]
[]

[Preconditioning]
  [./SMP_TempAux]
   type = SMP
   off_diag_row = 'concentration concentration  mu  mu n n'
   off_diag_column = 'mu n  concentration n  concentration mu'
  [../]
[]

[Kernels]
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
  [../]

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
    T = temperature
    scaling_factor = 2.9185832E-9
  [../]

  [./dndt]
    type = TimeDerivative
    variable = n
  [../]

  [./ACSolidn]
    type = ACCoupledCalphad
    variable = n
    mob_name = L
    n_OP_vars = 1
    OP_var_names = 'n'
    OP_number = 1
    w = mu
    T = temperature
    c = concentration
    scaling_factor = 2.9185832E-9
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n
    mob_name = L
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

    #n_OP_variables = 1
    OP_variable = 'n'
    concentration = concentration

    H_Zr_D0 = 7.00e-7    #m^2/s
    H_ZrH2_D0 = 1.53e-7  # m^2/s
    H_Zr_Q0 =  4.456e4   #J/mol
    H_ZrH2_Q0 = 5.885E4  #J/mol

    #ALWAYS have mobility_AC = 1 for nondimensionalization (L*)!!
    #we pick L = 1E0, and so L* = 1
    mobility_AC = 1E0
    #to scale mobility, M* = M/L*l^2
    CH_mobility_scaling = 1E-23

    #interface energies are scaled
    kappa_CH = 3
    kappa_AC = 3

    #well height and molar volume remain unscaled.
    well_height = 0
    molar_volume = 1.4E-5

    thermal_diffusivity = 1
    coupled_temperature = temperature
  [../]

  [./alphaZr]
   type = CalphadAB1CD1Material
   block = 0

   low_cutoff = 1e-6
   high_cutoff = 0.499

   pure_endpoint_low_coeffs = '-7827.595
                                 125.64905
                                 -24.1618
                                  -0.00437791
                               34971.0' #HCP_Zr

   pure_endpoint_high_coeffs = '8055.336
                                 -243.791
                                   18.3135
                                   -0.034513
                              -734182.8'  #H2_gas
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
   
   low_cutoff = 0.5
   high_cutoff = 0.665

   pure_endpoint_low_coeffs = '-227.595
                               124.74905
                               -24.1618
                                -0.00437791
                             34971' #FCC_Zr
     pure_endpoint_high_coeffs = '8055.336
                                 -243.791
                                   18.3135
                                   -0.034513
                              -734182.8'  #H2_gas
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

    #adjusted these to the temperature-dependent numbers from Fisher
    #485K
#    C_ijkl = '134.12E9 76.96E9 65.45E9 134.12E9 65.45E9 158.69E9 29.33E9 29.33E9 28.59E9'
    #this is rotated
#    C_ijkl = '134.12E9 65.45E9 76.96E9 158.69E9 65.45E9 134.12E9 29.33E9 28.59E9 29.33E9'

    #550k
#    C_ijkl = '131.112E9 78.232E9 65.654E9 131.112E9 65.654E9  156.59E9 28.476E9 28.476E9 26.436E9'
    #this is rotated
#    C_ijkl = '131.112E9 65.654E9 78.232E9 156.59E9 65.654E9 131.112E9 28.476E9 26.436E9 28.476E9 '

    #600k
#    C_ijkl = '128.858E9 78.978E9 65.754E9 128.858E9 65.754E9  155.036E9 27.876E9  27.876E9  26.436E9'
    #this is rotated
    #Because the simulation is in the xz plane and a 2D simulation, the tensor is rotated
    C_ijkl = '128.86E9 65.75E9 78.98E9 155.04E9 65.75E9 128.86E9 27.88E9 26.44E9 27.88E9'

    #650k
    #C_ijkl = '126.666E9 79.624E9 65.854E9 126.666E9 65.854E9  153.382E9 27.276E9  27.276E9  23.544E9'

    #adjusted these to delta ZrHy 1.5 from Blomquist
#    C_precipitate = '108E9    139E9     139E9     108E9    139E9     108E9      51E9     51E9     51E9'
    #homogeneous modulus approximation
    C_precipitate = '128.858E9 78.978E9 65.754E9 128.858E9 65.754E9  155.036E9 27.876E9  27.876E9  26.436E9'

    #reading          S_11    S_22   S_33   S_23 S_13 S_12
     e_matrix       = '0.0329  0.0329 0.0542 0.0  0.0  0.0'
    #this is rotated
#    e_matrix       = '0.0329  0.0542 0.0329 0.0  0.0  0.0'
    n_variants = 1
    variable_names = 'n'
    fill_method = symmetric9

    scaling_factor = 2.9185832E-9
    solute_name = concentration

    #THIS HAS TEMPERATURE DEPENDENCE
    temperature = temperature
   e_precipitate = '0.03888 0.03888 0.06646 0 0 0'
    #this is rotated
#    e_precipitate = '0.03888 0.06646 0.03888 0 0 0'
    misfit_temperature_coeffs = '2.315E-5 2.315E-5 1.9348E-5 0 0 0'
    #this is rotated
#    misfit_temperature_coeffs = '2.315E-5 1.9348E-5 2.315E-5 0 0 0'

    percent_matrix_misfit = 1
    percent_precip_misfit = 0.5
  [../]
[]


[BCs]
#  [./Stress_dispx]
#    type = StressBC
#    variable = disp_x
#    component = 0
#    #boundary_stress = '0.2494 0.2494 0 0 0 0'
#    boundary = '0 1 2 3'
#  [../]

#  [./Stress_dispy]
#    type = StressBC
#    variable = disp_y
#    component = 1
#   # boundary_stress = '0.2494 0.2494 0 0 0 0'
#    boundary = '0 1 2 3'
#  [../]

#  [./Stress_dispz]
#    type = StressBC
#    variable = disp_z
#    component = 1
#    boundary_stress = '0.2494 0.2494 0 0 0 0'
#    boundary = '0 1 2 3'
#  [../]

 [./pin_nodex]
    type = DirichletBC
    variable = disp_x
    value = 0.0
#   boundary = '0 1 2 3'
    boundary = '100'
  [../]

 [./pin_nodey]
    type = DirichletBC
    variable = disp_y
    value = 0.0
#    boundary = '0 1 2 3'
    boundary = '100'
  [../]
[]

[Postprocessors]
  [./VolumeFraction]
    type = NodalVolumeFraction
    bubble_volume_file = 2D_xz_singleParticle_vol.csv
    threshold = 0.5
    variable = n
    mesh_volume = Volume
  [../]

  [./Volume]
    type = VolumePostprocessor
    execute_on = initial
  [../]

  [./dofs]
   type = NumDOFs
  [../]
[]

[Adaptivity]
  marker = combo
  initial_steps = 4
  initial_marker = EFM_1
  max_h_level = 4
  [./Markers]
    [./EFM_1]
      type = ErrorFractionMarker
      coarsen = 0.075
      refine = 0.75
      indicator = GJI_1
    [../]
    [./EFM_2]
      type = ErrorFractionMarker
      coarsen = 0.075
      refine = 0.75
      indicator = GJI_2
    [../]
    [./EFM_3]
      type = ErrorFractionMarker
      coarsen = 0.075
      refine = 0.75
      indicator = GJI_3
    [../]
    [./EFM_4]
      type = ErrorFractionMarker
      coarsen = 0.075
      refine = 0.75
      indicator = GJI_4
    [../]

     [./combo]
       type = ComboMarker
       markers = 'EFM_1 EFM_2 EFM_3 EFM_4'
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
  type = MeshSolutionModify
  scheme = 'BDF2'

  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 5E-5
    percent_change = 0.05
  [../]

  adapt_nucleus = 4
  adapt_cycles = 1

  use_nucleation_userobject = false
  nucleation_userobject = NLUO

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_gmres_restart'
  petsc_options_value = ' asm      1              lu           30'

  l_max_its = 100
  l_tol = 1.0e-4

  nl_rel_tol = 1.0e-7
  nl_max_its = 20

  start_time = 0
  num_steps = 1
  end_time = 50
  dtmax = 1E0
  dtmin = 1E-5
[]

[Outputs]
  file_base = 2D_xz_singleParticle

  exodus = true
  interval = 1
  checkpoint = 1
  csv = true

  sync_times = '0.1 0.5 1.0 1.5 2.0 2.5 3.0 3.5 4.0 4.5
                5.0 5.5 6.0 6.5 7.0 7.5 8.0 8.5 9.0 9.5'
  [./console]
    type = Console
    interval = 1
    max_rows = 10
#    linear_residuals = true
  [../]
[]

