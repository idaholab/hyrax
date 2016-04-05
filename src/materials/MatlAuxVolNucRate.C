/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  1 September 2015
*
*************************************************************************/

#include "MatlAuxVolNucRate.h"

template<>
InputParameters validParams<MatlAuxVolNucRate>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<Real>("D_alpha", "Diffusion coefficent");
  params.addRequiredParam<Real>("molar_vol", "molar volume");

  return params;
}

MatlAuxVolNucRate::MatlAuxVolNucRate(const InputParameters & parameters) :
      Material(parameters),
      _d_alpha_input(getParam<Real>("D_alpha")),
      _molar_vol_input(getParam<Real>("molar_vol")),
      _D_alpha(declareProperty<Real>("D_alpha")),
      _molar_volume(declareProperty<Real>("molar_volume"))
{
}

void
MatlAuxVolNucRate::computeQpProperties()
{
  _D_alpha[_qp] = _d_alpha_input;
  _molar_volume[_qp] = _molar_vol_input;
}
