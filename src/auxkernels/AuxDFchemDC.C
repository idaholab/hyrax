/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  25 April 2015
*
*************************************************************************/

#include "AuxDFchemDC.h"

template<>
InputParameters validParams<AuxDFchemDC>()
{
  InputParameters params = validParams<AuxKernel>();

  // params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");
  params.addRequiredCoupledVar("concentration", "name of concentration variable");
  params.addRequiredCoupledVar("OP", "name of order parameter variable");

  return params;
}

AuxDFchemDC::AuxDFchemDC(const InputParameters & parameters) :
    AuxKernel(parameters),
    _X(coupledValue("concentration")),
    _OP(coupledValue("OP")),
    _Omega(getMaterialProperty<Real>("molar_volume")),
    _dGalpha_dc(getMaterialProperty<Real>("dGAB1CD1_dc")),
    _dGdelta_dc(getMaterialProperty<Real>("dGAB1CD2_dc"))
    //_scaling_factor(getParam<Real>("scaling_factor"))
{
}

Real
AuxDFchemDC::computeValue()
{
  Real H = 3*_OP[_qp]*_OP[_qp] - 2*_OP[_qp]*_OP[_qp]*_OP[_qp];

  return (1 - H)*_dGalpha_dc[_qp] + H*_dGdelta_dc[_qp];

}



