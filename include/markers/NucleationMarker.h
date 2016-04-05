/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  6 December 2012
*
*  NucleationMarker is basically a clone of RandomHitMarker in moose_test,
*  thanks go to Derek Gaston.
*
*************************************************************************/

#ifndef NUCLEATIONMARKER_H
#define NUCLEATIONMARKER_H

#include "Marker.h"

//Forward declarations
class NucleationLocationUserObject;
class NucleationMarker;

template<>
InputParameters validParams<NucleationMarker>();

class NucleationMarker : public Marker
{
public:
  NucleationMarker(const InputParameters & parameters);
  virtual  ~NucleationMarker() {}

protected:
  virtual MarkerValue computeElementMarker();

  const NucleationLocationUserObject & _nucleation_userobject;
  unsigned int _max_h_level;

  private:


};


#endif //NUCLEATIONMARKER_H
