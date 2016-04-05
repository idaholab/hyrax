/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  30 July 2013
*
*************************************************************************/

#include "NucleiInformation.h"
#include "NucleationLocationUserObject.h"

template<>
InputParameters validParams<NucleiInformation>()
{
  InputParameters params = validParams<GeneralPostprocessor>();

  params.addRequiredParam<UserObjectName>("nucleation_userobject", "The name of the UserObject to use for nucleation event location");
  params.addRequiredParam<int>("OP_number", "the order parameter variable for which to count # nuclei (starting from 1)");

  MultiMooseEnum execute_options(SetupInterface::getExecuteOptions());
  execute_options = "timestep";
  params.set<MultiMooseEnum>("execute_on") = execute_options;

 return params;
}

NucleiInformation::NucleiInformation(const InputParameters & parameters) :
    GeneralPostprocessor(parameters),
    _nucleation_userobject(getUserObject<NucleationLocationUserObject>("nucleation_userobject")),
    _OP_number(getParam<int>("OP_number")),
    _number(0)
{
}

void
NucleiInformation::initialize()
{
  // reinitialize counter to zero
  _number = 0;
}


void
NucleiInformation::execute()
{
  const std::vector<Nucleus> & nuclei = _nucleation_userobject.getNuclei();

  int orientation;

  for(unsigned int i = 0; i < nuclei.size(); ++i)
  {
    orientation = nuclei[i].getOrientation();

    if(orientation == _OP_number - 1)
      _number++;
  }
}

Real
NucleiInformation::getValue()
{
  return (Real) _number;
  //this is going to return cucmulative number of nuclei for whichever orientation variant
}

void
NucleiInformation::threadJoin(const UserObject & /*y*/)
{
}
