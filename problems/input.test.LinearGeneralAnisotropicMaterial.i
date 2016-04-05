[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  nz = 0
  xmin = 0
  xmax = 2
  ymin = 0
  ymax = 2
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  active = 'concentration n1 disp_x disp_y'

  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = ConstantIC
      var_name = concentration
      value = 0.1
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      var_name = n1
      value = 0.2
    [../]
  [../]

  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  active = 'dcdt dn1dt CHSolid CHInterface ACSolid ACInterface'

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
    coupled_var = n1
    first_landau = A1
    second_landau = A2
    first_well = C1
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
    mob_name = L
    coupled_var = concentration
    second_landau = A2
    third_landau = A3
    fourth_landau = A4
    second_well = C2
  [../]

  [./ACInterface]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./stress_div_disp_x]
    type = StressDivergence
    variable = disp_x
    component = 0
    #disp_x = disp_x
    #disp_y = disp_y
  [../]

  [./stress_div_disp_y]
    type = StressDivergence
    variable = disp_y
    component = 1
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[BCs]
active = 'Periodic'
  [./Periodic]
    [./left_right]
      primary = 0
      secondary = 2
      translation = '0 2 0'
    [../]

    [./top_bottom]
      primary = 1
      secondary = 3
      translation = '-2 0 0'
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

  [./test_material]
    type = LinearGeneralAnisotropicMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    C_matrix = '100 200 300 400 500 600 700 800 900'
    all_21 = false
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'crank-nicolson'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'




  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 15
  nl_max_its = 10

  start_time = 0.0
  num_steps = 1
  dt = 0.03
[]

[Output]
  linear_residuals = true
  file_base = out
  interval = 1
  exodus = true
  perf_log = true
[]
