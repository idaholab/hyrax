[Mesh]
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
  [./concentration]
    order = THIRD
    family = HERMITE
      [./InitialCondition]
      type = SmoothCircleIC
      int_width = 0.9
      invalue = 0.6
      outvalue = 0.1
      radius = 1.8
      x1 = 76.8
      y1 = 76.8
    [../]
  [../]

  [./n2]
    order = FIRST
    family = LAGRANGE
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      int_width = 0.9
      invalue = 1.6
      outvalue = 0.0
      radius = 1.8
      x1 = 76.8
      y1 = 76.8
    [../]
  [../]

  [./n3]
    order = FIRST
    family = LAGRANGE
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
#stresses
  [./s11_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./s12_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

#  [./s13_aux]
#    order = FIRST
#    family = LAGRANGE
#  [../]

  [./s22_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

#  [./s23_aux]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]

#  [./s33_aux]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]

#strains
  [./e11_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./e12_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

#  [./e13_aux]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]

  [./e22_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

#  [./e23_aux]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]

#  [./e33_aux]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
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
  [./matl_s11]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    variable = s11_aux
  [../]

 [./matl_s12]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 2
    variable = s12_aux
  [../]

#  [./matl_s13]
#    type = RankTwoAux
#    rank_two_tensor = stress
#    index_i = 1
#    index_j = 3
#    variable = s13_aux
#  [../]

  [./matl_s22]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 2
    index_j = 2
    variable = s22_aux
  [../]

#  [./matl_s23]
#    type = RankTwoAux
#    rank_two_tensor = stress
#    index_i = 2
#    index_j = 3
#    variable = s23_aux
#  [../]

#  [./matl_s33]
#    type = RankTwoAux
#    rank_two_tensor = stress
#    index_i = 3
#    index_j = 3
#    variable = s33_aux
#  [../]

  [./matl_e11]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 1
    variable = e11_aux
  [../]

 [./matl_e12]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 2
    variable = e12_aux
  [../]

#  [./matl_e13]
#    type = RankTwoAux
#    rank_two_tensor = elastic_strain
#    index_i = 1
#    index_j = 3
#    variable = e13_aux
#  [../]

  [./matl_e22]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    index_i = 2
    index_j = 2
    variable = e22_aux
  [../]

#  [./matl_e23]
#    type = RankTwoAux
#    rank_two_tensor = elastic_strain
#    index_i = 2
#    index_j = 3
#    variable = e23_aux
#  [../]

#  [./matl_e33]
#    type = RankTwoAux
#    rank_two_tensor = elastic_strain
#    index_i = 3
#    index_j = 3
#    variable = e33_aux
#  [../]
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
    n_variants = 3
    variable_names = 'n1 n2 n3'
    all_21 = false
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'
#  petsc_options = '-snes_mf_operator -ksp_monitor'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 30
  nl_max_its = 100
  #nl_abs_tol = 1.1e-5

  start_time = 0.0
  num_steps = 1000
  dt = 0.01
  abort_on_solve_fail = true

  [./Adaptivity]
   coarsen_fraction = 0.1
   refine_fraction = 0.1
   max_h_level = 4
  [../]
[]

[Output]
  file_base = LSXPM_n1_dt01_Dirichlet
  interval = 100
  exodus = true
  perf_log = true
[]

