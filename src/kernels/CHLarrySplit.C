/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  4 November 2013
*
*************************************************************************/

#include "CHLarrySplit.h"

template<>
InputParameters validParams<CHLarrySplit>()
{
  InputParameters params = validParams<SplitCHCRes>();
  params.addRequiredParam<Real>("Well_height", "well height of the double-well functional");

  return params;
}

CHLarrySplit::CHLarrySplit(const InputParameters & parameters):
    SplitCHCRes(parameters),
    _W(getParam<Real>("Well_height"))
{
}

Real
CHLarrySplit::computeDFDC(PFFunctionType type)
{
  switch (type)
  {
  case Residual:
    return _W*(2*_u[_qp] - 6*_u[_qp]*_u[_qp] + 4*_u[_qp]*_u[_qp]*_u[_qp]);

  case Jacobian:
    //this isn't necessarily perfect
    return _W*(2 - 12*_u[_qp] + 12*_u[_qp]*_u[_qp])*_phi[_j][_qp];
  }
  mooseError("invalid type passed in");
}
