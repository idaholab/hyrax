# This input file tests the AuxAMRNucleationProbability auxkernel.
#Uses ELEMENTAL aux variables!

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 4
  ny = 4
  nz = 0
  xmin = 0
  xmax = 8
  ymin = 0
  ymax = 8
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./concentration]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = RandomIC
    [../]
  [../]

  [./n1]
   order = FIRST
   family = LAGRANGE
   [./InitialCondition]
      type = RandomIC
      min = 0.0
      max = 1e-6
    [../]
  [../]
 []

[AuxVariables]
  [./elemental_Supersaturation]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elemental_NucleationRate]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elemental_temperature]
    order = CONSTANT
    family = MONOMIAL
    [./InitialCondition]
       type = ConstantIC
       value = 600
    [../]
  [../]

  [./elemental_NucleationProbability]
    order = CONSTANT
    family = MONOMIAL
  [../]
[../]

[Kernels]
  [./time_deriv_diff]
    type = TimeDerivative
    variable = concentration
  [../]

  [./time_deriv_diff2]
    type = TimeDerivative
    variable = n1
  [../]

 [./diffusion]
    type = Diffusion
    variable = concentration
  [../]

  [./diff2]
   type = Diffusion
   variable = n1
  [../]
[]

[AuxKernels]
  [./Supersaturation]
    type = AuxSupersaturation
    variable = elemental_Supersaturation
    coupled_var = concentration
    c1 = 0.1
    execute_on = timestep_end
  [../]

 [./NucleationRate]
    type = AuxVolumetricNucleationRate
    variable = elemental_NucleationRate
    coupled_bulk_energy_change = elemental_Supersaturation

    gamma = 0.18
    time_scale_factor = 1
    Kb = 1

    linear_density = 5
    T = elemental_temperature
    X = concentration

    jump_distance = 1
    length_scale_factor = 1
    rate_volume = 1

    execute_on = timestep_end
  [../]

  [./AuxNucleationProbability]
    type = AuxAMRNucleationProbability
    variable = elemental_NucleationProbability
    coupled_aux_var = elemental_NucleationRate
    coupled_variable = n1

    2D_mesh_height = 1

    execute_on = timestep_end
 [../]
[]

[Materials]
  [./simple_material]
    type = MatlAuxVolNucRate
    D_alpha = 2
    molar_vol = 3
    block = 0
  [../]
[]

[BCs]
 [./Periodic]
  [./all]
    variable = concentration
    auto_direction = 'x y'
  [../]
 [../]
[]

[Executioner]
  type = Transient
   dt = 0.001
   num_steps = 2

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
[]

[Outputs]
  file_base = AuxAMRNucleationProbability
  exodus = true
[]
