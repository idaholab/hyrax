/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  5 December 2014
*
*************************************************************************/

#ifndef DIAMONDIC_H
#define DIAMONDIC_H

#include "InitialCondition.h"
#include "libmesh/point.h"

// Forward declaration
class DiamondIC;

template<>
InputParameters validParams<DiamondIC>();

class DiamondIC : public InitialCondition
{
public:
  DiamondIC(const InputParameters & parameters);

  virtual Real value(const Point &p);

//  virtual Real shapeValue(const Point &p, const int j);

protected:

  Real _invalue;
  Real _outvalue;

  Real _radius;

  Real _int_width;

private:

};

#endif //DIAMONDIC_H
