/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  3 June 2015
*
*************************************************************************/

#ifndef DEPLETIONREGIONIC_H
#define DEPLETIONREGIONIC_H

#include "EllipsoidIC.h"
#include "libmesh/point.h"

// Forward declaration
class DepletionRegionIC;

template<>
InputParameters validParams<DepletionRegionIC>();

class DepletionRegionIC : public EllipsoidIC
{
public:
  DepletionRegionIC(const InputParameters & parameters);

  virtual Real value(const Point &p);

  virtual Real shapeValue(const Point &p, const int j);

protected:

//  int _n_seeds;
//  Real _int_width;

//  Real _invalue;
//  Real _outvalue;

//  std::vector<Real> _x_positions;
//  std::vector<Real> _y_positions;
//  std::vector<Real> _z_positions;

//  std::vector<Real> _coefficients;

//  std::vector<Point> _centers;

private:

  Real _depletion_width;
  Real _depletion_outvalue;

};

#endif //DEPLETIONREGIONIC_H
