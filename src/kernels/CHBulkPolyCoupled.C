/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  14 March 2012
*
*************************************************************************/

#include "CHBulkPolyCoupled.h"

/** CHBulkPolyCoupled handles the conserved order parameter(probably concentration),
 * evolved using the Cahn-Hilliard equation.  It couples to multiple order
 * parameters from multiple Alan-Cahn equations.
 */

template<>
InputParameters validParams<CHBulkPolyCoupled>()
{
  InputParameters params = CHBulk<Real>::validParams();
  params.addRequiredParam<int>("n_OP_variables", "# of coupled OP variables, >= 1");
  params.addRequiredCoupledVar("OP_variable_names", "Array of coupled OP variable names");

  return params;
}

CHBulkPolyCoupled::CHBulkPolyCoupled(const InputParameters & parameters)
    :CHBulk<Real>(parameters),
     _a1(getMaterialProperty<Real>("A1")),
     _a2(getMaterialProperty<Real>("A2")),
     _c1(getMaterialProperty<Real>("C1")),
     _n_OP_variables(getParam<int>("n_OP_variables"))
{
  // Create a vector of the coupled OP variables and gradients
  if(_n_OP_variables != coupledComponents("OP_variable_names"))
    mooseError("Please match the number of orientation variants to coupled OPs (CHBulkPolyCoupled).");

  _coupled_OP_variables.resize(_n_OP_variables);
  _coupled_OP_grads.resize(_n_OP_variables);

  for(unsigned int i=0; i< _n_OP_variables; i++)
  {
    _coupled_OP_variables[i] = &coupledValue("OP_variable_names", i);
    _coupled_OP_grads[i] = &coupledGradient("OP_variable_names", i);
  }
}

RealGradient
CHBulkPolyCoupled::computeGradDFDCons(PFFunctionType type)
{
  RealGradient op_sum;
  op_sum.zero();

  switch (type)
  {
  case Residual:
    for(unsigned int i=0; i<_n_OP_variables; i++)
    {
      op_sum += (*_coupled_OP_variables[i])[_qp]*(*_coupled_OP_grads[i])[_qp];
    }

    return _a1[_qp]*(_grad_u[_qp]) + _a2[_qp]*(op_sum);
    //return _a1[_qp]*(grad_c) + _a2[_qp]*(_coupled_var[_qp]*_coupled_grad[_qp]) ;

  case Jacobian:
    return _a1[_qp]*_grad_phi[_j][_qp];
  }

  mooseError("Invalid type passed in");
}
