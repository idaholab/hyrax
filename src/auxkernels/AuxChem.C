/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  4 September 2013
*
*************************************************************************/

#include "AuxChem.h"

template<>
InputParameters validParams<AuxChem>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredCoupledVar("coupled_conserved_var", "coupled conserved field variable");
  params.addRequiredCoupledVar("coupled_nonconserved_var", "coupled non-conserved field variable");
  params.addRequiredParam<Real>("precip_conserved", "value of the equilibrium 2nd phase conserved field variable");
  params.addRequiredParam<Real>("precip_nonconserved", "value of the equilibrium 2nd phase nonconserved field variable");
  params.addRequiredParam<int>("nonconserved_var_number", "the number (starting from 1) of the nonconserved variable");

  return params;
}

AuxChem::AuxChem(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_cons(coupledValue("coupled_conserved_var")),
    _coupled_noncons(coupledValue("coupled_nonconserved_var")),
    _precip_conserved(getParam<Real>("precip_conserved")),
    _precip_nonconserved(getParam<Real>("precip_nonconserved")),
    _a1(getMaterialProperty<Real>("A1")),
    _a2(getMaterialProperty<Real>("A2")),
    _a3(getMaterialProperty<Real>("A3")),
    _a4(getMaterialProperty<Real>("A4")),
    _c1(getMaterialProperty<Real>("C1")),
    _c2(getMaterialProperty<Real>("C2")),
    _noncons_var_num(getParam<int>("nonconserved_var_number"))
{
}

Real
AuxChem::computeValue()
{
  Real matrix_energy(0.0);
  Real precip_energy(0.0);
  Real differential(0.0);

  matrix_energy = computeEnergy(_coupled_cons[_qp], _coupled_noncons[_qp], true);
  precip_energy = computeEnergy(_precip_conserved, _precip_nonconserved, false);
  differential = computeDifferential(_coupled_cons[_qp], _coupled_noncons[_qp]);

  return (matrix_energy - precip_energy + differential);
}

Real
AuxChem::computeEnergy(const Real & conserved, const Real & nonconserved, bool /*matrix*/)
{
  Real fchem(0.0);

  fchem = computeFchem(conserved, nonconserved);

  return fchem;
}

Real
AuxChem::computeDifferential(const Real & coupled_conserved, const Real & coupled_nonconserved)
{
  // partial derivative of f_chem with respect to conserved variable
  Real dfchem_dcons(0.0);
  // partial derivative of f_chem with respect to nonconserved variable
  Real dfchem_dnoncons(0.0);

  Real first_term(0.0);
  Real second_term(0.0);

  dfchem_dcons = computeDfchemDcons(coupled_conserved, coupled_nonconserved);

  dfchem_dnoncons = computeDfchemDnoncons(coupled_conserved, coupled_nonconserved);

  first_term = (dfchem_dcons)*(_precip_conserved - coupled_conserved);
  second_term = (dfchem_dnoncons)*(_precip_nonconserved - coupled_nonconserved);

  return first_term + second_term;
}

Real
AuxChem::computeFchem(const Real & conserved, const Real & nonconserved)
{
  Real first_term;
  Real second_term;
  Real third_term;
  Real fourth_term;

  first_term = _a1[_qp]*0.5*(conserved - _c1[_qp])*(conserved - _c1[_qp]);
  second_term = _a2[_qp]*0.5*(conserved - _c2[_qp])*(nonconserved)*(nonconserved);
  third_term = _a3[_qp]*0.25*(std::pow(nonconserved, 4));
  fourth_term = _a4[_qp]*(std::pow(nonconserved, 6))/6;

  return first_term + second_term - third_term + fourth_term;
}

Real
AuxChem::computeDfchemDcons(const Real & coupled_conserved, const Real & coupled_nonconserved)
{
  Real first_term;
  Real second_term;

 first_term = _a1[_qp]*(coupled_conserved - _c1[_qp]);
 second_term =  0.5*_a2[_qp]*coupled_nonconserved*coupled_nonconserved;

 return first_term + second_term;
}

Real
AuxChem::computeDfchemDnoncons(const Real & coupled_conserved, const Real & coupled_nonconserved)
{
  Real first_term;
  Real second_term;
  Real third_term;

  first_term = _a2[_qp]*coupled_nonconserved*(coupled_conserved - _c2[_qp]);
  second_term = _a3[_qp]*std::pow(coupled_nonconserved, 3);
  third_term = _a4[_qp]*std::pow(coupled_nonconserved, 5);

  return first_term - second_term + third_term;
}
