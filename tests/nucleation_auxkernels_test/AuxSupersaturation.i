# This input file demonstrates the AuxSupersaturation aux kernel.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
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
      variable = concentration
    [../]
  [../]
[]

[AuxVariables]
  [./nodal_Supersaturation]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./Diffusion]
    type = Diffusion
    variable = concentration
  [../]
[]

[AuxKernels]
  [./Supersaturation]
    type = AuxSupersaturation
    variable = nodal_Supersaturation
    coupled_var = concentration
    c1 = 0.1
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./c_BC]
    type = DirichletBC
    variable = concentration
    boundary = '0 1'
    value = 1.0
  [../]

  [./c_BC2]
    type = DirichletBC
    variable = concentration
    boundary = '2 3'
    value = 0.0
  [../]
[]


[Executioner]
  type = Steady
  #scheme = 'crank-nicolson'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 15
  nl_max_its = 10
[]

[Outputs]
  file_base = AuxSupersaturation
  exodus = true
[]
