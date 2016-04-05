# this input file is to test the Actions created for concurrent nucleation and growth with 3 orientation variants

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 40
  nz = 0
  xmin = 0
  xmax = 6
  ymin = 0
  ymax = 6
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[MeshModifiers]
  [./AddExtraNodeset]
    boundary = 100
    coord = '3.0 3.0 3.3 3.0'
    type = AddExtraNodeset
  [../]
[]

[Variables]
  [./concentration]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
      radius = 1
      int_width = 0.9
      x1 = 3
      y1 = 3
      invalue = 0.6
      outvalue = 0.05
    [../]
  [../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      radius = 1
      int_width = 0.9
      x1 = 3
      y1 = 3
      invalue = 1.6
      outvalue = 0.0
    [../]
  [../]

  [./n2]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = RandomIC
      min = 0.001
      max = 0.01
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
  [./AuxChemElastic_n1]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./AuxChemElastic_n2]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./NucleationRate_n1]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./NucleationRate_n2]
    order = CONSTANT
    family = MONOMIAL
  [../]


  [./NucleationProbability_n1]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./NucleationProbability_n2]
    order = CONSTANT
    family = MONOMIAL
  [../]


  [./NucleationGStar_n1]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./NucleationGStar_n2]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./dcdt]
    type = TimeDerivative
    variable = concentration
  [../]

  [./CHSolid]
   type = CHBulkPolyCoupled
    variable = concentration
    mob_name = M
    n_OP_variables = 2
    OP_variable_names = 'n1 n2'
  [../]

  [./CHInterface]
    type = CHInterface
    variable = concentration
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
  [../]
[]

[TensorMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[OPVariantKernel]
    number_OPs = 2
    OP_name_base = n

    #for ACBulkPolyCoupled
    coupled_CH_var = concentration

    #for ACInterface
    kappa_name_OP = kappa_n

    #for TimeDerivative
[]

[NucleationAux]
  number_OPs = 2
  OP_name_base = n
  bulk_energy_name_base = AuxChemElastic_
  nucleation_rate_name_base = NucleationRate_
  nucleation_probability_name_base = NucleationProbability_
  deltaGstar_name_base = NucleationGStar_

  #for AuxChemElastic
  coupled_conserved_var = concentration
  precip_conserved = 0.6
  precip_nonconserved = 1.6

  #for NucleationProbability
  gamma = 3  # also for AuxDeltaGStar
  Kb = 1
  scale_factor = 1;
  Z = 1e-4
  Beta_star = 1e-4
  linear_density = 50
[]

[BCs]
  [./c_BC]
    type = NeumannBC
    variable = concentration
    boundary = '1'
    value = 1
  [../]

#  [./n1_BC]
#    type = NeumannBC
#    variable = n1
#    boundary = '0 1 2 3'
#    value = 0.0
#  [../]

  [./fixed_x_BC]
    type = DirichletBC
    variable = disp_x
    boundary = '100'
    value = 0.0
  [../]
  [./fixed_y_BC]
    type = DirichletBC
    variable = disp_y
    boundary = '100'
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
    n_variants = 2
    variable_names = 'n1 n2'
    all_21 = false
  [../]
[]

[Postprocessors]
 [./Volume]
    type = VolumePostprocessor
    execute_on = initial
    output = file
  [../]

  [./VolumeFraction_n1]
    type = NodalVolumeFraction
    output = both
    bubble_volume_file = stress0_a-1.csv
    #Avrami_file = Avrami_0-1_a.csv
    threshold = 0.75
    variable = n1
    mesh_volume = Volume
    equil_fraction = 0.67369140625
  [../]

  [./VolumeFraction_n2]
    type = NodalVolumeFraction
    output = both
    bubble_volume_file = stress0_a-2.csv
#    Avrami_file = Avrami_0-2_a.csv
    threshold = 0.75
    variable = n2
    mesh_volume = Volume
    equil_fraction = 0.67369140625
  [../]
[]

#[NucleationPostprocessor]
#  num_OPs = 2
#  OP_name_base = n
#  mesh_volume = Volume

#  particle_volume_name_base = CNG_distribution_
#  Avrami_name_base = CNG_avrami_
#  equil_fraction = 0.67369140625
#  threshold = 0.75
#[]

[Executioner]
  type = MeshSolutionModify
  scheme = 'bdf2'

  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 0.01
    percent_change = 0.1
  [../]

 # num_steps = 10
 # dt = 0.01
  dtmin = 0.0001
  dtmax = 0.1
  #percent_change = 0.1

  start_time = 0.0
  end_time = .03

#  abort_on_solve_fail = true
  adapt_nucleus = 5
  adapt_cycles = 1

  use_nucleation_userobject = false
#  nucleation_userobject = NLUO

  l_max_its = 50

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options = '' #-ksp_monitor'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Output]
  file_base = testActions
  interval = 1
  exodus = true
  perf_log = true
  postprocessor_csv = true
[]
