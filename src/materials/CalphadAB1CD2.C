#include "CalphadAB1CD2.h"
//#include "CalphadParameters.h"

#include <cmath>

CalphadAB1CD2::CalphadAB1CD2() :
    CalphadFreeEnergy(),
    _pure_EP1_phase1_coeffs()
{
}


void
CalphadAB1CD2::parameterize(Real & R, Real & low_conc, Real & high_conc, std::vector<Real> & low, std::vector<Real> & high, std::vector<Real> & mix,
                            std::vector<Real> L0, std::vector<Real> L1, std::vector<Real> Pure1)
{
  _pure_endpoint_1_coeffs = low;
  _pure_endpoint_2_coeffs = high;
  _mixture_coeffs = mix;

  _L0_coeffs = L0;
  _L1_coeffs = L1;

  _pure_EP1_phase1_coeffs = Pure1;

  _low_cutoff = low_conc;
  _high_cutoff = high_conc;
  _R = R;
}

Real
CalphadAB1CD2::calculateReference(const Real & c, const Real & T) const
{
  Real first_energy = calculateFirstLatticeGminusHser(c, T);
  Real second_energy = calculateSecondLatticeGminusHser(c, T);

  return  0.5*( (2 - 3*c)*first_energy + c*second_energy );
}


Real
CalphadAB1CD2::calculateIdeal(const Real & c, const Real & T) const
{
  return _R*T*( c*std::log(c/(2-2*c)) + (2-3*c)*std::log((2-3*c)/(2-2*c)) );
}

Real
CalphadAB1CD2::calculateExcess(const Real & c, const Real & T) const
{
  Real L0 = _L0_coeffs[0] + _L0_coeffs[1]*T;
  Real L1 = _L1_coeffs[0] + _L1_coeffs[1]*T;

  Real polynomial = (c-1)*L0 + (1-2*c)*L1;

  Real prefactor = 3*c*c - 2*c;
  prefactor /= 4*(c-1)*(c-1);

  return prefactor*polynomial;
}

Real
CalphadAB1CD2::calculateSecondLatticeGminusHser(const Real & /*c*/, const Real & T) const
{
  Real first_term = _mixture_coeffs[0]
    + _mixture_coeffs[1]*T
    + _mixture_coeffs[2]*T*std::log(T);

  Real second_term = _pure_EP1_phase1_coeffs[0]
    + _pure_EP1_phase1_coeffs[1]*T
    + _pure_EP1_phase1_coeffs[2]*T*std::log(T)
    + _pure_EP1_phase1_coeffs[3]*T*T
    + _pure_EP1_phase1_coeffs[4]/T;

  Real third_term = _pure_endpoint_2_coeffs[0]
    + _pure_endpoint_2_coeffs[1]*T
    + _pure_endpoint_2_coeffs[2]*T*std::log(T)
    + _pure_endpoint_2_coeffs[3]*T*T
    + _pure_endpoint_2_coeffs[4]/T;

  return first_term + second_term + third_term;
}

Real
CalphadAB1CD2::computeGMix(const Real & c, const Real & T) const
{
  Real c1;
  Real p = 0.0;

  //make this piecewise in concentration space
  if( c < _low_cutoff)
  {
    c1 = _low_cutoff;
    //Taylor expand to third ish order
    return CalphadFreeEnergy::computeGMix(c1, T)
      + computeDGMixDc(c1, T)*(c - c1)
      + 0.5*computeD2GMixDc2(c1,T)*(c-c1)*(c-c1)
      + (p*1.0/6.0)*computeD3GMixDc3(c1,T)*(c-c1)*(c-c1)*(c-c1);
  }
  else if (c > _high_cutoff)
  {
    c1 = _high_cutoff;
    //Taylor expand to third ish order
    return CalphadFreeEnergy::computeGMix(c1, T)
      + computeDGMixDc(c1, T)*(c - c1)
      + 0.5*computeD2GMixDc2(c1,T)*(c-c1)*(c-c1)
      + (p*1.0/6.0)*computeD3GMixDc3(c1,T)*(c-c1)*(c-c1)*(c-c1);
  }
  else
    return CalphadFreeEnergy::computeGMix(c, T);
}

