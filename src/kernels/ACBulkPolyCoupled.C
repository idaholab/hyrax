/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  9 March 2012
*
*  Happy Birthday, AMJ
*
*************************************************************************/

#include "ACBulkPolyCoupled.h"

/**
 * ACBulkPolyCoupled couples the bulk Allen-Cahn equation term with order parameter eta
 * to the conserved field variable term from the Cahn-Hilliard equation.  It also couples
 * with multiple order parameters.  It uses the PFMobilityLandau materials class.
 */

template<>
InputParameters validParams<ACBulkPolyCoupled>()
{
  InputParameters params = validParams<ACBulkCoupled>();
  params.addRequiredParam<int>("n_OP_vars", "# of coupled OP variables");
  params.addRequiredCoupledVar("OP_var_names", "Array of coupled OP variable names");
  params.addRequiredParam<int>("OP_number","# of the order parameter for this kernel, starting from 1");

  return params;
}

ACBulkPolyCoupled::ACBulkPolyCoupled(const InputParameters & parameters)
    : ACBulkCoupled(parameters),
      _n_OP_vars(getParam<int>("n_OP_vars")),
      _OP_number(getParam<int>("OP_number")),
      _a5(getMaterialProperty<Real>("A5")),
      _a6(getMaterialProperty<Real>("A6")),
      _a7(getMaterialProperty<Real>("A7"))
{
  // Create a vector of the coupled OP variables and set = 0 the one that the kernel
  // is operating on
  if(_n_OP_vars != coupledComponents("OP_var_names"))
    mooseError("Please match the number of orientation variants to coupled OPs (ACBulkPolyCoupled).");

  _coupled_OP_vars.resize(_n_OP_vars);

  for(unsigned int i=0; i< _n_OP_vars; i++)
  {
    if(i == _OP_number-1)
    {
      _coupled_OP_vars[i] = NULL;
    }
    else
    {
      _coupled_OP_vars[i] = &coupledValue("OP_var_names", i);
    }
  }
}

Real
ACBulkPolyCoupled::computeDFDOP(PFFunctionType type)
{
  Real square_sum, quad_sum, quad_mult;
  square_sum = quad_sum = 0.0;

  if (_n_OP_vars == 1)
    quad_mult = 0.0;
  else
    quad_mult = 1.0;

  // compute the coupled OP terms
  for(unsigned int i=0; i<_n_OP_vars; i++)
  {
    if(i != _OP_number-1)
    {
      square_sum += ((*_coupled_OP_vars[i])[_qp])*((*_coupled_OP_vars[i])[_qp]);
      quad_sum += ((*_coupled_OP_vars[i])[_qp])*((*_coupled_OP_vars[i])[_qp])*((*_coupled_OP_vars[i])[_qp])*((*_coupled_OP_vars[i])[_qp]);
      quad_mult *= ((*_coupled_OP_vars[i])[_qp])*((*_coupled_OP_vars[i])[_qp]);
    }

  }

  switch(type)
  {
  case Residual:

    return _a2[_qp]*(_coupled_CH_var[_qp]- _c2[_qp])*_u[_qp] - _a3[_qp]*_u[_qp]*_u[_qp]*_u[_qp]
      + _a4[_qp]*_u[_qp]*_u[_qp]*_u[_qp]*_u[_qp]*_u[_qp]
      + 2.0*_a5[_qp]*_u[_qp]*square_sum
      + _a6[_qp]*(4.0*_u[_qp]*_u[_qp]*_u[_qp]*square_sum + 2.0*_u[_qp]*quad_sum)
      + 2.0*_a7[_qp]*_u[_qp]*quad_mult;

  case Jacobian:
    return _phi[_j][_qp]*( _a2[_qp]*(_coupled_CH_var[_qp]- _c2[_qp]) - 3.0*_a3[_qp]*_u[_qp]*_u[_qp]
                           + 5.0*_a4[_qp]*_u[_qp]*_u[_qp]*_u[_qp]*_u[_qp]
                           + 2.0*_a5[_qp]*square_sum
                           + _a6[_qp]*(12.0*_u[_qp]*_u[_qp]*square_sum + 2.0*quad_sum)
                           +  2.0*_a7[_qp]*quad_mult) ;
  }
  mooseError("Invalid type passed in");
}
