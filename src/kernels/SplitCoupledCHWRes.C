/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  9 December 2013
*
*************************************************************************/

#include "SplitCoupledCHWRes.h"

template<>
InputParameters validParams<SplitCoupledCHWRes>()
{
  InputParameters params = validParams<SplitCHWRes>();

    params.addRequiredCoupledVar("c", "intermediate parameter--concentration");
    // params.addRequiredCoupledVar("n", "structural order parameter");
    params.addRequiredCoupledVar("T", "temperature");

    params.addParam<MaterialPropertyName>("mob_name","mobtemp","The mobility used with the kernel");

    params.addRequiredParam<int>("n_OP_vars", "# of coupled OP variables");
    params.addRequiredCoupledVar("OP_var_names", "Array of coupled OP variable names");

  return params;
}

SplitCoupledCHWRes::SplitCoupledCHWRes(const InputParameters & parameters) :
    SplitCHWRes(parameters),
    _mob(getMaterialProperty<Real>("mob_name")),

    // This _c_var is needed to compute off-diagonal Jacobian.
    _c_var(coupled("c")),
    //_n_var(coupled("n")),
    _T_var(coupled("T")),

    _c(coupledValue("c")),
    //_n(coupledValue("n")),
    _T(coupledValue("T")),
    _n_OP_vars(getParam<int>("n_OP_vars"))
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
SplitCoupledCHWRes::computeQpResidual()
{
  return  _mob[_qp]*_grad_u[_qp] * _grad_test[_i][_qp];
}

Real
SplitCoupledCHWRes::computeQpJacobian()
{
  return _mob[_qp]*_grad_phi[_j][_qp] * _grad_test[_i][_qp];
}

Real
SplitCoupledCHWRes::computeQpOffDiagJacobian(unsigned int jvar)
{
  for (unsigned int i=0; i<_n_OP_vars; i++)
  {
    if (jvar == _n_var[i])
      return 0;
  }

  if(jvar == _c_var)
    return 0.0;
  //else if(jvar == _n_var)
  //return 0.0;
  else if (jvar == _T_var)
    return 0.0;
  else
    mooseError("Screwed up SplitCoupledCHWRes::computeQpOffDiagonalJacobian.");
}
