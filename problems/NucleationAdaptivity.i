[Mesh]
# specify if generated mesh, mesh generated from Cubit, etc.
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  nz = 0
  xmin = 0
  xmax = 153.6 #0.3*512
  ymin = 0
  ymax = 153.6
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 3 # 160 elements, dx=0.96...
[]

[Variables]
# define all the PDE variables here and any initial conditions they may have
  [./concentration]
    order = THIRD
    family = HERMITE
      [./InitialCondition]
      type = ConstantIC
      value = 0.1
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
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
# define auxiliary variables (non-PDE variables) here.  In the stress and strain cases, this is just so
# that the materials properties can be reported out into the output mesh file.  The nucleation aux variables
# are used for actual calculations within the simulation.

  [./nodal_Supersaturation]
    order = FIRST
    family = LAGRANGE
  [../]

 [./nodal_NucleationRate]
    order = FIRST
    family = LAGRANGE
  [../]

 [./nodal_NucleationProbability]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
# all the PDE maths go here.
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
    n_OP_variables = 3
    OP_variable_names = 'n1 n2 n3'
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
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 1
  [../]

  [./ACSolidn2]
    type = ACBulkPolyCoupled
    variable = n2
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 2
  [../]

  [./ACSolidn3]
    type = ACBulkPolyCoupled
    variable = n3
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 3
    OP_var_names = 'n1 n2 n3'
    OP_number = 3
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACInterfacen2]
    type = ACInterface
    variable = n2
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./ACInterfacen3]
    type = ACInterface
    variable = n3
    mob_name = L
    kappa_name = kappa_n
  [../]
  [./ACTransformn1]
    type = ACTransformElasticDF
    variable = n1
    OP_number = 1
    OP_var_names = 'n1 n2 n3'
    n_OP_vars = 3
  [../]

  [./ACTransformn2]
    type = ACTransformElasticDF
    variable = n2
    OP_number = 2
    OP_var_names = 'n1 n2 n3'
    n_OP_vars = 3
  [../]

  [./ACTransformn3]
    type = ACTransformElasticDF
    variable = n3
    OP_number = 3
    OP_var_names = 'n1 n2 n3'
    n_OP_vars = 3
  [../]
[]

[AuxKernels]
# RankTwoAux and RankFourAux are just auxiliary kernels to pull materials properties out and report them
# into the mesh file.  The nucleation aux kernels are actually used for calculations.
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
    Kn2 = 0.33
    Z = 0.1
    Beta_star = .001
    linear_density = 10
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
[]


[BCs]
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

  [./n2_BC]
    type = NeumannBC
    variable = n2
    boundary = '0 1 2 3'
    value = 0.0
  [../]

  [./n3_BC]
    type = NeumannBC
    variable = n3
    boundary = '0 1 2 3'
    value = 0.0
  [../]
[]

[Materials]
# materials properties to hold the coefficients for the Landau polynomial and the elastic stiffnesses/misfit
# strains information.
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
# postprocessors process the field variables and return a scalar value each timestep.  The Nucleation
# Postprocessor is used to introduce order parameter nuclei into the system.

  [./ElementIntegralVariablePostprocessor_n1]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n1
  [../]

  [./ElementIntegralVariablePostprocessor_n2]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n2
  [../]

  [./ElementIntegralVariablePostprocessor_n3]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = n3
  [../]

  [./ElementIntegralVariablePostprocessor_c]
    output = file
    type = ElementIntegralVariablePostprocessor
    variable = concentration
  [../]

  [./NodalMaxValue_n1]
    output = file
    type = NodalMaxValue
    variable = n1
  [../]

  [./NodalMaxValue_n2]
    output = file
    type = NodalMaxValue
    variable = n2
  [../]

  [./NodalMaxValue_n3]
    output = file
    type = NodalMaxValue
    variable = n3
  [../]

  [./NodalMaxValue_c]
    output = file
    type = NodalMaxValue
    variable = concentration
  [../]

  [./Nucleation]
    type = NucleationPostprocessor
    variables = 'n1 n2 n3'
    coupled_aux = nodal_NucleationProbability
    radius = 1.8
    seed_value = 1.6
    dwell_time = 0.1
    int_width = 0.9
  [../]
[]

[Executioner]
# how MOOSE should be executed is specified here.
  type = Transient
  scheme = 'bdf2'
#  petsc_options = '-snes_mf_operator -ksp_monitor'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 30
  nl_max_its = 15
  #nl_abs_tol = 1.1e-5

  start_time = 0.0
  num_steps = 1000
  dt = 1.0e-3
  abort_on_solve_fail = true

  [./Adaptivity]
   coarsen_fraction = 0.05
   refine_fraction = 0.30
   max_h_level = 3
   cycles_per_step = 3
  []
[]

[Output]
# what output should come out of the application is specified here.
  file_base = NucleationAdaptivity
  interval = 10
  exodus = true
  perf_log = true
  postprocessor_csv = true
[]
