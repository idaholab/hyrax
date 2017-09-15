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

  params.addRequiredParam<Real>("omega_eq", "equilibrium canonical value");
  params.addRequiredCoupledVar("concentration", "concentration variable");
  params.addRequiredCoupledVar("OP", "order parameter variable");

  return params;
}

AuxGrandPotential::AuxGrandPotential(const InputParameters & parameters) :
    AuxKernel(parameters),
    _X(coupledValue("concentration")),
    _grad_X(coupledGradient("concentration")),
    _second_X(coupledSecond("concentration")),
    _grad_OP(coupledGradient("OP")),
    _fbulk(getMaterialProperty<Real>("f_bulk")),
    _dfbulk_dc(getMaterialProperty<Real>("df_bulk_dc")),
    _kappa_X(getMaterialProperty<Real>("kappa_c")),
    _kappa_OP(getMaterialProperty<Real>("kappa_n")),
    _omega_eq(getParam<Real>("omega_eq"))
{
}

Real
AuxGrandPotential::computeValue()
{

  //NB: all the energies are calculated as aJ/nm^3

  Real fgrad = 0.5*_kappa_X[_qp]*_grad_X[_qp].norm_sq()
    + 0.5*_kappa_OP[_qp]*_grad_OP[_qp].norm_sq();

  // return (_fbulk[_qp] + fgrad - _dfbulk_dc[_qp]*_X[_qp]) - _omega_eq;


  Real deltaFdeltaX = _dfbulk_dc[_qp] - _kappa_X[_qp] * _second_X[_qp].tr();

  return (_fbulk[_qp] + fgrad - deltaFdeltaX*_X[_qp]) - _omega_eq;


}
