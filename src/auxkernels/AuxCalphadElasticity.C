/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  22 November 2014
*
*************************************************************************/

#include "AuxCalphadElasticity.h"
#include <iostream>


template<>
InputParameters validParams<AuxCalphadElasticity>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredParam<Real>("precip_conserved", "value of equil 2nd phase conserved field var");
  params.addRequiredParam<Real>("precip_nonconserved", "value of equil 2nd phase nonconserved field var");
  params.addRequiredParam<Real>("self_energy", "Elastic self-interaction energy of precipitate in scaled or unscaled units according to the rest of the simulation");

  params.addRequiredCoupledVar("OP", "coupled order parameter");

  params.addRequiredCoupledVar("concentration", "coupled concentration variable");
  params.addParam<bool>("use_elastic_energy", true, "boolean for if chemical+elastic or only chem energy used in nucleation");

  return params;
}

AuxCalphadElasticity::AuxCalphadElasticity(const InputParameters & parameters) :
  AuxKernel(parameters),
  _precip_cons(getParam<Real>("precip_conserved")),
  _precip_noncons(getParam<Real>("precip_nonconserved")),
  _self_energy(getParam<Real>("self_energy")),

  _G_alpha(getMaterialProperty<Real>("G_AB1CD1")),
  _G_delta(getMaterialProperty<Real>("G_AB1CD2")),
  _G_alpha_precip(getMaterialProperty<Real>("G_AB1CD1_precip")),
  _G_delta_precip(getMaterialProperty<Real>("G_AB1CD2_precip")),
  _dG_alpha(getMaterialProperty<Real>("dGAB1CD1_dc")),
  _dG_delta(getMaterialProperty<Real>("dGAB1CD2_dc")),
  _use_elastic_energy(getParam<bool>("use_elastic_energy")),
  _stress(_use_elastic_energy ? getMaterialProperty<RankTwoTensor>("stress") : getMaterialProperty<RankTwoTensor>("void_tensor")),
  _precipitate_eigenstrain(_use_elastic_energy ? getMaterialProperty<RankTwoTensor>("precipitate_misfit_tensor") : getMaterialProperty<RankTwoTensor>("void_tensor")),
  _Omega(getMaterialProperty<Real>("molar_volume")),
  _W(getMaterialProperty<Real>("well_height")),
  _OP(coupledValue("OP")),
  _X(coupledValue("concentration"))
{
}

Real
AuxCalphadElasticity::computeValue()
{

  Real chem_matrix_energy = computeChemMatrixEnergy();
  Real chem_precip_energy = computeChemPrecipEnergy();
  Real chem_differential = computeDifferential();

  Real elastic_energy = 0;

  if(_use_elastic_energy)
    elastic_energy = computeElasticEnergy();

  //_console<<"chem_matrix_energy = "<<chem_matrix_energy<<std::endl;
  //_console<<"chem_precip_energy = "<<chem_precip_energy<<std::endl;
  //_console<<"chem_differential = "<<chem_differential<<std::endl;
  //_console<<"elastic_energy = "<<elastic_energy<<std::endl;

  //_console<<"deltaf_total = "<<(chem_matrix_energy - chem_precip_energy + chem_differential) - elastic_energy<<std::endl;

  return (chem_matrix_energy - chem_precip_energy + chem_differential) - elastic_energy;
}

Real
AuxCalphadElasticity::computeChemMatrixEnergy()
{
  //aJoules/nanometer^3
  return _G_alpha[_qp]/_Omega[_qp];
}

Real
AuxCalphadElasticity::computeChemPrecipEnergy()
{
 //this needs to be computed FOR the precipitate if it were this point...
 //ajoules/nanometer^3
 return _G_delta_precip[_qp] / _Omega[_qp];
}

Real
AuxCalphadElasticity::computeElasticEnergy()
{
  //needs to be the local elastic stress and the precipitate misfit tensor

  //NB that the precipitate_eigenstrain is locked to the amount of misfit strain relaxation
  //should probably unlink this - maybe specify the amount it should be relaxed by here?

  return ( _self_energy
           - _stress[_qp].doubleContraction(_precipitate_eigenstrain[_qp] ));
}

Real
AuxCalphadElasticity::computeDifferential()
{
  //aJoules/nanometer^3
  Real dfdc = _dG_alpha[_qp]*(_precip_cons - _X[_qp])/_Omega[_qp];

  // Real dfdOP = dfchem_dOP*(_precip_noncons - (*_OP[_OP_number-1])[_qp]);

  // _console<<"dfdc = "<<dfdc<<std::endl;
  // _console<<"dfdOP = "<<dfdOP<<std::endl;

  //return dfdc + dfdOP;
  return dfdc;
}
