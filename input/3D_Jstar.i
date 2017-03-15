#This simulation is in aJ, nm. It is to determine the nucleation rate as a function of temperature, hydrogen content, elastic strain energy, interfacial energy, etc.

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 1
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 1

#  elem_type = QUAD4
[]

[Variables]
  [./concentration]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      variable = concentration
      value = 0.1
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
      type = RandomIC
      variable = n
      min = 1e-9
      max = 0
    [../]
  [../]
[]

[AuxVariables]
  [./temperature]
    order = CONSTANT
    family = MONOMIAL
    [./InitialCondition]
       type = ConstantIC
       value = 600
    [../]
  [../]

  [./elem_ChemElastic]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elem_VolumetricRate]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elem_AMRProbability]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./AuxChemElastic]
    type = AuxCalphadElasticity
    variable = elem_ChemElastic
    concentration = concentration
    OP = n
    precip_conserved = 0.599 #this needs to be changed for each temperature
    precip_nonconserved = 1
    execute_on = timestep_end
    self_energy = 0
    use_elastic_energy = false
  [../]

  [./AuxVolumetricNucRate]
    type = AuxVolumetricNucleationRate
    variable = elem_VolumetricRate
    rate_volume = 1 #nm^3...everything in aJ and nm and nanoseconds
    coupled_bulk_energy_change = elem_ChemElastic
    T = temperature
    X = concentration
    gamma = 0.15 #aJ/nm^2
    jump_distance = 0.204 #nm
    execute_on = timestep_end
  [../]

  [./AuxAMRPRobability]
    type = AuxAMRNucleationProbability
    variable = elem_AMRProbability
    coupled_aux_var = elem_VolumetricRate
    coupled_variable = n
   # 2D_mesh_height = 20
    execute_on = timestep_end
  [../]
[]

[Preconditioning]
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
    n = n
    kappa_name = kappa_c
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

  [./AverageJstar]
   type = ElementAverageValue
   variable =  elem_VolumetricRate
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'BDF2'

 [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1e0
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

  num_steps = 10

#  end_time = 50
  dtmax = 1E6
  dtmin = 1E-5
[]

[Outputs]
  file_base = 3D_Jstar_test

  exodus = true
  interval = 1
  checkpoint = 1
  csv = true

  [./console]
    type = Console
    interval = 1
    max_rows = 10
#    linear_residuals = true
  [../]
[]
