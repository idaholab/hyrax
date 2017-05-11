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
#include "RankTwoTensor.h"

template<>
InputParameters validParams<ZrHCalphadDiffusivity>()
{
  InputParameters params = validParams<Material>();

//  params.addParam<Real>("mobility_CH", 1, "isotropic mobility for Cahn-Hilliard equation (isotropic)");
  params.addRequiredParam<Real>("mobility_AC", "isotropic kinetic coefficient for Allen-Cahn equation");
  params.addRequiredParam<Real>("kappa_CH", "CH gradient energy coefficient (isotropic)");
  params.addRequiredParam<Real>("kappa_AC", "AC gradient energy coefficient (isotropic)");
  params.addRequiredParam<Real>("well_height", "well height for barrier g(n)");
  params.addRequiredParam<Real>("molar_volume", "molar volume of the material (constant)");

  params.addRequiredParam<Real>("H_Zr_D0", "Diffusion prefactor for H in hcp Zr (isotropic)");
  params.addRequiredParam<Real>("H_ZrH2_D0", "Diffusion prefactor for H in fcc ZrH2 (isotropic)");
  params.addRequiredParam<Real>("H_Zr_Q0", "Activation energy for H in hcp Zr");
  params.addRequiredParam<Real>("H_ZrH2_Q0", "Activaton energy for H in fcc ZrH2");

  //aJ, amol, Kelvin
  params.addParam<Real>("R", 8.3144, "gas constant");
  params.addParam<Real>("k", 1.38E-5, "Boltzmann constant");

  params.addRequiredCoupledVar("OP_variable", "coupled OP variable");
  params.addRequiredCoupledVar("concentration", "coupled concentration variable");
  params.addRequiredCoupledVar("temperature", "temperature of the system");

  return params;
}

ZrHCalphadDiffusivity::ZrHCalphadDiffusivity(const InputParameters & parameters)
    : Material(parameters),
      _mobility_AC(getParam<Real>("mobility_AC")),
      _kappa_CH(getParam<Real>("kappa_CH")),
      _kappa_AC(getParam<Real>("kappa_AC")),
      _well_height(getParam<Real>("well_height")),
      _molar_volume(getParam<Real>("molar_volume")),

      _M(declareProperty<Real>("M")),
      //_grad_M(declareProperty<RealGradient>("grad_M")),
      _L(declareProperty<Real>("L")),
      _kappa_c(declareProperty<Real>("kappa_c")),
      _kappa_n(declareProperty<Real>("kappa_n")),
      _W(declareProperty<Real>("well_height")),
      _molar_vol(declareProperty<Real>("molar_volume")),

      _H_Zr_D0(getParam<Real>("H_Zr_D0")),
      _H_ZrH2_D0(getParam<Real>("H_ZrH2_D0")),
      _H_Zr_Q0(getParam<Real>("H_Zr_Q0")),
      _H_ZrH2_Q0(getParam<Real>("H_ZrH2_Q0")),
      _R(getParam<Real>("R")),
      _k(getParam<Real>("k")),

      _Galpha(getMaterialPropertyByName<Real>("G_AB1CD1")),
      _Gdelta(getMaterialPropertyByName<Real>("G_AB1CD2")),
      _dGalpha_dc(getMaterialPropertyByName<Real>("dGAB1CD1_dc")),
      _dGdelta_dc(getMaterialPropertyByName<Real>("dGAB1CD2_dc")),
      _d2Galpha_dc2(getMaterialPropertyByName<Real>("d2GAB1CD1_dc2")),
      _d2Gdelta_dc2(getMaterialPropertyByName<Real>("d2GAB1CD2_dc2")),

      _D_alpha(declareProperty<Real>("D_alpha")),
      _D_delta(declareProperty<Real>("D_delta")),

      _fbulk(declareProperty<Real>("f_bulk")),
      _dfbulkdc(declareProperty<Real>("df_bulk_dc")),
      _d2fbulkdc2(declareProperty<Real>("d2f_bulk_dc2")),
      _dfbulkdOP(declareProperty<Real>("df_bulk_dOP")),
      _d2fbulkdOP2(declareProperty<Real>("d2f_bulk_dOP2")),

      _d2fbulkdcdOP(declareProperty<Real>("d2f_bulk_dcdOP")),

      _h(declareProperty<Real>("interpolation_fxn")),
      _dhdOP(declareProperty<Real>("dinterpolation_dOP")),
      _d2hdOP2(declareProperty<Real>("d2interpolation_dOP2")),
      _g(declareProperty<Real>("barrier_fxn")),
      _dgdOP(declareProperty<Real>("dbarrier_dOP")),
      _d2gdOP2(declareProperty<Real>("d2barrier_dOP2")),
      _c(coupledValue("concentration")),
      _OP(coupledValue("OP_variable")),
      _temperature(coupledValue("temperature")),
      _void_tensor(declareProperty<RankTwoTensor>("void_tensor"))
{
}

