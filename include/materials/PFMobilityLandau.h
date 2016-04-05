/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  16 November 2011
*
*************************************************************************/

#ifndef PFMOBILITYLANDAU_H
#define PFMOBILITYLANDAU_H

#include "Material.h"

//Forward Declarations
class PFMobilityLandau;

template<>
InputParameters validParams<PFMobilityLandau>();

/**
 * PFMobilityLandau holds the materials information for a phase field model using a Landau polynomial for
 * the bulk (chemical) free energy term.
 */

class PFMobilityLandau : public Material
{
public:
  PFMobilityLandau(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:

  // Variables for getting values from input file
  // I have to see if this can be cleaned up a bit.
  Real _mob_CH;
  Real _kappa_CH;

  Real _mob_AC;
  Real _kappa_AC;

  Real _a1_i;
  Real _a2_i;
  Real _a3_i;
  Real _a4_i;
  Real _a5_i;
  Real _a6_i;
  Real _a7_i;

  Real _c1_i;
  Real _c2_i;

  // Cahn-Hilliard equation
  MaterialProperty<Real> & _M;                  ///< Cahn-Hilliard mobility (isotropic)
  MaterialProperty<RealGradient> & _grad_M;
  MaterialProperty<Real> & _kappa_c;            ///< CH gradient energy coefficient (isotropic)

  // Alan-Cahn equation
  MaterialProperty<Real> & _L;                  ///< Allen-Cahn kinetic coefficient (isotropic)
  MaterialProperty<Real> & _kappa_n;            ///< AC gradient energy coefficient (isotropic)

  // Landau polynomial coefficients
  MaterialProperty<Real> & _a1;                 ///< Landau polynomial coefficients
  MaterialProperty<Real> & _a2;
  MaterialProperty<Real> & _a3;
  MaterialProperty<Real> & _a4;
  MaterialProperty<Real> & _a5;
  MaterialProperty<Real> & _a6;
  MaterialProperty<Real> & _a7;

  // Landau polynomial parameters
  MaterialProperty<Real> & _c1;  ///< Landau polynomial: 1st well position-ish - terminal solid solubility
  MaterialProperty<Real> & _c2;  ///< Landau polynomial: 2nd well position-ish - concentration in 2nd phase

};

#endif //PFMOBILITYLANDAU_H
