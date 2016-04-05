[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  nz = 0
  xmin = 0
  xmax = 76.8 #0.3*256
  ymin = 0
  ymax = 76.8
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 3 # 80 elements, dx=0.96...
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
      [./InitialCondition]
#      type = SmoothCircleIC
#      int_width = 0.9
#      invalue = 0.6
#      outvalue = 0.1
#      radius = 1.8
#      x1 = 38.4
#      y1 = 38.4
      type = ConstantIC
      value = 0.05
    [../]
  [../]

  [./n2]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
#      type = SmoothCircleIC
#      int_width = 0.9
#      invalue = 1.6
#      outvalue = 0.0
#      radius = 1.8
#      x1 = 38.4
#      y1 = 38.4
      type = RandomIC
      max = 0.1
      min = 0.001
   [../]
  [../]

  [./n3]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./dcdt]
    type = TimeDerivative
    variable = concentration
  [../]

  [./dn1dt]
    type = TimeDerivative
    variable = n1
  [../]

  [./dn2dt]
    type = TimeDerivative
    variable = n2
  [../]

  [./dn3dt]
    type = TimeDerivative
    variable = n3
  [../]

  [./CHSolid]
    type = CHBulkPolyCoupled
    variable = concentration
    mob_name = M
    n_OP_variables = 3
    OP_variable_names = 'n1 n2 n3'
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
  [../]

  [./ACSolidn1]
    type = ACBulkPolyCoupled
    variable = n1
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 1
  [../]

  [./ACSolidn2]
    type = ACBulkPolyCoupled
    variable = n2
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 2
  [../]

  [./ACSolidn3]
    type = ACBulkPolyCoupled
    variable = n3
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 3
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACInterfacen2]
    type = ACInterface
    variable = n2
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACInterfacen3]
    type = ACInterface
    variable = n3
    mob_name = L
    kappa_name = kappa_n
  [../]
[]

[BCs]
  [./c_BC]
    type = NeumannBC
    variable = concentration
    boundary = '0 1 2 3'
    value = 0.0
  [../]

  [./n1_BC]
    type = NeumannBC
    variable = n1
    boundary = '0 1 2 3'
    value = 0.0
  [../]

  [./n2_BC]
    type = NeumannBC
    variable = n2
    boundary = '0 1 2 3'
    value = 0.0
  [../]

  [./n3_BC]
    type = NeumannBC
    variable = n3
    boundary = '0 1 2 3'
    value = 0.0
  [../]
[]

[Materials]
  [./constant]
    type = PFMobilityLandau
    block = 0
    mob_CH = 0.4
    mob_AC = 0.4
    kappa_CH = 1.5
    kappa_AC = 1.5
    A1 = 18.5
    A2 = -8.5
    A3 = 11.5
    A4 = 4.5
    C1 = 0.006
    C2 = 0.59
  [../]
[]

[Postprocessors]
  [./ElementIntegralVariablePostprocessor_n1]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n1
  [../]

  [./ElementIntegralVariablePostprocessor_n2]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n2
  [../]

  [./ElementIntegralVariablePostprocessor_n3]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n3
  [../]

  [./ElementIntegralVariablePostprocessor_c]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = concentration
  [../]

  [./NodalMaxValue_n1]
    output = file
    type = NodalMaxValue
    variable = n1
  [../]

  [./NodalMaxValue_n2]
    output = file
    type = NodalMaxValue
    variable = n2
  [../]

  [./NodalMaxValue_n3]
    output = file
    type = NodalMaxValue
    variable = n3
  [../]

  [./NodalMaxValue_c]
    output = file
    type = NodalMaxValue
    variable = concentration
  [../]

  [./ElemMaxVal_n1]
    output = file
    type = MaxElementValue
    variable = n1
  [../]

  [./ElemMaxVal_n2]
    output = file
    type = MaxElementValue
    variable = n2
  [../]

  [./ElemMaxVal_n3]
    output = file
    type = MaxElementValue
    variable = n3
  [../]

  [./ElemMaxVal_c]
    output = file
    type = MaxElementValue
    variable = concentration
  [../]

  [./OneSeedn1]
    output = file
    type = OneSeed
    variables = 'n1'
    radius = 1.8
    int_width = 0.9
    dwell_time = 1.0
    seed_value = 1.6
    x_position = 38.4
    y_position = 38.4
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'
#  petsc_options = '-snes_mf_operator -ksp_monitor'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 30
  nl_max_its = 15
  #nl_abs_tol = 1.1e-5

  start_time = 0.0
  num_steps = 5000
  dt = 1.0e-3
  abort_on_solve_fail = true

#  [./Adaptivity]
#   coarsen_fraction = 0.05
#   refine_fraction = 0.1
#   max_h_level = 3
#  []
[]

[Output]
  file_base = LSXPM_n1_OPnuc_dt1em3_OneSeedPP
  interval = 100
  exodus = true
  perf_log = true
  postprocessor_csv = true
[]
