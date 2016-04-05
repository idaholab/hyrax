/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  3 March 2012
*
*************************************************************************/

#include "PolySpecifiedSmoothCircleIC.h"

#include "libmesh/libmesh.h"

/**
 * PolySpecifiedSmoothCircleIC creates n number of circles of a given radius centered over n specified
 * points in the domain.  If int_width > 0, the border of the circle will smoothly transition from
 * invalue to outvalue.  (Basically reworks and extends SmoothCircleIC)
 */

template<>
InputParameters validParams<PolySpecifiedSmoothCircleIC>()
{
  InputParameters params = validParams<InitialCondition>();

  params.addRequiredParam<int>("n_seeds","number of initial seeds to set up for variable");
  params.addRequiredParam<std::vector<Real> >("x_positions","The x-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("y_positions","The y-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("z_positions", "The z-coordinate for each circle center");

  // pulled from SmoothCircleIC, but can't inherit from SmoothCircleIC easily.
  params.addRequiredParam<Real>("invalue", "The variable value inside the circles");
  params.addRequiredParam<Real>("outvalue", "The variable value outside the circles");
  params.addParam<Real>("radius", 0, "The radius of the circles");
  params.addRequiredParam<Real>("int_width", "The seed interface width (0.0 for sharp interface)");

  params.addParam<bool>("multiple_radii", false, "if multiple radii are to be used");
  params.addParam<std::vector<Real> >("radii", "radii for particles with differing radii");

  return params;
}

PolySpecifiedSmoothCircleIC::PolySpecifiedSmoothCircleIC(const InputParameters & parameters)
    : InitialCondition(parameters),
      _n_seeds(getParam<int>("n_seeds")),
      _invalue(getParam<Real>("invalue")),
      _outvalue(getParam<Real>("outvalue")),
      _radius(getParam<Real>("radius")),
      _int_width(getParam<Real>("int_width")),
      _x_positions(getParam<std::vector<Real> >("x_positions")),
      _y_positions(getParam<std::vector<Real> >("y_positions")),
      _z_positions(getParam<std::vector<Real> >("z_positions")),
      _multiple_radii(getParam<bool>("multiple_radii")),
      _radii(getParam<std::vector<Real> >("radii"))
{
  int x_size, y_size, z_size;
  x_size = _x_positions.size();
  y_size = _y_positions.size();
  z_size = _z_positions.size();

  // check to make sure the input file is set up correctly
  if((_n_seeds != x_size)||(_n_seeds != y_size)||(_n_seeds != z_size))
    mooseError("Please match the number of seeds to the size of the position vectors.");

  if(_multiple_radii)
    if(_n_seeds != static_cast<int>(_radii.size()))
      mooseError("Please match the number of seeds to the size of the radii vector.");

  //fill in our radii vector if we don't have multiple radii
  if(!_multiple_radii)
    _radii.resize(_n_seeds, _radius);


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
PolySpecifiedSmoothCircleIC::value(const Point & p)
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

// pulled from SmoothCircleIC with minor changes
Real
PolySpecifiedSmoothCircleIC::shapeValue(const Point &p, const int j)
{
  Real value = 0.0;
  Real rad = 0.0;

  // calculate if within the radius of the specified circle at this point
  for(unsigned int i=0; i<LIBMESH_DIM; i++)
    rad += (p(i)-_centers[j](i)) * (p(i)-_centers[j](i));

  rad = sqrt(rad);

  // set the value depending on if in seed, in interface, or outside of seed
  if (rad <= _radii[j] - _int_width/2.0)
    value = _invalue;
  else if (rad < _radii[j] + _int_width/2.0)
  {
      // no division by zero because we are in the finite interface width case
    Real int_pos = (rad - _radii[j] + _int_width/2.0)/_int_width;
    value = _outvalue + (_invalue - _outvalue)*(1 + std::cos(int_pos*libMesh::pi))/2;
//    _seed_value*(1.0 - std::cos(-interface_position + libMesh::pi/2.0));
  }
    else
      value = _outvalue;

  return value;
}


// from SmoothCircleIC - don't know if I need this or not.
//
// RealGradient
// SmoothCircleIC::gradient(const Point & p)
// {
//   Real DvalueDr = 0.0;

//   Real rad = 0.0;

//   for(unsigned int i=0; i<LIBMESH_DIM; i++)
//     rad += (p(i)-_center(i)) * (p(i)-_center(i));

//   rad = sqrt(rad);

//   if (rad < _radius + _int_width/2.0 && rad > _radius - _int_width/2.0)
//   {
//     Real int_pos = (rad - _radius + _int_width/2.0)/_int_width;
//     Real Dint_posDr = 1.0/_int_width;
//     DvalueDr = Dint_posDr*(_invalue-_outvalue)*(-sin(int_pos*3.14159)*3.14159)/2.0;
//   }

//   if (rad != 0.0)
//     return Gradient((p(0) - _center(0))*DvalueDr/rad,
//                     (p(1) - _center(1))*DvalueDr/rad,
//                     (p(2) - _center(2))*DvalueDr/rad);
//   else
//     return Gradient(0.0,0.0,0.0);

// }
