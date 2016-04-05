/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  7 November 2013
*
*************************************************************************/

#include "CalphadAB1CD2Material.h"

template<>
InputParameters validParams<CalphadAB1CD2Material>()
{
  InputParameters params = validParams<CalphadEnergyMaterial>();

  params.addRequiredParam<std::vector<Real> >("pure_EP1_phase1_coeffs", "coeffs of pure endpoint at low composition in the first phase");

  params.addParam<Real>("low_cutoff", 0.001, "linearization cutoff, low end");
  params.addParam<Real>("high_cutoff", 0.655, "linearization cutoff, high end");
  params.addParam<Real>("precip_conc", 0.6, "concentration of precipitate");

  return params;
}

CalphadAB1CD2Material::CalphadAB1CD2Material(const InputParameters & parameters) :
    CalphadEnergyMaterial(parameters),
    _energy(),
    _low_cutoff(getParam<Real>("low_cutoff")),
    _high_cutoff(getParam<Real>("high_cutoff")),
    _precip_conc(getParam<Real>("precip_conc")),
    _pure_EP1_phase1_coeffs(getParam<std::vector<Real> >("pure_EP1_phase1_coeffs")),
    _G_AB1CD2(declareProperty<Real>("G_AB1CD2")),
    _dG_dc(declareProperty<Real>("dGAB1CD2_dc")),
    _d2G_dc2(declareProperty<Real>("d2GAB1CD2_dc2")),
    // _d3G_dc3(declareProperty<Real>("d3GAB1CD2_dc3"))
    _G_AB1CD2_precip(declareProperty<Real>("G_AB1CD2_precip")),
    _d2G_dc2_precip(declareProperty<Real>("d2GAB1CD2_dc2_precip")),
    _d2G_dc2_cutoff(declareProperty<Real>("d2GAB1CD2_dc2_cutoff"))
{
  _energy.parameterize(_R, _low_cutoff, _high_cutoff, _pure_endpoint_low_coeffs, _pure_endpoint_high_coeffs, _mixture_coeffs,
                       _L0_coeffs, _L1_coeffs, _pure_EP1_phase1_coeffs);
}

void
CalphadAB1CD2Material::computeQpProperties()
{
  //Real c;

  /*
  if (_c[_qp]  < _low_cutoff)
    c = _low_cutoff;
  else if (_c[_qp] > _high_cutoff)
    c = _high_cutoff;
  else c = _c[_qp];
  */

  _G_AB1CD2[_qp] = _energy.computeGMix(_c[_qp], _T[_qp]);
  _dG_dc[_qp] = _energy.computeDGMixDc(_c[_qp], _T[_qp]);
  _d2G_dc2[_qp] = _energy.computeD2GMixDc2(_c[_qp], _T[_qp]);

  _G_AB1CD2_precip[_qp] = _energy.computeGMix(_precip_conc, _T[_qp]);
  _d2G_dc2_precip[_qp] = _energy.computeD2GMixDc2(_precip_conc, _T[_qp]);

  _d2G_dc2_cutoff[_qp] = _energy.computeD2GMixDc2(0.5, _T[_qp]);
}
