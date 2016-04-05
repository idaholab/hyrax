# This input file is designed to test the ACTransformElasticDF kernel, which relies on the
# LinearSingleCrystalPrecipitate class.

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
 #[conc]
 #  order = FIRST
 #  family = LAGRANGE
 #[../]

  [./n1]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
     type = SmoothCircleIC
     invalue = 1.0
     outvalue = 0.0
     radius = 10.0
     int_width = 2.0
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

[Kernels]
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
  [../]

  [./dn1dt]
    type = TimeDerivative
    variable = n1
  [../]

  [./ACTransform]
    type = ACTransformElasticDF
    variable = n1
    OP_var_names = 'n1'
    n_OP_vars = 1
    OP_number = 1
  [../]
[]

[Materials]
  [./test_material]
    type = LinearSingleCrystalPrecipitateMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    C_ijkl = '155.4 68.03 64.60 155.4 64.6 172.51 36.31 36.31 44.09'
    C_precipitate = '155.4 68.03 64.60 155.4 64.6 172.51 36.31 36.31 44.09'
#    C_precipitate = '100 200 300 400 500 600  700 800 900'
    e_precipitate = '0.00551 0.0564 0.0570 0.0 0.0 0.0'
    n_variants = 1
    variable_names = 'n1'
    fill_method = symmetric9
  [../]

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
  type = Transient

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  scheme = 'bdf2'

  dt = 0.01
  num_steps = 2
[]

[Outputs]
  file_base = ACTransformElasticDF_out
  exodus = true
[]
