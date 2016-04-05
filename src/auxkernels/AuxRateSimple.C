/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  18 April 2013
*
*************************************************************************/

#include "AuxRateSimple.h"
#include "MooseMesh.h"

#include <cmath>
#include <ostream>

/**
 * AuxRateSimple is designed to work with AuxSupersaturation (following
 * algorithm in J. P. Simmons (2000) paper).  It results in a volumetric
 * nucleation rate (does not take into account mesh volume).
 * */

template<>
InputParameters validParams<AuxRateSimple>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<Real>("Kn1", "First nucleation rate coefficient");
  params.addRequiredParam<Real>("Kn2", "Second nucleation rate coefficient");
  params.addRequiredCoupledVar("coupled_aux_var","coupled auxiliary variable for free energy change");
  //supersaturation, or chem_elastic, etc

  return params;
}

AuxRateSimple::AuxRateSimple(const InputParameters & parameters)
  : AuxKernel(parameters),
    _mesh_dimension(_mesh.dimension()),
    _coupled_energy(coupledValue("coupled_aux_var")),
    _Kn1(getParam<Real>("Kn1")),
    _Kn2(getParam<Real>("Kn2"))
{
}

Real
AuxRateSimple::computeValue()
{
  return _Kn1*std::exp(-1.0*_Kn2/_coupled_energy[_qp]);
}

