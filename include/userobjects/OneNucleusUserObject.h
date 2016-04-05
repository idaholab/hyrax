/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  22 February 2013
*
*************************************************************************/

#ifndef ONENUCLEUSUSEROBJECT_H
#define ONENUCLEUSUSEROBJECT_H

#include "NucleationLocationUserObject.h"

//Forward declarations
class OneNucleusUserObject;

template<>
InputParameters validParams<OneNucleusUserObject>();

class OneNucleusUserObject : public NucleationLocationUserObject
{
public:
  OneNucleusUserObject(const InputParameters & parameters);
  virtual ~OneNucleusUserObject() {}

  virtual void execute();

protected:

private:

  Point _nucleus_point;

};

#endif //ONENUCLEUSUSEROBJECT_H
