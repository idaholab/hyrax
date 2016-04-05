[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  nz = 0
  xmin = -25
  xmax = 25
  ymin = -25
  ymax = 25
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./diffused]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = DiamondIC
      invalue = 0.6
      outvalue = 0.1
      radius = 10
      int_width = 2
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
  file_base = diamond_IC
  exodus = true
[]
