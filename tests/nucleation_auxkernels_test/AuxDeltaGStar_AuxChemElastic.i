# This input file demonstrates the coupled multiple Allen-Cahn, Cahn-Hilliard
# equations and explicit nucleation.  It tests calculation of critical activation
# energy.

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
#  uniform_refine = 4
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = RandomIC
      variable = concentration
      min = 0.0
      max = 0.01
    [../]
  [../]

 [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = RandomIC
      max = 1.6
      min = 0.0
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

[AuxVariables]
  [./elem_ChemElastic]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elem_DeltaGStar]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
  [../]

  [./CH]
    type = CHBulkPolyCoupled
    variable = concentration
    mob_name = M
    n_OP_variables = 1
    OP_variable_names = 'n1'
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
  [../]

  [./ACSolid1]
    type = ACBulkPolyCoupled
    variable = n1
    mob_name = L
    coupled_CH_var = concentration
    n_OP_vars = 1
    OP_var_names = 'n1'
    OP_number = 1
  [../]

  [./ACInterfacen1]
    type = ACInterface
    variable = n1
    mob_name = L
    kappa_name = kappa_n
  [../]

  [./dcdt]
    type = TimeDerivative
    variable = concentration
  [../]

  [./dn1dt]
    type = TimeDerivative
    variable = n1
  [../]

  [./ACTransformn1]
    type = ACTransformElasticDF
    variable = n1
    OP_number = 1
    OP_var_names = 'n1'
    n_OP_vars = 1
  [../]
[]

[AuxKernels]
  [./ChemElastic_n1]
    type = AuxGuoEnergy #AuxChemElastic
    variable = elem_ChemElastic
    coupled_conserved_var = concentration
    coupled_nonconserved_var = n1
    nonconserved_var_number = 1
    precip_conserved = 0.6
    precip_nonconserved = 1.6
  #  functional_c1 = 0.006
    execute_on = timestep_end
  [../]

  [./DeltaGStar]
    type = AuxDeltaGStar
    variable = elem_DeltaGStar
    coupled_aux_var = elem_ChemElastic

    gamma = 0.18
    scale_factor = 1
    Kb = 1

    execute_on = timestep_end
  [../]
[]

[BCs]
  [./Dirichlet_dispx]
    type = DirichletBC
    variable = disp_x
    value = 0.0
    boundary = '0 1 2 3'
  [../]

 [./Dirichlet_dispy]
    type = DirichletBC
    variable = disp_y
    value = 0.0
    boundary = '0 1 2 3'
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
    fill_method = symmetric9
  [../]
[]

[Executioner]
  type = MeshSolutionModify
#type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'




  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

#  l_max_its = 15
#  nl_max_its = 10

  start_time = 0.0
  num_steps = 3
  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 0.01
  [../]
  dtmin = 1e-4
  dtmax = 1e-1

  abort_on_solve_fail = true
[]

[Outputs]
  file_base = AuxDeltaGStar_AuxChemElastic
  exodus = true
[]
