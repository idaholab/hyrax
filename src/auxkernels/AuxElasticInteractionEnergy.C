/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  5 December 2014
*
*************************************************************************/

#include "AuxElasticInteractionEnergy.h"

template<>
InputParameters validParams<AuxElasticInteractionEnergy>()
{
  InputParameters params = validParams<AuxKernel>();

  return params;
}

AuxElasticInteractionEnergy::AuxElasticInteractionEnergy(const InputParameters & parameters) :
    AuxKernel(parameters),
    _stress(getMaterialProperty<RankTwoTensor>("stress")),
    _precipitate_misfit_tensor(getMaterialProperty<RankTwoTensor>("precipitate_misfit_tensor"))
{
}

Real
AuxElasticInteractionEnergy::computeValue()
{
  return  -1*_stress[_qp].doubleContraction(_precipitate_misfit_tensor[_qp]);
}
