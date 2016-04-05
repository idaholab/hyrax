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
  uniform_refine = 5
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
    [./InitialCondition]
     type = PolySpecifiedSmoothCircleIC
     outvalue = 0
     invalue = 1.6
     radius = 1.8
     int_width = 0.9
     x_positions = '56 54 13  124 101 84 51  69 25 146 14 114 59 131 87 139  90  45 80 3'
     y_positions = '102 9 127 100 152 45 129 27 56 80  55 69  28 97  98 108 109 103 69 133'
     z_positions = '0   0 0   0   0   0  0   0  0  0   0  0   0  0   0  0   0   0   0  0'
     n_seeds = 20
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
[]

#[BCs]
#  [./Neumann_C]
#    type = NeumannBC
#    variable = concentration
#    boundary = '0 1 2 3'
#  [../]

#  [./Neumann_n1]
#    type = NeumannBC
#    variable = n1
#    boundary = '0 1 2 3'
#  [../]
[]

[Materials]
  [./constant]
    type = PFMobilityLandau
    block = 0
    mob_CH = 0.4
    mob_AC = 0.4
    kappa_CH = 0.8
    kappa_AC = 0.8
    A1 = 18.5
    A2 = -8.5
    A3 = 11.5
    A4 = 4.5
    C1 = 0.006
    C2 = 0.59
  [../]
[]

[Postprocessors]
  [./ElementIntegral_n1]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n1
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

  [./ndofs]
    type = NumDOFs
    output = file
  [../]
[]

[Executioner]
  type = Transient
#  type = SolutionTimeAdaptive
  scheme = 'crank-nicolson'


  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'

  start_time = 0.0
#  num_steps = 50
  end_time = 100
  dt = 0.01

  abort_on_solve_fail = true
[]

[Adaptivity]
   marker = EFMHM
 [./Markers]
#    [./NM]
#      type = NucleationMarker
#      nucleation_userobject = NLUO
#      max_h_level = 5
#    [../]
    [./EFMHM]
      type = ErrorFractionMaxHMarker
      coarsen = 0.05
      refine = 0.75
      max_h_level = 5
      indicator = GJI
    [../]
#    [./combo]
#      type = ComboMarker
#      markers = 'NM EFMHM'
#    [../]
 [../]

 [./Indicators]
   [./GJI]
    type = GradientJumpIndicator
    variable = n1
   [../]
 [../]
[]

[Output]
  file_base = testSiteSaturation_h-adapt
  postprocessor_csv = true
  interval = 100
  exodus = true
  perf_log = true

 [./OverSampling]
   exodus = true
   refinements = 5
#   interval = 5
 [../]
[]
