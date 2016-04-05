# This input file is designed to test the LinearSingleCrystalPrecipitate class.  This test is
# for regression testing.  This just takes the material properties and puts them into
# aux variables; the diffusion kernel is just to have a simple kernel to run the test.

# THIS TEST TESTS THE "ELASTICITY_TENSOR" MATERIAL PROPERTY.

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
 # uniform_refine = 1
[]

[Variables]
  [./diffused]
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
  [./diff]
    type = Diffusion
    variable = diffused
  [../]
[]


[Materials]
  [./test_material]
    type = LinearSingleCrystalPrecipitateMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    C_ijkl = '1.0e6 0.2e6 0.2e6 1.0e6 0.2e6 1.0e6 0.5e6 0.5e6 0.5e6'
    C_precipitate ='1.0e6 0.2e6 0.2e6 1.0e6 0.2e6 1.0e6 0.5e6 0.5e6 0.5e6'
    e_precipitate = '0.00551 0.0564 0.0570 0.0 0.0 0.0'
    n_variants = 1
    variable_names = 'diffused'
    fill_method = symmetric9
  [../]
[]

[BCs]
  [./bottom]
    type = DirichletBC
    variable = diffused
    boundary = '1'
    value = 1
  [../]

  [./top]
    type = DirichletBC
    variable = diffused
    boundary = '2'
    value = 0
  [../]

  [./disp_x_BC]
    type = DirichletBC
    variable = disp_x
    boundary = '0 2'
    value = 0.5
  [../]

  [./disp_x_BC2]
    type = DirichletBC
    variable = disp_x
    boundary = '1 3'
    value = 0.01
  [../]

  [./disp_y_BC]
    type = DirichletBC
    variable = disp_y
    boundary = '0 2'
    value = 0.8
  [../]

  [./disp_y_BC2]
    type = DirichletBC
    variable = disp_y
    boundary = '1 3'
    value = 0.02
  [../]
[]

[Executioner]
  type = Steady

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


[]

[Outputs]
  file_base = Linear_Single_Xstal_Precip_Matl_out
  exodus = true
[]
