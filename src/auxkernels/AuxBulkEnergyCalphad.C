/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 9 January 2014
*
*************************************************************************/

#include "AuxBulkEnergyCalphad.h"

template<>
InputParameters validParams<AuxBulkEnergyCalphad>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredCoupledVar("conserved_variable", "coupled conserved field variable");
  params.addRequiredCoupledVar("OP_variable_names", "array of coupled OP variable names");
  params.addRequiredParam<unsigned int>("n_OP_variables", "# of coupled OP variables, >=1");
  params.addParam<Real>("scaling_factor", 1, "free energy scaling factor for nondimensionalization");

  return params;
}

AuxBulkEnergyCalphad::AuxBulkEnergyCalphad(const InputParameters & parameters) :
    AuxKernel(parameters),
    _C(coupledValue("conserved_variable")),
    _n_OP_variables(getParam<unsigned int>("n_OP_variables")),
    _scaling_factor(getParam<Real>("scaling_factor")),
    _W(getMaterialProperty<Real>("well_height")),
    _Omega(getMaterialProperty<Real>("molar_volume")),
    _G_alpha(getMaterialProperty<Real>("G_AB1CD1")),
    _G_delta(getMaterialProperty<Real>("G_AB1CD2"))
{
  if(_n_OP_variables != coupledComponents("OP_variable_names"))
    mooseError("Please match the # of orientation variants to coupled OPs (AuxBulkEnergyCalphad)");

  _OP.resize(_n_OP_variables);

  for(unsigned int i=0; i< _n_OP_variables; i++)
    _OP[i] = &coupledValue("OP_variable_names", i);
}

Real
AuxBulkEnergyCalphad::computeValue()
{
  Real h = computeHeaviside();
  Real g = computeBarrier();

  return _scaling_factor*( (1-h)*_G_alpha[_qp] + h*_G_delta[_qp]  + _W[_qp]*g ) / _Omega[_qp];
}

Real
AuxBulkEnergyCalphad::computeHeaviside()
{
  Real heaviside_first(0);
  Real heaviside_second(0);

  //may need to put some checking in here so that OP fixed between 0 and 1
  for(unsigned int i=0; i<_n_OP_variables; i++)
  {
    heaviside_first += std::pow((*_OP[i])[_qp], 2);
    heaviside_second += std::pow((*_OP[i])[_qp], 3);
  }

  return 3*heaviside_first - 2*heaviside_second;
}

Real
AuxBulkEnergyCalphad::computeBarrier()
{
  Real first(0);
  Real second(0);
  Real third(0);
  Real fourth(0);
  Real fifth(0);

  Real sixth;
  if(_n_OP_variables == 1)
    sixth = 0;
  else
    sixth = 1;

  for (unsigned int i=0; i<_n_OP_variables; i++)
  {
    first += std::pow((*_OP[i])[_qp], 2);
    second += std::pow((*_OP[i])[_qp], 3);
    third += std::pow((*_OP[i])[_qp], 4);

    Real square_sum(0);
    Real quad_sum(0);
    for (unsigned int j=0; j < _n_OP_variables; j++)
    {
      if (j > i)
        square_sum += std::pow((*_OP[j])[_qp], 2);

      if (j != i)
        quad_sum += std::pow((*_OP[j])[_qp], 4);
    }

    fourth +=  ( std::pow((*_OP[i])[_qp], 2) )*square_sum;
    fifth += ( std::pow((*_OP[i])[_qp], 2) )*quad_sum;

    sixth *= std::pow((*_OP[i])[_qp], 2);
  }

  return first - 2*second + third + fourth + fifth + sixth;
}
