# This test is designed as part of a suite with input.testCH_AC_coupled, input.testACNucleation_init, and input.testACNucleation.

# This input file is designed to test the coupled Allen-Cahn, Cahn-Hilliard equation system, with no initial seed and then an OP seed at a later time.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  nz = 0
  xmin = 0
  xmax = 38.4
  ymin = 0
  ymax = 38.4
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 2
[]

[Variables]
  active = 'concentration n1'

  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = RandomIC
      min = 0.099
      max = 0.101
      #value = 0.1
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      value = 0.0
    [../]
  [../]
[]

[Kernels]
#  active = 'dcdt dn1dt CHSolid CHInterface ACSolid ACInterface'

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
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
  [../]

  [./ACSolid]
    type = ACBulkCoupled
    variable = n1
    coupled_CH_var = concentration
    mob_name = L
  [../]

  [./ACInterface]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACNucleus]
    type = ACNucleus
    variable = n1
    n_value = 1.6
    radius = 1.8
    x_center = 19.2
    y_center = 19.2
    time_periods = 'p2'
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

  start_time = 0.0
  num_steps = 25
  dt = 0.3

  time_periods = 'p1 p2 p3'
  time_period_starts = '0 0.6 2.6'
   #time_period_ends =
[]

[Output]
  file_base = testACNucleation5
  interval = 1
  exodus = true
  perf_log = true

  [./OverSampling]
    exodus = true
    refinements = 4
    interval = 1
  [../]
[]
