# This input file demonstrates the coupled multiple Allen-Cahn, Cahn-Hilliard
# equations and explicit nucleation.  It tests calculation of critical activation
# energy.

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
      type = ConstantIC
      value = 1.0
    [../]
  [../]

  [./n1]
   order = FIRST
   family = LAGRANGE
  [../]
 []

[AuxVariables]
  [./elem_Supersaturation]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elem_DeltaGStar]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
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
    variable = elem_Supersaturation
    coupled_var = concentration
    functional_c1 = 0.006
    execute_on = timestep_end
  [../]

  [./DeltaGStar]
    type = AuxDeltaGStar
    variable = elem_DeltaGStar
    coupled_aux_var = elem_Supersaturation

    gamma = 0.18
    scale_factor = 1
    Kb = 1

    execute_on = timestep_end
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
  type = Steady

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

[]

[Outputs]
  file_base = AuxDeltaGStar
  exodus = true
[]
