/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  16 November 2011
*
*************************************************************************/

#include "ACBulkCoupled.h"

/**
 * ACBulkCoupled couples the bulk Alan-Cahn equation term with order parameter eta to the
 * conserved field variable term from the Cahn-Hilliard equation (typically concentration).
 * It uses the PFMobilityLandau materials class.
 */

template<>
InputParameters validParams<ACBulkCoupled>()
{
  InputParameters params = ACBulk<Real>::validParams();
  params.addRequiredCoupledVar("coupled_CH_var", "The concentration to be coupled to the AC equation");

  return params;
}

ACBulkCoupled::ACBulkCoupled(const InputParameters & parameters):
    ACBulk<Real>(parameters),
    // Get the material properties for the Landau coefficients
    _a2(getMaterialProperty<Real>("A2")),
    _a3(getMaterialProperty<Real>("A3")),
    _a4(getMaterialProperty<Real>("A4")),
    _c2(getMaterialProperty<Real>("C2")),
    // Make the coupled value whatever is directed in the input file
    _coupled_CH_var(coupledValue("coupled_CH_var"))
{
}

Real
ACBulkCoupled::computeDFDOP(PFFunctionType type)
{
  switch (type)
  {
  case Residual:
    return _a2[_qp]*(_coupled_CH_var[_qp]- _c2[_qp])*_u[_qp] - _a3[_qp]*_u[_qp]*_u[_qp]*_u[_qp]
           + _a4[_qp]*_u[_qp]*_u[_qp]*_u[_qp]*_u[_qp]*_u[_qp] ;

  case Jacobian:
    return _phi[_j][_qp]*( _a2[_qp]*(_coupled_CH_var[_qp]- _c2[_qp]) - 3.0*_a3[_qp]*_u[_qp]*_u[_qp] +
           5.0*_a4[_qp]*_u[_qp]*_u[_qp]*_u[_qp]*_u[_qp] ) ;
  }

  mooseError("Invalid type passed in");
}
