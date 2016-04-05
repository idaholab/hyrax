#This simulation is non-dimensionalized

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100 #500
  ny = 100 #500
  nz = 0
  xmin = 200
  xmax = 300 #500
  ymin = 200
  ymax = 300 #500
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  #elem_type = QUAD9
  #elem_type = HEX8
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

[AuxVariables]
  [./temperature]
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
[]

[ICs]
  [./PSSCIC_c]
      type = SmoothCircleIC
      variable = concentration
      int_width = 3
      invalue = 0.6
      outvalue = 0.1
      radius = 10
      x1 = 250
      y1 = 250
  [../]

  [./PSSCIC_n]
      type = SmoothCircleIC
      variable = n
      int_width = 3
      invalue = 1
      outvalue = 0
      radius = 10
      x1 = 250
      y1 = 250
  [../]

  [./t_init]
    type = ConstantIC
    variable = temperature
    value = 600
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
    type = ACTransformElasticDF
    variable = n
    OP_number = 1
    OP_var_names = 'n'
    n_OP_vars = 1
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
    type = LinearSingleCrystalPrecipitateMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    #reading C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66
    C_ijkl = '155.4E9 68.03E9 64.6E9 155.4E9  64.6E9 172.51E9 36.31E9 36.31E9 44.09E9'
    C_precipitate ='155.4E9 68.03E9 64.6E9 155.4E9 64.6E9 172.51E9 36.31E9 36.31E9 44.09E9'
    #reading        S_11   S_22  S_33 S_23 S_13 S_12
    #e_precipitate = '0.00551  0.0564  0.0570  0.0  0.0  0.0'
    n_variants = 1
    variable_names = 'n'
    all_21 = false
    scaling_factor = 2.49410145E-9

    #THIS HAS TEMPERATURE DEPENDENCE
    temperature = temperature
    e_precipitate = '0.03888 0.0388 0.06646 0 0 0'
    misfit_temperature_coeffs = '2.315E-5 2.315E-5 1.9348E-5 0 0 0'

  [../]
[]

[BCs]
  [./Dirichlet_dispx]
    type = DirichletBC
    variable = disp_x
    value = 0.0
    boundary = '0 1 2 3'
  [../]

 [./Dirichlet_dispy]
    type = DirichletBC
    variable = disp_y
    value = 0.0
    boundary = '0 1 2 3'
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
    output = both
  [../]

 [./GradN_energy]
    type = ElementIntegralVariablePostprocessor
    variable = Grad_N_energy
    output = both
  [../]

 [./Elastic_energy]
    type = ElementIntegralVariablePostprocessor
    variable = elastic_energy
    output = both
  [../]
[]

[Executioner]
  type = Transient
  #scheme = 'crank-nicolson'
  scheme = 'BDF2'

  [./TimeStepper]
     type = SolutionTimeAdaptiveDT
    #ype = ConstantDT
    dt = 1E-5
    percent_change = 0.1
  [../]


  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  #solve_type = 'FD'

  petsc_options_iname = '-pc_type -ksp_gmres_restart'
  petsc_options_value = 'ilu 50'

   l_max_its = 50
  #l_tol = 1.0e-8

  #nl_max_its = 40
  #nl_rel_tol = 3.0e-7
  #nl_max_its = 20
  #nl_abs_tol = 2.0E-5

  start_time = 0
  num_steps = 50
  dtmax = 1E-1
  dtmin = 1E-7
[]

[Output]
  file_base = CalphadOldElasNewDiff
  interval = 1
  linear_residuals = true
  exodus = true
  perf_log = true
  all_var_norms = true

  postprocessor_csv = true
[]