void
ZrHCalphadDiffusivity::computeQpProperties()
{
  _L[_qp] = _mobility_AC;

  _kappa_c[_qp] = _kappa_CH;
  _kappa_n[_qp] = _kappa_AC;

  _W[_qp] = _well_height;
  _molar_vol[_qp] = _molar_volume;

  //this is here because I need to figure out how to make a material property zero
  RankTwoTensor empty; empty.zero();
  _void_tensor[_qp] = empty;

  _D_alpha[_qp] = _H_Zr_D0*std::exp(-_H_Zr_Q0/(_R*_temperature[_qp]));
  _D_delta[_qp] = _H_ZrH2_D0*std::exp(-_H_ZrH2_Q0/(_R*_temperature[_qp]));

  Real OP = _OP[_qp];

  _h[_qp] = OP*OP*OP*(6*OP*OP - 15*OP + 10);
  _dhdOP[_qp] = 30*OP*OP*(OP*OP - 2*OP + 1);
  _d2hdOP2[_qp] = 60*OP*(2*OP*OP - 3*OP + 1);

  _g[_qp] = OP*OP*(1 - OP)*(1 - OP);
  _dgdOP[_qp] = 2*OP - 6*OP*OP + 4*OP*OP*OP;
  _d2gdOP2[_qp] = 2 - 12*OP + 12*OP*OP;

  //_M[_qp] = ((1-_h[_qp])*(_D_alpha[_qp]/_d2Galpha_dc2[_qp]) + _h[_qp]*_D_delta[_qp]/_d2Gdelta_dc2[_qp]);
  //homogeneous mobility approximation
  //_M[_qp] = _D_alpha[_qp]/_d2Galpha_dc2[_qp];
  //constant mobility approximation
  //_M[_qp] = 5e-4;

  //early hyrax approximation (interstitial solution)
  _M[_qp] = (_D_alpha[_qp]*_c[_qp])/(_R*_temperature[_qp]);

  //multiply by molar volume to make the units work out
  _M[_qp] *= _molar_volume;

  //keep things from exploding
  if (_M[_qp] < 0)
    _M[_qp] = 0;

  //would be a good idea to get the dM/dX, dM/deta terms in there for the off-diagonal Jacobian.


  _fbulk[_qp] = ( (1-_h[_qp])*_Galpha[_qp] + _h[_qp]*_Gdelta[_qp] + _W[_qp]*_g[_qp] )/_molar_vol[_qp];
  _dfbulkdc[_qp] = ( (1-_h[_qp])*_dGalpha_dc[_qp] + _h[_qp]*_dGdelta_dc[_qp] )/_molar_vol[_qp];
  _d2fbulkdc2[_qp] = ( (1-_h[_qp])*_d2Galpha_dc2[_qp] + _h[_qp]*_d2Gdelta_dc2[_qp] )/_molar_vol[_qp];
  _dfbulkdOP[_qp] = ( (_Gdelta[_qp] - _Galpha[_qp])*_dhdOP[_qp] + _W[_qp]*_dgdOP[_qp])/_molar_vol[_qp];
  _d2fbulkdOP2[_qp] = ( (_Gdelta[_qp] - _Galpha[_qp])*_d2hdOP2[_qp] + _W[_qp]*_d2gdOP2[_qp])/_molar_vol[_qp];

  _d2fbulkdcdOP[_qp] = (_dGdelta_dc[_qp] - _dGalpha_dc[_qp])*(_dhdOP[_qp])/_molar_vol[_qp];

  //add off diagonal terms here
}
