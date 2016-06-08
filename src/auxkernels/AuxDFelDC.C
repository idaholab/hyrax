/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  25 May 2015
*
*************************************************************************/

#include "AuxDFelDC.h"

template<>
InputParameters validParams<AuxDFelDC>()
{
  InputParameters params = validParams<AuxKernel>();

  return params;
}

AuxDFelDC::AuxDFelDC(const InputParameters & parameters) :
    AuxKernel(parameters),
    _dfel_dc(getMaterialProperty<Real>("dfel_dX"))
{
}

Real
AuxDFelDC::computeValue()
{

  return _dfel_dc[_qp];

}
