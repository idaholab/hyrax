#ifndef SMOOTHBOXIC_H
#define SMOOTHBOXIC_H

#include "InitialCondition.h"


// Forward Declarations
class SmoothBoxIC;

template<>
InputParameters validParams<SmoothBoxIC>();

class SmoothBoxIC : public InitialCondition
{
public:

  SmoothBoxIC(const InputParameters & parameters);

  virtual Real value(const Point & p);

private:

  unsigned int _prob_dim;
  Real _length;
};

#endif //SMOOTHBOXIC_H
