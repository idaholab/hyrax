#This simulation is in 1D to find the interfacial energy, aJ/nm^2.
#To use this, get the equilibrium omega value first by setting omega_eq = 0 and allowing the simulation to run to equilibrium.
#The omega value at either end of the simulation should be the same, and that's omega_eq.
#Then run the simulation again to find the interface energy in aJ/nm^2 by supplying omega_eq to the auxkernel.
#interface energy It outputs as gamma, the postprocessor

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 150
  ny = 0
  nz = 0
  xmin = 0
  xmax = 50
  ymin = 0
  ymax = 0
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
      outvalue = 0.01
      radius = 20
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
      radius = 20
      int_width = 1
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

  [./omega]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./omega_calc]
    type = AuxGrandPotential
    variable = omega
    omega_eq = -1.9093 #600K  # -1.45135 #485K
    concentration = concentration
    OP = n
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
    type = SplitCHWRes
    variable = mu
    mob_name = M
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

    kappa_CH = 1 #aJ/nm
    kappa_AC = 1 #aJ/nm

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
  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_gmres_restart'
  petsc_options_value = ' asm      1              lu           30'

  l_max_its = 100
  l_tol = 1.0e-4

  nl_rel_tol = 1.0e-8
  nl_abs_tol = 5e-10
  nl_max_its = 20

  start_time = 0
  num_steps = 10000
  #end_time = 500
  #dtmax = 1E0
  #dtmin = 1E-8
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
  file_base = 1D_interfaceEnergy_600K_kc0kn05_eq

  exodus = true
  interval = 20
  checkpoint = 0
  csv = true

  [./console]
    type = Console
    interval = 20
    max_rows = 10
    output_linear = 0
  [../]
[]
