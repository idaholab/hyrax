/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  11 January 2012
*
*************************************************************************/

#include "ACTransformElasticDF.h"

/**
 * ACTransformElasticDF handles the elastic energy term for a solid-solid transformation
 * in a phase field model, with coupled conserved and non-conserved parameters.  The
 * variable this kernel is working with is an order parameter.  It can handle an
 * arbitrary number of order parameters.
 */

template<>
InputParameters validParams<ACTransformElasticDF>()
{
  InputParameters params = ACBulk<Real>::validParams();
  params.addRequiredCoupledVar("OP_var_names", "Array of coupled variable names");
  params.addRequiredParam<int>("n_OP_vars", "# of orientation variants for precips in single crystal");
  params.addRequiredParam<int>("OP_number", "# of the order parameter for this kernel, starting from 1");

  params.addParam<Real>("scaling_factor", 1, "elastic energy scaling factor for nondimensionalization");

  return params;
}

ACTransformElasticDF::ACTransformElasticDF(const InputParameters & parameters) :
    ACBulk<Real>(parameters),
    _elasticity_tensor(getMaterialProperty<ElasticityTensorR4>("elasticity_tensor")),
    _eigenstrains_rotated_MP(getMaterialProperty<std::vector<RankTwoTensor> >("eigenstrains_MP")),
    _local_strain(getMaterialProperty<RankTwoTensor>("local_strain")),
    _elastic_strain(getMaterialProperty<RankTwoTensor>("elastic_strain")),
    _d_eigenstrains_rotated_MP(getMaterialProperty<std::vector<RankTwoTensor > >("d_eigenstrains_MP")),
    _n_OP_vars(getParam<int>("n_OP_vars")),
    _OP_number(getParam<int>("OP_number")),
    _scaling_factor(getParam<Real>("scaling_factor"))
{
  // Create a vector of the coupled variables and set = 0 the one that the kernel
  // is operating on
  if(_n_OP_vars != coupledComponents("OP_var_names"))
    mooseError("Please match the number of orientation variants to coupled OPs (ACTransform).");

  _coupled_vars.resize(_n_OP_vars);

  for(unsigned int i=0; i< _n_OP_vars; i++)
  {
    if(i == _OP_number-1)
      _coupled_vars[i] = NULL;
    else
      _coupled_vars[i] = &coupledValue("OP_var_names", i);
  }
}

Real
ACTransformElasticDF::computeDFDOP(PFFunctionType type)
{
  RankTwoTensor a = _elasticity_tensor[_qp]*(_d_eigenstrains_rotated_MP[_qp])[_OP_number-1]*(-1);
  RankTwoTensor b = _elasticity_tensor[_qp]*(_elastic_strain[_qp]);

  Real first = 0.5*a.doubleContraction(_elastic_strain[_qp]);
  Real second = 0.5*b.doubleContraction( (_d_eigenstrains_rotated_MP[_qp])[_OP_number-1]*(-1) );

  switch(type)
  {
  case Residual:
    return _scaling_factor*(first + second);

  case Jacobian:
    return 0;
  }

  mooseError("invalid type passed in");
}

/*

// Follow mostly what is in ACGrGrElasticDF:

  Real first_term(0.0), second_term(0.0);
  //Real misfit_term;

  switch (type)
  {
  case Residual:
    // first term contains the local stiffness derivative
    first_term = calculateFirstTerm();

    // second term contains the e_ij derivative = e_kl derivative
    second_term = calculateSecondTerm();

    return _scaling_factor*( first_term + 2.0*second_term);

  case Jacobian:
    return _scaling_factor*( 0.0 );
  }
  mooseError("Invalid type passed in");
}

Real
ACTransformElasticDF::calculateFirstTerm()
{
  return 0.0;
}

Real
ACTransformElasticDF::calculateSecondTerm()
{
  RankTwoTensor a;

  a = _elasticity_tensor[_qp]*(_d_eigenstrains_rotated_MP[_qp])[_OP_number-1]*(-1.0);

  // elasticity_tensor*d_eigenstrains_rotated[OP_number]*elastic_strain
  return 0.5*a.doubleContraction(_elastic_strain[_qp]);
}

Real
ACTransformElasticDF::calculateFirstJacobianTerm()
{
  return 0.0;
}

Real
ACTransformElasticDF::calculateSecondJacobianTerm()
{
  return 0.0;
}
  */
