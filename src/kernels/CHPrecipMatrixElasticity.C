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

  // params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");

  //params.addRequiredCoupledVar("OP_var", "coupled OP variable");

  //params.addRequiredCoupledVar("w","chem poten");
  //params.addRequiredCoupledVar("T", "temperature");

  params.addRequiredParam<bool>("use_elasticity", "boolean if using elasticity or not");


  return params;
}

CHPrecipMatrixElasticity::CHPrecipMatrixElasticity(const InputParameters & parameters)
    : SplitCHCRes(parameters),
      _dfbulk_dc(getMaterialProperty<Real>("df_bulk_dc")),
      _d2fbulk_dc2(getMaterialProperty<Real>("d2f_bulk_dc2")),
      _d2fbulk_dcdOP(getMaterialProperty<Real>("d2f_bulk_dcdOP")),
      _use_elasticity(getParam<bool>("use_elasticity")),
      _dfel_dX(_use_elasticity ? getMaterialProperty<Real>("dfel_dX") : getMaterialProperty<Real>("df_bulk_dc")),
      _d2fel_dX2(_use_elasticity ? getMaterialProperty<Real>("dfel_dX") : getMaterialProperty<Real>("df_bulk_dc"))
      // _elasticity_tensor(getMaterialProperty<ElasticityTensorR4>("elasticity_tensor")),
      //_dn_elasticity_tensor(getMaterialProperty<std::vector<ElasticityTensorR4> >("dn_elasticity_tensor")),
      //_elastic_strain(getMaterialProperty<RankTwoTensor>("elastic_strain")),
      //_dc_misfit_strain(getMaterialProperty<RankTwoTensor>("dc_misfit_strain")),
      //_dn_misfit_strain(getMaterialProperty<std::vector<RankTwoTensor> >("dn_misfit_strain")),
      //_dcdn_misfit_strain(getMaterialProperty<std::vector<RankTwoTensor> >("dcdn_misfit_strain")),
      //_scaling_factor(getParam<Real>("scaling_factor")),
      //_w_var(coupled("w")),
      //_T_var(coupled("T")),
      //_n_var(coupled("OP_var"))
{


/*
  if(_n_OP_vars != coupledComponents("OP_var_names"))
    mooseError("Please match the number of orientation variants to coupled OPs (ACCoupledCalphad).");

  _n_var.resize(_n_OP_vars);
  _OP.resize(_n_OP_vars);

  for (unsigned int i=0; i<_n_OP_vars; i++)
  {
    _n_var[i] = coupled("OP_var_names", i);
    _OP[i] = &coupledValue("OP_var_names", i);
  }
*/
}

Real
CHPrecipMatrixElasticity::computeDFDC(PFFunctionType type)
{
  switch (type)
  {
    case Residual:
      return _dfbulk_dc[_qp];

    case Jacobian:
      return _phi[_j][_qp]*(_d2fbulk_dc2[_qp]);

  }

  mooseError("invalid type passed in");

}

Real
CHPrecipMatrixElasticity::computeDEDC(PFFunctionType type)
{
  /*
  RankTwoTensor a = _elasticity_tensor[_qp]*(_elastic_strain[_qp]);
  RankTwoTensor b = _elasticity_tensor[_qp]*_dc_misfit_strain[_qp]*(-1);
  //in this case, dc_elasticity_tensor = 0

  Real first  = a.doubleContraction( (_dc_misfit_strain[_qp])*(-1) );
  Real second = b.doubleContraction( _elastic_strain[_qp] );
  */

  if(_use_elasticity)
  {

    switch(type)
    {
      case Residual:
        //return _scaling_factor*0.5*(first + second);
        return _dfel_dX[_qp];

      case Jacobian:
        //return _scaling_factor*_phi[_j][_qp]*b.doubleContraction( (_dc_misfit_strain[_qp])*(-1) );
        return _phi[_j][_qp]*_d2fel_dX2[_qp];
    }

    mooseError("invalid type passed in");
  }
  else
    return 0;

}

//need to do this
/*
Real
CHPrecipMatrixElasticity::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _n_var)
  {
    return _phi[_j][_qp]*_test[_i][_qp]*( _dhdOP[_qp]*(_df)_dGdelta_dc[_qp]
                                          - _dhdOP[_qp]*_dGalpha_dc[_qp] )/_Omega[_qp];
  }

 if (jvar == _w_var)
    return -_phi[_j][_qp] * _test[_i][_qp];

 else
   return 0;

 //   mooseError("Screwed up CHCoupledCalphadSplit::computeQpOffDiagJacobian()");
}
*/

/*Real
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
*/
