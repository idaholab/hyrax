# This input file is designed to test the coupled Allen-Cahn, Cahn-Hilliard
# equation system with nucleus introduction by way of the
# ChangeVariableData postprocessor.

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
 # uniform_refine = 2
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
 #     var_name = concentration
      int_width = 3.0
      invalue = 0.6
      outvalue = 0.1
      radius = 5.0
      x1 = 25.0
      y1 = 25.0
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
  #    var_name = n1
      int_width = 3.0
      invalue = 1.6
      outvalue = 0.0
      radius = 5
      x1 = 25.0
      y1 = 25.0
    [../]
  [../]
[]

[AuxVariables]
  [./nodal_testSwitch]
    order = FIRST
    family = LAGRANGE
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

[AuxKernels]
  [./TestSwitch]
    type = AuxTestFlip
    variable = nodal_testSwitch
    coupled_c = concentration
    radius = 2.0
    center_point = '25.0 25.0 0.0'
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

[Postprocessors]
  [./FlipValues]
    type = ValuePlusOne
    variables = n1
    coupled_aux = nodal_testSwitch
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'crank-nicolson'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 20
  l_tol = 1.0e-5

  nl_max_its = 40
  nl_rel_tol = 5.0e-14

  start_time = 0.0
  num_steps = 1
  dt = 0.03
[]

[Outputs]
  file_base = Postprocessor_changeValue_out
  exodus = true
[]
