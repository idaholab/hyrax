/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  26 September 2012
*
*************************************************************************/

#ifndef ONESEED_H
#define ONESEED_H

#include "ChangeVariableData.h"

//forward declarations
class OneSeed;
class GeneratedMesh;

template<>
InputParameters validParams<OneSeed>();

class OneSeed : public ChangeVariableData
{
public:
  OneSeed(const InputParameters & parameters);

  virtual void initialize();

  // other things here

  virtual void modifySolutionVector();

  virtual Real getValue();
protected:


private:

  Real _radius;
  Real _dwell_time;
  Real _seed_value;
  Real _int_width;

  Real _x_position;
  Real _y_position;
  Real _z_position;

  bool _restart;
  Real _restart_time;

  Node _location;

};

#endif //ONESEED_H
