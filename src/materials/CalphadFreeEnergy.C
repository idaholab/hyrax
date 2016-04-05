#include "CalphadFreeEnergy.h"

#include <vector>
#include <cmath>

CalphadFreeEnergy::CalphadFreeEnergy() :
    _pure_endpoint_1_coeffs(),
    _pure_endpoint_2_coeffs(),
    _mixture_coeffs(),
    _L0_coeffs(),
    _L1_coeffs(),
    _low_cutoff(0),
    _high_cutoff(0),
    _R(0)
{
}

Real
CalphadFreeEnergy::calculateFirstLatticeGminusHser(const Real & /*c*/, const Real & T) const
{
  return _pure_endpoint_1_coeffs[0]
    + _pure_endpoint_1_coeffs[1]*T
    + _pure_endpoint_1_coeffs[2]*T*std::log(T)
    + _pure_endpoint_1_coeffs[3]*T*T
    + _pure_endpoint_1_coeffs[4]/T;
}

Real
CalphadFreeEnergy::computeGMix(const Real & c, const Real & T) const
{
  return calculateReference(c, T) + calculateIdeal(c, T) + calculateExcess(c, T);
}
