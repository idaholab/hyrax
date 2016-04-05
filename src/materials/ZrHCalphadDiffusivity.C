/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  13 February 2014
*
*************************************************************************/

#include "ZrHCalphadDiffusivity.h"

template<>
InputParameters validParams<ZrHCalphadDiffusivity>()
{
  InputParameters params = validParams<ZrHCalphad>();

  params.addRequiredParam<Real>("H_Zr_D0", "Diffusion prefactor for H in hcp Zr (isotropic)");
  params.addRequiredParam<Real>("H_ZrH2_D0", "Diffusion prefactor for H in fcc ZrH2 (isotropic)");
  params.addRequiredParam<Real>("H_Zr_Q0", "Activation energy for H in hcp Zr");
  params.addRequiredParam<Real>("H_ZrH2_Q0", "Activaton energy for H in fcc ZrH2");
  params.addParam<Real>("R", 8.3144, "gas constant");
  params.addParam<Real>("k", 1.38E-23, "Boltzmann constant");

  params.addRequiredCoupledVar("OP_variable", "coupled OP variable");
  params.addRequiredCoupledVar("concentration", "coupled concentration variable");

  params.addParam<Real>("CH_mobility_scaling", 1, "scaling factor to divide by to nondimensionalize mobility");
  params.addParam<Real>("Q_transport", 0, "heat of transport of H in hcp Zr");

  return params;
}

ZrHCalphadDiffusivity::ZrHCalphadDiffusivity(const InputParameters & parameters)
    : ZrHCalphad(parameters),

      _H_Zr_D0(getParam<Real>("H_Zr_D0")),
      _H_ZrH2_D0(getParam<Real>("H_ZrH2_D0")),
      _H_Zr_Q0(getParam<Real>("H_Zr_Q0")),
      _H_ZrH2_Q0(getParam<Real>("H_ZrH2_Q0")),
      _R(getParam<Real>("R")),
      _k(getParam<Real>("k")),
      _mobility_CH_scaling(getParam<Real>("CH_mobility_scaling")),
      _d2Galpha_dc2(getMaterialPropertyByName<Real>("d2GAB1CD1_dc2")),
      _d2Gdelta_dc2(getMaterialPropertyByName<Real>("d2GAB1CD2_dc2")),
      _D_alpha(declareProperty<Real>("D_alpha")),
      _D_delta(declareProperty<Real>("D_delta")),
      _c(coupledValue("concentration")),
      _OP(coupledValue("OP_variable")),
      _L1Q(declareProperty<Real>("L1Q")),
      _Q_transport(getParam<Real>("Q_transport")),
      _d2Galpha_dcdT(getMaterialPropertyByName<Real>("d2GAB1CD1_dcdT"))
{
}

void
ZrHCalphadDiffusivity::computeQpProperties()
{
  _D_alpha[_qp] = _H_Zr_D0*std::exp(-_H_Zr_Q0/(_R*_temperature[_qp]));
  _D_delta[_qp] = _H_ZrH2_D0*std::exp(-_H_ZrH2_Q0/(_R*_temperature[_qp]));

  //nondimensionalize the mobility here

  Real solute = _c[_qp];
  if (solute < 0)
    solute = 0;

  Real h = computeHeaviside();
  Real hd = computeHeavisideDelta();

  //_M[_qp] = ((1-h)*(_D_alpha[_qp]/_d2Galpha_dc2[_qp]) + h*_D_delta[_qp]/_d2Gdelta_dc2[_qp])/_mobility_CH_scaling;

//  if (solute < 0.5)
//     _M[_qp] = ((1-h)*(_D_alpha[_qp]/_d2Galpha_dc2[_qp]) + h*_D_delta[_qp]/_d2Gdelta_dc2_cutoff[_qp])/_mobility_CH_scaling;
//  else
//     _M[_qp] = ((1-h)*(_D_alpha[_qp]/_d2Galpha_dc2[_qp]) + h*_D_delta[_qp]/_d2Gdelta_dc2[_qp])/_mobility_CH_scaling;

  _M[_qp] = ((1-h)*(_D_alpha[_qp]/_d2Galpha_dc2[_qp]) + hd*_D_delta[_qp]/_d2Gdelta_dc2[_qp])/_mobility_CH_scaling;
  //_M[_qp] = ((1-h)*(_D_alpha[_qp]/_d2Galpha_dc2[_qp]) + hd*_D_delta[_qp]/_d2Gdelta_dc2_precip[_qp])/_mobility_CH_scaling;

  // _M[_qp] = ((solute*_D_alpha[_qp])/(_R*_temperature[_qp]))/_mobility_CH_scaling;

  //multiply by molar volume to get the units to actually work out
  _M[_qp] *= _molar_volume;

  if (_M[_qp] < 0)
  {
    //   _console<<"negative mobility"<<std::endl;
       _M[_qp] = 0;
  }
//  if ( _c[_qp] < 0)
//    _M[_qp] = 0;

  _grad_M[_qp] = 0.0;

  _L[_qp] = _mobility_AC;

  _kappa_c[_qp] = _kappa_CH;
  _kappa_n[_qp] = _kappa_AC;

  _W[_qp] = _well_height;
  _molar_vol[_qp] = _molar_volume;

  _thermal_diff[_qp] = _thermal_diffusivity;
  _dThermDiff_dT[_qp] = _dThermal_diffusivity_dT;
}

Real
ZrHCalphadDiffusivity::computeHeaviside()
{
   Real OP = _OP[_qp];

  if(OP > 1)
    return (OP-1)*(OP-1) + 1;
  else
    return 3*OP*OP - 2*OP*OP*OP;
}

Real
ZrHCalphadDiffusivity::computeHeavisideDelta()
{
  Real OP = _OP[_qp];

  if(OP < 0)
    return -1*OP*OP;
  else
    return 3*OP*OP - 2*OP*OP*OP;
}
