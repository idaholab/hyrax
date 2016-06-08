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

  params.addRequiredCoupledVar("n", "structural order parameter");

  return params;
}

CHCoupledCalphadSplit::CHCoupledCalphadSplit(const InputParameters & parameters) :
    SplitCHCRes(parameters),
    _W(getMaterialProperty<Real>("well_height")),
    _Omega(getMaterialProperty<Real>("molar_volume")),
    _dfbulk_dc(getMaterialProperty<Real>("df_bulk_dc")),
    _d2fbulk_dc2(getMaterialProperty<Real>("d2f_bulk_dc2")),
    _d2fbulk_dcdn(getMaterialProperty<Real>("d2f_bulk_dcdOP")),
//    _dGalpha_dc(getMaterialProperty<Real>("dGAB1CD1_dc")),
//    _d2Galpha_dc2(getMaterialProperty<Real>("d2GAB1CD1_dc2")),
//    _dGdelta_dc(getMaterialProperty<Real>("dGAB1CD2_dc")),
//    _d2Gdelta_dc2(getMaterialProperty<Real>("d2GAB1CD2_dc2")),
//    _h(getMaterialProperty<Real>("interpolation_function")),
//    _dhdn(getMaterialProperty<Real>("dinterpolation_dOP")),

    _n_var(coupled("n")),
    _w_var(coupled("w")),
    _n(coupledValue("n")),
    _w(coupledValue("w"))
{
}

Real
CHCoupledCalphadSplit::computeDFDC(PFFunctionType type)
{

  switch (type)
  {
  case Residual:
    // return ( (1 - _h[_qp])*_dGalpha_dc[_qp] + _h[_qp]*_dGdelta_dc[_qp] )/_Omega[_qp];
    return _dfbulk_dc[_qp];

  case Jacobian:
    //return _phi[_j][_qp]*((1 - _h[_qp])*_d2Galpha_dc2[_qp] + _h[_qp]*_d2Gdelta_dc2[_qp] )/_Omega[_qp];
    return _phi[_j][_qp]*_d2fbulk_dc2[_qp];

  }
  mooseError("invalid type passed in");
}

Real
CHCoupledCalphadSplit::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _n_var)
  {
    return _phi[_j][_qp]*_test[_i][_qp]*_d2fbulk_dcdn[_qp];
  }

 if (jvar == _w_var)
    return -_phi[_j][_qp] * _test[_i][_qp];

 else
   return 0;

 //   mooseError("Screwed up CHCoupledCalphadSplit::computeQpOffDiagJacobian()");
}
