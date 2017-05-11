#This tests the chemical free energy-based work for Hyrax.  units are attojoules, nanometers, microseconds

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 25
  nz = 0
  xmin = 0
  xmax = 25
  ymin = 0
  ymax = 25
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
[]

[AuxVariables]
  [./temperature]
    order = FIRST
    family = MONOMIAL
  [../]

  [./Galpha]
    order = FIRST
    family = MONOMIAL
  [../]

  [./Gdelta]
    order = FIRST
    family = MONOMIAL
  [../]

  [./dGalphadc]
    order = FIRST
    family = MONOMIAL
  [../]

  [./dGdeltadc]
    order = FIRST
    family = MONOMIAL
  [../]


  [./d2Galphadc2]
    order = FIRST
    family = MONOMIAL
  [../]

  [./d2Gdeltadc2]
    order = FIRST
    family = MONOMIAL
  [../]

  [./chem_energy]
    order = FIRST
    family = MONOMIAL
  [../]

  [./grad_energy]
    order = FIRST
    family = MONOMIAL
  [../]

  [./dfchemdc]
    order = FIRST
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

  [./auxdGalphaDc]
    type = MaterialRealAux
    variable = dGalphadc
    property = dGAB1CD1_dc
  [../]

  [./auxdGdeltaDc]
    type = MaterialRealAux
    variable = dGdeltadc
    property = dGAB1CD2_dc
  [../]

  [./auxd2Galphadc2]
    type = MaterialRealAux
    variable = d2Galphadc2
    property = d2GAB1CD1_dc2
  [../]

   [./auxd2Gdeltadc2]
    type = MaterialRealAux
    variable = d2Gdeltadc2
    property = d2GAB1CD2_dc2
   [../]

  [./auxchemenergy]
    type = AuxBulkEnergyCalphad
    variable = chem_energy
  [../]

   [./auxgradenergy]
     type = AuxGradientEnergy
     variable = grad_energy
     c = concentration
     OP = n
   [../]

   [./auxdfchemdc]
     type = AuxDFchemDC
     variable = dfchemdc
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
      x1 = 12.5
      y1 = 12.5
  [../]

  [./PSSCIC_n]
      type = SmoothCircleIC
      variable = n
      int_width = 3
      invalue = 1
      outvalue = 0
      radius = 4
      x1 = 12.5
      y1 = 12.5
  [../]

  [./t_init]
    type = ConstantIC
    variable = temperature
    value = 600
  [../]
[]

[Preconditioning]
 active = 'SMP'
  [./SMP]
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
    type = SplitCHWRes
    variable = mu
    mob_name = M
  [../]

  [./conc_residual]
    type = CHCoupledCalphadSplit
    variable = concentration
    kappa_name = kappa_c
    w = mu
    n = n
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

   #still diffusion-controlled?
    mobility_AC = 1E-1 #nm^3/(aJ microsecond)

    #I have no idea what this needs to be
    kappa_CH = 0 #aJ/nm
    kappa_AC = 1 #aJ/nm

    #well height and molar volume remain unscaled.
    well_height = 0 #aJ/amol
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
				  0.0
                               34971.0' #HCP_Zr

   pure_endpoint_high_coeffs = '-9522.97393
   			           78.5273873
				  -31.35707
				    0.0027589925
				   -7.46390667E-7
				56582.3' #H2_gas
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
				 0.0
                             34971' #FCC_Zr

   pure_endpoint_high_coeffs = '-9522.97393
   			           78.5273873
				  -31.35707
				    0.0027589925
				   -7.46390667E-7
				56582.3' #H2_gas

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
				  0.0
                               34971.0' #HCP_Zr
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'BDF2'

 [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1e-2
    cutback_factor = 0.25
    growth_factor = 1.05
    optimal_iterations = 5
    iteration_window = 1
    linear_iteration_ratio = 100
  [../]

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -sub_pc_type'
  petsc_options_value = ' ksp      lu'

  l_max_its = 50
  l_tol = 1.0e-4

  nl_rel_tol = 1.0e-6
  nl_abs_tol = 5e-10
  nl_max_its = 10

  start_time = 0

  num_steps = 2

#  end_time = 50
  dtmax = 1E6
  dtmin = 1E-5
[]

[Outputs]
  file_base = ZrHCalphad

  exodus = true
  interval = 1
  checkpoint = false
  csv = false

  [./console]
    type = Console
    interval = 1
    max_rows = 10
#    linear_residuals = true
  [../]
[]
