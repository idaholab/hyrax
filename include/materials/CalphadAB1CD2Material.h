/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  7 November 2013
*
*************************************************************************/

#ifndef CALPHADAB1CD2MATERIAL_H
#define CALPHADAB1CD2MATERIAL_H

#include "CalphadEnergyMaterial.h"
#include "CalphadAB1CD2.h"

class CalphadAB1CD2Material;

template<>
InputParameters validParams<CalphadAB1CD2Material>();

class CalphadAB1CD2Material : public CalphadEnergyMaterial
{
public:
  CalphadAB1CD2Material(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  /* virtual Real calculateReference(Real c);
  virtual Real calculateIdeal(Real c);
  virtual Real calculateExcess(Real c);

  virtual Real calculateSecondLatticeGminusHser();

  virtual Real computeGMix(Real c);
  virtual Real computeDGMixDc(Real c);
  virtual Real computeD2GMixDc2();
  virtual Real computeD3GMixDc3();*/

  CalphadAB1CD2 _energy;

  Real _low_cutoff;
  Real _high_cutoff;
  Real _precip_conc;

  std::vector<double> _pure_EP1_phase1_coeffs;
  MaterialProperty<Real> & _G_AB1CD2;
  MaterialProperty<Real> & _dG_dc;
  MaterialProperty<Real> & _d2G_dc2;
  //MaterialProperty<Real> & _d3G_dc3;
  MaterialProperty<Real> & _G_AB1CD2_precip;
  MaterialProperty<Real> & _d2G_dc2_precip;
  MaterialProperty<Real> & _d2G_dc2_cutoff;

private:

};

#endif //CALPHADAB1CD2MATERIAL_H
