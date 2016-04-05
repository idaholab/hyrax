/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  30 July 2013
*
*************************************************************************/

#ifndef NUCLEIINFORMATION_H
#define NUCLEIINFORMATION_H

#include "GeneralPostprocessor.h"
#include "NucleationLocationUserObject.h"

//forward declaration
class NucleiInformation;
class NucleationLocationUserObject;

template<>
InputParameters validParams<NucleiInformation>();

class NucleiInformation : public GeneralPostprocessor
{
public:
  NucleiInformation(const InputParameters & parameters);

  virtual void initialize();
  virtual void execute();
  virtual Real getValue();
  virtual void threadJoin(const UserObject & y);

protected:

private:
  //some stuff here - i need to get the user object

  const NucleationLocationUserObject & _nucleation_userobject;
  int _OP_number;
  int _number;
};

#endif //NUCLEIINFORMATION_H
