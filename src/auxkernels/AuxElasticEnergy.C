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

template<>
InputParameters validParams<AuxElasticEnergy>()
{
  InputParameters params = validParams<AuxKernel>();

  // params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");


  return params;
}

AuxElasticEnergy::AuxElasticEnergy(const InputParameters & parameters) :
    AuxKernel(parameters),
    _elasticity_tensor(getMaterialProperty<ElasticityTensorR4>("elasticity_tensor")),
    _elastic_strain(getMaterialProperty<RankTwoTensor>("elastic_strain"))//,
    //_scaling_factor(getParam<Real>("scaling_factor"))
{
}

Real
AuxElasticEnergy::computeValue()
{
  RankTwoTensor stress = _elasticity_tensor[_qp]*_elastic_strain[_qp];

  return 0.5*stress.doubleContraction(_elastic_strain[_qp]);
}



