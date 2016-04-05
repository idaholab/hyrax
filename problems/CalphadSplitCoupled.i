[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50 #100
  ny = 50 #100
  nz = 0 #100
  xmin = 25E-9
  xmax = 75E-9
  ymin = 25E-9
  ymax = 75E-9
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  #elem_type = QUAD9
  #elem_type = HEX8
[]

[Variables]
  [./concentration]
   # order = SECOND
    order = FIRST
    family = LAGRANGE
  [../]

  [./mu]
    order = FIRST
    #order = SECOND
    family = LAGRANGE
  [../]

  [./n]
    order = FIRST
    family = LAGRANGE
  [../]

  [./temperature]
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

  [./dGalphadc]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./dGdeltadc]
    order = CONSTANT
    family = MONOMIAL
  [../]


  [./d2Galphadc2]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./d2Gdeltadc2]
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
[]

[ICs]
  [./PSSCIC_c]
      type = SmoothCircleIC
      #type = ConstantIC
      #type = RandomIC
      #value = 0.1
      variable = concentration
      #min = 0.6549
      #max = 0.6551
      int_width = 3E-9
      invalue = 0.6
      outvalue = 0.1
      radius = 5E-9
      x1 = 50E-9
      y1 = 50E-9
  [../]

  [./PSSCIC_n]
      type = SmoothCircleIC
      variable = n
      int_width = 3E-9
      invalue = 1
      outvalue = 0
      radius = 5E-9
      x1 = 50E-9
      y1 = 50E-9
      #type = RandomIC
      #min = 0.0
      #max = 0.001
  [../]

  [./t_init]
    type = ConstantIC
    variable = temperature
    value = 600
  [../]
[]

#[Preconditioning]
#active = 'SMP'
#  [./PBP]
#   type = PBP
#   solve_order = 'w c'
#   preconditioner = 'AMG ASM'
#   off_diag_row = 'c '
#   off_diag_column = 'w '
#  [../]

#  [./SMP]
#   type = SMP
#   off_diag_row = 'mu concentration'
#   off_diag_column = 'concentration mu'
#  [../]
#[]

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
    c = concentration
  [../]

  [./conc_residual]
    type = CHCoupledCalphadSplit
    variable = concentration
    kappa_name = kappa_c
    coupled_OP_var = n
    w = mu
    #Well_height = 1
  [../]

  [./dndt]
    type = TimeDerivative
    variable = n
  [../]

  [./ACSolidn]
    type = ACCoupledCalphad
    variable = n
    mob_name = L
    #coupled_CH_var = concentration
    n_OP_vars = 1
    OP_var_names = 'n'
    OP_number = 1
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./dTdt]
    type = TimeDerivative
    variable = temperature
  [../]


  [./THeat]
    type = Heat
    variable = temperature
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

    mobility_AC = 2E3
    #mobility_CH = 2E-10
    mobility_CH = 2E0

    kappa_CH = 7.812E-10
    kappa_AC = 7.812E-9

    well_height = 60000

    molar_volume = 1.4E-5

    thermal_diffusivity = 1

    coupled_temperature = temperature
  [../]

  [./alphaZr]
   type = CalphadAB1CD1
   block = 0

   pure_endpoint_low_coeffs = '-7827.595
                                 125.64905
                                 -24.1618
                                  -0.00437791
                               34971.0' #HCP_Zr
  #this is fucked...

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
#   low_coeffs = '   -26549.141703672653
#                 -31205591.610163368
#                       295.24365293254311
#                7516799743.7832689
#                        -0.42527422354460137
#                     -4439.0959906994285
#                         0.00022153720566517584
#                    -75983.150207815517
#                  27656668.097620554
#                       -18.137337020144734'

