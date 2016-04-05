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

template<>
InputParameters validParams<AuxVolumetricNucleationRate>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<Real>("rate_volume", "volume for which the nucleation rate is calculated, ensure correct dimensionality");
  params.addRequiredCoupledVar("coupled_bulk_energy_change","coupled auxiliary variable for free energy change");

  params.addRequiredParam<Real>("gamma", "Surface energy");
  params.addParam<Real>("Kb", 1.3806503e-23, "Boltzmann's constant, make sure units all match");
  params.addParam<Real>("time_scale_factor", 1, "time factor to scale (multiply) nucleation rate by");

  params.addRequiredParam<Real>("linear_density", "linear density of hopping sites matrix");

  params.addRequiredCoupledVar("T", "temperature variable");
  params.addRequiredCoupledVar("X", "atomic fraction of solute variable");

  params.addRequiredParam<Real>("jump_distance", "atomic distance for hopping");

  params.addParam<Real>("length_scale_factor", "characteristic length the simulation is scaled by");

  return params;
}

AuxVolumetricNucleationRate::AuxVolumetricNucleationRate(const InputParameters & parameters)
    : AuxKernel(parameters),
      _coupled_energy(coupledValue("coupled_bulk_energy_change")),
      _Z(),
      _beta_star(),
      _linear_density(getParam<Real>("linear_density")),
      _r_star(0),
      _G_star(0),
      _gamma(getParam<Real>("gamma")),
      _Kb(getParam<Real>("Kb")),
      _time_scale_factor(getParam<Real>("time_scale_factor")),
      _T(coupledValue("T")),
      _X(coupledValue("X")),
      _D(getMaterialProperty<Real>("D_alpha")),
      _jump_distance(getParam<Real>("jump_distance")),
      _Omega(getMaterialProperty<Real>("molar_volume")),
      _length_scale_factor(getParam<Real>("length_scale_factor")),
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

  /*
  _console<<"time_scale_factor = "<<_time_scale_factor<<std::endl;
  _console<<"Z = "<<_Z<<std::endl;
  _console<<"N = "<<_N<<std::endl;
  _console<<"beta_star = "<<_beta_star<<std::endl;
  _console<<"G_star = "<<_G_star<<std::endl;
  _console<<"Kb = "<<_Kb<<std::endl;
  _console<<"T = "<<_T[_qp]<<std::endl;
  _console<<"exp(-Gstar/kT) = "<<std::exp( (-1*_G_star)/ (_Kb*_T[_qp]) )<<std::endl;
  _console<<"Jstar unscaled = "<< _Z*_N*_beta_star*std::exp( (-1*_G_star)/ (_Kb*_T[_qp]) )<<std::endl;
  */

  Real total_scale_factor = _time_scale_factor*std::pow(_length_scale_factor, 3);

  Real rate = total_scale_factor*( _Z*_N*_beta_star*std::exp( (-1*_G_star)/ (_Kb*_T[_qp]) ));

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
  _r_star = 2*_gamma/_coupled_energy[_qp];

  //_console<<"r* = "<<_r_star<<std::endl;
  //_console<<"delta f = "<<_coupled_energy[_qp]<<std::endl;
}

void
AuxVolumetricNucleationRate::computeCriticalEnergy()
{
  //this is calculated as if in 3D
  Real alpha = 16*libMesh::pi/3;

  _G_star = alpha*std::pow(_gamma, 3)/std::pow(_coupled_energy[_qp], 2.0);

  //_console<<"G* in Joules = "<<_G_star<<std::endl;
}

void
AuxVolumetricNucleationRate::computeZeldovichFactor()
{
  //assuming spherical nucleus shape
  //Real critical_volume = 4*libMesh::pi*_r_star*_r_star*_r_star/3;

  //here I'm assuming that the molar volume is constant across phases.. meh
  //Real Nc = critical_volume*6.02214E23/_Omega[_qp];

  //_Z =std::sqrt( _G_star/( 3*libMesh::pi*Nc*Nc*_Kb*_T[_qp] ));

  //following Robson's 2004 formulation for Z
  _Z = (_Omega[_qp]/6.02214E23)*std::pow(_coupled_energy[_qp], 2.0)/(8*libMesh::pi*std::sqrt(_gamma*_gamma*_gamma*_Kb*_T[_qp]));

  _console<<"Z = "<<_Z<<std::endl;
}

void
AuxVolumetricNucleationRate::computeCriticalFrequency()
{
  //Real Zc = 4*libMesh::pi*_r_star*_r_star*_linear_density*_linear_density;

  /*
  _console<<"Zc = "<<Zc<<std::endl;
  _console<<"X = "<<_X[_qp]<<std::endl;
  _console<<"D = "<<_D[_qp]<<std::endl;
  _console<<"jump_dist^2 = "<< ( std::pow(_jump_distance,2))<<std::endl;
  */


  //see Robson 2004 paper
  _beta_star = (16*libMesh::pi*_X[_qp]*_D[_qp])/(std::pow(_coupled_energy[_qp], 2.0)*std::pow(_jump_distance, 4.0) );

  // _beta_star = Zc*_X[_qp]*_D[_qp]/ ( std::pow(_jump_distance,2)) ;

  _console<<"beta* = "<<_beta_star<<std::endl;
}

void
AuxVolumetricNucleationRate::computeNumAtoms()
{
 // molar volume divided by number of atoms per mol
 //Real atomic_volume = _Omega[_qp]/6.02214E23;

 //the rate volume is the volume in real dimensions that the rate is calculated for
// _N = _rate_volume/atomic_volume;

  //N is the number of hydrogen-filled tetrahedral sites for 1 volume (probably m^3)
  //
  //see Robson (2004) paper
  //this is hard-coded as: H at fraction* density of Zr in g/cm^3 converted to g/m^3 * Avogadro's # * #tetrahedral sites/zr atom in hcp (2) / atomic mass of Zr
  _N = _X[_qp]*6.52*100*100*100*6.02214E23*2/91.224;
  _console<<"N = "<<_N<<std::endl;

}
