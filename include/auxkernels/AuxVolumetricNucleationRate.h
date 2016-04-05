/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 13 January 2015
*
*************************************************************************/

#ifndef AUXVOLUMETRICNUCLEATIONRATE_H
#define AUXVOLUMETRICNUCLEATIONRATE_H

#include "AuxKernel.h"

class AuxVolumetricNucleationRate;

/**
 *AuxVolumetricNucleationRate calculates the nucleation rate given the full
 *list of variables (see Balluffi, Chapter 19).  It returns a volumetric nucleation rate!
 *It also has scaling factors for time and length, so make sure you comprehend
 *the dimensionality of what you are doing/calculating.  It assumes a spherical
 *nucleus shape
 */

template<>
InputParameters validParams<AuxVolumetricNucleationRate>();

class AuxVolumetricNucleationRate : public AuxKernel
{
public:
  AuxVolumetricNucleationRate(const InputParameters & parameters);

protected:

  virtual Real computeValue();

  virtual void computeCriticalRadius();
  virtual void computeCriticalEnergy();

  virtual void computeZeldovichFactor();
  virtual void computeCriticalFrequency();
  virtual void computeNumAtoms();

  const VariableValue & _coupled_energy;          // J/m^3 free energy change of transformation

  Real _Z;                                  // Zeldovich non-equilibrium factor
  Real _N;                                  // Number of atoms in the computational volume
  Real _beta_star;                          // frequency factor (critical -> supercritical nucleus)
  Real _linear_density;                     // linear atomic density of material

  Real _r_star;                             // Critical radius (2 or 3D)
  Real _G_star;                             // critical activation energy (J)


  Real _gamma;
  Real _Kb;
  Real _time_scale_factor;

  const VariableValue & _T;
  const VariableValue & _X;

  const MaterialProperty<Real> & _D;

  Real _jump_distance;

  const MaterialProperty<Real> & _Omega;
  Real _length_scale_factor;

private:
  Real _rate_volume;

};

#endif //AUXVOLUMETRICNUCLEATIONRATE_H
