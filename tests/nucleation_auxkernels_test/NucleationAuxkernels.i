# This input file demonstrates the nucleation aux kernels.  Uses ELEMENTAL aux variables!

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
    [./InitialCondition]
      type = SmoothCircleIC
      variable = concentration
      int_width = 3
      invalue = 0.6
      outvalue = 0.1
      radius = 4
      x1 = 12.5
      y1 = 12.5
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
      type = SmoothCircleIC
      variable = n
      int_width = 3
      invalue = 1
      outvalue = 0
      radius = 4
      x1 = 12.5
      y1 = 12.5
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

  [./elem_DeltaGStar]
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

  [./AuxDeltaGStar]
    type = AuxDeltaGStar
    variable = elem_DeltaGStar
    coupled_aux_var = elem_ChemElastic
    gamma = 0.1
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
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
  [../]
 [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]

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
    type = CHPrecipMatrixElasticity #CHCoupledCalphadSplit
    variable = concentration
    kappa_name = kappa_c
    w = mu
    use_elasticity = true #false
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
  [../]
[]

[BCs]
  [./Dirichlet_dispx]
    type = DirichletBC
    variable = disp_x
    value = 0.0
    boundary = '0'
  [../]

 [./Dirichlet_dispy]
    type = DirichletBC
    variable = disp_y
    value = 0.0
    boundary = '0'
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
    kappa_CH = 1 #aJ/nm
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

  [./Zr_system]
    type = TwoPhaseLinearElasticMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y

    #units: aJ/nm^3

    #reading         C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66

    #adjusted these to the temperature-dependent numbers from Fisher
    #600k
    #this is rotated
    #Because the simulation is in the xz plane and a 2D simulation, the tensor is rotated (aJ/nm^3)
    Cijkl_matrix = '128.86 65.75 78.98 155.04 65.75 128.86 27.88 26.44 27.88'


    #adjusted these to delta ZrHy 1.5 from Olsson
    Cijkl_precip = '162 103 103 162 103 162 69.3 69.3 69.3'

    #reading          S_11    S_22   S_33   S_23 S_13 S_12
    #this is rotated
    matrix_eigenstrain       = '0.0329  0.0542 0.0329 0.0  0.0  0.0'

    order_parameter = 'n'
    matrix_fill_method = symmetric9
    precip_fill_method = symmetric9

    atomic_fraction = concentration

    #THIS HAS TEMPERATURE DEPENDENCE
    temperature = temperature
    #this is rotated
    precipitate_eigenstrain = '0.03888 0.06646 0.03888 0 0 0'
    #this is rotated
    precip_misfit_T_coeffs = '2.315E-5 1.9348E-5 2.315E-5 0 0 0'
    percent_precip_misfit = 0.5
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

#  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_gmres_restart'
#  petsc_options_value = ' asm      4              lu           30'


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
  file_base = NucleationAuxkernels

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
