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

  return params;
}

AuxDFchemDC::AuxDFchemDC(const InputParameters & parameters) :
    AuxKernel(parameters),
    _dfbulk_dc(getMaterialProperty<Real>("df_bulk_dc"))
{
}

Real
AuxDFchemDC::computeValue()
{

  return _dfbulk_dc[_qp];

}
