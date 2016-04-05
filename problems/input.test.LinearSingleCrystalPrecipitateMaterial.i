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
  uniform_refine = 3
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
      [./InitialCondition]
      type = SmoothCircleIC
      int_width = 1.5
      invalue = 0.6
      outvalue = 0.1
      radius = 2.0
      x1 = 25.0
      y1 = 25.0
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      int_width = 1.5
      invalue = 1.6
      outvalue = 0.0
      radius = 2.0
      x1 = 25.0
      y1 = 25.0
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

[TensorMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
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
    mob_name = L
    coupled_CH_var = concentration
  [../]

  [./ACInterface]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACTransform]
    type = ACTransformElasticDF
    variable = n1
    OP_number = 1
    OP_var_names = 'n1'
    n_OP_vars = 1
  [../]
[]

[BCs]
  [./disp_x_BC]
    type = DirichletBC
    variable = disp_x
    boundary = '0 1 2 3'
    value = 0.0
  [../]

  [./disp_y_BC]
    type = DirichletBC
    variable = disp_y
    boundary = '0 1 2 3'
    value = 0.0
  [../]

  [./c_BC]
    type = NeumannBC
    variable = concentration
    boundary = '0 1 2 3'
    value = 0.0
  [../]

  [./n1_BC]
    type = NeumannBC
    variable = n1
    boundary = '0 1 2 3'
    value = 0.0
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
    type = LinearSingleCrystalPrecipitateMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    #reading C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66
    C_ijkl ='155.4 68.03 64.6 155.4  64.6 172.51 36.31 36.31 44.09'
    C_precipitate ='155.4 68.03 64.6 155.4  64.6 172.51 36.31 36.31 44.09'
    #reading        S_11   S_22  S_33 S_23 S_13 S_12
    e_precipitate = '0.00551  0.0564  0.0570  0.0  0.0  0.0'
    n_variants = 1
    variable_names = 'n1'
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
  num_steps = 200
  dt = 0.03
[]

[Output]
  linear_residuals = true
  file_base = TM_LSXPM
  interval = 1
  exodus = true
  perf_log = true
[]
