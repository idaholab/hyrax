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

template<>
InputParameters validParams<AuxGradientEnergy>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("field_variable", "field variable to calculate gradient energy of");
  params.addRequiredParam<std::string>("kappa_name", "name of gradient energy coefficient");

  return params;
}

AuxGradientEnergy::AuxGradientEnergy(const InputParameters & parameters) :
    AuxKernel(parameters),
    _grad_var(coupledGradient("field_variable")),
    _kappa_name(getParam<std::string>("kappa_name")),
    _kappa(getMaterialProperty<Real>(_kappa_name))
{
}

Real
AuxGradientEnergy::computeValue()
{
 return 0.5*_kappa[_qp]*_grad_var[_qp].norm_sq();
}


