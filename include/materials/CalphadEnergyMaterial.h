/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  1 November 2013
*
*************************************************************************/

#ifndef CALPHADENERGYMATERIAL_H
#define CALPHADENERGYMATERIAL_H

#include "Material.h"

//forward declaration
class CalphadEnergyMaterial;

template<>
InputParameters validParams<CalphadEnergyMaterial>();

class CalphadEnergyMaterial : public Material
{
public:
  CalphadEnergyMaterial(const InputParameters & parameters);

protected:

  virtual void computeQpProperties();

  /*virtual Real calculateReference(Real c);
  virtual Real calculateIdeal(Real c);
  virtual Real calculateExcess(Real c);
  virtual Real calculateFirstLatticeGminusHser();
  virtual Real calculateSecondLatticeGminusHser();

  virtual Real computeGMix(Real c);
  virtual Real computeDGMixDc(Real c);
  virtual Real computeD2GMixDc2();
  virtual Real computeD3GMixDc3();*/

  //vectors to hold the coefficients of the 2 endpoints for mixing
  std::vector<Real> _pure_endpoint_low_coeffs;
  std::vector<Real> _pure_endpoint_high_coeffs;
  std::vector<Real> _mixture_coeffs;

  //vectors to hold the Rudlich-Kister polynomial coefficents
  std::vector<Real> _L0_coeffs;
  std::vector<Real> _L1_coeffs;

  Real _R;                                      //Universal gas constant

  //COUPLED VARIABLES
  const VariableValue & _T;                           //coupled Temperature field
  const VariableValue & _c;                           //coupled concentration field

private:

  // Inherited classes will put their energies as materials properties
  // here and declare with unique names
  // Also need to add their particular energy object, for example:
  // CalphadEnergy _energy;

};

#endif //CALPHADENERGY_H
