/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  7 November 2013
*
*************************************************************************/

#include "CalphadAB1CD1Material.h"
#include "CalphadAB1CD1.h"

template<>
InputParameters validParams<CalphadAB1CD1Material>()
{
  InputParameters params = validParams<CalphadEnergyMaterial>();

  params.addParam<Real>("low_cutoff", 0.001, "linearization cutoff, low end");
  params.addParam<Real>("high_cutoff", 0.499, "linearization cutoff, high end");
  params.addParam<Real>("precip_conc", 0.6, "concentration of precipitate");

  return params;
}

CalphadAB1CD1Material::CalphadAB1CD1Material(const InputParameters & parameters) :
    CalphadEnergyMaterial(parameters),
    _energy(),
    _low_cutoff(getParam<Real>("low_cutoff")),
    _high_cutoff(getParam<Real>("high_cutoff")),
    _precip_conc(getParam<Real>("precip_conc")),
    _G_AB1CD1(declareProperty<Real>("G_AB1CD1")),
    _dG_dc(declareProperty<Real>("dGAB1CD1_dc")),
    _d2G_dc2(declareProperty<Real>("d2GAB1CD1_dc2")),
    //_d3G_dc3(declareProperty<Real>("d3GAB1CD1_dc3"))
    _d2G_dcdT(declareProperty<Real>("d2GAB1CD1_dcdT")),
    _G_AB1CD1_precip(declareProperty<Real>("G_AB1CD1_precip"))
{
  _energy.parameterize(_R,_low_cutoff, _high_cutoff, _pure_endpoint_low_coeffs, _pure_endpoint_high_coeffs, _mixture_coeffs);
}

void
CalphadAB1CD1Material::computeQpProperties()
{
  //Real c;

  /*
  if (_c[_qp]  < _low_cutoff)
    c = _low_cutoff;
  else if (_c[_qp] > _high_cutoff)
    c = _high_cutoff;
  else c = _c[_qp];
  */


  _G_AB1CD1[_qp] = _energy.computeGMix(_c[_qp], _T[_qp]);
  _dG_dc[_qp] = _energy.computeDGMixDc(_c[_qp], _T[_qp]);
  _d2G_dc2[_qp] = _energy.computeD2GMixDc2(_c[_qp], _T[_qp]);

  //_d3G_dc3[_qp] = computeD3GMixDc3();
  _d2G_dcdT[_qp] = _energy.computeD2GMixDcDT(_c[_qp], _T[_qp]);

  _G_AB1CD1_precip[_qp] = _energy.computeGMix(_precip_conc, _T[_qp]);
}
