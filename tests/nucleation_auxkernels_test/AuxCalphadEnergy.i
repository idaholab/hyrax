# This input file demonstrates the AuxchemElastic aux kernel.  Uses ELEMENTAL aux variables!

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
#  uniform_refine = 4
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

  [./n1]
    order = FIRST
    family = LAGRANGE
  [../]

  [./temperature]
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

[Preconditioning]
 active = 'SMP'
  [./SMP]
   type = SMP
   off_diag_row = 'concentration concentration concentration mu mu mu n1 n1 n1 temperature temperature temperature'
   off_diag_column = 'mu n1 temperature concentration n1 temperature concentration mu temperature concentration mu n1'
  [../]
[]

[AuxVariables]
  [./elemental_Calphad]
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

[ICs]
  [./SSIC_c]
    type = SmoothCircleIC
    variable = concentration
    int_width = 3
    invalue = 0.61
    outvalue = 0.1
    radius = 3
    x1 = 12.5
    y1 = 12.5
  [../]

  [./SSIC_n]
    type = SmoothCircleIC
    variable = n1
    int_width = 3
    invalue = 1
    outvalue = 0
    radius = 3
    x1 = 12.5
    y1 = 12.5
  [../]

  [./t_init]
    type = ConstantIC
    variable = temperature
    value = 600
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
    T = temperature
    n_OP_vars = 1
    OP_var_names = 'n1'
  [../]

  [./conc_residual]
    type = CHCoupledCalphadSplit
    variable = concentration
    kappa_name = kappa_c
    w = mu
    n_OP_vars = 1
    OP_var_names = 'n1'
    T = temperature
    #Well_height = 1
    scaling_factor = 2.49410145E-9
  [../]

  [./dndt]
    type = TimeDerivative
    variable = n1
  [../]

  [./ACSolidn]
    type = ACCoupledCalphad
    variable = n1
    mob_name = L
    #coupled_CH_var = concentration
    n_OP_vars = 1
    OP_var_names = 'n1'
    OP_number = 1
    w = mu
    T = temperature
    c = concentration
    scaling_factor = 2.49410145E-9
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

[./ACTransform]
    type = ACTransformElasticDF
    variable = n1
    OP_number = 1
    OP_var_names = 'n1'
    n_OP_vars = 1
    #scaling_factor = 2.49410145E-9
    #c = concentration
    #w = mu
    #T = temperature
  [../]

  [./dTdt]
    type = TimeDerivative
    variable = temperature
  [../]


  [./THeat]
    type = Heat
    variable = temperature
    w = mu
    c = concentration
    OP_var_names = 'n1'
    n_OP_vars = 1
  [../]
[]

[AuxKernels]
  [./Calphad_n1]
    type = AuxCalphadEnergy
    coupled_temperature = temperature
    hcp_Zr_coeffs = '-7827.595 125.64905 -24.1618 -0.00437791 34971.0' #HCP_Zr
    H2_coeffs = '2589675 -34719.21 5126.9713 -3.250187 -208070050'  #H2_gas
    hcp_ZrH_coeffs = '-45965 41.6 0'
    fcc_Zr_coeffs = '-227.595 124.74905 -24.1618 -0.00437791 34971' #FCC_Zr
    fcc_ZrH2_coeffs = '-170490 208.2 -9.47' #FCC_ZrH2'
    L0_coeffs = '14385 -6.0'
    L1_coeffs = '-106445 87.3'

    variable = elemental_Calphad
    coupled_conserved_var = concentration
    coupled_nonconserved_var = n1
    nonconserved_var_number = 1
    precip_conserved = 0.6
    precip_nonconserved = 1.6
  #  functional_c1 = 0.006
    execute_on = timestep_end
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


  pure_endpoint_high_coeffs = '2589675
                                 -34719.21
                                   5126.9713
                                     -3.250187
                             -208070050'  #H2_gas
   mixture_coeffs = '-45965
                         41.6
                          0'  #HCP_ZrH
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

  [./test_material]
    type = LinearSingleCrystalPrecipitateMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    #reading C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66
    C_ijkl = '155.4E9 68.03E9 64.6E9 155.4E9  64.6E9 172.51E9 36.31E9 36.31E9 44.09E9'
    C_precipitate ='155.4E9 68.03E9 64.6E9 155.4E9 64.6E9 172.51E9 36.31E9 36.31E9 44.09E9'
    #reading        S_11   S_22  S_33 S_23 S_13 S_12
    #Gamma hydride misfit strain here
    e_precipitate = '0.00551  0.0564  0.0570  0.0  0.0  0.0'
    n_variants = 1
    variable_names = 'n1'
    fill_method = symmetric9
    scaling_factor = 2.49410145E-9
  [../]
[]

[Executioner]
  type = MeshSolutionModify
#type = Transient
  scheme = 'bdf2'
  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 1E-5
  [../]

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 250
#  nl_max_its = 10

  start_time = 0
  num_steps = 3
  dtmin = 1e-5
  dtmax = 1e-1

  abort_on_solve_fail = true
[]

[Outputs]
  file_base = AuxCalphadEnergy
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
