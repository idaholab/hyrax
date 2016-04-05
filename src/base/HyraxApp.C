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
#include "SolidMechanicsApp.h"
#include "TensorMechanicsApp.h"
#include "PhaseFieldApp.h"
#include "HeatConductionApp.h"
#include "MiscApp.h"

//Kernels
#include "ACBulkCoupled.h"
#include "CHBulkCoupled.h"
#include "ACTransformElasticDF.h"
#include "CHBulkPolyCoupled.h"
#include "ACBulkPolyCoupled.h"
#include "CHCoupledCalphad.h"
#include "ACCoupledCalphad.h"
#include "CHLarry.h"
#include "CHLarrySplit.h"
#include "CHCoupledSplit.h"
#include "CHCoupledCalphadSplit.h"
#include "SplitCoupledCHWRes.h"
#include "CHPrecipMatrixElasticity.h"
#include "ACPrecipMatrixElasticity.h"
//#include "CHCoupledSplitHackathon.h"
//#include "ACBulkPolyCoupledHackathon.h"
//#include "SplitCoupledCHWResHackathon.h"

//Auxiliary Kernels
#include "AuxSupersaturation.h"
#include "AuxChemElastic.h"
#include "AuxDeltaGStar.h"
#include "AuxRateSimple.h"
#include "AuxChem.h"
#include "AuxTemperature.h"
#include "AuxGuoEnergy.h"
#include "AuxCalphadEnergy.h"
//#include "AuxTestFlip.h"
#include "ReporterAux.h"
#include "AuxBulkEnergyCalphad.h"
#include "AuxGradientEnergy.h"
#include "AuxElasticEnergy.h"
#include "AuxCalphadElasticity.h"
#include "AuxElasticInteractionEnergy.h"
#include "AuxAMRNucleationProbability.h"
#include "AuxVolumetricNucleationRate.h"
#include "AuxDFchemDC.h"
#include "AuxDFelDC.h"
#include "AuxCanonicalEnsemble.h"

//Dirac Kernels

//Boundary Conditions
#include "StressBC.h"

//Materials
#include "PFMobilityLandau.h"
#include "LinearSingleCrystalPrecipitateMaterial.h"
#include "ZrHCalphad.h"
#include "CalphadEnergyMaterial.h"
#include "CalphadAB1CD1Material.h"
#include "CalphadAB1CD2Material.h"
#include "PrecipitateMatrixMisfitMaterial.h"
#include "ZrHCalphadDiffusivity.h"
#include "FreeEnergy.h"
#include "MatlAuxVolNucRate.h"
//#include "HackathonFreeEnergy.h"

//Initial Conditions
#include "PolySpecifiedSmoothCircleIC.h"
#include "EllipsoidIC.h"
#include "SmoothBoxIC.h"
#include "DiamondIC.h"
#include "DepletionRegionIC.h"

//Dampers

//Executioners
#include "MeshSolutionModify.h"

//Post Processors
#include "NucleationPostprocessor.h"
#include "OneSeed.h"
#include "NucleiInformation.h"
#include "ElementIntegralFreeEnergy.h"

//TimeSteppers
#include "InitialSolutionAdaptiveDT.h"

//Actions

//UserObjects
#include "NucleationLocationUserObject.h"
#include "NucleusIntroductionSolutionModifier.h"
#include "OneNucleusUserObject.h"

//Markers
#include "NucleationMarker.h"


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
  SolidMechanicsApp::registerObjects(_factory);
  TensorMechanicsApp::registerObjects(_factory);
  HeatConductionApp::registerObjects(_factory);
  MiscApp::registerObjects(_factory);

  // Associate Syntax from SolidMechanics Module
  Moose::associateSyntax(_syntax, _action_factory);
  PhaseFieldApp::associateSyntax(_syntax, _action_factory);
  SolidMechanicsApp::associateSyntax(_syntax, _action_factory);
  TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
  HeatConductionApp::associateSyntax(_syntax, _action_factory);
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
  //Kernels
  registerKernel(CHBulkCoupled);
  registerKernel(ACBulkCoupled);
  registerKernel(ACTransformElasticDF);
  registerKernel(ACBulkPolyCoupled);
  registerKernel(CHBulkPolyCoupled);
  registerKernel(CHCoupledCalphad);
  registerKernel(ACCoupledCalphad);
  registerKernel(CHLarry);
  registerKernel(CHLarrySplit);
  registerKernel(CHCoupledSplit);
  registerKernel(CHCoupledCalphadSplit);
  registerKernel(SplitCoupledCHWRes);
  registerKernel(CHPrecipMatrixElasticity);
  registerKernel(ACPrecipMatrixElasticity);
  //registerKernel(CHCoupledSplitHackathon);
  //registerKernel(ACBulkPolyCoupledHackathon);
  //registerKernel(SplitCoupledCHWResHackathon);

  //Auxiliary Kernels
  registerAux(AuxSupersaturation);
  registerAux(AuxChemElastic);
  registerAux(AuxDeltaGStar);
  registerAux(ReporterAux);
  registerAux(AuxRateSimple);
  registerAux(AuxChem);
  registerAux(AuxTemperature);
  registerAux(AuxGuoEnergy);
  registerAux(AuxCalphadEnergy);
  registerAux(AuxBulkEnergyCalphad);
  registerAux(AuxGradientEnergy);
  registerAux(AuxElasticEnergy);
  registerAux(AuxCalphadElasticity);
  registerAux(AuxElasticInteractionEnergy);
  registerAux(AuxAMRNucleationProbability);
  registerAux(AuxVolumetricNucleationRate);
  registerAux(AuxDFchemDC);
  registerAux(AuxDFelDC);
  registerAux(AuxCanonicalEnsemble);

  //Dirac Kernels

  //Boundary Conditions
  registerBoundaryCondition(StressBC);

  //Materials
  registerMaterial(PFMobilityLandau);
  registerMaterial(LinearSingleCrystalPrecipitateMaterial);
  registerMaterial(ZrHCalphad);
  registerMaterial(CalphadEnergyMaterial);
  registerMaterial(CalphadAB1CD1Material);
  registerMaterial(CalphadAB1CD2Material);
  registerMaterial(PrecipitateMatrixMisfitMaterial);
  registerMaterial(ZrHCalphadDiffusivity);
  registerMaterial(FreeEnergy);
  registerMaterial(MatlAuxVolNucRate);
  //registerMaterial(HackathonFreeEnergy);

  //Initial Conditions
  registerInitialCondition(PolySpecifiedSmoothCircleIC);
  registerInitialCondition(EllipsoidIC);
  registerInitialCondition(SmoothBoxIC);
  registerInitialCondition(DiamondIC);
  registerInitialCondition(DepletionRegionIC);

  //Dampers

  //Executioners
  registerExecutioner(MeshSolutionModify);

  //Postprocessors
  registerPostprocessor(NucleationPostprocessor);
  registerPostprocessor(OneSeed);
  registerPostprocessor(NucleiInformation);
  registerPostprocessor(ElementIntegralFreeEnergy);

  //TimeSteppers
  registerTimeStepper(InitialSolutionAdaptiveDT);

  // UserObjects
  registerUserObject(NucleationLocationUserObject);
  registerUserObject(NucleusIntroductionSolutionModifier);
  registerUserObject(OneNucleusUserObject);

  // Markers
  registerMarker(NucleationMarker);

}

void
HyraxApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  // Actions
}
