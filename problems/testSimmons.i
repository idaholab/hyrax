# This file is to test/duplicate the results from Simmons 2001 concurrent nucleation and growth paper
# to duplicate the site saturation results.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  nz = 0
  xmin = 0
  xmax = 153.6 #512*0.3
  ymin = 0
  ymax = 153.6 #512.0.3
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  #uniform_refine = 3
[]

[Variables]
 [./concentration]
    order = THIRD
    family = HERMITE
   [./InitialCondition]
     # type = RandomIC
     # min = 0
     # max = 1
     type = SmoothCircleIC
     outvalue = 0.38
     invalue = 0.93
     radius = 3
     x1 = 78
     y1 = 78
   [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
     # type = RandomIC
     # min = 0
     # max = 2
     type = SmoothCircleIC
     outvalue = 0
     invalue = 1.75
     radius = 3
     x1 = 78
     y1 = 78
    [../]
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
    type = CHBulkSimmons
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
    type = ACBulkSimmons
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
[]

[BCs]
  [./Neumann_C]
    type = NeumannBC
    variable = concentration
    boundary = '0 1 2 3'
  [../]

  [./Neumann_n1]
    type = NeumannBC
    variable = n1
    boundary = '0 1 2 3'
  []
[]

[Materials]
  [./constant]
    type = PFMobilityLandau
    block = 0
    mob_CH = 0.4
    mob_AC = 0.4
    kappa_CH = 0.8
    kappa_AC = 0.8
    A1 = 7.5
    A2 = 4.0
    A3 = 1.0
    A4 = 0.5
    C1 = 0.5
    C2 = 0.1
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
  num_steps = 1000
  dt = 0.01
[]

[Output]
  file_base = testSimmons
  interval = 1
  exodus = true
  perf_log = true

#  [./OverSampling]
#    exodus = true
#    refinements = 4
#
#    interval = 1
#  [../]
[]
