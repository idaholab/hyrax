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
    order = FIRST
    family = MONOMIAL
  [../]

  [./elem_ChemElastic]
    order = FIRST
    family = MONOMIAL
  [../]

  [./s11_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./s12_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./s22_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./s13_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./s23_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./s33_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./e11_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./e12_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./e22_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./e13_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./e23_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./e33_aux]
    order = FIRST
    family = MONOMIAL
  [../]

  [./dfeldc]
    order = FIRST
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

  [./matl_s11]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 0
    index_j = 0
    variable = s11_aux
  [../]

 [./matl_s12]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 0
    index_j = 1
    variable = s12_aux
  [../]

  [./matl_s22]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    variable = s22_aux
  [../]

  [./matl_s13]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 0
    index_j = 2
    variable = s13_aux
  [../]

 [./matl_s23]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 2
    variable = s23_aux
  [../]

  [./matl_s33]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 2
    index_j = 2
    variable = s33_aux
  [../]

  [./matl_e11]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 0
    index_j = 0
    variable = e11_aux
  [../]

 [./matl_e12]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 0
    index_j = 1
    variable = e12_aux
  [../]

  [./matl_e22]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 1
    variable = e22_aux
  [../]

  [./matl_e13]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 0
    index_j = 2
    variable = e13_aux
  [../]

 [./matl_e23]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 2
    variable = e23_aux
  [../]

  [./matl_e33]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 2
    index_j = 2
    variable = e33_aux
  [../]

   [./auxdfeldc]
     type = AuxDFelDC
     variable = dfeldc
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
    type = CHPrecipMatrixElasticity
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

  [./ACTransform]
    type = ACTransformElasticDF
    variable = n
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

 [./Zr_system]
    type = TwoPhaseLinearElasticMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y

    #units: aJ/nm^3

    #reading         C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66
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

[BCs]
[./pin_nodex]
    type = DirichletBC
    variable = disp_x
    value = 0.0
    boundary = '0'
  [../]

 [./pin_nodey]
    type = DirichletBC
    variable = disp_y
    value = 0.0
    boundary = '0'
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

  num_steps = 2

#  end_time = 50
  dtmax = 1E6
  dtmin = 1E-5
[]

[Outputs]
  file_base = Elasticity

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
