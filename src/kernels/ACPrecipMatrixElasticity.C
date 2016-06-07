/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  14 February 2014
*
*************************************************************************/

#include "ACPrecipMatrixElasticity.h"

template<>
InputParameters validParams<ACPrecipMatrixElasticity>()
{
  InputParameters params = ACBulk<Real>::validParams();
  params.addRequiredParam<int>("OP_number","# of the order parameter for this kernel, starting from 1");
 params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");

  return params;
}

ACPrecipMatrixElasticity::ACPrecipMatrixElasticity(const InputParameters & parameters) :
    ACBulk<Real>(parameters),
    _elasticity_tensor(getMaterialProperty<RankFourTensor>("elasticity_tensor")),
    _dn_elasticity_tensor(getMaterialProperty<std::vector<RankFourTensor> >("dn_elasticity_tensor")),
    _dndn_elasticity_tensor(getMaterialProperty<std::vector<RankFourTensor> >("dndn_elasticity_tensor")),
    _elastic_strain(getMaterialProperty<RankTwoTensor>("elastic_strain")),
    _dn_misfit_strain(getMaterialProperty<std::vector<RankTwoTensor> >("dn_misfit_strain")),
    _dndn_misfit_strain(getMaterialProperty<std::vector<RankTwoTensor> >("dndn_misfit_strain")),
    _OP_number(getParam<int>("OP_number")),
    _scaling_factor(getParam<Real>("scaling_factor"))
{
}

Real
ACPrecipMatrixElasticity::computeDFDOP(PFFunctionType type)
{
  unsigned int i = _OP_number - 1;

  RankTwoTensor a = _elasticity_tensor[_qp]*(_elastic_strain[_qp]);
  RankTwoTensor b = _elasticity_tensor[_qp]*(( _dn_misfit_strain[_qp])[i]*(-1) );
  RankTwoTensor c = (_dn_elasticity_tensor[_qp])[i]*_elastic_strain[_qp];

  RankTwoTensor Jb =  _elasticity_tensor[_qp]*(( _dndn_misfit_strain[_qp])[i]*(-1) );
  RankTwoTensor Jc = (_dndn_elasticity_tensor[_qp])[i]*( _elastic_strain[_qp]);
  RankTwoTensor Jd = (_dndn_elasticity_tensor[_qp])[i]*(( _dndn_misfit_strain[_qp])[i]*(-1) );

  Real first  = a.doubleContraction( (_dn_misfit_strain[_qp])[i]*(-1) );
  Real second = b.doubleContraction( _elastic_strain[_qp] );
  Real third  = c.doubleContraction( _elastic_strain[_qp] );

  Real Jaa = a.doubleContraction( (_dndn_misfit_strain[_qp])[i]*(-1) );
  Real Jbb = Jb.doubleContraction( _elastic_strain[_qp] );
  Real Jcc = Jc.doubleContraction( _elastic_strain[_qp] );
  Real Jab = b.doubleContraction( (_dn_misfit_strain[_qp])[i]*(-1) );
  Real Jca = c.doubleContraction( (_dn_misfit_strain[_qp])[i]*(-1) );
  Real Jcb = Jd.doubleContraction( _elastic_strain[_qp] );

  switch(type)
  {
  case Residual:
    return _scaling_factor*0.5*(first + second + third);

  case Jacobian:
    return _scaling_factor*0.5*_phi[_j][_qp]*(Jaa + Jbb + Jcc + 2*Jab + 2*Jca + 2*Jcb);
    //return 0;
  }

  mooseError("invalid type passed in");
}
