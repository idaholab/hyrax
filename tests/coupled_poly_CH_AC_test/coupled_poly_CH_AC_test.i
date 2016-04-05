# This input file is designed to test the coupled Allen-Cahn, Cahn-Hilliard equation system with multiple
# non-conserved order parameters and one conserved variable. This test is for regression testing.

#[Debug]
#  show_top_residuals=50
#[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 25
  nz = 0
  xmin = 0
  xmax = 50
  ymin = 0
  ymax = 50
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  #uniform_refine = 1
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = PolySpecifiedSmoothCircleIC
#      var_name = concentration
      invalue = 0.6
      outvalue = 0.1
      radius = 3.0
      int_width = 1.5
      x_positions = '15 20 25'
      y_positions = '15 20 25'
      z_positions = '0 0 0'
      n_seeds = 3
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = PolySpecifiedSmoothCircleIC
#      var_name = n1
      invalue = 1.6
      outvalue = 0.0
      radius = 3.0
      int_width = 1.5
      x_positions = '15'
      y_positions = '15'
      z_positions = '0'
      n_seeds = 1
    [../]
  [../]

  [./n2]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = PolySpecifiedSmoothCircleIC
#      var_name = n2
      invalue = 1.6
      outvalue = 0.0
      radius = 3.0
      int_width = 1.5
      x_positions = '20'
      y_positions = '20'
      z_positions = '0'
      n_seeds = 1
    [../]
  [../]

  [./n3]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = PolySpecifiedSmoothCircleIC
#      var_name = n3
      invalue = 1.6
      outvalue = 0.0
      radius = 3.0
      int_width = 1.5
      x_positions = '25'
      y_positions = '25'
      z_positions = '0'
      n_seeds = 1
    [../]
  [../]
[../]

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
#    coupled_var = n1
#    first_landau = A1
#   second_landau = A2
#    first_well = C1
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
  [../]

  [./ACSolid1]
    type = ACBulkPolyCoupled
    variable = n1
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 1
  [../]

  [./ACInterface1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACSolid2]
    type = ACBulkPolyCoupled
    variable = n2
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 2
  [../]

  [./ACInterface2]
    type = ACInterface
    variable = n2
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACSolid3]
    type = ACBulkPolyCoupled
    variable = n3
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 3
  [../]

  [./ACInterface3]
    type = ACInterface
    variable = n3
    mob_name = L
    kappa_name = kappa_n
  [../]
[]

[BCs]
active = 'Periodic'
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
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
    A5 = 1.0
    A6 = 1.0
    A7 = 1.0
    C1 = 0.006
    C2 = 0.59
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'crank-nicolson'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 20
  l_tol = 1.0e-5

  nl_max_its = 40
  nl_rel_tol = 5.0e-14

  start_time = 0.0
  num_steps = 2
  dt = 0.3
[]

[Outputs]
  file_base = coupled_poly_CH_AC_out
  exodus = true
[]
