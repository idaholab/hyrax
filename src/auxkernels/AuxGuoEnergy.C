/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  27 December 2013
*
*************************************************************************/

#include "AuxGuoEnergy.h"

template<>
InputParameters validParams<AuxGuoEnergy>()
{
  InputParameters params = validParams<AuxChemElastic>();

  /*
  params.addRequiredCoupledVar("coupled_conserved_var", "coupled conserved field variable");
  params.addRequiredCoupledVar("coupled_nonconserved_var", "coupled non-conserved field variable");
  params.addRequiredParam<Real>("precip_conserved", "value of the equilibrium 2nd phase conserved field variable");
  params.addRequiredParam<Real>("precip_nonconserved", "value of the equilibrium 2nd phase nonconserved field variable");
  params.addRequiredParam<int>("nonconserved_var_number", "the number (starting from 1) of the nonconserved variable");
  */

  return params;
}

AuxGuoEnergy::AuxGuoEnergy(const InputParameters & parameters) :
    AuxChemElastic(parameters),
/*
    _coupled_cons(coupledValue("coupled_conserved_var")),
    _coupled_noncons(coupledValue("coupled_nonconserved_var")),
    _precip_conserved(getParam<Real>("precip_conserved")),
    _precip_nonconserved(getParam<Real>("precip_nonconserved")),
*/
    _a1(getMaterialProperty<Real>("A1")),
    _a2(getMaterialProperty<Real>("A2")),
    _a3(getMaterialProperty<Real>("A3")),
    _a4(getMaterialProperty<Real>("A4")),
    _c1(getMaterialProperty<Real>("C1")),
    _c2(getMaterialProperty<Real>("C2")) //,
/*
    _noncons_var_num(getParam<int>("nonconserved_var_number")),
    _eigenstrains_rotated_MP(getMaterialProperty<std::vector<RankTwoTensor> >("eigenstrains_MP")),
    _elasticity_tensor(getMaterialProperty<RankFourTensor>("elasticity_tensor")),
    _precipitate_eigenstrain_rotated(getMaterialProperty<std::vector<RankTwoTensor> >("precipitate_eigenstrain")),
    _precipitate_elasticity(getMaterialProperty<RankFourTensor>("Cijkl_precipitates_MP")),
    _local_strain(getMaterialProperty<RankTwoTensor>("local_strain")),
    _d_eigenstrains_rotated_MP(getMaterialProperty<std::vector<RankTwoTensor> >("d_eigenstrains_MP"))
*/
{
}

/*
Real
AuxGuoEnergy::computeValue()
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
*/

/*
Real
AuxGuoEnergy::computeEnergy(Real & conserved, Real & nonconserved, bool matrix)
{
  Real fchem(0.0);
  Real self_elastic_energy(0.0);
  Real interaction_elastic_energy(0.0);

  fchem = computeFchem(conserved, nonconserved);
  self_elastic_energy = computeSelfElasticEnergy(matrix);
  interaction_elastic_energy = computeInteractionElasticEnergy(matrix);

  return fchem + self_elastic_energy - interaction_elastic_energy;
}
*/

/*
Real
AuxGuoEnergy::computeDifferential(Real & coupled_conserved, Real & coupled_nonconserved)
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

  // first_term = (dfchem_dcons + dself_dcons + dint_dcons)*(coupled_conserved - _precip_conserved);
  // second_term = (dfchem_dnoncons + dself_dnoncons + dint_dnoncons)*(coupled_nonconserved - _precip_nonconserved);

  first_term = (dfchem_dcons + dself_dcons + dint_dcons)*(_precip_conserved - coupled_conserved);
  second_term = (dfchem_dnoncons + dself_dnoncons + dint_dnoncons)*(_precip_nonconserved - coupled_nonconserved);

  return first_term + second_term;
}
*/

Real
AuxGuoEnergy::computeFchem(Real & conserved, Real & nonconserved)
{
  Real first_term;
  Real second_term;
  Real third_term;
  Real fourth_term;

  first_term = _a1[_qp]*0.5*(conserved - _c1[_qp])*(conserved - _c1[_qp]);
  second_term = _a2[_qp]*0.5*(conserved - _c2[_qp])*(nonconserved)*(nonconserved);
  third_term = _a3[_qp]*0.25*(std::pow(nonconserved, 4));
  fourth_term = _a4[_qp]*(std::pow(nonconserved, 6))/6;

  return first_term + second_term - third_term + fourth_term;
}

/*
Real
AuxGuoEnergy::computeSelfElasticEnergy(bool matrix)
{
  RankTwoTensor eigenstrain;
  RankTwoTensor c;
  RankFourTensor elasticity;

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
*/

/*
Real
AuxGuoEnergy::computeInteractionElasticEnergy(bool matrix)
{
  RankTwoTensor eigenstrain;
  RankTwoTensor c;
  RankFourTensor elasticity;

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
*/

Real
AuxGuoEnergy::computeDfchemDcons(Real & coupled_conserved, Real & coupled_nonconserved)
{
  Real first_term;
  Real second_term;

 first_term = _a1[_qp]*(coupled_conserved - _c1[_qp]);
  second_term =  0.5*_a2[_qp]*coupled_nonconserved*coupled_nonconserved;

  return first_term + second_term;
}

Real
AuxGuoEnergy::computeDselfDcons()
{
  //this is legitimately 0 for the current formulation

  return 0.0;
}

Real
AuxGuoEnergy::computeDintDcons()
{
// this is legitimately 0 for the current formulation
  return 0.0;
}

Real
AuxGuoEnergy::computeDfchemDnoncons(Real & coupled_conserved, Real & coupled_nonconserved)
{
  Real first_term;
  Real second_term;
  Real third_term;

  first_term = _a2[_qp]*coupled_nonconserved*(coupled_conserved - _c2[_qp]);
  second_term = _a3[_qp]*std::pow(coupled_nonconserved, 3);
  third_term = _a4[_qp]*std::pow(coupled_nonconserved, 5);

  return first_term - second_term + third_term;
}

/*
Real
AuxGuoEnergy::computeDselfDnoncons()
{

  RankTwoTensor eigenstrain;
  RankTwoTensor d_eigenstrain;
  RankTwoTensor c;
  RankFourTensor elasticity;

  eigenstrain = (_eigenstrains_rotated_MP[_qp])[_noncons_var_num-1];
  d_eigenstrain =( _d_eigenstrains_rotated_MP[_qp])[_noncons_var_num-1];
  elasticity = _elasticity_tensor[_qp];

  c = elasticity*eigenstrain;

  return 2.0*c.doubleContraction(d_eigenstrain);
}
*/

 /*
Real
AuxGuoEnergy::computeDintDnoncons()
{
  RankTwoTensor d_eigenstrain;
  RankTwoTensor c;
  RankFourTensor elasticity;

  d_eigenstrain = (_precipitate_eigenstrain_rotated[_qp])[_noncons_var_num-1];
  elasticity = _precipitate_elasticity[_qp];

  c = elasticity*d_eigenstrain;

  return -2.0*c.doubleContraction(_local_strain[_qp]);
}
*/
