#This simulation is in 1D to test the coupled CH-AC system with no elasticity.
#units are aJ, nm, microseconds, attomol, and K.
#Except in the diffusivity for Q, R, T.  Can just do J/mol there; default values in J mol K are for R, kB, etc.


[Mesh]
  #dimensions in nm
  type = GeneratedMesh
  dim = 1
  nx = 400
  ny = 1
  nz = 0
  xmin = 0
  xmax = 100
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 0
  elem_type = EDGE2
[]

[Variables]
  [./concentration]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      variable = concentration
      x1 = 0
      y1 = 0
      z1 = 0
      invalue = 0.6
      outvalue = 0.03
      radius = 5
      int_width = 3
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
      type = FunctionIC
      variable = n
      type = SmoothCircleIC
      variable = concentration
      x1 = 0
      y1 = 0
      z1 = 0
      invalue = 1
      outvalue = 0
      radius = 5
      int_width = 3
    [../]
  [../]
[]

[AuxVariables]
  [./temperature]
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
[]

[Preconditioning]
  [./SMP_TempAux]
   type = SMP
   full = true
  [../]
[]

[Kernels]
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

    kappa_CH = 1e0 #aJ/nm
    kappa_AC = 1e0 #aJ/nm

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
[]

[Executioner]
  type = Transient
  scheme = 'BDF2'

  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 1e0
    percent_change = 0.05
  [../]

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_gmres_restart'
  petsc_options_value = ' asm      1              lu           30'

  l_max_its = 100
  l_tol = 1.0e-4

  nl_rel_tol = 1.0e-6
  nl_abs_tol = 5e-10
  nl_max_its = 20

  start_time = 0
  num_steps = 10
[]

[Outputs]
  file_base = 1D_CH_AC

  exodus = true
  interval = 1
  csv = false
[]
