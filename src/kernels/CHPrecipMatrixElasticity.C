/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  14 February 2014
*
*************************************************************************/

#include "CHPrecipMatrixElasticity.h"

template<>
InputParameters validParams<CHPrecipMatrixElasticity>()
{
  InputParameters params = validParams<SplitCHCRes>();

  params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");

  params.addRequiredParam<int>("n_OP_vars", "# of coupled OP variables");
  params.addRequiredCoupledVar("OP_var_names", "Array of coupled OP variable names");

   params.addRequiredCoupledVar("w","chem poten");
   params.addRequiredCoupledVar("T", "temperature");

  return params;
}

CHPrecipMatrixElasticity::CHPrecipMatrixElasticity(const InputParameters & parameters)
    : SplitCHCRes(parameters),
      _elasticity_tensor(getMaterialProperty<ElasticityTensorR4>("elasticity_tensor")),
      _dn_elasticity_tensor(getMaterialProperty<std::vector<ElasticityTensorR4> >("dn_elasticity_tensor")),
      _elastic_strain(getMaterialProperty<RankTwoTensor>("elastic_strain")),
      _dc_misfit_strain(getMaterialProperty<RankTwoTensor>("dc_misfit_strain")),
      _dn_misfit_strain(getMaterialProperty<std::vector<RankTwoTensor> >("dn_misfit_strain")),
      _dcdn_misfit_strain(getMaterialProperty<std::vector<RankTwoTensor> >("dcdn_misfit_strain")),
      _scaling_factor(getParam<Real>("scaling_factor")),
      _n_OP_vars(getParam<int>("n_OP_vars")),
      _w_var(coupled("w")),
      _T_var(coupled("T"))
{
  if(_n_OP_vars != coupledComponents("OP_var_names"))
    mooseError("Please match the number of orientation variants to coupled OPs (ACCoupledCalphad).");

  _n_var.resize(_n_OP_vars);
  _OP.resize(_n_OP_vars);

  for (unsigned int i=0; i<_n_OP_vars; i++)
  {
    _n_var[i] = coupled("OP_var_names", i);
    _OP[i] = &coupledValue("OP_var_names", i);
  }
}

Real
CHPrecipMatrixElasticity::computeDFDC(PFFunctionType type)
{
  RankTwoTensor a = _elasticity_tensor[_qp]*(_elastic_strain[_qp]);
  RankTwoTensor b = _elasticity_tensor[_qp]*_dc_misfit_strain[_qp]*(-1);
  //in this case, dc_elasticity_tensor = 0

  Real first  = a.doubleContraction( (_dc_misfit_strain[_qp])*(-1) );
  Real second = b.doubleContraction( _elastic_strain[_qp] );

  switch(type)
  {
  case Residual:
    return _scaling_factor*0.5*(first + second);

  case Jacobian:
    return _scaling_factor*_phi[_j][_qp]*b.doubleContraction( (_dc_misfit_strain[_qp])*(-1) );
    //return 0;
  }

  mooseError("invalid type passed in");
}


Real
CHPrecipMatrixElasticity::computeQpOffDiagJacobian(unsigned int jvar)
{
  for (unsigned int i=0; i<_n_OP_vars; i++)
  {
    if (jvar == _n_var[i])
    {
      RankTwoTensor a = _elasticity_tensor[_qp]*_elastic_strain[_qp];
      RankTwoTensor b = _elasticity_tensor[_qp]*(_dn_misfit_strain[_qp])[i]*(-1);
      RankTwoTensor c = (_dn_elasticity_tensor[_qp])[i]*_elastic_strain[_qp];

      Real Jaa = a.doubleContraction( ( (_dcdn_misfit_strain[_qp])[i])*(-1) );
      Real Jab = b.doubleContraction( (_dc_misfit_strain[_qp])*(-1) );
      Real Jac = c.doubleContraction( (_dc_misfit_strain[_qp])*(-1) );

      return _scaling_factor*_phi[_j][_qp]*_test[_i][_qp]*(Jaa + Jab + Jac);
    }
  }

  if (jvar == _w_var)
    return -_phi[_j][_qp] * _test[_i][_qp];

  else if (jvar == _T_var)
    return 0;

  else
    mooseError("Screwed up CHCoupledCalphadSplit::computeQpOffDiagJacobian()");
}
