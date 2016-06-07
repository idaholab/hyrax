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
  params.addRequiredCoupledVar("temperature", "temperature to be used to calculating Gibbs energies");

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
//need off diagonal terms here
      _h(declareProperty<Real>("interpolation_fxn")),
      _dhdOP(declareProperty<Real>("dinterpolation_dOP")),
      _d2hdOP2(declareProperty<Real>("d2interpolation_dOP2")),
      _g(declareProperty<Real>("barrier_fxn")),
      _dgdOP(declareProperty<Real>("dbarrier_dOP")),
      _d2gdOP2(declareProperty<Real>("d2barrier_dOP2")),
      _c(coupledValue("concentration")),
      _OP(coupledValue("OP_variable")),
      _temperature(coupledValue("temperature"))
{
}

void
ZrHCalphadDiffusivity::computeQpProperties()
{
  _D_alpha[_qp] = _H_Zr_D0*std::exp(-_H_Zr_Q0/(_R*_temperature[_qp]));
  _D_delta[_qp] = _H_ZrH2_D0*std::exp(-_H_ZrH2_Q0/(_R*_temperature[_qp]));

//  Real solute = _c[_qp];
//  if (solute < 0)
//    solute = 0;

  Real OP = _OP[_qp];

  _h[_qp] = OP*OP*OP*(6*OP*OP - 15*OP + 10);
  _dhdOP[_qp] = 30*OP*OP*(OP*OP - 2*OP + 1);
  _d2hdOP2[_qp] = 60*OP*(2*OP*OP - 3*OP + 1);

  _g[_qp] = OP*OP*(1 - OP)*(1 - OP);
  _dgdOP[_qp] = 2*OP - 6*OP*OP + 4*OP*OP*OP;
  _d2gdOP2[_qp] = 2 - 12*OP + 12*OP*OP;

  _M[_qp] = ((1-_h[_qp])*(_D_alpha[_qp]/_d2Galpha_dc2[_qp]) + _h[_qp]*_D_delta[_qp]/_d2Gdelta_dc2[_qp]);

  if (_M[_qp] < 0)
       _M[_qp] = 0;

  //this should probably be computed.  I probably need to go to the non-constant mobility stuff Daniel's worked out.
  _grad_M[_qp] = 0.0;

  _L[_qp] = _mobility_AC;

  _kappa_c[_qp] = _kappa_CH;
  _kappa_n[_qp] = _kappa_AC;

  _W[_qp] = _well_height;
  _molar_vol[_qp] = _molar_volume;

  _fbulk[_qp] = ( (1-_h[_qp])*_Galpha[_qp] + _h[_qp]*_Gdelta[_qp] + _W[_qp]*_g[_qp] )/_molar_vol[_qp];
  _dfbulkdc[_qp] = ( (1-_h[_qp])*_dGalpha_dc[_qp] + _h[_qp]*_dGdelta_dc[_qp] )/_molar_vol[_qp];
  _d2fbulkdc2[_qp] = ( (1-_h[_qp])*_d2Galpha_dc2[_qp] + _h[_qp]*_d2Gdelta_dc2[_qp] )/_molar_vol[_qp];
  _dfbulkdOP[_qp] = ( (_Gdelta[_qp] - _Galpha[_qp])*_dhdOP[_qp] + _W[_qp]*_dgdOP[_qp])/_molar_vol[_qp];
  _d2fbulkdOP2[_qp] = ( (_Gdelta[_qp] - _Galpha[_qp])*_d2hdOP2[_qp] + _W[_qp]*_d2gdOP2[_qp])/_molar_vol[_qp];

  _d2fbulkdcdOP[_qp] = (_Gdelta[_qp] - _Galpha[_qp])*(_dhdOP[_qp])/_molar_vol[_qp];

  //add off diagonal terms here
}

Real
ZrHCalphadDiffusivity::computeHeaviside()
{
   Real OP = _OP[_qp];

   return 30*OP*OP*OP*OP - 60*OP*OP*OP + 10*OP*OP;
}