Real
CalphadAB1CD2::computeDGMixDc(const Real & c, const Real & T) const
{
  Real ref;
  Real ideal;
  Real c1;
  Real p = 0.0;
  Real second(0);
  Real third(0);

  if( c < _low_cutoff)
  {
    //Taylor expansion to third ish order
    c1 = _low_cutoff;
    second = computeD2GMixDc2(c1,T)*(c-c1);
    third = (p*0.5)*computeD3GMixDc3(c1,T)*(c-c1)*(c-c1);
  }
  else if (c > _high_cutoff)
  {
    //Taylor expansion to third ish order
    c1 = _high_cutoff;
    second = computeD2GMixDc2(c1,T)*(c-c1);
    third = (p*0.5)*computeD3GMixDc3(c1,T)*(c-c1)*(c-c1);
  }
  else
    c1 = c;

  ref = -1.5*calculateFirstLatticeGminusHser(c1, T) + 0.5*calculateSecondLatticeGminusHser(c1, T);

  ideal = _R*T*( std::log(4*c1/(1-c1)) - 3*std::log((2-3*c1)/(1-c1)) );

  Real xs;

  Real L0 = _L0_coeffs[0] + _L0_coeffs[1]*T;
  Real L1 = _L1_coeffs[0] + _L1_coeffs[1]*T;

  Real polynomial = (3*c1*c1*c1 -9*c1*c1 + 8*c1 -2)*L0 + (-6*c1*c1*c1 + 18*c1*c1 -12*c1 +2)*L1;

  Real prefactor = 1/(4*(c1-1)*(c1-1)*(c1-1) );

  xs = prefactor*polynomial;

  return ref + ideal + xs + second + third;
}

Real
CalphadAB1CD2::computeD2GMixDc2(const Real & c, const Real & T) const
{
  Real c1;
  Real ref;
  Real ideal;
  Real second(0);
  Real third(0);
  Real p = 0.0;

  //make this piecewise in concentration space
  if( c < _low_cutoff)
  {
    //Taylor expansion to third ish order
    c1 = _low_cutoff;

    second = computeD2GMixDc2(c1,T);
    third = p*computeD3GMixDc3(c1,T)*(c-c1);
  }

  else if (c > _high_cutoff)
  {
    //Taylor expansion to third order ish
    c1 = _high_cutoff;
    second = computeD2GMixDc2(c1,T);
    third = p*computeD3GMixDc3(c1,T)*(c-c1);


  }
  else
    c1 = c;

    ref = 0;
    ideal = _R*T*( 2/( c1*(3*c1*c1 - 5*c1 + 2)) );

    Real L0 = _L0_coeffs[0] + _L0_coeffs[1]*T;
    Real L1 = _L1_coeffs[0] + _L1_coeffs[1]*T;
    Real polynomial = (c1 - 1)*L0 + (3 - 6*c1)*L1;
    Real prefactor = 1/(2*std::pow(c1 - 1, 4));
    Real xs = prefactor*polynomial;

    return ref + ideal + xs + second + third;
}

Real
CalphadAB1CD2::computeD3GMixDc3(const Real & c, const Real & T) const
{
  Real c1;
  Real ref;
  Real ideal;

  //might not want the cutoffs here since we don't ever use third order anywhere else?
  if( c < _low_cutoff)
  {
    c1 = _low_cutoff;
  }
  else if (c > _high_cutoff)
  {
    //Taylor expansion to third order
    c1 = _high_cutoff;
  }
  else
    c1 = c;

  ref = 0;
  ideal = _R*T*( (4-20*c1*18*c1*c1)/( c1*c1*std::pow(3*c*c - 5*c + 2,2.0)) );

  Real L0 = _L0_coeffs[0] + _L0_coeffs[1]*T;
  Real L1 = _L1_coeffs[0] + _L1_coeffs[1]*T;

  Real polynomial = 3*(L0 - L0*c1 + L1*(6*c1 - 2));

  Real prefactor = 1/(2*std::pow(c - 1, 5));

  Real xs = prefactor*polynomial;

return ref + ideal + xs;
}
