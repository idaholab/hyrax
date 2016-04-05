/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*
*  11 June 2013
*
*************************************************************************/

#include "CHCoupledCalphad.h"
#include <cmath>

/**
 * CHCoupledCalphad handles the conserved order parameter (atomic fraction)
 * evolved using the Cahn-Hilliard equation coupled to an arbitrary number of
 * non-conserved structural order parameters for the Zr-delta hydride system.
 * It uses free energy equations taken from A.T. Dinsdale 15 (1991) CALPHAD 317,
 * N. Dupin et al, J. Nuc. Mater. 275 (1999) 287 and J.D. Cox et al., CODATA Key
 * Values for Thermodynamics, Hemisphere Publishing Corporation, 1989.
 */

template<>
InputParameters validParams<CHCoupledCalphad>()
{
  InputParameters params = CHBulk<Real>::validParams();
  params.addRequiredParam<int>("n_OP_variables", "# of coupled OP variables, >=1");
  params.addRequiredCoupledVar("OP_variable_names", "Array of coupled OP variable names");

  return params;
}

CHCoupledCalphad::CHCoupledCalphad(const InputParameters & parameters)
    : CHBulk<Real>(parameters),
      _W(getMaterialProperty<Real>("well_height")),
      _Omega(getMaterialProperty<Real>("molar_volume")),
      _dGalpha_dc(getMaterialProperty<Real>("dGAB1CD1_dc")),
      _d2Galpha_dc2(getMaterialProperty<Real>("d2GAB1CD1_dc2")),
      _d3Galpha_dc3(getMaterialProperty<Real>("d3GAB1CD1_dc3")),
      _dGdelta_dc(getMaterialProperty<Real>("dGAB1CD2_dc")),
      _d2Gdelta_dc2(getMaterialProperty<Real>("d2GAB1CD2_dc2")),
      _d3Gdelta_dc3(getMaterialProperty<Real>("d3GAB1CD2_dc3")),

      _n_OP_variables(getParam<int>("n_OP_variables"))
{
  // Create a vector of the coupled OP variables and gradients
  if(_n_OP_variables != coupledComponents("OP_variable_names"))
    mooseError("Please match the # of orientation variants to coupled OPs (CHCoupledCalphad)");

  _OP.resize(_n_OP_variables);
  _grad_OP.resize(_n_OP_variables);

  for(unsigned int i=0; i< _n_OP_variables; i++)
  {
    _OP[i] = &coupledValue("OP_variable_names", i);
    _grad_OP[i] = &coupledGradient("OP_variable_names", i);
  }
}

RealGradient
CHCoupledCalphad::computeGradDFDCons(PFFunctionType type)
{
  //_console<<"in CHCoupledCalphad computeDFDOP"<<std::endl;
  RealGradient grad_conserved_term, grad_nonconserved_term;

  _Heaviside = computeHeaviside();
  computeDHeaviside();

 switch (type)
  {
  case Residual:
    //calculate the d/dx (dfchem/dx) grad x term
    grad_conserved_term = computeGradConservedTerm();

    //loop to calculate the d/dn (dfchem/dx) grad n terms
    grad_nonconserved_term = computeGradNonconservedTerm();

    // _console<<"Omega = "<<_Omega[_qp]<<std::endl;
    // _console<<"grad_conserved_term = "<<grad_conserved_term<<std::endl;
    // _console<<"grad_nonconserved_term = "<<grad_nonconserved_term<<std::endl;
    // _console<<"grad_conserved_term + grad_nonconserved_term) / _Omega[_qp] = "<<(grad_conserved_term + grad_nonconserved_term) / _Omega[_qp]<<std::endl;

    return (grad_conserved_term + grad_nonconserved_term) / _Omega[_qp];

  case Jacobian:
    RealGradient nonconserved, conserved_first, conserved_second;
    nonconserved.zero();
    // nonconserved = computeGradOPDHeavisideDOP()*_phi[_j][_qp]*(-1*computeD2GalphaDx2() + computeD2GdeltaDx2(L0, L1));

    conserved_first = (1-_Heaviside)*(_d3Galpha_dc3[_qp]*_phi[_j][_qp]*_grad_u[_qp]
                                      + _d2Galpha_dc2[_qp]*_grad_phi[_j][_qp]);

    conserved_second = _Heaviside*(_d3Gdelta_dc3[_qp]*_phi[_j][_qp]*_grad_u[_qp]
                                   + _d2Gdelta_dc2[_qp]*_grad_phi[_j][_qp]);

    for(unsigned int i(0); i < _n_OP_variables; ++i)
      nonconserved += _dHeaviside[i]*(*_grad_OP[i])[_qp]*_phi[_j][_qp];

    nonconserved *= _d2Gdelta_dc2[_qp] - _d2Galpha_dc2[_qp];

    return nonconserved + conserved_first + conserved_second;
  }

  mooseError("Invalid type passed in");
}

Real
CHCoupledCalphad::computeHeaviside()
{
  Real heaviside_first(0);
  Real heaviside_second(0);

  //may need to put some checking in here so that OP fixed between 0 and 1
  for(unsigned int i=0; i<_n_OP_variables; i++)
  {
    heaviside_first += std::pow((*_OP[i])[_qp], 2);
    heaviside_second += std::pow((*_OP[i])[_qp], 3);
  }

  return 3*heaviside_first - 2*heaviside_second;
}

void
CHCoupledCalphad::computeDHeaviside()
{
  _dHeaviside.resize(_n_OP_variables);

  for(unsigned int i(0); i<_n_OP_variables; ++i)
  {
    _dHeaviside[i] = 6*(*_OP[i])[_qp]*(1 - (*_OP[i])[_qp] );
  }
}

RealGradient
CHCoupledCalphad::computeGradConservedTerm()
{
  Real d2Galphadx2, d2Gdeltadx2;

  d2Galphadx2 = (1 - _Heaviside)*_d2Galpha_dc2[_qp];
  d2Gdeltadx2 = _Heaviside*_d2Gdelta_dc2[_qp];

  return (d2Galphadx2 + d2Gdeltadx2)*_grad_u[_qp];
}

RealGradient
CHCoupledCalphad::computeGradNonconservedTerm()
{
  RealGradient sum;
  sum.zero();

  for (unsigned int i(0); i < _n_OP_variables; ++i)
    sum += _dHeaviside[i]*(_dGdelta_dc[_qp] - _dGalpha_dc[_qp])*(*_grad_OP[i])[_qp];

  return sum;
}
