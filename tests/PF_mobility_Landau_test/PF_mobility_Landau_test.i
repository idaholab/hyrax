# This input file is designed to test the PFMobilityLandau material class.  This test is
# for regression testing.  This just takes the material properties and puts them into
# aux variables; the diffusion kernel is just to have a simple kernel to run the test.

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
[]

[Variables]
  [./diffused]
    order = FIRST
    family = LAGRANGE
  [../]
[]

# Materials properties into AuxVariables - these are elemental variables, not nodal variables.
[AuxVariables]
  [./a1_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./a2_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./a3_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./a4_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./a5_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./a6_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./a7_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./c1_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./c2_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./M_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./L_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./kappa_c_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./kappa_n_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = diffused
  [../]
[]

[AuxKernels]
  [./matl_a1]
    type = MaterialRealAux
    property = A1
    variable = a1_aux
  [../]

 [./matl_a2]
   type = MaterialRealAux
   property = A2
   variable = a2_aux
 [../]

  [./matl_a3]
    type = MaterialRealAux
    property = A3
    variable = a3_aux
  [../]

  [./matl_a4]
    type = MaterialRealAux
    property = A4
    variable = a4_aux
  [../]

  [./matl_a5]
    type = MaterialRealAux
    property = A5
    variable = a5_aux
  [../]

  [./matl_a6]
    type = MaterialRealAux
    property = A6
    variable = a6_aux
  [../]

  [./matl_a7]
    type = MaterialRealAux
    property = A7
    variable = a7_aux
  [../]

  [./matl_c1]
    type = MaterialRealAux
    property = C1
    variable = c1_aux
  [../]

  [./matl_c2]
    type = MaterialRealAux
    property = C2
    variable = c2_aux
  [../]

  [./matl_M]
    type = MaterialRealAux
    property = M
    variable = M_aux
  [../]

  [./matl_L]
    type = MaterialRealAux
    property = L
    variable = L_aux
  [../]

  [./matl_kappa_c]
    type = MaterialRealAux
    property = kappa_c
    variable = kappa_c_aux
  [../]

  [./matl_kappa_n]
    type = MaterialRealAux
    property = kappa_n
    variable = kappa_n_aux
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
    A5 = 1.0
    A6 = 2.0
    A7 = 3.0
    C1 = 0.006
    C2 = 0.59
  [../]
[]

[BCs]
active = 'bottom top'
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
[]

[Executioner]
  type = Steady

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


[]

[Outputs]
  file_base = PF_mobility_Landau_out
  exodus = true
[]
