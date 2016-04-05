#This simulation is non-dimensionalized

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20 #100
  ny = 20 #100
  nz = 0 #100
  xmin = 0 #25
  xmax = 20 #75
  ymin = 0 #25
  ymax = 20 #75
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]

  [./mu]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n2]
    order = FIRST
    family = LAGRANGE
  [../]

  [./t]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./Galpha]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Gdelta]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
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
[]

[ICs]
  [./PSSCIC_c]
      type = PolySpecifiedSmoothCircleIC
      variable = c
      int_width = 1
      invalue = 0.6
      outvalue = 0.4
      radius = 5
      x_positions = '7.5 12.5'
      y_positions = '7.5 12.5'
      z_positions = '0 0'
      n_seeds = 2
  [../]

#  [./PSSCIC_n1]
#      type = PolySpecifiedSmoothCircleIC
#      variable = n1
#      int_width = 1
#      invalue = 1
#      outvalue = 0
#      radius = 5
#      x_positions = '12.5 25 37.5'
#      y_positions = '12.5 25 37.5'
#      z_positions = '0 0 0'
#      n_seeds = 3
#  [../]

  [./PSSCIC_n1]
      type = SmoothCircleIC
      variable = n1
      int_width = 3
      invalue = 1
      outvalue = 0
      radius = 5
      x1 = 7.5
      y1 = 7.5
  [../]

  [./PSSCIC_n2]
      type = SmoothCircleIC
      variable = n2
      int_width = 3
      invalue = 1
      outvalue = 0
      radius = 5
      x1 = 12.5
      y1 = 12.5
  [../]

  [./t_init]
    type = ConstantIC
    variable = t
    value = 600
  [../]
[]

[Preconditioning]
  [./SMP]
   type = SMP
   off_diag_row =    'c  c  c   c   mu mu mu mu   n1 n1 n1 n1  n2 n2 n2 n2   t t  t  t'
   off_diag_column = 'mu n1 n2  t   c  n1 n2 t    c  mu t  n2  c  mu t  n1   c mu n1 n2'
  [../]
[]

[Kernels]
  [./dcdt]
    type = CoupledTimeDerivative
    variable = mu
    v = c
  [../]

  [./mu_residual]
    type = SplitCoupledCHWRes
    variable = mu
    mob_name = M
    c = c
    T = t
    n_OP_vars = 2
    OP_var_names = 'n1 n2'
  [../]

  [./conc_residual]
    type = CHCoupledCalphadSplit
    variable = c
    kappa_name = kappa_c
    w = mu
    n_OP_vars = 2
    OP_var_names = 'n1 n2'
    T = t
    scaling_factor = 2.49410145E-9
  [../]

  [./dn1dt]
    type = TimeDerivative
    variable = n1
  [../]

  [./dn2dt]
    type = TimeDerivative
    variable = n2
  [../]

  [./ACSolidn2]
    type = ACCoupledCalphad
    variable = n2
    mob_name = L
    n_OP_vars = 2
    OP_var_names = 'n1 n2'
    OP_number = 2
    w = mu
    T = t
    c = c
    scaling_factor = 2.49410145E-9
  [../]

  [./ACSolidn1]
    type = ACCoupledCalphad
    variable = n1
    mob_name = L
    n_OP_vars = 2
    OP_var_names = 'n1 n2'
    OP_number = 1
    w = mu
    T = t
    c = c
    scaling_factor = 2.49410145E-9
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACInterfacen2]
    type = ACInterface
    variable = n2
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./dTdt]
    type = TimeDerivative
    variable = t
  [../]

  [./THeat]
    type = Heat
    variable = t
    w = mu
    c = c
    OP_var_names = 'n1 n2'
    n_OP_vars = 2
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

    coupled_temperature = t
  [../]

  [./alphaZr]
   type = CalphadAB1CD1Material
   block = 0

   pure_endpoint_low_coeffs = '-7827.595
                                 125.64905
                                 -24.1618
                                  -0.00437791
                               34971.0' #HCP_Zr

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

   coupled_temperature = t
   coupled_concentration = c
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


   coupled_temperature = t
   coupled_concentration = c

   pure_EP1_phase1_coeffs = '-7827.595
                                125.64905
                                -24.1618
                                 -0.00437791
                              34971.0' #HCP_Zr
  [../]
[]

[Postprocessors]
  [./ElementIntegral_c]
    type = ElementIntegralVariablePostprocessor
    variable = c
    output = file
  [../]

  [./max_c]
    type = NodalMaxValue
    variable = c
    output = file
  [../]

  [./max_n1]
    type = NodalMaxValue
    variable = n1
    output = file
  [../]

  [./max_n2]
    type = NodalMaxValue
    variable = n2
    output = file
  [../]

  [./volume_fraction_n1]
    type = NodalVolumeFraction
    variable = 'n1'
    use_single_map = true
    threshold = 0.5
    execute_on = timestep_end
    mesh_volume = Volume
  [../]

  [./volume_fraction_n2]
    type = NodalVolumeFraction
    variable = 'n2'
    use_single_map = true
    threshold = 0.5
    execute_on = timestep_end
    mesh_volume = Volume
  [../]

  [./Volume]
    type = VolumePostprocessor
    execute_on = initial
  [../]
[]

[Executioner]
  type = Transient
  #scheme = 'crank-nicolson'
  scheme = 'BDF2'

  [./TimeStepper]
    #type = SolutionTimeAdaptiveDT
    #type = ConstantDT
    type = InitialSolutionAdaptiveDT
    #type = PhaseFractionDT

    dt = 1E-5
    #growth_factor = 0.1

    initial_dt = 1E-6
    has_initial_dt = true
    n_initial_steps = 2

   # postprocessor = volume_fraction_n1

  [../]


  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -ksp_gmres_restart'
  petsc_options_value = 'ilu 50'

  l_max_its = 250
  #l_tol = 1.0e-5

  #nl_max_its = 40
  #nl_rel_tol = 3.0e-7
  #nl_max_its = 20
  #nl_abs_tol = 2.15E-6

  start_time = 0.0
  num_steps = 3

[]

[Outputs]
  file_base = ZrHCalphad_3OP
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
  csv = true
[]
