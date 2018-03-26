/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  9 January 2014
*
*************************************************************************/

#include "AuxElasticEnergy.h"

registerMooseObject("HyraxApp", AuxElasticEnergy);

template<>
InputParameters validParams<AuxElasticEnergy>()
{
  InputParameters params = validParams<AuxKernel>();

  return params;
}

AuxElasticEnergy::AuxElasticEnergy(const InputParameters & parameters) :
    AuxKernel(parameters),
    _fel(getMaterialProperty<Real>("f_elastic"))
{
}

Real
AuxElasticEnergy::computeValue()
{

  return _fel[_qp];

}
