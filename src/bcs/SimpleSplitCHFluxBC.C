/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*
*  CHiMaD/MOOSE
*
*  10 June 2016
*
*************************************************************************/

#include "SimpleSplitCHFluxBC.h"

template<>
InputParameters validParams<SimpleSplitCHFluxBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  //add parameters here
  params.addRequiredParam<RealGradient>("flux", "the flux vector set at the boundary");
  params.addParam<MaterialPropertyName>("mobility_name", "M", "name of the mobility used");

  return params;
}

SimpleSplitCHFluxBC::SimpleSplitCHFluxBC(const InputParameters & parameters) :
    IntegratedBC(parameters),
    _flux(getParam<RealGradient>("flux")),
    _M(getMaterialProperty<Real>("mobility_name"))
{
}

Real
SimpleSplitCHFluxBC::computeQpResidual()
{
  return (_flux - _M[_qp] * _grad_u[_qp])*_normals[_qp]*_test[_i][_qp];
}
Real
SimpleSplitCHFluxBC::computeQpJacobian()
{
  return -(_M[_qp] * _grad_phi[_j][_qp])*_normals[_qp]*_test[_i][_qp];
}
