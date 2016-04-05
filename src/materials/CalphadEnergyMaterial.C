/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  1 November 2013
*
*************************************************************************/

#include "CalphadEnergyMaterial.h"

template<>
InputParameters validParams<CalphadEnergyMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<std::vector<Real> >("pure_endpoint_low_coeffs", "A, B, C, D, E Gibbs coeffs for low-concentration pure endpoint");
  params.addRequiredParam<std::vector<Real> >("pure_endpoint_high_coeffs", "A, B, C, D, E Gibbs coeffs for high-concentration pure endpoint");
  params.addRequiredParam<std::vector<Real> >("mixture_coeffs", "A, B, C, D, E Gibbs coeffs for mixture endpoint");

  params.addRequiredParam<std::vector<Real> >("L0_coeffs", "Rudlich-Kister L0 polynomial coefficients");
  params.addRequiredParam<std::vector<Real> >("L1_coeffs", "Rudlich-Kister L1 polynomial coefficients");

  //Default in J/mol-K
  params.addParam<Real>("gas_constant", 8.3144621, "Universal gas constant");

  params.addRequiredCoupledVar("coupled_temperature", "temperature to be used to calculating Gibbs energies");
  params.addRequiredCoupledVar("coupled_concentration", "concentration to be used to calculating Gibbs energies");

  return params;
}

CalphadEnergyMaterial::CalphadEnergyMaterial(const InputParameters & parameters)
    : Material(parameters),
      _pure_endpoint_low_coeffs(getParam<std::vector<Real> >("pure_endpoint_low_coeffs")),
      _pure_endpoint_high_coeffs(getParam<std::vector<Real> >("pure_endpoint_high_coeffs")),
      _mixture_coeffs(getParam<std::vector<Real> >("mixture_coeffs")),
      _L0_coeffs(getParam<std::vector<Real> >("L0_coeffs")),
      _L1_coeffs(getParam<std::vector<Real> >("L1_coeffs")),
      _R(getParam<Real>("gas_constant")),
      _T(coupledValue("coupled_temperature")),
      _c(coupledValue("coupled_concentration"))
{
  //Make sure everything is set up from input correctly
  if( (_pure_endpoint_low_coeffs.size() != 5) ||
      (_pure_endpoint_high_coeffs.size() != 5) ||
      (_mixture_coeffs.size() != 3) ||
      (_L0_coeffs.size() != 2) ||
      (_L1_coeffs.size() != 2) )
    mooseError("Please supply the correct # of values for the Gibbs coefficients (CalphadEnergy).");
}

void
CalphadEnergyMaterial::computeQpProperties()
{
}

/*Real
CalphadEnergy::calculateFirstLatticeGminusHser()
{
  return _pure_endpoint_low_coeffs[0]
    + _pure_endpoint_low_coeffs[1]*_T[_qp]
    + _pure_endpoint_low_coeffs[2]*_T[_qp]*std::log(_T[_qp])
    + _pure_endpoint_low_coeffs[3]*_T[_qp]*_T[_qp]
    + _pure_endpoint_low_coeffs[4]/_T[_qp];
}

Real
CalphadEnergy::computeGMix(Real c)
{
  return calculateReference(c) + calculateIdeal(c) + calculateExcess(c);
}

Real
CalphadEnergy::calculateReference(Real c)
{
  return 0;
}

Real
CalphadEnergy::calculateIdeal(Real c)
{
  return 0;
}

Real
CalphadEnergy::calculateExcess(Real c)
{
  return 0;
}

Real
CalphadEnergy::calculateSecondLatticeGminusHser()
{
  return 0;
}

Real
CalphadEnergy::computeDGMixDc(Real c)
{
  return 0;
}

Real
CalphadEnergy::computeD2GMixDc2()
{
  return 0;
}

Real
CalphadEnergy::computeD3GMixDc3()
{
  return 0;
}
*/
