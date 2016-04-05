/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  24 December 2013
*
*************************************************************************/

#ifndef ELLIPSOIDIC_H
#define ELLIPSOIDIC_H

#include "InitialCondition.h"
#include "libmesh/point.h"

// Forward declaration
class EllipsoidIC;

template<>
InputParameters validParams<EllipsoidIC>();

class EllipsoidIC : public InitialCondition
{
public:
  EllipsoidIC(const InputParameters & parameters);

  virtual Real value(const Point &p);

  virtual Real shapeValue(const Point &p, const int j);

protected:

  int _n_seeds;
  Real _int_width;

  Real _invalue;
  Real _outvalue;

  std::vector<Real> _x_positions;
  std::vector<Real> _y_positions;
  std::vector<Real> _z_positions;

  std::vector<Real> _coefficients;

  std::vector<Point> _centers;

private:

};

#endif //ELLIPSOIDIC_H
