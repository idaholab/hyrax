/*************************************************************************
*************************************************************************/

#include "FreeEnergy.h"

template<>
InputParameters validParams<FreeEnergy>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("coupled_conc", "Coupled concentration variable name");
  params.addRequiredCoupledVar("coupled_n", "coupled order parameter variable name");
  params.addParam<Real>("length_scale_factor", 1, "length scale factor in simulation (large unit per small unit)");
  params.addParam<Real>("energy_scale_factor", 1, "energy scale factor in simulation");

  return params;
}

FreeEnergy::FreeEnergy(const InputParameters & parameters)
    : Material(parameters),
      _length_scale(getParam<Real>("length_scale_factor")),
      _energy_scale(getParam<Real>("energy_scale_factor")),
      _kappa_c(getMaterialPropertyByName<Real>("kappa_c")),
      _kappa_n(getMaterialPropertyByName<Real>("kappa_n")),
      _W(getMaterialPropertyByName<Real>("well_height")),
      _molar_vol(getMaterialPropertyByName<Real>("molar_volume")),
      _alpha_energy(getMaterialPropertyByName<Real>("G_AB1CD1")),
      _delta_energy(getMaterialPropertyByName<Real>("G_AB1CD2")),
      _c(coupledValue("coupled_conc")),
      _n(coupledValue("coupled_n")),
      _grad_c(coupledGradient("coupled_conc")),
      _grad_n(coupledGradient("coupled_n")),
      _free_energy_density(declareProperty<Real>("free_energy_density")),
      _alpha_energy_density(declareProperty<Real>("alpha_phase_energy_density")),
      _delta_energy_density(declareProperty<Real>("delta_phase_energy_density"))
{
}

void
FreeEnergy::computeQpProperties()
{

  Real solute = _c[_qp];
  if (solute < 0)
    solute = 0;

  Real OP = _n[_qp];
  if (OP < 0) OP = 0;
  if (OP > 1) OP = 1;

  Real Heaviside = 3*OP*OP - 2*OP*OP*OP;
  Real g = OP*OP*(OP-1)*(OP-1);

  Real fchem_scale_factor = (_length_scale*_length_scale*_length_scale/_molar_vol[_qp]);
  Real grad_scale_factor = _energy_scale*_length_scale*_length_scale;

  //the energies are in J/mol
  Real fchem = (1 - Heaviside)*_alpha_energy[_qp] + Heaviside*_delta_energy[_qp] + _W[_qp]*g;

  //this puts kappas as J/m
  Real grad_c_term = 0.5*grad_scale_factor*_kappa_c[_qp]*_grad_c[_qp].norm_sq();
  Real grad_n_term = 0.5*grad_scale_factor*_kappa_n[_qp]*_grad_n[_qp].norm_sq();

  //convert energy to J/nm^3 and J/nm
  grad_c_term *= _length_scale;
  grad_n_term *= _length_scale;

  _free_energy_density[_qp] = fchem*fchem_scale_factor + grad_c_term + grad_n_term;


   //  _console<<"fchem = "<<fchem*fchem_scale_factor<<std::endl;
   // _console<<"gradc = "<<grad_c_term*grad_scale_factor<<std::endl;
   //_console<<"gradn = "<<grad_n_term*grad_scale_factor<<std::endl;

  //this is in J/m^3
  _alpha_energy_density[_qp] = _alpha_energy[_qp]/_molar_vol[_qp];
  _delta_energy_density[_qp] = _delta_energy[_qp]/_molar_vol[_qp];
}
