/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  30 May 2012
*
*************************************************************************/


#include "Moose.h"
#include "HyraxApp.h"
#include "Factory.h"
#include "AppFactory.h"
#include "ActionFactory.h"
#include "MooseSyntax.h"

//Module Includes
//#include "SolidMechanicsApp.h"
#include "TensorMechanicsApp.h"
#include "PhaseFieldApp.h"
//#include "HeatConductionApp.h"
#include "MiscApp.h"

template<>
InputParameters validParams<HyraxApp>()
{
  InputParameters params = validParams<MooseApp>();
  params.set<bool>("use_legacy_output_syntax") = false;
  return params;
}

HyraxApp::HyraxApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  HyraxApp::registerObjects(_factory);

  // Register Modules
  PhaseFieldApp::registerObjects(_factory);
  //SolidMechanicsApp::registerObjects(_factory);
  TensorMechanicsApp::registerObjects(_factory);
  //HeatConductionApp::registerObjects(_factory);
  MiscApp::registerObjects(_factory);

  // Associate Syntax from SolidMechanics Module
  Moose::associateSyntax(_syntax, _action_factory);
  PhaseFieldApp::associateSyntax(_syntax, _action_factory);
  //SolidMechanicsApp::associateSyntax(_syntax, _action_factory);
  TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
  //HeatConductionApp::associateSyntax(_syntax, _action_factory);
  MiscApp::associateSyntax(_syntax, _action_factory);

  //Associate syntax for Hyrax Actions
  HyraxApp::associateSyntax(_syntax, _action_factory);
}

extern "C" void HyraxApp__registerApps() { HyraxApp::registerApps(); }
void
HyraxApp::registerApps()
{
  registerApp(HyraxApp);
}

void
HyraxApp::registerObjects(Factory & factory)
{
  Registry::registerObjectsTo(factory, {"HyraxApp"});
}

void
HyraxApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  // Actions
}
