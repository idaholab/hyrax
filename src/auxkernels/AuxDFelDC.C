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

  // params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");


  return params;
}

AuxDFelDC::AuxDFelDC(const InputParameters & parameters) :
    AuxKernel(parameters),
    _elasticity_tensor(getMaterialProperty<ElasticityTensorR4>("elasticity_tensor")),
    _elastic_strain(getMaterialProperty<RankTwoTensor>("elastic_strain")),
    _dc_misfit_strain(getMaterialProperty<RankTwoTensor>("dc_misfit_strain"))//,
    //_scaling_factor(getParam<Real>("scaling_factor"))
{
}

Real
AuxDFelDC::computeValue()
{
//  RankTwoTensor stress = _elasticity_tensor[_qp]*_elastic_strain[_qp];

//  return 0.5*stress.doubleContraction(_elastic_strain[_qp]);



  RankTwoTensor a = _elasticity_tensor[_qp]*(_elastic_strain[_qp]);
  RankTwoTensor b = _elasticity_tensor[_qp]*_dc_misfit_strain[_qp]*(-1);
  //in this case, dc_elasticity_tensor = 0

  Real first  = a.doubleContraction( (_dc_misfit_strain[_qp])*(-1) );
  Real second = b.doubleContraction( _elastic_strain[_qp] );

  return 0.5*(first + second);

}



