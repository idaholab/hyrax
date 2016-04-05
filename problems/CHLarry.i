[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 64
  ny = 64
  nz = 64
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 1
  elem_type = QUAD4
  elem_type = HEX8
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
  [../]
[]

[ICs]
  [./PSSCIC]
      type = PolySpecifiedSmoothCircleIC
      variable = concentration
      int_width = 0.046
      invalue = 0.9
      outvalue = 0.1
      multiple_radii = true
      radii = '0.1 0.08 0.115 0.07'
      n_seeds = 4
      x_positions = '0.25 0.25 0.75 0.75'
      y_positions = '0.25 0.75 0.25 0.75'
      z_positions = '0 0 0 0'
  [../]
[]

[Kernels]
  active = 'dcdt CHSolid CHInterface'

  [./dcdt]
    type = TimeDerivative
    variable = concentration
#    implicit = true
  [../]

  [./CHSolid]
    type = CHLarry
    variable = concentration
    mob_name = M
    W = 1
#    implicit = false
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
#    implicit = false
  [../]
[]

[Materials]
  [./constant]
    type = PFMobility
    block = 0
    mob = 1
    #mob_AC = 1
    kappa = 0.03535534
    #kappa_AC = 0.03535534
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'crank-nicolson'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'


  #l_max_its = 20
  #l_tol = 1.0e-5

  #nl_max_its = 40
  #nl_rel_tol = 1.0e-8

  start_time = 0.0
  num_steps = 1
  dt = 1.16e-8
[]

[Output]
  linear_residuals = true
  file_base = CHLarry3D
  interval = 1
  exodus = true
  perf_log = true
  all_var_norms = true
[]
