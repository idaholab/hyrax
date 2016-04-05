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
[]

[Variables]
  [./diff_messed_with]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./diff_untouched]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = diff_messed_with
  [../]
[]

[AuxKernels]
  [./TestSwitch]
    type = ReporterAux
    variable = diff_untouched
    coupled = diff_messed_with
    execute_on = timestep_begin
  [../]
[]

[BCs]
  active = 'left right'
  [./left]
    type = DirichletBC
    variable = diff_messed_with
    boundary = left
    value = 0
  [../]

  [./right]
    type = DirichletBC
    variable = diff_messed_with
    boundary = right
    value = 1
  [../]
[]

[Postprocessors]
  [./FlipValues]
    type = ChangeVariableData
    variables = diff_messed_with
    coupled_aux = diff_untouched
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'crank-nicolson'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  nl_abs_tol = 1e-10

  start_time = 0.0
  num_steps = 4
  dt = 1.0
[]

[Outputs]
  exodus = true
  console = true
[]
