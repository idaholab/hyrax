/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 9 January 2014
*
*************************************************************************/

#include "AuxBulkEnergyCalphad.h"

registerMooseObject("HyraxApp", AuxBulkEnergyCalphad);

template<>
InputParameters validParams<AuxBulkEnergyCalphad>()
{
  InputParameters params = validParams<AuxKernel>();

  return params;
}

AuxBulkEnergyCalphad::AuxBulkEnergyCalphad(const InputParameters & parameters) :
    AuxKernel(parameters),
    _fbulk(getMaterialProperty<Real>("f_bulk"))
{
}

Real
AuxBulkEnergyCalphad::computeValue()
{
  return _fbulk[_qp];
}
