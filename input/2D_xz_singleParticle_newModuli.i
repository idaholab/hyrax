#This simulation is in aJ, nm

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 35
  ny = 35
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
    order = SECOND
    family = MONOMIAL
  [../]

  [./s11_aux]
    order = SECOND
    family = MONOMIAL
  [../]

  [./s12_aux]
    order = SECOND
    family = MONOMIAL
  [../]

  [./s22_aux]
    order = SECOND
    family = MONOMIAL
  [../]

  [./e11_aux]
    order = SECOND
    family = MONOMIAL
  [../]

  [./e12_aux]
    order = SECOND
    family = MONOMIAL
  [../]

  [./e22_aux]
    order = SECOND
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
   full = true
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
    type = SplitCHWRes #SplitCoupledCHWRes
    variable = mu
    mob_name = M
    #T = temperature
    #c = concentration
    #n_OP_vars = 1
    #OP_var_names = n
  [../]

  [./conc_residual]
    type = CHCoupledCalphadSplit
    variable = concentration
    kappa_name = kappa_c
    n = 'n'
    w = mu
  [../]

  [./dndt]
    type = TimeDerivative
    variable = n
  [../]

  [./ACSolidn]
    type = ACCoupledCalphad
    variable = n
    mob_name = L
    w = mu
    c = concentration
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n
    mob_name = L
    kappa_name = kappa_n
  [../]

 [./ACTransform]
    type = ACTransformElasticDF
    variable = n
    #OP_number = 1
  [../]

  [./CHTransform]
    type = CHPrecipMatrixElasticity
    variable = concentration
    kappa_name = kappa_c
    w = mu
    #n_OP_vars = 1
    #OP_var_names = 'n'
    #T = temperature
  [../]
[]

[Materials]
   [./calphad]
    type = ZrHCalphadDiffusivity
    block = 0

    #n_OP_variables = 1
    OP_variable = 'n'
    concentration = concentration

    H_Zr_D0 = 7.00e5    #nm^2/microsecond
    H_ZrH2_D0 = 1.53e5  #nm^2/microsecond
    H_Zr_Q0 =  4.456e4   #J/mol
    H_ZrH2_Q0 = 5.885E4  #J/mol

    mobility_AC = 1E-1 #nm^3/(aJ microsecond)
#    CH_mobility_scaling = 1E-23

    kappa_CH = 0 #aJ/nm
    kappa_AC = 5e-1 #aJ/nm

    #well height and molar volume remain unscaled.
    well_height = 0 #aJ/amol?
    molar_volume = 1.4e4 #nm^3/amol

    temperature = temperature
  [../]

  [./alphaZr]
   type = CalphadAB1CD1Material
   block = 0

   low_cutoff = 1e-6
   high_cutoff = 0.49

   #aJ/amol
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

   #aJ/amol
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
    type = TwoPhaseLinearElasticMaterial
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
    #Because the simulation is in the xz plane and a 2D simulation, the tensor is rotated (aJ/nm^3)
    Cijkl_matrix = '128.86 65.75 78.98 155.04 65.75 128.86 27.88 26.44 27.88'
    Cijkl_precip = '128.86 65.75 78.98 155.04 65.75 128.86 27.88 26.44 27.88'

    #650k
    #C_ijkl = '126.666E9 79.624E9 65.854E9 126.666E9 65.854E9  153.382E9 27.276E9  27.276E9  23.544E9'

    #adjusted these to delta ZrHy 1.5 from Olsson
    #Cijkl_precip = '162 103 103 162 103 162 69.3 69.3 69.3'

    #homogeneous modulus approximation
    #C_precipitate = '128.858E9 78.978E9 65.754E9 128.858E9 65.754E9  155.036E9 27.876E9  27.876E9  26.436E9'

    #reading          S_11    S_22   S_33   S_23 S_13 S_12
    # e_matrix       = '0.0329  0.0329 0.0542 0.0  0.0  0.0'
    #this is rotated
    matrix_eigenstrain       = '0.0329  0.0542 0.0329 0.0  0.0  0.0'

    order_parameter = 'n'
    matrix_fill_method = symmetric9
    precip_fill_method = symmetric9

    #scaling_factor = 1
    atomic_fraction = concentration

    #THIS HAS TEMPERATURE DEPENDENCE
    temperature = temperature
    #precipitate_eigenstrain = '0.03888 0.03888 0.06646 0 0 0'
    #this is rotated
    precipitate_eigenstrain = '0.03888 0.06646 0.03888 0 0 0'
#    misfit_temperature_coeffs = '2.315E-5 2.315E-5 1.9348E-5 0 0 0'
    #this is rotated
    precip_misfit_T_coeffs = '2.315E-5 1.9348E-5 2.315E-5 0 0 0'

    #percent_matrix_misfit = 1
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
#    bubble_volume_file = 2D_xz_singleParticle_vol.csv
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
      coarsen = 0.05
      refine = 0.25
      indicator = GJI_2
    [../]
    [./EFM_3]
      type = ErrorFractionMarker
      coarsen = 0.05
      refine = 0.25
      indicator = GJI_3
    [../]
    [./EFM_4]
      type = ErrorFractionMarker
      coarsen = 0.05
      refine = 0.25
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
    dt = 1e-2
    percent_change = 0.05
  [../]

  adapt_nucleus = 4
  adapt_cycles = 1

  use_nucleation_userobject = false
  nucleation_userobject = NLUO

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_gmres_restart'
  petsc_options_value = ' asm      4              lu           30'

  l_max_its = 100
  l_tol = 1.0e-4

  nl_rel_tol = 1.0e-6
  nl_max_its = 20

  start_time = 0
  num_steps = 20
  end_time = 50
  dtmax = 1E0
  dtmin = 1E-5
[]

[Outputs]
  file_base = 2D_xz_singleParticle_homModuli

  exodus = true
  interval = 5
  checkpoint = 1
  csv = true

  [./console]
    type = Console
    interval = 1
    max_rows = 10
#    linear_residuals = true
  [../]
[]
