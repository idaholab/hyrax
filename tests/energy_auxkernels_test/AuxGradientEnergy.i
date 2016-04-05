#This simulation is non-dimensionalized

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50 #100
  ny = 50 #100
  nz = 0 #100
  xmin = 0 #25
  xmax = 50   #75
  ymin = 0 #25
  ymax = 50  #75
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
[]

[AuxVariables]
  [./temperature]
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
[]

[AuxKernels]
  [./auxtemp]
    type = AuxTemperature
    variable = temperature
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
[]

[ICs]
  [./PSSCIC_c]
      type = SmoothCircleIC
      variable = concentration
      int_width = 3
      invalue = 0.6
      outvalue = 0.1
      radius = 4
      x1 = 25
      y1 = 25
  [../]

  [./PSSCIC_n]
      type = SmoothCircleIC
      variable = n
      int_width = 3
      invalue = 1
      outvalue = 0
      radius = 4
      x1 = 25
      y1 = 25
  [../]

  [./t_init]
    type = ConstantIC
    variable = temperature
    value = 600
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
[]


[Materials]
  [./calphad]
    type = ZrHCalphad
    block = 0

    #H_Zr_D0 = 7.00e5 #um^2/s
    #H_ZrH2_D0 = 1.53e5 # um^2/s
    #H_Zr_Q0 =  4.456e4 #J/mol
    #H_ZrH2_Q0 = 5.885E4 #J/mol

    mobility_AC = 1E0
    #mobility_CH = 2E-10
    mobility_CH = 1E3

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
[]

[Postprocessors]
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
[]

[Executioner]
  type = Transient
  #scheme = 'crank-nicolson'
  scheme = 'BDF2'

  [./TimeStepper]
    type = ConstantDT
    dt = 1E-4
  [../]


  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  #solve_type = 'FD'

  petsc_options_iname = '-pc_type -ksp_gmres_restart'
  petsc_options_value = 'ilu 50'

   l_max_its = 250
  #l_tol = 1.0e-8

  #nl_max_its = 40
  #nl_rel_tol = 3.0e-7
  #nl_max_its = 20
  #nl_abs_tol = 2.0E-5

  start_time = 0
  num_steps = 1
[]

[Outputs]
  file_base = AuxGradientEnergy
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
  csv = true
[]
