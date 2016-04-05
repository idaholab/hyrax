/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  18 November 2013
*
*************************************************************************/

#include "CHCoupledSplit.h"

template<>
InputParameters validParams<CHCoupledSplit>()
{
  InputParameters params = validParams<SplitCHCRes>();
  // params.addRequiredParam<Real>("Well_height", "well height of the double-well functional");
    params.addRequiredCoupledVar("coupled_OP_var", "The order parameter coupled to the CH eqn");

  return params;
}

CHCoupledSplit::CHCoupledSplit(const InputParameters & parameters):
    SplitCHCRes(parameters),
    //   _W(getParam<Real>("Well_height"))
    // Get the material properties for the Landau coefficients
    _a1(getMaterialProperty<Real>("A1")),
    _a2(getMaterialProperty<Real>("A2")),
    _c1(getMaterialProperty<Real>("C1")),
    // Make the coupled value whatever is directed in the input file
    _coupled_OP(coupledValue("coupled_OP_var"))
    // _coupled_OP_grad(coupledGradient("coupled_OP_var"))
{
}

Real
CHCoupledSplit::computeDFDC(PFFunctionType type)
{
  switch (type)
  {
  case Residual:
    // return _W*(2*_u[_qp] - 6*_u[_qp]*_u[_qp] + 4*_u[_qp]*_u[_qp]*_u[_qp]);
    return _a1[_qp]*(_u[_qp] - _c1[_qp]) + 0.5*_a2[_qp]*(_coupled_OP[_qp])*(_coupled_OP[_qp]);

  case Jacobian:
    //this isn't necessarily perfect
    // return _W*(2 - 12*_u[_qp] + 12*_u[_qp]*_u[_qp])*_phi[_j][_qp];
    return _a1[_qp]*_phi[_j][_qp];

  }
  mooseError("invalid type passed in");
}
