# This input file is designed to test the coupled Allen-Cahn, Cahn-Hilliard
# equation system with nucleus introduction by way of the Nucleation
# postprocessor.

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
  uniform_refine = 1
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
      int_width = 3.0
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
      int_width = 3.0
      invalue = 1.6
      outvalue = 0.0
      radius = 2.0
      x1 = 25.0
      y1 = 25.0
    [../]
  [../]
[]

[AuxVariables]
  [./nodal_Supersaturation]
    order = FIRST
    family = LAGRANGE
  [../]

[./nodal_NucleationProbability]
    order = FIRST
    family = LAGRANGE
  [../]

[./nodal_NucleationRate]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n1_untouched]
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
  [./Supersaturation]
    type = AuxSupersaturation
    variable = nodal_Supersaturation
    coupled_var = concentration
    functional_c1 = 0.006
    execute_on = timestep_end
  [../]

  [./NucleationRate]
    type = AuxNucleationRate
    variable = nodal_NucleationRate
    coupled_aux_var = nodal_Supersaturation
    Kn1 = 0.08
    Kn2 = 0.25
    execute_on = timestep_end
  [../]

  [./NucleationProbability]
    type = AuxNucleationProbability
    variable = nodal_NucleationProbability
    coupled_aux_var = nodal_NucleationRate
    coupled_variable = n1
    execute_on = timestep_end
  [../]

  [./Report_n1]
    type = ReporterAux
    variable = n1_untouched
    coupled = n1
    execute_on  = timestep_end
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
[./Nucleation]
    type = NucleationPostprocessor
    variables = 'n1'
    coupled_aux = nodal_NucleationProbability
    radius = 2.0
    seed_value = 1.6
    dwell_time = 0.35
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
  nl_rel_tol = 5.0e-12

  start_time = 0.0
  num_steps = 10

  dt = 0.03

#  [./Adaptivity]
#    refine_fraction = 0.9
#    coarsen_fraction = 0.1
#    max_h_level = 3
#    error_estimator = LaplacianErrorEstimator
#    initial_adaptivity = 2
#    cycles_per_step = 2
#  [../]
[]

[Output]
  file_base = NucleationPostprocessorMulti
  interval = 1
  exodus = true
  perf_log = true

  #[./OverSampling]
  #  refinements = 2
  #[../]
[]
