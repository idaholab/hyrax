/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*
* 20 March 2015
*
*************************************************************************/

#include "AuxVolumetricNucleationRate.h"

#include <cmath>
#include <iostream>

registerMooseObject("HyraxApp", AuxVolumetricNucleationRate);

template<>
InputParameters validParams<AuxVolumetricNucleationRate>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<Real>("rate_volume", "volume for which the nucleation rate is calculated, ensure correct dimensionality");
  params.addRequiredCoupledVar("coupled_bulk_energy_change","coupled auxiliary variable for free energy change");

  params.addRequiredParam<Real>("gamma", "Surface energy");
  params.addParam<Real>("Kb", 1.3806503e-5, "Boltzmann's constant, make sure units all match, default aJ/K");

  params.addRequiredCoupledVar("T", "temperature variable");
  params.addRequiredCoupledVar("X", "atomic fraction of solute variable");

  params.addRequiredParam<Real>("jump_distance", "atomic distance for hopping");

  return params;
}

AuxVolumetricNucleationRate::AuxVolumetricNucleationRate(const InputParameters & parameters)
    : AuxKernel(parameters),
      _coupled_energy(coupledValue("coupled_bulk_energy_change")),
      _Z(),
      _beta_star(),
      _r_star(0),
      _G_star(0),
      _gamma(getParam<Real>("gamma")),
      _Kb(getParam<Real>("Kb")),
      _T(coupledValue("T")),
      _X(coupledValue("X")),
      _D(getMaterialProperty<Real>("D_alpha")),
      _jump_distance(getParam<Real>("jump_distance")),
      _Omega(getMaterialProperty<Real>("molar_volume")),
      _rate_volume(getParam<Real>("rate_volume"))
{
}

Real
AuxVolumetricNucleationRate::computeValue()
{

  computeCriticalRadius();
  computeCriticalEnergy();
  computeZeldovichFactor();
  computeCriticalFrequency();
  computeNumAtoms();

  //_console<<"Z = "<<_Z<<std::endl;
  //_console<<"N = "<<_N<<std::endl;
  //_console<<"beta_star = "<<_beta_star<<std::endl;
  //_console<<"G_star = "<<_G_star<<std::endl;
  //_console<<"Kb = "<<_Kb<<std::endl;
  //_console<<"T = "<<_T[_qp]<<std::endl;
  //_console<<"exp(-Gstar/kT) = "<<std::exp( (-1*_G_star)/ (_Kb*_T[_qp]) )<<std::endl;
  //_console<<"Jstar unscaled = "<< _Z*_N*_beta_star*std::exp( (-1*_G_star)/ (_Kb*_T[_qp]) )<<std::endl;

  Real rate = ( _Z*_N*_beta_star*std::exp( (-1*_G_star)/ (_Kb*_T[_qp]) ));

  if (rate < 0)
    return 0;
  else
    return rate;

}

void
AuxVolumetricNucleationRate::computeCriticalRadius()
{
  //this is calculated as if in 3D
  //assuming spherical nucleus shape
  //unit: nm
  _r_star = 2*_gamma/_coupled_energy[_qp];
}

void
AuxVolumetricNucleationRate::computeCriticalEnergy()
{
  //this is calculated as if in 3D
  //unit: aJ

  Real alpha = 16*libMesh::pi/3;

  _G_star = alpha*_gamma*_gamma*_gamma/(_coupled_energy[_qp]*_coupled_energy[_qp]);
}

void
AuxVolumetricNucleationRate::computeZeldovichFactor()
{
  //assuming spherical nucleus shape

  //following Robson's 2004 formulationor Z
  _Z = (_Omega[_qp]*(1.0e18/6.02214E23)*(_coupled_energy[_qp]*_coupled_energy[_qp]))/(8*libMesh::pi*std::sqrt(_gamma*_gamma*_gamma*_Kb*_T[_qp]));
}

void
AuxVolumetricNucleationRate::computeCriticalFrequency()
{
  //unit: 1/microsecond
  //see Robson 2004 paper
  _beta_star = 16*libMesh::pi*_X[_qp]*_gamma*_gamma*_D[_qp];
  _beta_star /= _coupled_energy[_qp]*_coupled_energy[_qp]*_jump_distance*_jump_distance*_jump_distance*_jump_distance;

  //this is from Baluffi, Kinetics of Materials
  // _beta_star = Zc*_X[_qp]*_D[_qp]/ ( std::pow(_jump_distance,2)) ;
}

void
AuxVolumetricNucleationRate::computeNumAtoms()
{
  //N is the number of hydrogen-filled tetrahedral sites for 1 volume (nm^3)
  //see Robson (2004) paper
  //this is hard-coded as: H at fraction* density of Zr in g/cm^3 converted to g/nm^3 * Avogadro's # * #tetrahedral sites/zr atom in hcp (2) / atomic mass of Zr

  _N = _X[_qp]*6.52*(1E-21)*6.02214E23*2/91.224;
}
