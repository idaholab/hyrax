/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  21 December 2015
*
*************************************************************************/

#include "AuxCanonicalEnsemble.h"

template<>
InputParameters validParams<AuxCanonicalEnsemble>()
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

AuxCanonicalEnsemble::AuxCanonicalEnsemble(const InputParameters & parameters) :
    AuxKernel(parameters),
    _X(coupledValue("concentration")),
    _OP(coupledValue("OP")),
    _grad_X(coupledGradient("concentration")),
    _grad_OP(coupledGradient("OP")),
    _molar_vol(getMaterialProperty<Real>("molar_volume")),
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
AuxCanonicalEnsemble::computeValue()
{
  Real H = 3*_OP[_qp]*_OP[_qp] - 2*_OP[_qp]*_OP[_qp]*_OP[_qp];

  //NB: all the energies are calculated as J/m^3
  
  Real fchem = ((1-H)*_Galpha[_qp] + H*_Gdelta[_qp])/_molar_vol[_qp];

  //NB: need the kappa scale factor because the simulation is
  //supplied scaled kappas
  
  Real kappa_scale_factor = _length_scale_factor*_length_scale_factor*_energy_scale_factor;
  
  Real fgrad = 0.5*_kappa_X[_qp]*kappa_scale_factor*_grad_X[_qp].norm_sq()
    + 0.5*_kappa_OP[_qp]*kappa_scale_factor*_grad_OP[_qp].norm_sq();

  Real mu_c = ((1 - H)*_dGalpha_dc[_qp] + H*_dGdelta_dc[_qp])/_molar_vol[_qp];
  
  Real scale = _length_scale_factor*_length_scale_factor*_length_scale_factor;

  //NB: now the length must be scaled again to match the length scale of the simulation (typically in nm) so that when this value is integrated, the units come out properly
  
 return (fchem + fgrad - mu_c*_X[_qp])*scale - _omega_eq;

}



