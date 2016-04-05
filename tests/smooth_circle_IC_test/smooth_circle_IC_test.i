# This input file is designed to test the coupled Allen-Cahn, Cahn-Hilliard equation system.  This test is
# for regression testing.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 25
  nz = 0
  xmin = 0
  xmax = 100
  ymin = 0
  ymax = 100
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 1
[]

[Variables]
  [./diffused]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = PolySpecifiedSmoothCircleIC
      int_width = 1.5
      invalue = 0.6
      outvalue = 0.1
      radius = 5.0
      x_positions = '25 50 75'
      y_positions = '25 50 75'
      z_positions = '0 0 0'
      n_seeds = 3
    [../]
  [../]
[]

[Kernels]

  [./diff]
    type = Diffusion
    variable = diffused
  [../]
[]

[BCs]
active = 'bottom top'
  [./bottom]
    type = DirichletBC
    variable = diffused
    boundary = '1'
    value = 1
  [../]

  [./top]
    type = DirichletBC
    variable = diffused
    boundary = '2'
    value = 0
  [../]
[]


[Executioner]
  type = Steady

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


[]

[Outputs]
  file_base = smooth_circle_IC_out
  exodus = true
[]
