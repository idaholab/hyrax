# This input file is for creating/testing forced mesh refinement for the element in which a Dirac point
# source is introduced.

# The purpose of this seemingly hare-brained exercise is to model nucleation in a phase field model.  In
# this case, there is a coupled Allen-Cahn (variable=n1), Cahn-Hilliard (variable=c) equation system.  A
# Dirac point source for the order parameter variable is introduced and the mesh must be refined there such
# that there are enough points across the interfaces for a phase field model to be stable.  The
# concentration will naturally increase at that point to create a proper nucleus.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  nz = 0
  xmin = 0
  xmax = 10
  ymin = 0
  ymax = 10
  zmin = 0
  zmax = 0
  elem_type = QUAD4

  uniform_refine = 2
[]
[Variables]
  active = 'concentration n1'

  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      value = 0.1
      type = ConstantIC
      var_name = concentration
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      value = 0.0
      type = ConstantIC
      var_name = n1
    [../]
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
[]

[DiracKernels]
  active = 'dirac'
  [./dirac]
    type = DiracForcedAMR
    variable = n1
    value = 500.0
    point = '6.2 6.2 0'
    active_after = 2
    active_for = 100
  [../]
[]

[BCs]
active = 'Periodic'
  [./Periodic]
    [./left_right]
      primary = 0
      secondary = 2
      translation = '0 10 0'
    [../]

    [./top_bottom]
      primary = 1
      secondary = 3
      translation = '-10 0 0'
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

[Executioner]
#  type = TransientMultiAMR
  type = SolutionTimeAdaptive
  scheme = 'crank-nicolson'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'




  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 15
  nl_max_its = 10

  start_time = 0.0
  num_steps = 100
  dt = 0.003

#  num_refines = 2

# The adaptivity section should probably be played with at will.
  [./Adaptivity]
    error_estimator = LaplacianErrorEstimator
    refine_fraction = 0.99
#    coarsen_fraction = 0.1
    max_h_level = 4
#  [../]
[]

[Postprocessors]
  [./dt]
    type = TimestepSize
  [../]

  [./elapsed_time]
    type = PrintElapsedTime
  [../]

  [./num_elem]
    type = NumElems
  [../]
[]


[Output]
  linear_residuals = true
  file_base = out
  interval = 1
  exodus = true
  perf_log = true
[]