#   high_coeffs = '-1.1748697700305443E7
#                   7.1428722799052447E7
#                   2.0711053625846262E2
#                  -1.4437278547848886E8
#                   3.5284806111023137E-1
#                   9.7186016495869264E7
#                  -1.9493853217896482E-4
#                  -1.9533177347714059E3
#                   2.0853116312248094E3
#                  -3.9319750123563216E-3'

   low_coeffs = ' -2.3193005500010659E3
                  -9.6686378992784186E4
                  -1.9584791279287277E1
                   2.7064582471779820E7
                  -4.0344221621395518E-2
                  -4.8360390794611511E9
                   8.8627414820406476E-6
                  -2.6809498774745819E1
                  -5.6036536163730307E3
                   2.0832427364711584E-2'

   high_coeffs = ' -6.4402100857458472E8
                   3.8572536909359312E9
                   1.2750473955256184E4
                  -7.7004063005537968E9
                   3.2145786231203610E-1
                   5.1241392707094860E9
                  -1.9850611115707398E-4
                  -5.2116707213338450E4
                   5.2220794055700229E4
                    7.2767994212535686E-2'

    coupled_temperature = temperature
    coupled_concentration = concentration
  [../]

  [./deltaZrH2]
   type = CalphadAB1CD2
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

#   low_coeffs =  '2.6978175664339593E4
#                 -5.2267355124714538E7
#                  7.7077694451557377E1
#                  2.5671915976342548E10
#                 -3.0872406268790027E-2
#                 -4.5394250810591288E1
#                 -1.5479208178631023E-5
#                 -8.3176435859210935E4
#                  4.4517036341637028E6
#                  2.1739434332085402E1'

#    high_coeffs = ' -4.8353171133552969E7
#                     2.2151622968687481E8
#                    -6.0924808041164090E2
#                    -3.3818504692030811E8
#                     5.3781670052188990E-1
#                     1.7206754571759129E8
#                    -2.6254489645679825E-4
#                     7.6102442020895887E2
#                    -4.4943040350977139E2
#                    -9.0858040404436430E-2'

   low_coeffs = ' 5.1885861560133571E3
                 -4.8811605580390577E4
                 -2.0128059775256556E1
                  1.0164702396392035E4
                 -4.0913939010625185E-2
                 -5.9160467838341198E1
                  9.1808114941039917E-6
                 -3.9730434705971000E1
                  5.3456168131031118E3
                 -1.2961561364383261E-2'

    high_coeffs = '-4.8514324670638323E7
                    2.2227380532295138E8
                   -6.3829116146213414E2
                   -3.3936440910766947E8
                    5.4323628798569534E-1
                    1.7267597255673271E+8
                   -2.6269091463396069E-4
                    8.3948759619112002E2
                   -5.0208515875980800E2
                   -9.8630149789327354E-2'


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
  [./ElementIntegral_c]
    type = ElementIntegralVariablePostprocessor
    variable = concentration
    output = file
  [../]

  [./ElementIntegral_n]
    type = ElementIntegralVariablePostprocessor
    variable = n
    output = file
  [../]

  [./max_c]
    type = NodalMaxValue
    variable = concentration
    output = file
  [../]

  [./max_n]
    type = NodalMaxValue
    variable = n
    output = file
  [../]

  [./volume_fraction]
    type = NodalVolumeFraction
    variable = 'n'
    use_single_map = true
    threshold = 0.75
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
    type = SolutionTimeAdaptiveDT
    #type = ConstantDT
    dt = 1E-16
    percent_change = 0.1
  [../]


  #Preconditioned JFNK (default)
  #solve_type = 'PJFNK'
  solve_type = 'JFNK'
  #solve_type = 'NEWTON'

  #petsc_options_iname = '-pc_type'
  #petsc_options_value = 'lu'

  l_max_its = 25
  #l_tol = 1.0e-5

  #nl_max_its = 40
  nl_rel_tol = 3.0e-7
  #nl_max_its = 20
  nl_abs_tol = 2.15E-6

  start_time = 0.0
  num_steps = 50
  dtmax = 1E0
  dtmin = 1E-20
[]

[Output]
  file_base = CHCoupledCalphadSplit
  interval = 1
  linear_residuals = true
  exodus = true
  perf_log = true
  all_var_norms = true

  postprocessor_csv = true
[]
