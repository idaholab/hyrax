/*************************************************************************
*
*   Welcome to HYRAX!
*
*   Developer: Andrea Jokisaari
*
*   5 April 2016
*
*  StressBC is pulled from Ferret, thanks to O. Heinonen et al.
*
****************************************************************/

#include "StressBC.h"

template<>
InputParameters validParams<StressBC>()
{
    InputParameters params = validParams<IntegratedBC>();

    params.addRequiredParam<int>("component","Which component(0 for x, 1 for y, 2 for z) in traction is used");
    params.addRequiredParam<std::vector<Real> >("boundary_stress", "Boundary stress: s11, s22, s33, s23, s13, s12");

    return params;
}

StressBC::StressBC(const InputParameters & parameters) :
    IntegratedBC(parameters),
    _stress_vector(getParam<std::vector<Real> >("boundary_stress")),
    _Jacobian_mult(getMaterialProperty<RankFourTensor>("Jacobian_mult")),
    _component(getParam<int>("component"))
{
  _boundary_stress.fillFromInputVector(_stress_vector);

}

Real
StressBC::computeQpResidual()
{
  return -_test[_i][_qp]*(_boundary_stress.row(_component)*_normals[_qp]);
}

Real
StressBC::computeQpJacobian()
{
  return 0;
}
