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

#include "NucleationMarker.h"
#include "NucleationLocationUserObject.h"
#include <ostream>

template<>
InputParameters validParams<NucleationMarker>()
{
  InputParameters params = validParams<Marker>();
    params.addRequiredParam<UserObjectName>("nucleation_userobject", "The name of the UserObject that calculates nucleation event positions");
    params.addRequiredParam<unsigned int>("max_h_level", "Maximum h-adapt level for the mesh");

  return params;
}

NucleationMarker::NucleationMarker(const InputParameters & parameters) :
    Marker(parameters),
    _nucleation_userobject(getUserObject<NucleationLocationUserObject>("nucleation_userobject")),
    _max_h_level(getParam<unsigned int>("max_h_level"))
{
}

Marker::MarkerValue
NucleationMarker::computeElementMarker()
{
  if(_current_elem->level() < _max_h_level)
  {
    if(_nucleation_userobject.elementWasHit(_current_elem))
      return REFINE;
  }

  return DONT_MARK;
}
