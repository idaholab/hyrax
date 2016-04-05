/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  1 September 2015
*
*************************************************************************/

#ifndef MATLAUXVOLNUCRATE_H
#define MATLAUXVOLNUCRATE_H

#include "Material.h"

//forward declaration
class MatlAuxVolNucRate;

/**
 *This material provides material information required to test the auxkernel
 *AuxVolumetricNucleationRate.
*/

template<>
 InputParameters validParams<MatlAuxVolNucRate>();

 class MatlAuxVolNucRate : public Material
 {
 public:
   MatlAuxVolNucRate(const InputParameters & parameters);

 protected:
   virtual void computeQpProperties();

 private:

  Real _d_alpha_input;
  Real _molar_vol_input;
  MaterialProperty<Real> & _D_alpha;
  MaterialProperty<Real> & _molar_volume;

};

#endif //MATLAUXVOLNUCRATE_H
