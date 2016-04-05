/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  16 November 2011
*
*************************************************************************/

#include "CHBulkCoupled.h"

/** CHBulkCoupled handles the conserved order parameter(probably concentration),
 * evolved using the Cahn-Hilliard equation.  It couples to an order
 * parameter from the Alan-Cahn equation.
 */

template<>
InputParameters validParams<CHBulkCoupled>()
{
  InputParameters params = CHBulk<Real>::validParams();
  params.addRequiredCoupledVar("coupled_OP_var", "The order parameter coupled to the CH eqn");

  return params;
}

CHBulkCoupled::CHBulkCoupled(const InputParameters & parameters)
  :CHBulk<Real>(parameters),
   // Get the material properties for the Landau coefficients
   _a1(getMaterialProperty<Real>("A1")),
   _a2(getMaterialProperty<Real>("A2")),
   _c1(getMaterialProperty<Real>("C1")),
   // Make the coupled value whatever is directed in the input file
   _coupled_OP_var(coupledValue("coupled_OP_var")),
   _coupled_OP_grad(coupledGradient("coupled_OP_var"))
{
}

RealGradient
CHBulkCoupled::computeGradDFDCons(PFFunctionType type)
{
  switch (type)
  {
  case Residual:
    return _a1[_qp]*(_grad_u[_qp]) + _a2[_qp]*(_coupled_OP_var[_qp]*_coupled_OP_grad[_qp]) ;

  case Jacobian:
    return _a1[_qp]*_grad_phi[_j][_qp] ;
  }

  mooseError("Invalid type passed in");
}
