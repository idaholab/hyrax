/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  21 December 2015
*
*************************************************************************/

#include "AuxGrandPotential.h"

template<>
InputParameters validParams<AuxGrandPotential>()
{
  InputParameters params = validParams<AuxKernel>();

  // params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");
  params.addRequiredCoupledVar("concentration", "name of concentration variable");
  params.addRequiredCoupledVar("OP", "name of order parameter variable");
  params.addRequiredParam<Real>("omega_eq", "equilibrium canonical value");

  params.addParam<Real>("length_scale_factor", 1, "factor simulation length is scaled by");
  params.addParam<Real>("energy_scale_factor", 1, "factor energy is scaled by");

  return params;
}

AuxGrandPotential::AuxGrandPotential(const InputParameters & parameters) :
    AuxKernel(parameters),
    _X(coupledValue("concentration")),
    _OP(coupledValue("OP")),
    _grad_X(coupledGradient("concentration")),
    _grad_OP(coupledGradient("OP")),
    _molar_vol(getMaterialProperty<Real>("molar_volume")),
    _w(getMaterialProperty<Real>("well_height")),
    _Galpha(getMaterialProperty<Real>("G_AB1CD1")),
    _Gdelta(getMaterialProperty<Real>("G_AB1CD2")),
    _dGalpha_dc(getMaterialProperty<Real>("dGAB1CD1_dc")),
    _dGdelta_dc(getMaterialProperty<Real>("dGAB1CD2_dc")),
    _kappa_X(getMaterialProperty<Real>("kappa_c")),
    _kappa_OP(getMaterialProperty<Real>("kappa_n")),
    _omega_eq(getParam<Real>("omega_eq")),
    _length_scale_factor(getParam<Real>("length_scale_factor")),
    _energy_scale_factor(getParam<Real>("energy_scale_factor"))
    //_scaling_factor(getParam<Real>("scaling_factor"))
{
}

Real
AuxGrandPotential::computeValue()
{
  Real OP = _OP[_qp];

  Real H = 6*OP*OP*OP*OP*OP - 15*OP*OP*OP*OP + 10*OP*OP*OP;
  Real G = OP*OP*(1-OP)*(1-OP);


  //NB: all the energies are calculated as aJ/Nm^3

  Real fchem = ((1-H)*_Galpha[_qp] + H*_Gdelta[_qp] + _w[_qp]*G)/_molar_vol[_qp];

  Real fgrad = 0.5*_kappa_X[_qp]*_grad_X[_qp].norm_sq()
    + 0.5*_kappa_OP[_qp]*_grad_OP[_qp].norm_sq();

  Real mu_c = ((1 - H)*_dGalpha_dc[_qp] + H*_dGdelta_dc[_qp])/_molar_vol[_qp];

 return (fchem + fgrad - mu_c*_X[_qp]) - _omega_eq;

}
