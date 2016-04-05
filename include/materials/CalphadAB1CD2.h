#ifndef CALPHADAB1CD2_H
#define CALPHADAB1CD2_H

#include "CalphadFreeEnergy.h"

class CalphadAB1CD2 : public CalphadFreeEnergy
{
public:
  CalphadAB1CD2();

  virtual void parameterize(Real & R, Real & low_conc, Real & high_conc, std::vector<Real> & low, std::vector<Real> & high, std::vector<Real> & mix, std::vector<Real> L0, std::vector<Real> L1, std::vector<Real> Pure1);

  virtual Real calculateReference(const Real & c, const Real & T) const;
  virtual Real calculateIdeal(const Real & c, const Real & T) const;
  virtual Real calculateExcess(const Real & c, const Real & T) const;

  //virtual Real calculateFirstLatticeGminusHser();
  virtual Real calculateSecondLatticeGminusHser(const Real & c, const Real & T) const;

  virtual Real computeGMix(const Real & c, const Real & T) const;
  virtual Real computeDGMixDc(const Real & c, const Real & T) const;
  virtual Real computeD2GMixDc2(const Real & c, const Real & T) const;
  virtual Real computeD3GMixDc3(const Real & c, const Real & T) const;

protected:
  std::vector<Real> _pure_EP1_phase1_coeffs;


private:
};

#endif //CALPHADAB1CD2_H
