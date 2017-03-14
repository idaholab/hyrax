#include "CalphadAB1CD1.h"
//#include "CalphadParameters.h"

#include <cmath>

CalphadAB1CD1::CalphadAB1CD1() :
    CalphadFreeEnergy()
{
}

void
CalphadAB1CD1::parameterize(Real & R, Real & low_conc, Real & high_conc, std::vector<Real> & low, std::vector<Real> & high, std::vector<Real> & mix)
{
  _pure_endpoint_1_coeffs = low;
  _pure_endpoint_2_coeffs = high;
  _mixture_coeffs = mix;

  _low_cutoff = low_conc;
  _high_cutoff = high_conc;
  _R = R;
}

Real
CalphadAB1CD1::calculateReference(const Real & c, const Real & T) const
{
  Real first_energy = calculateFirstLatticeGminusHser(c, T);
  Real second_energy = calculateSecondLatticeGminusHser(c, T);

  return  (1 - 2*c)*first_energy + c*second_energy;
}

Real
CalphadAB1CD1::calculateIdeal(const Real & c, const Real & T) const
{
  return _R*T*( c*std::log(c/(1-c)) + (1-2*c)*std::log((1-2*c)/(1-c)) );
}

Real
CalphadAB1CD1::calculateExcess(const Real & /*c*/, const Real & /*T*/) const
{
  return 0;
}

Real
CalphadAB1CD1::calculateSecondLatticeGminusHser(const Real & /*c*/, const Real & T) const
{
  Real first_term = _mixture_coeffs[0] + _mixture_coeffs[1]*T;

  Real second_term = _pure_endpoint_1_coeffs[0]
    + _pure_endpoint_1_coeffs[1]*T
    + _pure_endpoint_1_coeffs[2]*T*std::log(T)
    + _pure_endpoint_1_coeffs[3]*T*T
    + _pure_endpoint_1_coeffs[4]*T*T*T
    + _pure_endpoint_1_coeffs[5]/T;

  Real third_term = 0.5*(_pure_endpoint_2_coeffs[0]
                           + _pure_endpoint_2_coeffs[1]*T
                           + _pure_endpoint_2_coeffs[2]*T*std::log(T)
                           + _pure_endpoint_2_coeffs[3]*T*T
                           + _pure_endpoint_2_coeffs[4]*T*T*T
                           + _pure_endpoint_2_coeffs[5]/T );

  return first_term + second_term + third_term;
}

Real
CalphadAB1CD1::computeGMix(const Real & c, const Real & T) const
{
  Real c1;

  //make this piecewise in concentration space
  if( c < _low_cutoff)
  {
    c1 = _low_cutoff;

    return CalphadFreeEnergy::computeGMix(c1, T)
      + computeDGMixDc(c1, T)*(c - c1)
      + 0.5*computeD2GMixDc2(c1,T)*(c-c1)*(c-c1);
  }
  else if (c > _high_cutoff)
  {
    c1 = _high_cutoff;

    return CalphadFreeEnergy::computeGMix(c1, T)
      + computeDGMixDc(c1, T)*(c - c1)
      + 0.5*computeD2GMixDc2(c1,T)*(c-c1)*(c-c1);
  }
    else
      return CalphadFreeEnergy::computeGMix(c, T);
}

Real
CalphadAB1CD1::computeDGMixDc(const Real & c, const Real & T) const
{
  Real ref;
  Real ideal;
  Real c1;
  Real second(0);

   if( c < _low_cutoff)
   {

     c1 = _low_cutoff;
     second = computeD2GMixDc2(c1,T)*(c-c1);
   }
   else if (c > _high_cutoff)
   {
     c1 = _high_cutoff;
     second = computeD2GMixDc2(c1,T)*(c-c1);
   }

   else
     c1 = c;

   ref = -2*calculateFirstLatticeGminusHser(c1, T) + calculateSecondLatticeGminusHser(c1, T);
   ideal = _R*T*( std::log(c1/(1-c1)) - 2*std::log((1-2*c1)/(1-c1)) );

   return ref + ideal + second;
}

Real
CalphadAB1CD1::computeD2GMixDc2(const Real & c, const Real & T) const
{
  Real ref;
  Real ideal;
  Real c1;
  Real second(0);

  if( c < _low_cutoff)
  {
    c1 = _low_cutoff;

    second = computeD2GMixDc2(c1,T);
  }

  else if (c > _high_cutoff)
  {
    c1 = _high_cutoff;

    second = computeD2GMixDc2(c1,T);
  }
  else
  {
    c1 = c;
  }

    ref = 0;
    ideal = _R*T*( 1/( c1*(2*c1*c1 - 3*c1 + 1)) );

    return ref + ideal + second;
}

Real
CalphadAB1CD1::computeD2GMixDcDT(const Real & c, const Real & T) const
{
  Real ref;
  Real ideal;
  Real c1;

  //WHAT IS GOING ON HERE.  DO WE EVEN NEED THIS FUNCTION
   if( c < _low_cutoff)
     c1 = _low_cutoff;

   else if (c > _high_cutoff)
     c1 = _high_cutoff;

   else
     c1 = c;

   ref = -2*calculateDFirstLatticeGminusHserDT(c1, T) + calculateDSecondLatticeGminusHserDT(c1, T);

   ideal = _R*( std::log(c1/(1-c1)) - 2*std::log((1-2*c1)/(1-c1)) );

   return ref + ideal;
}

Real
CalphadAB1CD1::calculateDFirstLatticeGminusHserDT(const Real & /*c*/, const Real & T) const
{
  return _pure_endpoint_1_coeffs[1]
    + _pure_endpoint_1_coeffs[2]*(1 + std::log(T))
    + _pure_endpoint_1_coeffs[3]*2*T
    + _pure_endpoint_1_coeffs[4]*3*T*T
    - _pure_endpoint_1_coeffs[5]/(T*T);
}

Real
CalphadAB1CD1::calculateDSecondLatticeGminusHserDT(const Real & /*c*/, const Real & T) const
{
  Real first_term = _mixture_coeffs[1];

  Real second_term = _pure_endpoint_1_coeffs[1]
    + _pure_endpoint_1_coeffs[2]*(1 + std::log(T))
    + _pure_endpoint_1_coeffs[3]*2*T
    + _pure_endpoint_1_coeffs[4]*3*T*T
    - _pure_endpoint_1_coeffs[5]/(T*T);

  Real third_term = 0.5*(_pure_endpoint_2_coeffs[1]
                         + _pure_endpoint_2_coeffs[2]*(1 + std::log(T))
                         + _pure_endpoint_2_coeffs[3]*2*T
                         + _pure_endpoint_2_coeffs[4]*3*T*T
                         - _pure_endpoint_2_coeffs[5]/(T*T) );

  return first_term + second_term + third_term;
}
