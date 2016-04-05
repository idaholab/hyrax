/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  5 December 2014
*
*************************************************************************/

#include "DiamondIC.h"
#include <cmath>

//DiamondIC is centered around ( 0, 0, 0)

template<>
InputParameters validParams<DiamondIC>()
{
  InputParameters params = validParams<InitialCondition>();

  params.addRequiredParam<Real>("invalue", "The value inside the diamond");
  params.addRequiredParam<Real>("outvalue", "The value outside the diamond");
  params.addRequiredParam<Real>("radius", "the radius of the diamond");

  params.addParam<Real>("int_width", 0, "width of interface");

  return params;
}

DiamondIC::DiamondIC(const InputParameters & parameters)
    : InitialCondition(parameters),
      _invalue(getParam<Real>("invalue")),
      _outvalue(getParam<Real>("outvalue")),
      _radius(getParam<Real>("radius")),
      _int_width(getParam<Real>("int_width"))
{
}

Real
DiamondIC::value(const Point & p)
{
  Real point_val = std::abs( p(0)) + std::abs( p(1)) + std::abs( p(2));

  if( point_val > _radius + _int_width)
    return _outvalue;
  else if(point_val <= _radius)
    return _invalue;
  else
    return (1-((point_val - _radius)/_int_width))*(_invalue - _outvalue) + _outvalue;

}
