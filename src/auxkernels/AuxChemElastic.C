/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  6 February 2013
*
*************************************************************************/

#include "AuxChemElastic.h"

template<>
InputParameters validParams<AuxChemElastic>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredCoupledVar("coupled_conserved_var", "coupled conserved field variable");
  params.addRequiredCoupledVar("coupled_nonconserved_var", "coupled non-conserved field variable");
  params.addRequiredParam<Real>("precip_conserved", "value of the equilibrium 2nd phase conserved field variable");
  params.addRequiredParam<Real>("precip_nonconserved", "value of the equilibrium 2nd phase nonconserved field variable");
  params.addRequiredParam<int>("nonconserved_var_number", "the number (starting from 1) of the nonconserved variable");

  return params;
}

AuxChemElastic::AuxChemElastic(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_cons(coupledValue("coupled_conserved_var")),
    _coupled_noncons(coupledValue("coupled_nonconserved_var")),
    _precip_conserved(getParam<Real>("precip_conserved")),
    _precip_nonconserved(getParam<Real>("precip_nonconserved")),
/*    _a1(getMaterialProperty<Real>("A1")),
    _a2(getMaterialProperty<Real>("A2")),
    _a3(getMaterialProperty<Real>("A3")),
    _a4(getMaterialProperty<Real>("A4")),
    _c1(getMaterialProperty<Real>("C1")),
    _c2(getMaterialProperty<Real>("C2")), */
    _noncons_var_num(getParam<int>("nonconserved_var_number")),
    _eigenstrains_rotated_MP(getMaterialProperty<std::vector<RankTwoTensor> >("eigenstrains_MP")),
    _elasticity_tensor(getMaterialProperty<ElasticityTensorR4>("elasticity_tensor")),
    _precipitate_eigenstrain_rotated(getMaterialProperty<std::vector<RankTwoTensor> >("precipitate_eigenstrain")),
    _precipitate_elasticity(getMaterialProperty<ElasticityTensorR4>("Cijkl_precipitates_MP")),
    _local_strain(getMaterialProperty<RankTwoTensor>("local_strain")),
    _d_eigenstrains_rotated_MP(getMaterialProperty<std::vector<RankTwoTensor> >("d_eigenstrains_MP"))
{
}

Real
AuxChemElastic::computeValue()
{
  Real matrix_energy(0.0);
  Real precip_energy(0.0);
  Real differential(0.0);

  matrix_energy = computeEnergy(_coupled_cons[_qp], _coupled_noncons[_qp], true);
  precip_energy = computeEnergy(_precip_conserved, _precip_nonconserved, false);
  differential = computeDifferential(_coupled_cons[_qp], _coupled_noncons[_qp]);

  // return (precip_energy - matrix_energy + differential);
  return (matrix_energy - precip_energy + differential);
}

Real
AuxChemElastic::computeEnergy(const Real & conserved, const Real & nonconserved, bool matrix)
{
  Real fchem(0.0);
  Real self_elastic_energy(0.0);
  Real interaction_elastic_energy(0.0);

  fchem = computeFchem(conserved, nonconserved);
  self_elastic_energy = computeSelfElasticEnergy(matrix);
  interaction_elastic_energy = computeInteractionElasticEnergy(matrix);

  return fchem + self_elastic_energy - interaction_elastic_energy;
}

Real
AuxChemElastic::computeDifferential(const Real & coupled_conserved, const Real & coupled_nonconserved)
{
  // partial derivative of f_chem with respect to conserved variable
  Real dfchem_dcons(0.0);
  // partial derivative of f_chem with respect to nonconserved variable
  Real dfchem_dnoncons(0.0);

  // partial derivatives of self-elastic energies
  Real dself_dcons(0.0);
  Real dself_dnoncons(0.0);

  // partial derivative of interaction elastic energy
  Real dint_dcons(0.0);
  Real dint_dnoncons(0.0);

  Real first_term(0.0);
  Real second_term(0.0);

  dfchem_dcons = computeDfchemDcons(coupled_conserved, coupled_nonconserved);
  dself_dcons = computeDselfDcons();
  dint_dcons = computeDintDcons();

  dfchem_dnoncons = computeDfchemDnoncons(coupled_conserved, coupled_nonconserved);
  dself_dnoncons = computeDselfDnoncons();
  dint_dnoncons = computeDintDnoncons();

  first_term = (dfchem_dcons + dself_dcons + dint_dcons)*(_precip_conserved - coupled_conserved);
  second_term = (dfchem_dnoncons + dself_dnoncons + dint_dnoncons)*(_precip_nonconserved - coupled_nonconserved);

  return first_term + second_term;
}


Real
AuxChemElastic::computeFchem(const Real & /*conserved*/, const Real & /*nonconserved*/)
{
  return 0;
}


Real
AuxChemElastic::computeSelfElasticEnergy(bool matrix)
{
  RankTwoTensor eigenstrain;
  RankTwoTensor c;
  ElasticityTensorR4 elasticity;

  if(matrix)
  {
    eigenstrain = (_eigenstrains_rotated_MP[_qp])[_noncons_var_num-1];
    elasticity = _elasticity_tensor[_qp];
  }
  else
  {
    eigenstrain = (_precipitate_eigenstrain_rotated[_qp])[_noncons_var_num-1];
    elasticity = _precipitate_elasticity[_qp];
  }

  c = elasticity*eigenstrain;

  return 0.5*c.doubleContraction(eigenstrain);
}

Real
AuxChemElastic::computeInteractionElasticEnergy(bool matrix)
{
  RankTwoTensor eigenstrain;
  RankTwoTensor c;
  ElasticityTensorR4 elasticity;

  if(matrix)
  {
    eigenstrain = (_eigenstrains_rotated_MP[_qp])[_noncons_var_num-1];
    elasticity = _elasticity_tensor[_qp];
  }
  else
  {
    eigenstrain = (_precipitate_eigenstrain_rotated[_qp])[_noncons_var_num-1];
    elasticity = _precipitate_elasticity[_qp];
  }

  c = elasticity*eigenstrain;

  return c.doubleContraction(_local_strain[_qp]);
}


Real
AuxChemElastic::computeDfchemDcons(const Real & /*coupled_conserved*/, const Real & /*coupled_nonconserved*/)
{
  return 0;
}

Real
AuxChemElastic::computeDselfDcons()
{
  return 0.0;
}

Real
AuxChemElastic::computeDintDcons()
{
  return 0.0;
}

Real
AuxChemElastic::computeDfchemDnoncons(const Real & /*coupled_conserved*/, const Real & /*coupled_nonconserved*/)
{
  return 0;
}

Real
AuxChemElastic::computeDselfDnoncons()
{

  RankTwoTensor eigenstrain;
  RankTwoTensor d_eigenstrain;
  RankTwoTensor c;
  ElasticityTensorR4 elasticity;

  eigenstrain = (_eigenstrains_rotated_MP[_qp])[_noncons_var_num-1];
  d_eigenstrain =( _d_eigenstrains_rotated_MP[_qp])[_noncons_var_num-1];
  elasticity = _elasticity_tensor[_qp];

  c = elasticity*eigenstrain;

  return 2.0*c.doubleContraction(d_eigenstrain);
}

Real
AuxChemElastic::computeDintDnoncons()
{
  RankTwoTensor d_eigenstrain;
  RankTwoTensor c;
  ElasticityTensorR4 elasticity;

  d_eigenstrain = (_precipitate_eigenstrain_rotated[_qp])[_noncons_var_num-1];
  elasticity = _precipitate_elasticity[_qp];

  c = elasticity*d_eigenstrain;

  return -2.0*c.doubleContraction(_local_strain[_qp]);
}

