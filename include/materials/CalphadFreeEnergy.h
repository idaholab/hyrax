#ifndef CALPHADFREEENERGY_H
#define CALPHADFREEENERGY_H

//#include "CalphadParameters.h"
#include "libmesh/libmesh.h"
#include <vector>

using libMesh::Real;

class CalphadFreeEnergy
{
public:
  CalphadFreeEnergy();

  //virtual void parameterize(CalphadParameters & params) = 0;

  virtual Real calculateReference(const Real & c, const Real & T) const = 0;
  virtual Real calculateIdeal(const Real & c, const Real & T) const = 0;
  virtual Real calculateExcess(const Real & c, const Real & T) const = 0;
  virtual Real calculateFirstLatticeGminusHser(const Real & c, const Real & T) const;
  virtual Real calculateSecondLatticeGminusHser(const Real & c, const Real & T) const = 0;

  virtual Real computeGMix(const Real & c, const Real & T) const;

  virtual Real computeDGMixDc(const Real & c, const Real & T) const = 0;

  virtual Real computeD2GMixDc2(const Real & c, const Real & T) const = 0;


  //vectors to hold the coeffients of the 2 endpoints for mixing
  std::vector<Real> _pure_endpoint_1_coeffs;
  std::vector<Real> _pure_endpoint_2_coeffs;
  std::vector<Real> _mixture_coeffs;

  //vectors to hold the Rudlich-Kister polynomial coefficents
  std::vector<Real> _L0_coeffs;
  std::vector<Real> _L1_coeffs;

  //low and high taylor expansion cutoff concentrations
  Real _low_cutoff;
  Real _high_cutoff;

  //Gas constant
  Real _R;

protected:
private:
};

#endif //CALPHADFREEENERGY_H
