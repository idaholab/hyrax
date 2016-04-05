/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  7 November 2013
*
*************************************************************************/

#ifndef CALPHADAB1CD1MATERIAL_H
#define CALPHADAB1CD1MATERIAL_H

#include "CalphadEnergyMaterial.h"
#include "CalphadAB1CD1.h"

class CalphadAB1CD1Material;

template<>
InputParameters validParams<CalphadAB1CD1Material>();

class CalphadAB1CD1Material : public CalphadEnergyMaterial
{
public:
  CalphadAB1CD1Material(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  CalphadAB1CD1 _energy;

  Real _low_cutoff;
  Real _high_cutoff;
  Real _precip_conc;

  MaterialProperty<Real> & _G_AB1CD1;
  MaterialProperty<Real> & _dG_dc;
  MaterialProperty<Real> & _d2G_dc2;
  //MaterialProperty<Real> & _d3G_dc3;
  MaterialProperty<Real> & _d2G_dcdT;

  MaterialProperty<Real> & _G_AB1CD1_precip;

private:
};

#endif //CALPHADAB1CD1MATERIAL_H
