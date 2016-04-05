/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  27 December 2013
*
*************************************************************************/

#include "AuxCalphadEnergy.h"
#include <iostream>

template<>
InputParameters validParams<AuxCalphadEnergy>()
{
  InputParameters params = validParams<AuxKernel>();

  ///params.addRequiredCoupledVar("coupled_nonconserved_var", "coupled non-conserved field variable");
  params.addRequiredParam<Real>("precip_conserved", "value of the equilibrium 2nd phase conserved field variable");
  params.addRequiredParam<Real>("precip_nonconserved", "value of the equilibrium 2nd phase nonconserved field variable");

  params.addRequiredParam<int>("OP_number", "the number (starting from 1) of the nonconserved variable");
  params.addRequiredParam<int>("n_OP_vars", "# of coupled OP variables");
  params.addRequiredCoupledVar("OP_var_names", "Array of coupled OP variable names");

  params.addRequiredCoupledVar("concentration", "coupled concentration variable");

  return params;
}

AuxCalphadEnergy::AuxCalphadEnergy(const InputParameters & parameters) :
    AuxKernel(parameters),
    _precip_cons(getParam<Real>("precip_conserved")),
    _precip_noncons(getParam<Real>("precip_nonconserved")),

    _G_alpha(getMaterialProperty<Real>("G_AB1CD1")),
    _G_delta(getMaterialProperty<Real>("G_AB1CD2")),
    _G_alpha_precip(getMaterialProperty<Real>("G_AB1CD1_precip")),
    _G_delta_precip(getMaterialProperty<Real>("G_AB1CD2_precip")),
    _dG_alpha(getMaterialProperty<Real>("dGAB1CD1_dc")),
    _dG_delta(getMaterialProperty<Real>("dGAB1CD2_dc")),

    _dn_misfit_strain(getMaterialProperty<std::vector<RankTwoTensor> >("dn_misfit_strain")),
    _dc_misfit_strain(getMaterialProperty<RankTwoTensor>("dc_misfit_strain")),
    _elastic_strain(getMaterialProperty<RankTwoTensor>("elastic_strain")),
    _local_strain(getMaterialProperty<RankTwoTensor>("local_strain")),

    _elasticity_tensor(getMaterialProperty<ElasticityTensorR4>("elasticity_tensor")),
    _Cijkl_MP(getMaterialProperty<ElasticityTensorR4>("Cijkl_MP")),
    _Cijkl_precipitate_MP(getMaterialProperty<ElasticityTensorR4>("Cijkl_precipitates_MP")),

    _precipitate_eigenstrain(getMaterialProperty<std::vector<RankTwoTensor> >("precipitate_eigenstrain")),
    _matrix_eigenstrain(getMaterialProperty<RankTwoTensor>("matrix_eigenstrain")),

    _Omega(getMaterialProperty<Real>("molar_volume")),
    _W(getMaterialProperty<Real>("well_height")),

    _OP_number(getParam<int>("OP_number")),
    _n_OP_vars(getParam<int>("n_OP_vars")),
    _X(coupledValue("concentration")),

    _H(0),
    _g(0),
    _dH_dOP(0),
    _dg_dOP(0)
//need to add in strains here
{
  if(_n_OP_vars != coupledComponents("OP_var_names"))
    mooseError("Please match the number of orientation variants to coupled OPs (ACCoupledCalphad).");

  _OP.resize(_n_OP_vars);

  for (unsigned int i=0; i<_n_OP_vars; i++)
    _OP[i] = &coupledValue("OP_var_names", i);
}

Real
AuxCalphadEnergy::computeValue()
{
  computeHeaviside();
  computeDHeaviside();
  computeBarrier();
  computeDBarrier();

  Real matrix_energy = computeMatrixEnergy();

  Real precip_energy = computePrecipEnergy();

  Real differential = computeDifferential();

  //this is ugly, but I want to give it a try...
  /* if (matrix_energy - precip_energy + differential > matrix_energy - precip_energy)
    return matrix_energy - precip_energy + differential;
  else
    return matrix_energy - precip_energy;
  */
  // _console<<"matrix energy ="<<matrix_energy<<std::endl;
  //_console<<"precip energy ="<<precip_energy<<std::endl;
  //_console<<"differential = "<<differential<<std::endl;

  return (matrix_energy - precip_energy + differential);
}

Real
AuxCalphadEnergy::computeMatrixEnergy()
{
  //Joules/meter^3
  Real chemical_energy =  ( (1 - _H)*_G_alpha[_qp] + _H*_G_delta[_qp] + _W[_qp]*_g) / _Omega[_qp];
  RankTwoTensor a = _elasticity_tensor[_qp]*(_elastic_strain[_qp]);

  Real elastic_energy = 0.5*a.doubleContraction( _elastic_strain[_qp]);

  //_console<<"matrix elastic energy = "<<elastic_energy<<std::endl;
  //_console<<"matrix chemical energy = "<<chemical_energy<<std::endl;

  return chemical_energy + elastic_energy;
}

