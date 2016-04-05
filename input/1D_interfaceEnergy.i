#This simulation is in 1D to find the interfacial energy, J/m^2. 
#Units are going to need to be re-dimensionalised.
#To use this, get the equilibrium omega value first by setting omega_eq = 0 and allowing the simulation to run to equilibrium.  The omega value at either end of the simulation should be the same, and that's omega_eq.  Then run the simulation again to find the interface energy in J/unit^2 (nm here) by supplying omega_eq to the auxkernel.  It outputs as gamma, the postprocessor 

#Note that the auxkernel does not include a non-zero g(eta) term in the free energy.

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 50
  ny = 1
  nz = 0
  xmin = 0
  xmax = 20
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 0
  elem_type = EDGE2
  #uniform_refine = 2
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
      outvalue = 0.06
      radius = 2
      int_width = 1
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
      radius = 2
      int_width = 1
    [../]
  [../]
[]

[AuxVariables]
  [./temperature]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./omega]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./auxtemp]
    type = AuxTemperature
    variable = temperature
    temp_in_K = 485
  [../]

  [./omega_calc]
    type = AuxCanonicalEnsemble
    variable = omega
    OP = n
    concentration = concentration
    omega_eq = -1.451e-18 #485K #-1.7036e-18 #550K #-2.1247e-18 #650K -1.90922e-18 #600K
    length_scale_factor = 1e-9
    energy_scale_factor = 4.009e8
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
    scaling_factor = 2.494E-9
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
    scaling_factor = 2.494E-9
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

    H_Zr_D0 = 7.00e-7    #m^2/s
    H_ZrH2_D0 = 1.53e-7  # m^2/s
    H_Zr_Q0 =  4.456e4   #J/mol
    H_ZrH2_Q0 = 5.885E4  #J/mol

    #ALWAYS have mobility_AC = 1 for nondimensionalization (L*)!!
    #we pick L = 1E-5, and so L* = 1
    mobility_AC = 1E0
    #to scale mobility, M* = M/L*l^2
    CH_mobility_scaling = 1E-23

    #interface energies are scaled
    kappa_CH = 2.5
    kappa_AC = 2.5

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
[]

[Postprocessors]
  [./VolumeFraction]
    type = NodalVolumeFraction
    bubble_volume_file = 1D_interfaceEnergy_vol.csv
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

  [./Gamma]
    type = ElementIntegralVariablePostprocessor
    variable = omega
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'BDF2'

  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 5E-5
    percent_change = 0.05
  [../]

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_gmres_restart'
  petsc_options_value = ' asm      1              lu           30'

  l_max_its = 100
  l_tol = 1.0e-4

  nl_rel_tol = 1.0e-8
  nl_abs_tol = 5e-10
  nl_max_its = 200

  start_time = 0
  num_steps = 10000
  end_time = 50
  dtmax = 1E0
  dtmin = 1E-8
[]


[Adaptivity]
  marker = combo
  initial_steps = 3
  initial_marker = EFM_1
  max_h_level = 3
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
     [./combo]
       type = ComboMarker
       markers = 'EFM_1 EFM_2'
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
  [../]
[]

[Outputs]
  file_base = 1D_interfaceEnergy_485K_kckn2p5

  exodus = true
  interval = 10
  checkpoint = 0
  csv = true

  [./console]
    type = Console
    interval = 10
    max_rows = 10
    output_linear = 0
  [../]
[]

