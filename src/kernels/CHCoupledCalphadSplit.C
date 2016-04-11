/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*
*  19 November 2013
*
*************************************************************************/

#include "CHCoupledCalphadSplit.h"

template<>
InputParameters validParams<CHCoupledCalphadSplit>()
{
  InputParameters params = validParams<SplitCHCRes>();

  //params.addRequiredCoupledVar("coupled_OP_var", "The order parameter coupled to the CH eqn");
  //params.addParam<Real>("scaling_factor", 1, "free energy scaling factor for nondimensionalization");

  params.addRequiredCoupledVar("n", "structural order parameter");

  return params;
}

CHCoupledCalphadSplit::CHCoupledCalphadSplit(const InputParameters & parameters) :
    SplitCHCRes(parameters),
    _W(getMaterialProperty<Real>("well_height")),
    _Omega(getMaterialProperty<Real>("molar_volume")),
    _dGalpha_dc(getMaterialProperty<Real>("dGAB1CD1_dc")),
    _d2Galpha_dc2(getMaterialProperty<Real>("d2GAB1CD1_dc2")),
    _dGdelta_dc(getMaterialProperty<Real>("dGAB1CD2_dc")),
    _d2Gdelta_dc2(getMaterialProperty<Real>("d2GAB1CD2_dc2")),
//    _scaling_factor(getParam<Real>("scaling_factor")),
    _n_var(coupled("n")),
    _w_var(coupled("w")),
    _n(coupledValue("n")),
    _w(coupledValue("w"))
{
}

Real
CHCoupledCalphadSplit::computeDFDC(PFFunctionType type)
{
  Real Heaviside;

  Heaviside = computeHeaviside();

  switch (type)
  {
  case Residual:
    return ( (1 - Heaviside)*_dGalpha_dc[_qp] + Heaviside*_dGdelta_dc[_qp] )/_Omega[_qp];

  case Jacobian:
    return _phi[_j][_qp]*((1 - Heaviside)*_d2Galpha_dc2[_qp] + Heaviside*_d2Gdelta_dc2[_qp] )/_Omega[_qp];

  }
  mooseError("invalid type passed in");
}

Real
CHCoupledCalphadSplit::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _n_var)
  {
    return _phi[_j][_qp]*_test[_i][_qp]*( computeDHeaviside()*_dGdelta_dc[_qp]
                                          - computeDHeaviside()*_dGalpha_dc[_qp] )/_Omega[_qp];
  }

 if (jvar == _w_var)
    return -_phi[_j][_qp] * _test[_i][_qp];

  else
    mooseError("Screwed up CHCoupledCalphadSplit::computeQpOffDiagJacobian()");
}

Real
CHCoupledCalphadSplit::computeHeaviside()
{
   Real OP = _n[_qp];

   return 6*OP*OP*OP*OP*OP - 15*OP*OP*OP*OP + 10*OP*OP*OP;
}

Real
CHCoupledCalphadSplit::computeDHeaviside()
{
   Real OP = _n[_qp];

   return 30*OP*OP*OP*OP - 60*OP*OP*OP + 30*OP*OP;
}