Real
AuxCalphadEnergy::computePrecipEnergy()
{
  //this needs to be computed FOR the precipitate if it were this point...
//joules/meter^3
 Real chemical_energy = _G_delta_precip[_qp] / _Omega[_qp];

 RankTwoTensor elastic_precip_strain = _local_strain[_qp] - (_precipitate_eigenstrain[_qp])[_OP_number-1];

 RankTwoTensor a = _Cijkl_precipitate_MP[_qp]*elastic_precip_strain;

 Real elastic_energy = 0.5*a.doubleContraction( elastic_precip_strain);

 //_console<<"precip elastic energy = "<<elastic_energy<<std::endl;
 //_console<<"precip chemical energy = "<<chemical_energy<<std::endl;


 return chemical_energy + elastic_energy;
}

Real
AuxCalphadEnergy::computeDifferential()
{
  //Joules/meter^3
  Real dfchem_dOP = ( (_G_delta[_qp] - _G_alpha[_qp])*_dH_dOP + _W[_qp]*_dg_dOP ) / _Omega[_qp];

  Real dfchem_dX = ( (1-_H)*_dG_alpha[_qp] + _H*_dG_delta[_qp] ) / _Omega[_qp];

  RankTwoTensor a = _Cijkl_MP[_qp]*_elastic_strain[_qp];
  RankTwoTensor b = _Cijkl_MP[_qp]*(_dc_misfit_strain[_qp])*(-1);

  Real e1 = a.doubleContraction( (_dc_misfit_strain[_qp])*(-1) );
  Real e2 = b.doubleContraction( _elastic_strain[_qp] );
  Real e3 = 0;

  Real dfel_dX = 0.5*(e1 + e2 + e3);

  // _console<<"dfel_dX = "<<dfel_dX<<std::endl;

  ElasticityTensorR4 dCijkl = (_Cijkl_precipitate_MP[_qp] - _Cijkl_MP[_qp])*(-1*_dH_dOP);
  b = _Cijkl_MP[_qp]*( (_dn_misfit_strain[_qp])[_OP_number-1]) *(-1);
  RankTwoTensor c = dCijkl*_elastic_strain[_qp];

  e1 = a.doubleContraction( ( (_dn_misfit_strain[_qp])[_OP_number-1] )*(-1) );
  e2 = b.doubleContraction( _elastic_strain[_qp]);
  e3 = c.doubleContraction( _elastic_strain[_qp]);

  Real dfel_dOP = 0.5*(e1 + e2 + e3);

  // _console<<"dfel_dOP = "<<dfel_dOP<<std::endl;

  Real dfdc = (dfchem_dX + dfel_dX)*(_precip_cons - _X[_qp]);

  Real dfdOP = (dfchem_dOP + dfel_dOP)*(_precip_noncons - (*_OP[_OP_number-1])[_qp]);

  return dfdc + dfdOP;
}

void
AuxCalphadEnergy::computeHeaviside()
{
  Real heaviside_first(0);
  Real heaviside_second(0);

  //may need to put some checking in here so that OP fixed between 0 and 1
  for(unsigned int i=0; i<_n_OP_vars; i++)
  {
    heaviside_first += std::pow((*_OP[i])[_qp], 2);
    heaviside_second += std::pow((*_OP[i])[_qp], 3);
  }

  _H = 3*heaviside_first - 2*heaviside_second;
}

void
AuxCalphadEnergy::computeBarrier()
{
  Real first(0);
  Real second(0);
  Real third(0);
  Real fourth(0);
  Real fifth(0);

  Real sixth;
  if(_n_OP_vars == 1)
    sixth = 0;
  else
    sixth = 1;

  for (unsigned int i=0; i<_n_OP_vars; i++)
  {
    first += std::pow((*_OP[i])[_qp], 2);
    second += std::pow((*_OP[i])[_qp], 3);
    third += std::pow((*_OP[i])[_qp], 4);

    Real square_sum(0);
    Real quad_sum(0);
    for (unsigned int j=0; j < _n_OP_vars; j++)
    {
      if (j > i)
        square_sum += std::pow((*_OP[j])[_qp], 2);

      if (j != i)
        quad_sum += std::pow((*_OP[j])[_qp], 4);
    }

    fourth +=  ( std::pow((*_OP[i])[_qp], 2) )*square_sum;
    fifth += ( std::pow((*_OP[i])[_qp], 2) )*quad_sum;

    sixth *= std::pow((*_OP[i])[_qp], 2);
  }

  _g = first - 2*second + third + fourth + fifth + sixth;
}

void
AuxCalphadEnergy::computeDHeaviside()
{
   _dH_dOP = 6*(*_OP[_OP_number-1])[_qp]*(1 - (*_OP[_OP_number-1])[_qp]);
}

void
AuxCalphadEnergy::computeDBarrier()
{
  Real n = (*_OP[_OP_number-1])[_qp];

  Real square_sum, quad_sum, square_mult;
  square_sum = quad_sum = 0.0;

  if (_n_OP_vars == 1)
    square_mult = 0.0;
  else
    square_mult = 1.0;

  //compute the coupled OP terms
  for(unsigned int i=0; i<_n_OP_vars; i++)
  {
    if(i != _OP_number-1)
    {
      Real OP;
      OP = (*_OP[i])[_qp];

      square_sum += OP*OP;
      quad_sum += OP*OP*OP*OP;
      square_mult *= OP*OP;
    }
  }

   _dg_dOP = 2*n - 6*n*n + 4*n*n*n + 2*n*square_sum + 2*n*quad_sum + 4*n*n*n*square_sum
     + 2*n*square_mult;
}

