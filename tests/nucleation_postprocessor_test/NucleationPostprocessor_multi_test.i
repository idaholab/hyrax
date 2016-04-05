# This input file is designed to test the multi coupled Allen-Cahn, Cahn-Hilliard
# equation system with nucleus introduction by way of the Nucleation
# postprocessor.

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

  [./n2]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n3]
    order = FIRST
    family = LAGRANGE
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

  [./n2_untouched]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n3_untouched]
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

  [./dn2dt]
    type = TimeDerivative
    variable = n2
  [../]

  [./dn3dt]
    type = TimeDerivative
    variable = n3
  [../]

  [./CHSolid]
    type = CHBulkPolyCoupled
    variable = concentration
    mob_name = M
    OP_variable_names = 'n1 n2 n3'
    n_OP_variables = 3
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
  [../]

  [./ACSolidn1]
    type = ACBulkPolyCoupled
    variable = n1
    coupled_CH_var = concentration
    mob_name = L
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 1
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACSolidn2]
    type = ACBulkPolyCoupled
    variable = n2
    coupled_CH_var = concentration
    mob_name = L
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 2
  [../]
n
  [./ACInterfacen2]
    type = ACInterface
    variable = n2
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACSolidn3]
    type = ACBulkPolyCoupled
    variable = n3
    coupled_CH_var = concentration
    mob_name = L
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 3
  [../]

  [./ACInterfacen3]
    type = ACInterface
    variable = n3
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
    #Kn1 = 0.8
    #Kn2 = 0.001
    gamma = 0.18
    scale_factor = 900e-22

    Z = 0.1
    Beta_star = 100e6
    linear_density = 0.113137085
    OP_var_names = 'n1 n2 n3'
    n_OP_vars = 3
    execute_on = timestep_end
  [../]

  [./NucleationProbability]
    type = AuxNucleationProbability
    variable = nodal_NucleationProbability
    coupled_aux_var = nodal_NucleationRate
    coupled_variables = 'n1 n2 n3'
    n_OP_vars = 3
    execute_on = timestep_end
  [../]

  [./Report_n1]
    type = ReporterAux
    variable = n1_untouched
    coupled = n1
    execute_on  = timestep_end
   [../]

  [./Report_n2]
    type = ReporterAux
    variable = n2_untouched
    coupled = n2
    execute_on  = timestep_end
   [../]

  [./Report_n3]
    type = ReporterAux
    variable = n3_untouched
    coupled = n3
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
    variables = 'n1 n2 n3'
    coupled_aux = nodal_NucleationProbability
    radius = 2.0
    seed_value = 1.6
    dwell_time = 0.35
    int_width = 2.0
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
  num_steps = 2

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

[Outputs]
  file_base = NucleationPostprocessor_multi_out
  exodus = true
[]
