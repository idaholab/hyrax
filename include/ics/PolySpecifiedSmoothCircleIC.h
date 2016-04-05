/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  3 March 2012
*
*************************************************************************/

#ifndef POLYSPECIFIEDSMOOTHCIRCLEIC_H
#define POLYSPECIFIEDSMOOTHCIRCLEIC_H

#include "InitialCondition.h"
#include "libmesh/point.h"

/**
 * PolySpecifiedSmoothCircleIC creates n number of circles of a given radius centered over n specified points
 * in the domain.  If int_width > 0, the border of the circle will smoothly transition from invalue to outvalue.
 */

// Forward declaration
class PolySpecifiedSmoothCircleIC;

template<>
InputParameters validParams<PolySpecifiedSmoothCircleIC>();

class PolySpecifiedSmoothCircleIC : public InitialCondition
{
public:
  PolySpecifiedSmoothCircleIC(const InputParameters & parameters);

  virtual Real value(const Point &p);

  virtual Real shapeValue(const Point &p, const int j);

  // don't know if I need this.
  //virtual RealGradient gradient(const Point &p);

protected:

  int _n_seeds;
  Real _invalue;
  Real _outvalue;
  Real _radius;
  Real _int_width;

  std::vector<Real> _x_positions;
  std::vector<Real> _y_positions;
  std::vector<Real> _z_positions;

  std::vector<Point> _centers;

  bool _multiple_radii;
  std::vector<Real> _radii;

private:

};

#endif //POLYSPECIFIEDSMOOTHCIRCLEIC_H
