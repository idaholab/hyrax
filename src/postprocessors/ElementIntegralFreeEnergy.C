/****************************************************************/
/****************************************************************/

#include "ElementIntegralFreeEnergy.h"

template<>
InputParameters validParams<ElementIntegralFreeEnergy>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  params.addRequiredParam<std::string>("phase1_energy", "The name of the material property of phase 1 energy");
  params.addRequiredParam<std::string>("phase2_energy", "The name of the material property of phase 2 energy");

  params.addRequiredParam<std::string>("grad_OP_matprop", "The name of the order parameter material property");
  params.addRequiredParam<std::string>("grad_C_matprop", "the name of the concentration material property");

  params.addRequiredParam<std::string>("Heaviside_matprop", "the name of the Heaviside material property");

  return params;
}

ElementIntegralFreeEnergy::ElementIntegralFreeEnergy(const InputParameters & parameters) :
    ElementIntegralPostprocessor(parameters),
    _phase1_energy(getMaterialProperty<Real>(getParam<std::string>("phase1_energy"))),
    _phase2_energy(getMaterialProperty<Real>(getParam<std::string>("phase2_energy"))),
    _grad_OP_term(getMaterialProperty<Real>(getParam<std::string>("grad_OP_matprop"))),
    _grad_C_term(getMaterialProperty<Real>(getParam<std::string>("grad_C_matprop"))),
    _Heaviside(getMaterialProperty<Real>(getParam<std::string>("Heaviside_matprop")))
{
  _console<<"in ElementIntegralConstructor"<<std::endl;
}

Real
ElementIntegralFreeEnergy::computeQpIntegral()
{
//  Real OP = _OP[_qp];
//  Real Heaviside = 3*OP*OP - 2*OP*OP*OP;

  Real energy(0);
  Real fchem(0);
  // Real grad_OP(0);
  // Real grad_C(0);

  fchem = (1-_Heaviside[_qp])*_phase1_energy[_qp] + _Heaviside[_qp]*_phase2_energy[_qp];

  energy = fchem + _grad_OP_term[_qp] + _grad_C_term[_qp];

//  return _phase1_energy[_qp];
//  return _OP[_qp] + _phase1_energy[_qp];
  return energy;
//  return 0;
}
