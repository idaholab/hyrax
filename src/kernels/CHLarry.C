/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  2 October 2013
*
*************************************************************************/

#include "CHLarry.h"

template<>
InputParameters validParams<CHLarry>()
{
  InputParameters params = CHBulk<Real>::validParams();
  params.addRequiredParam<Real>("W", "well height of the double-well functional");

  return params;
}

CHLarry::CHLarry(const InputParameters & parameters)
    :CHBulk<Real>(parameters),
    _W(getParam<Real>("W"))
{
}

RealGradient
CHLarry::computeGradDFDCons(PFFunctionType type)
{
  switch (type)
  {
  case Residual:
    return _W*(2.0 - 12.0*_u[_qp] + 12.0*_u[_qp]*_u[_qp]) * _grad_u[_qp];

  case Jacobian:
    //this isn't necessarily perfect
    return _W*_grad_phi[_j][_qp]*(-12.0*_phi[_j][_qp] + 24.0*_u[_qp]*_phi[_j][_qp]);
  }
  mooseError("invalid type passed in");
}
