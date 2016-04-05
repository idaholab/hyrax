/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*************************************************************************/

#include "AuxSupersaturation.h"

/**
 * AuxSupersaturation handles the supersaturation calculation (typically C - C1) in the concurrent
 * nucleation and growth algorithm first proposed by J.P. Simmons (2000).
 */

template<>
InputParameters validParams<AuxSupersaturation>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled_var","concentration");
  params.addRequiredParam<Real>("c1", "phase boundary solubility limit");
  return params;
}

AuxSupersaturation::AuxSupersaturation(const InputParameters & parameters)
  : AuxKernel(parameters),
  _coupled_conc(coupledValue("coupled_var")),
  _c1(getParam<Real>("c1"))
{
}

Real
AuxSupersaturation::computeValue()
{
  _supersaturation = _coupled_conc[_qp] - _c1;
  if (_supersaturation <= 0.0)
  {
    // fixed to some arbitrary small value but preventing division by zero in j_star calculation
    _supersaturation = 1.0e-10;
  }
 return _supersaturation;
}

