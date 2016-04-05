[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 15
  ny = 15
  nz = 0
  xmin = 0
  xmax = 38.4 #0.3*256
  ymin = 0
  ymax = 38.4
  zmin = 0
  zmax = 0
  elem_type = QUAD4
#  uniform_refine = 4
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = ConstantIC
      value = 0.0562
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
#    [./InitialCondition]
#      type = RandomIC
#      max = 0.001
#      min = 0.0001
#   [../]
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

  [./CHSolid]
    type = CHBulkCoupled
    variable = concentration
    mob_name = M
    coupled_OP_var = n1
    #n_OP_variables = 3
    #OP_variable_names = 'n1 n2 n3'
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
  [../]

  [./ACSolidn1]
    type = ACBulkCoupled
    variable = n1
    mob_name = L
    coupled_CH_var = concentration
 #   n_OP_vars = 3
 #   OP_var_names = 'n1 n2 n3'
 #   OP_number = 1
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]
[]

#[BCs]
#  [./c_BC]
#    type = NeumannBC
#    variable = concentration
#    boundary = '0 1 2 3'
#    value = 0.0
#  [../]

#  [./n1_BC]
#    type = NeumannBC
#    variable = n1
#    boundary = '0 1 2 3'
#    value = 0.0
#  [../]
#[]

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
  [./NodalVolumeFraction_n1]
    output = file
    type = NodalVolumeFraction
    variable = n1
    threshold = 0.75
    execute_on = timestep_end
    mesh_volume = Volume
  [../]

  [./Volume]
    type = VolumePostprocessor
    execute_on = initial
  [../]

  [./ElementIntegral_c]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = concentration
  [../]

  [./NodalMaxValue_n1]
    output = file
    type = NodalMaxValue
    variable = n1
  [../]

  [./NodalMaxValue_c]
    output = file
    type = NodalMaxValue
    variable = concentration
  [../]

  [./OneSeedn1]
    output = file
    type = OneSeed
    variables = 'n1'
    radius = 1.8
    int_width = 0.9
    dwell_time = 0.00101
    seed_value = 1.6
    x_position = 19.2
    y_position = 19.2
  [../]

  [./centervalue_conc]
    type = PointValue
    variable = concentration
    point = '19.2 19.2 0'
  [../]

  [./centervalue_n1]
    type = PointValue
    variable = n1
    point = '19.2 19.2 0'
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

  start_time = 0.0
  end_time = 2.0
  dt = 0.01
  abort_on_solve_fail = true

  [./Adaptivity]
   coarsen_fraction = 0.05
   refine_fraction = 0.75
   max_h_level = 5
   initial_adaptivity = 5
  [../]
[]

[Output]
  file_base = n1_OPnuc
  interval = 10
  exodus = true
  perf_log = true
  postprocessor_csv = true
[]
