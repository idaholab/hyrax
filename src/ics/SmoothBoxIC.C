#include "SmoothBoxIC.h"
#include "libmesh/libmesh.h"
#include <iostream>
#include <cmath>

template<>
InputParameters validParams<SmoothBoxIC>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addParam<unsigned int>("problem_dimension", 2, "the problem dimension");
  params.addRequiredParam<Real>("length", "length of the side of the cube");

  return params;
}


SmoothBoxIC::SmoothBoxIC(const InputParameters & parameters):
    InitialCondition(parameters),
    _prob_dim(getParam<unsigned int>("problem_dimension")),
    _length(getParam<Real>("length"))
{
}

Real
SmoothBoxIC::value(const Point & p)
{
  Real value = 1.0;
  Real test;

  for(unsigned int i = 0; i < _prob_dim; i++)
  {
    test = 0.5*(std::tanh( p(i) + _length/2) - std::tanh( p(i) - _length/2) );

    if( value > test)
      value = test;
  }

  return value;
}
