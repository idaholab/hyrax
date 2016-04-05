#ifndef CALPHADAB1CD1_H
#define CALPHADAB1CD1_H

#include "CalphadFreeEnergy.h"

//#include "CalphadParameters.h"

class CalphadAB1CD1 : public CalphadFreeEnergy
{
public:
  CalphadAB1CD1();

  virtual void parameterize(Real & R, Real & low_conc, Real & high_conc, std::vector<Real> & low, std::vector<Real> & high, std::vector<Real> & mix);

  virtual Real calculateReference(const Real & c, const Real & T) const;
  virtual Real calculateIdeal(const Real & c, const Real & T) const;
  virtual Real calculateExcess(const Real & c, const Real & T) const;

  //virtual Real calculateFirstLatticeGminusHser();
  virtual Real calculateSecondLatticeGminusHser(const Real & c, const Real & T) const;
  virtual Real calculateDFirstLatticeGminusHserDT(const Real & c, const Real & T) const;
   virtual Real calculateDSecondLatticeGminusHserDT(const Real & c, const Real & T) const;

  virtual Real computeGMix(const Real & c, const Real & T) const;
  virtual Real computeDGMixDc(const Real & c, const Real & T) const;
  virtual Real computeD2GMixDc2(const Real & c, const Real & T) const;
  virtual Real computeD3GMixDc3(const Real & c, const Real & T) const;

  virtual Real computeD2GMixDcDT(const Real & c, const Real & T) const;

protected:
private:
};

#endif //CALPHADAB1CD1_H
