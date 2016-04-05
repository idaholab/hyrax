#This simulation is non-dimensionalized

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  nz = 0
  xmin = 0
  xmax = 6
  ymin = 0
  ymax = 6
  zmin = 0
  zmax = 0
  elem_type = QUAD4
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
  [./RIC_c]
      type = RandomIC
      variable = concentration
      min = 0.05
      max = 0.04
  [../]

  [./RIC_n]
      type = RandomIC
      variable = n
      min = 0.01
      max = 0.001
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
[]

[AuxKernels]
  [./auxtemp]
    type = AuxTemperature
    variable = temperature
    temp_in_K = 590
  [../]

 [./ChemElastic]
    type = AuxCalphadElasticity
    variable = elem_ChemElastic
    concentration = concentration
    OP_var_names = 'n'
    OP_number = 1
    n_OP_vars = 1
    precip_conserved = 0.6
    precip_nonconserved = 1
    execute_on = timestep_end
    self_energy = 0.12
    scaling_factor = 2.9185832E-9
    use_elastic_energy = false
  [../]
[]

[Preconditioning]
 active = 'SMP_TempAux'
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

    OP_variable = n
    concentration = concentration

    H_Zr_D0 = 7.00e-7    #m^2/s
    H_ZrH2_D0 = 1.53e-7  # m^2/s
    H_Zr_Q0 =  4.456e4   #J/mol
    H_ZrH2_Q0 = 5.885E4  #J/mol

    #ALWAYS have mobility_AC = 1 for nondimensionalization (L*)!!
    #we pick L = 1E0, and so L* = 1
    mobility_AC = 1E0
    #to scale mobility, M* = M/L*l^2
    CH_mobility_scaling = 1E-18

    #interface energies are scaled
    kappa_CH = 1.7708
    kappa_AC = 17.708

    #well height and molar volume remain unscaled.
    well_height = 40500
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

   pure_endpoint_high_coeffs = '8055.336
                                 -243.791
                                   18.3135
                                   -0.034513
                              -734182.8'  #H2_gas
   mixture_coeffs = '-45965
                         41.6
                          0'  #HCP_ZrH
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
    #reading         C_11       C_12     C_13     C_22      C_23      C_33      C_44      C_55      C_66
    #adjusted these to the temperature-dependent numbers from Fisher
    C_ijkl =          '128.858E9 78.978E9 65.754E9 128.858E9 65.754E9  155.036E9 27.876E9  27.876E9  26.436E9'
    #C_precipitate = '108E9 139E9 139E9 108E9 139E9 108E9 51E9  51E9  51E9'
    #adjusted these to the temperature-dedendent numbers from Fisher (for Zr)
    C_precipitate =  '128.858E9 78.978E9 65.754E9 128.858E9 65.754E9  155.036E9 27.876E9  27.876E9  26.436E9'
    #reading          S_11    S_22   S_33   S_23 S_13 S_12
    #e_precipitate = '0.00551 0.0564 0.0570 0.0  0.0  0.0'
    e_matrix       = '0.0329  0.0329 0.0542 0.0  0.0  0.0'
    #e_matrix = '0 0 0 0 0 0'
    n_variants = 1
    variable_names = 'n'
    fill_method = symmetric9
    scaling_factor = 2.9185832E-9
    solute_name = concentration

    #THIS HAS TEMPERATURE DEPENDENCE
    temperature = temperature
    e_precipitate = '0.03888 0.03888 0.06646 0 0 0'
    misfit_temperature_coeffs = '2.315E-5 2.315E-5 1.9348E-5 0 0 0'

    percent_matrix_misfit = 1
    percent_precip_misfit = 0.5
  [../]
[]

[BCs]
  [./dispx]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = '0 1 2 3'
  [../]

  [./dispy]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = '0 1 2 3'
  [../]
[]
[Executioner]
  type = Transient
  scheme = 'BDF2'

  [./TimeStepper]
    type = ConstantDT
    dt = 1E-4
  [../]

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  #petsc_options_iname = '-pc_type -ksp_gmres_restart'
  #petsc_options_value = 'lu 50'
  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_gmres_restart'
  petsc_options_value = ' asm      1              lu           30'

  l_max_its = 100
  l_tol = 1.0e-4
  nl_max_its = 20

  start_time = 0
  num_steps = 1
[]

[Outputs]
  file_base = AuxCalphadElasticity
  output_final = true
  exodus = true
  checkpoint = 1
  csv = true

  [./console]
    type = Console
      max_rows = 10
  [../]
[]
