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

  //params.addRequiredParam<int>("OP_number", "the number (starting from 1) of the nonconserved variable");
  //params.addRequiredParam<int>("n_OP_vars", "# of coupled OP variables");
  //params.addRequiredCoupledVar("OP_var_names", "Array of coupled OP variable names");

  params.addRequiredCoupledVar("OP", "coupled order parameter");

  params.addRequiredCoupledVar("concentration", "coupled concentration variable");
  //params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");
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
  _stress(getMaterialProperty<RankTwoTensor>("stress")),
  _precipitate_eigenstrain(getMaterialProperty<RankTwoTensor>("precipitate_misfit_tensor")),
  _Omega(getMaterialProperty<Real>("molar_volume")),
  _W(getMaterialProperty<Real>("well_height")),

//_OP_number(getParam<int>("OP_number")),
//  _n_OP_vars(getParam<int>("n_OP_vars")),
  _OP(coupledValue("OP")),
  _X(coupledValue("concentration")),
//_scaling_factor(getParam<Real>("scaling_factor")),
  _use_elastic_energy(getParam<bool>("use_elastic_energy")),

  _H(0),
  _g(0),
  _dH_dOP(0),
  _dg_dOP(0)
{
  //if(_n_OP_vars != coupledComponents("OP_var_names"))
  //  mooseError("Please match the number of orientation variants to coupled OPs (ACCoupledCalphad).");

  // _OP.resize(_n_OP_vars);

  // for (unsigned int i=0; i<_n_OP_vars; i++)
  //   _OP[i] = &coupledValue("OP_var_names", i);
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

  //_console<<"deltaf_total = "<<(chem_matrix_energy - chem_precip_energy + chem_differential) + elastic_energy<<std::endl;

  return (chem_matrix_energy - chem_precip_energy + chem_differential) - elastic_energy;
}

Real
AuxCalphadElasticity::computeChemMatrixEnergy()
{
  //Joules/meter^3
  //return ( (1 - _H)*_G_alpha[_qp] + _H*_G_delta[_qp] + _W[_qp]*_g) / _Omega[_qp];
  return _G_alpha[_qp]/_Omega[_qp];
}

Real
AuxCalphadElasticity::computeChemPrecipEnergy()
{
 //this needs to be computed FOR the precipitate if it were this point...
 //joules/meter^3
 return _G_delta_precip[_qp] / _Omega[_qp];
}

Real
AuxCalphadElasticity::computeElasticEnergy()
{
  //needs to be the local elastic stress and the precipitate misfit tensor
  //this needs to be orientation dependent, so we need to use OP_number
  //if Cijkl is scaled, then this energy will be scaled. Self_energy must also
  //be scaled so that the whole computation is unscaled using the scaling factor.

  // Real inverse = 1/_scaling_factor;

  return ( _self_energy
           - _stress[_qp].doubleContraction(_precipitate_eigenstrain[_qp] ));
}

Real
AuxCalphadElasticity::computeDifferential()
{
  //Joules/meter^3
  // Real dfchem_dOP = ( (_G_delta[_qp] - _G_alpha[_qp])*_dH_dOP + _W[_qp]*_dg_dOP ) / _Omega[_qp];

  // Real dfchem_dX = ( (1-_H)*_dG_alpha[_qp] + _H*_dG_delta[_qp] ) / _Omega[_qp];

  // Real dfdc = dfchem_dX*(_precip_cons - _X[_qp]);
  Real dfdc = _dG_alpha[_qp]*(_precip_cons - _X[_qp])/_Omega[_qp];

  // Real dfdOP = dfchem_dOP*(_precip_noncons - (*_OP[_OP_number-1])[_qp]);

  // _console<<"dfdc = "<<dfdc<<std::endl;
  // _console<<"dfdOP = "<<dfdOP<<std::endl;

  //return dfdc + dfdOP;
  return dfdc;
}
