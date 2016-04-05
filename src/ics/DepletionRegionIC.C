/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  3 June 2015
*
*************************************************************************/

#include "DepletionRegionIC.h"

#include "libmesh/libmesh.h"
#include <iostream>

template<>
InputParameters validParams<DepletionRegionIC>()
{
  InputParameters params = validParams<EllipsoidIC>();

  params.addRequiredParam<Real>("depletion_width", "The width of the depletion region");
  params.addRequiredParam<Real>("depletion_outvalue", "The variable value in the depletion region");

  return params;
}

DepletionRegionIC::DepletionRegionIC(const InputParameters &  parameters)
    : EllipsoidIC(parameters),
      _depletion_width(getParam<Real>("depletion_width")),
      _depletion_outvalue(getParam<Real>("depletion_outvalue"))
{
  // check to make sure the input file is set up correctly
  if ( _n_seeds != static_cast<int>(_x_positions.size()) ||
       _n_seeds != static_cast<int>(_y_positions.size()) ||
       _n_seeds != static_cast<int>(_z_positions.size()) )
    mooseError("Please match the number of seeds to the size of the position vectors (DepletionRegionIC).");

  // check to make sure we have 3 coefficients for ellipsoid
  if (_coefficients.size() != 3)
    mooseError("Please input 3 coefficients for ellipsoid (DepletionRegionIC).");

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
DepletionRegionIC::value(const Point & p)
{
  Real return_val = _outvalue;
  Real test_val = 0.0;

  for (int j=0; j<_n_seeds; j++)
  {
    test_val = shapeValue(p, j);

    return_val = test_val;
  }

  return return_val;
}

Real
DepletionRegionIC::shapeValue(const Point &p, const int j)
{

  Real value = 0;
  Real sum = 0;

  // calculate if within the dimensions of the specified ellipsoid at this point
  for(unsigned int i=0; i<LIBMESH_DIM; i++)
    sum += ( (p(i)-_centers[j](i)) * (p(i)-_centers[j](i)) )/(_coefficients[i]*_coefficients[i]);

  if (sum <= 1 - _int_width/2)
  {
    value = _invalue;
  }

  else if (sum < 1 + _int_width/2)
  {
    // no division by zero because we are in the finite interface width case
    //no idea if this is right
    Real int_pos = (sum - 1 + _int_width/2)/_int_width;

    value = _depletion_outvalue + (_invalue - _depletion_outvalue)*(1 + std::cos(int_pos*libMesh::pi))/2;

  }
  //depletion region
  else if (sum < 1 + _int_width + _depletion_width)
  {

    value = _depletion_outvalue;
  //smooth the interface of the depletion region
  }

  else if (sum < 1 + _int_width + _depletion_width + _int_width)
  {
    Real int_pos = (sum -1 + _depletion_width + _int_width/2)/_int_width
      ;
    value = _depletion_outvalue + (_outvalue - _depletion_outvalue)*(1+std::sin(int_pos*libMesh::pi))/2;

  }

  else
  {
    value = _outvalue;
  }

  return value;
}
