/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  22 February 2013
*
*************************************************************************/

#include "OneNucleusUserObject.h"

template<>
InputParameters validParams<OneNucleusUserObject>()
{
  InputParameters params = validParams<NucleationLocationUserObject>();
  params.addRequiredParam<Point>("nucleus_point", "location of the nucleus to introduce");

  return params;
}


OneNucleusUserObject::OneNucleusUserObject(const InputParameters & parameters) :
    NucleationLocationUserObject(parameters),
    _nucleus_point(getParam<Point>("nucleus_point"))
{
}

void
OneNucleusUserObject::execute()
{
  if(_current_elem->contains_point(_nucleus_point))
  {
      Nucleus current_nucleus;
      current_nucleus.setLocation(_nucleus_point);
      current_nucleus.setStartTime(_t);
      current_nucleus.setEndTime(_t+1);

      current_nucleus.setOrientation(0);
      _local_nucleus.push_back(current_nucleus);
  }
}
