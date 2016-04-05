/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  24 December 2013
*
*************************************************************************/

#include "EllipsoidIC.h"

#include "libmesh/libmesh.h"

template<>
InputParameters validParams<EllipsoidIC>()
{
  InputParameters params = validParams<InitialCondition>();

  params.addRequiredParam<int>("n_seeds","number of initial seeds to set up for variable");
  params.addRequiredParam<Real>("int_width", "The seed interface width (0.0 for sharp interface)");

  params.addRequiredParam<Real>("invalue", "The variable value inside the circles");
  params.addRequiredParam<Real>("outvalue", "The variable value outside the circles");

  params.addRequiredParam<std::vector<Real> >("x_positions","The x-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("y_positions","The y-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("z_positions", "The z-coordinate for each circle center");

  params.addRequiredParam<std::vector<Real> >("coefficients", "a, b, and c coefficients for the ellipsoid");

  return params;
}

EllipsoidIC::EllipsoidIC(const InputParameters & parameters)
    : InitialCondition(parameters),
      _n_seeds(getParam<int>("n_seeds")),
      _int_width(getParam<Real>("int_width")),
      _invalue(getParam<Real>("invalue")),
      _outvalue(getParam<Real>("outvalue")),
      //_radius(getParam<Real>("radius")),

      _x_positions(getParam<std::vector<Real> >("x_positions")),
      _y_positions(getParam<std::vector<Real> >("y_positions")),
      _z_positions(getParam<std::vector<Real> >("z_positions")),

      _coefficients(getParam<std::vector<Real> >("coefficients")),
      _centers()
      //_multiple_radii(getParam<bool>("multiple_radii")),
      //_radii(getParam<std::vector<Real> >("radii"))
{
  // check to make sure the input file is set up correctly
  if ( _n_seeds != static_cast<int>(_x_positions.size()) ||
       _n_seeds != static_cast<int>(_y_positions.size()) ||
       _n_seeds != static_cast<int>(_z_positions.size()) )
    mooseError("Please match the number of seeds to the size of the position vectors (EllipsoidIC).");

  // check to make sure we have 3 coefficients for ellipsoid
  if (_coefficients.size() != 3)
    mooseError("Please input 3 coefficients for ellipsoid (EllipsoidIC).");

  //resize the vector of Points
  _centers.resize(_n_seeds);

  // fill in the vector of center points
  for(int i=0; i<_n_seeds; i++)
  {
    _centers[i](0) = _x_positions[i];
    _centers[i](1) = _y_positions[i];
    _centers[i](2) = _z_positions[i];
  }
}

Real
EllipsoidIC::value(const Point & p)
{
  Real return_val = _outvalue;
  Real test_val = 0.0;

  for (int j=0; j<_n_seeds; j++)
  {
    test_val = shapeValue(p, j);

    // test to see if we've already got a seed there - if so, don't wipe it out!
    if (test_val > return_val)
      return_val = test_val;
  }

  return return_val;
}

Real
EllipsoidIC::shapeValue(const Point &p, const int j)
{
  Real value = 0;
  Real sum = 0;

  // calculate if within the dimensions of the specified ellipsoid at this point
  for(unsigned int i=0; i<LIBMESH_DIM; i++)
    sum += ( (p(i)-_centers[j](i)) * (p(i)-_centers[j](i)) )/(_coefficients[i]*_coefficients[i]);

  if (sum <= 1 - _int_width/2)
    value = _invalue;
  else if (sum < 1 + _int_width/2)
  {
    // no division by zero because we are in the finite interface width case
    //no idea if this is right
    Real int_pos = (sum - 1 + _int_width/2)/_int_width;
    value = _outvalue + (_invalue - _outvalue)*(1 + std::cos(int_pos*libMesh::pi))/2;
  }
  else
    value = _outvalue;

  return value;

}
