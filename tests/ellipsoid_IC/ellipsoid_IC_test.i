[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
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
  [./diffused]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = EllipsoidIC
      int_width = 2
      invalue = 0.6
      outvalue = 0.1
      coefficients = '5 10 15'
      x_positions = '15 30'
      y_positions = '15 30'
      z_positions = '0 0'
      n_seeds = 2
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
  file_base = ellipsoid_IC
  exodus = true
[]
