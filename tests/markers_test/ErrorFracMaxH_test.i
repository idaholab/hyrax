# this input file is to test the error fraction marker with maximum h-level set

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 5
  ny = 5
  nz = 0
  xmin = 0
  xmax = 50
  ymin = 0
  ymax = 50
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./concentration]
    order = FIRST
    family = LAGRANGE
      [./InitialCondition]
      type = RandomIC
    [../]
  [../]
[]

[Kernels]
  [./dcdt]
    type = TimeDerivative
    variable = concentration
  [../]

  [./diff]
    type = Diffusion
    variable = concentration
  [../]
[]

[BCs]
  [./c_BC]
    type = NeumannBC
    variable = concentration
    boundary = '0 1 2 3'
    value = 0.0
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'crank-nicolson'
  num_steps = 4
  dt = 0.01
#  adapt_cycles = 2


  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'

[]

[Adaptivity]
   marker = EFMHM
 [./Markers]
    [./EFMHM]
      type = ErrorFractionMaxHMarker
      coarsen = 0.01
      refine = 0.8
      max_h_level = 2
      indicator = GJI
    [../]
 [../]

 [./Indicators]
   [./GJI]
    type = GradientJumpIndicator
    variable = concentration
   [../]
 [../]
[]

[Outputs]
  file_base = ErrorFracMaxH_test
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[../]
