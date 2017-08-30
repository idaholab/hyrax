#This simulation is in aJ, nm.  I'm slowly building all the components up to be correct.
#NB that the solid mechanics tensor for hcp Zr is ROTATED to be in the XZ plane (to see anisotropy).
#1: Calphad free energy only, coupled CH/AC with 1 order parameter
#2: Solid mechanics with elastic energy correctly into the coupled split formulation
#3: nucleation with the correct units

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  xmin = 0
  xmax = 50
  ymin = 0
  ymax = 50

#  elem_type = QUAD4
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
      outvalue = 0.045
      x_positions = '50'
      y_positions = '50'
      z_positions = '0'
      coefficients = '10 10 10'
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
      x_positions = '50'
      y_positions = '50'
      z_positions = '0'
      coefficients = '10 10 10'
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
    precip_conserved = 0.6 #this needs to be changed for each temperature
    precip_nonconserved = 1
    execute_on = timestep_end
    self_energy = 0
    use_elastic_energy = false
  [../]

  [./AuxVolumetricNucRate]
    type = AuxVolumetricNucleationRate
    variable = elem_VolumetricRate
    rate_volume = 1 #nm^3...everything in aJ and nm and microseconds ..right?
    coupled_bulk_energy_change = elem_ChemElastic
    T = temperature
    X = concentration
    gamma = 0.1 #aJ/nm^2
    jump_distance = 0.204 #nm
    execute_on = timestep_end
  [../]

  [./AuxAMRPRobability]
    type = AuxAMRNucleationProbability
    variable = elem_AMRProbability
    coupled_aux_var = elem_VolumetricRate
    coupled_variable = n
    2D_mesh_height = 20
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
    kappa_CH = 0.07 #aJ/nm
    kappa_AC = 0.2  #aJ/nm

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
#    [./EFM_2]
#      type = ErrorFractionMarker
#      coarsen = 0.05
#      refine = 0.25
#      indicator = GJI_2
#    [../]
    [./NM]
      type = NucleationMarker
      nucleation_userobject = NLUO
      max_h_level = 4
    [../]

     [./combo]
       type = ComboMarker
       markers = 'EFM_1 NM' # EFM_2 NM'
     [../]
  [../]

  [./Indicators]
    [./GJI_1]
     type = GradientJumpIndicator
      variable = n
    [../]
#    [./GJI_2]
#     type = GradientJumpIndicator
#      variable = concentration
#    [../]
  [../]
[]

[UserObjects]
  [./NLUO]
    type = NucleationLocationUserObject
    coupled_probability = elem_AMRProbability
    dwell_time = 10
    execute_on = timestep_end
    random_seed = 1000
  [../]

  [./NISM]
    type = NucleusIntroductionSolutionModifier
    nucleation_userobject = NLUO
    variables = n
    num_vars = 1
    seed_value = 1
    radius = 3
    int_width = 1
    execute_on = custom
  [../]
[]

[Executioner]
  type = MeshSolutionModify
  scheme = 'BDF2'

 [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 5e-4
    cutback_factor = 0.25
    growth_factor = 1.05
    optimal_iterations = 5
    iteration_window = 1
    linear_iteration_ratio = 100
  [../]

  adapt_nucleus = 4
  adapt_cycles = 1

  use_nucleation_userobject = true
  nucleation_userobject = NLUO

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

  num_steps = 14

#  end_time = 50
  dtmax = 1E6
  dtmin = 1E-5
[]

[Outputs]
  file_base = Nuclei

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
