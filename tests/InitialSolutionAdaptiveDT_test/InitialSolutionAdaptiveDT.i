# This input file is designed to test the coupled Allen-Cahn, Cahn-Hilliard equation system.  This test is
# for regression testing.

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
  #uniform_refine = 2
[]

[Variables]
  [./n1]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./n1_init]
    type = SmoothCircleIC
    variable = n1
    int_width = 1.5
    invalue = 1.6
    outvalue = 0.0
    radius = 5
    x1 = 25.0
    y1 = 25.0
  [../]
[]

[Kernels]
  [./dn1dt]
    type = TimeDerivative
    variable = n1
  [../]

  [./diff]
    type = Diffusion
    variable = n1
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'crank-nicolson'

  [./TimeStepper]
    type = InitialSolutionAdaptiveDT
    dt = 1E0
    initial_dt = 1E-1
    has_initial_dt = true
    n_initial_steps = 3
  [../]

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type'
  petsc_options_value = 'ilu'

  start_time = 0.0
  num_steps = 5
[]

[Outputs]
  file_base = InitialSolutionAdaptiveDT
  exodus = true
  checkpoint = true
[]
