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

  params.addRequiredParam<int>("OP_number", "the number (starting from 1) of the nonconserved variable");

  // params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");

  return params;
}

AuxElasticInteractionEnergy::AuxElasticInteractionEnergy(const InputParameters & parameters) :
    AuxKernel(parameters),
    _stress(getMaterialProperty<RankTwoTensor>("stress")),
    _precipitate_eigenstrain(getMaterialProperty<std::vector<RankTwoTensor> >("precipitate_eigenstrain")),
    _OP_number(getParam<int>("OP_number"))
    //_scaling_factor(getParam<Real>("scaling_factor"))
{
}

Real
AuxElasticInteractionEnergy::computeValue()
{
  return  -1*_stress[_qp].doubleContraction( (_precipitate_eigenstrain[_qp])[_OP_number-1] );
}
