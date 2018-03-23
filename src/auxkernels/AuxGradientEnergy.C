/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 9 January 2014
*
*************************************************************************/

#include "AuxGradientEnergy.h"

registerMooseObject("HyraxApp", AuxGradientEnergy);

template<>
InputParameters validParams<AuxGradientEnergy>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("c", "name of concentration variable");
  params.addRequiredCoupledVar("OP", "name of order parameter variable");

  return params;
}

AuxGradientEnergy::AuxGradientEnergy(const InputParameters & parameters) :
    AuxKernel(parameters),
    _grad_c(coupledGradient("c")),
    _grad_OP(coupledGradient("OP")),
    _kappa_c(getMaterialProperty<Real>("kappa_c")),
    _kappa_OP(getMaterialProperty<Real>("kappa_n"))
{
}

Real
AuxGradientEnergy::computeValue()
{
  return 0.5*_kappa_c[_qp]*_grad_c[_qp].norm_sq() + 0.5*_kappa_OP[_qp]*_grad_OP[_qp].norm_sq();
}
