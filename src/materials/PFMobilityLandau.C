/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  16 November 2011
*
*************************************************************************/

#include "PFMobilityLandau.h"

// libMesh includes
#include "libmesh/quadrature.h"

/**
 * PFMobilityLandau holds the materials information for a phase field model using a Landau polynomial for
 * the bulk (chemical) free energy term.
 */

template<>
InputParameters validParams<PFMobilityLandau>()
{
  InputParameters params = validParams<Material>();
  // Add the names that go on LHS of input file; e.g., mob_CH = 1.0
  params.addRequiredParam<Real>("mob_CH", "The mobility value for the Cahn-Hilliard equation");
  params.addRequiredParam<Real>("mob_AC", "The mobility value for the Alan-Cahn equation");
  params.addRequiredParam<Real>("kappa_CH", "The gradient energy coefficient for the Cahn-Hilliard equation");
  params.addRequiredParam<Real>("kappa_AC", "The gradient energy coefficient for the Alan-Cahn equation");
  params.addRequiredParam<Real>("A1", "First Landau coefficient");
  params.addRequiredParam<Real>("A2", "Second Landau coefficient");
  params.addRequiredParam<Real>("A3", "Third Landau coefficient");
  params.addRequiredParam<Real>("A4", "Fourth Landau coefficient");
  params.addParam<Real>("A5", 1.0, "Fifth Landau coefficient");
  params.addParam<Real>("A6", 1.0, "Sixth Landau coefficient");
  params.addParam<Real>("A7", 1.0, "Seventh Landau coefficient");
  params.addParam<Real>("C1", 1.0, "First Landau well");
  params.addParam<Real>("C2", 1.0, "Second Landau well");

  return params;
}

PFMobilityLandau::PFMobilityLandau(const InputParameters & parameters)
  :Material(parameters),

   // Get the values from the input file into our temporary variable, e.g., reads from mob_CH = 1.0
   _mob_CH(getParam<Real>("mob_CH")),
   _kappa_CH(getParam<Real>("kappa_CH")),
   _mob_AC(getParam<Real>("mob_AC")),
   _kappa_AC(getParam<Real>("kappa_AC")),
   _a1_i(getParam<Real>("A1")),
   _a2_i(getParam<Real>("A2")),
   _a3_i(getParam<Real>("A3")),
   _a4_i(getParam<Real>("A4")),
   _a5_i(getParam<Real>("A5")),
   _a6_i(getParam<Real>("A6")),
   _a7_i(getParam<Real>("A7")),
   _c1_i(getParam<Real>("C1")),
   _c2_i(getParam<Real>("C2")),

    // Declare the fact that these properties will exist for kernel use, with the name "M", etc
   _M(declareProperty<Real>("M")),
   _grad_M(declareProperty<RealGradient>("grad_M")),
   _kappa_c(declareProperty<Real>("kappa_c")),
   _L(declareProperty<Real>("L")),
   _kappa_n(declareProperty<Real>("kappa_n")),
   _a1(declareProperty<Real>("A1")),
   _a2(declareProperty<Real>("A2")),
   _a3(declareProperty<Real>("A3")),
   _a4(declareProperty<Real>("A4")),
   _a5(declareProperty<Real>("A5")),
   _a6(declareProperty<Real>("A6")),
   _a7(declareProperty<Real>("A7")),
   _c1(declareProperty<Real>("C1")),
   _c2(declareProperty<Real>("C2"))

{
}

void
PFMobilityLandau::computeProperties()
{
  for(unsigned int qp=0; qp<_qrule->n_points(); qp++)
  {
    _M[qp] = _mob_CH;
    _grad_M[qp] = 0.0;
    _kappa_c[qp] = _kappa_CH;
    _L[qp]  = _mob_AC;
    _kappa_n[qp] = _kappa_AC;
    _a1[qp] = _a1_i;
    _a2[qp] = _a2_i;
    _a3[qp] = _a3_i;
    _a4[qp] = _a4_i;
    _a5[qp] = _a5_i;
    _a6[qp] = _a6_i;
    _a7[qp] = _a7_i;
    _c1[qp] = _c1_i;
    _c2[qp] = _c2_i;
  }
}
