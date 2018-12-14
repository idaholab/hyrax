/******************************************************
 *
 *   Welcome to HYRAX!
 *
 *   Developer: Andrea Jokisaari
 *
 *   5 April 2016
 *
 *****************************************************/

#include "TwoPhaseLinearElasticMaterial.h"
#include "MooseMesh.h"
#include <cmath>

registerMooseObject("HyraxApp", TwoPhaseLinearElasticMaterial);

template<>
InputParameters validParams<TwoPhaseLinearElasticMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<std::vector<Real> >("Cijkl_matrix", "Stiffness tensor of matrix");
  params.addParam<MooseEnum>("matrix_fill_method", RankFourTensor::fillMethodEnum() = "symmetric9", "Cijkl_matrix fill method");
  params.addRequiredParam<std::vector<Real> >("Cijkl_precip", "Stiffness tensor of precip");
  params.addParam<MooseEnum>("precip_fill_method", RankFourTensor::fillMethodEnum() = "symmetric9", "Cijkl_precip fill method");

  params.addRequiredParam<std::vector<Real> >("precipitate_eigenstrain", "eigenstrain between precip and matrix");
  params.addParam<std::vector<Real> >("precip_misfit_T_coeffs", "misfit strain temperature coefficients, e11, e22, e33, e33, e23, e13, e12");
  params.addParam<Real>("percent_precip_misfit", 1, "percent of misfit strain of precip to be applied, between 0 and 1");

  params.addRequiredParam<std::vector<Real> >("matrix_eigenstrain", "eigenstrain in matrix due to H in solution");

  params.addRequiredCoupledVar("order_parameter", "order parameter variable");
  params.addRequiredCoupledVar("atomic_fraction", "atomic fraction variable");
  params.addRequiredCoupledVar("temperature", "temperature in K");
  params.addRequiredCoupledVar("disp_x", "The x displacement");
  params.addRequiredCoupledVar("disp_y", "The z displacement");
  params.addCoupledVar("disp_z", "The z displacement");

  return params;
}

TwoPhaseLinearElasticMaterial::TwoPhaseLinearElasticMaterial(const InputParameters & parameters) :
    Material(parameters),
    _OP(coupledValue("order_parameter")),
    _X(coupledValue("atomic_fraction")),
    _T(coupledValue("temperature")),
    _grad_disp_x(coupledGradient("disp_x")),
    _grad_disp_y(coupledGradient("disp_y")),
    _grad_disp_z(_mesh.dimension() == 3 ? coupledGradient("disp_z") : _grad_zero),
    _h(getMaterialProperty<Real>("interpolation_fxn")),
    _dhdn(getMaterialProperty<Real>("dinterpolation_dOP")),
    _d2hdn2(getMaterialProperty<Real>("d2interpolation_dOP2")),
    _stress(declareProperty<RankTwoTensor>("stress")),
    _total_strain(declareProperty<RankTwoTensor>("total_strain")),
    _elastic_strain(declareProperty<RankTwoTensor>("elastic_strain")),
    _misfit_strain(declareProperty<RankTwoTensor>("misfit_strain")),
    _precipitate_misfit_tensor(declareProperty<RankTwoTensor>("precipitate_misfit_tensor")),
    _elasticity_tensor(declareProperty<RankFourTensor>("elasticity_tensor")),
    _Jacobian_multiplier(declareProperty<RankFourTensor>("Jacobian_mult")),
    _f_el(declareProperty<Real>("f_elastic")),
    _dfel_dOP(declareProperty<Real>("dfel_dOP")),
    _d2fel_dOP2(declareProperty<Real>("d2fel_dOP2")),
    _dfel_dX(declareProperty<Real>("dfel_dX")),
    _d2fel_dX2(declareProperty<Real>("d2fel_dX2")),
    _Cijkl_matrix(getParam<std::vector<Real> >("Cijkl_matrix"), (RankFourTensor::FillMethod)(int)getParam<MooseEnum>("matrix_fill_method")),
    _Cijkl_precip(getParam<std::vector<Real> >("Cijkl_precip"), (RankFourTensor::FillMethod)(int)getParam<MooseEnum>("precip_fill_method")),
    _precipitate_eigenstrain_vector(getParam<std::vector<Real> >("precipitate_eigenstrain")),
    _matrix_eigenstrain_vector(getParam<std::vector<Real> >("matrix_eigenstrain")),
    _precip_misfit_T_coeffs_vector(getParam<std::vector<Real> >("precip_misfit_T_coeffs")),
    _percent_precip_misfit(getParam<Real>("percent_precip_misfit")),
    _current_precip_misfit()
{
  // make sure you don't do something stupid with the temperature coeffcients in the input file
  _console<<_precip_misfit_T_coeffs_vector.size();

  if (_precip_misfit_T_coeffs_vector.size() != 6)
    mooseError("No misfit temperature coefficients specified (TwoPhaseLEM).");

  //make sure your misfit strain application is between 0 and 100%
  if (_percent_precip_misfit > 1 || _percent_precip_misfit < 0)
    mooseError("Please give a percent precip misfit between 0 and 1 (TwoPhaseLEM)");

  _precipitate_eigenstrain.fillFromInputVector(_precipitate_eigenstrain_vector);
  _matrix_eigenstrain.fillFromInputVector(_matrix_eigenstrain_vector);
  _precip_misfit_T_coeffs.fillFromInputVector(_precip_misfit_T_coeffs_vector);
}

void
TwoPhaseLinearElasticMaterial::computeQpProperties()
{
//  computeInterpolation();

  computeQpElasticityTensor();

  computeQpStrain();

  computeQpStress();

  computeEnergy();
}

//void
//TwoPhaseLinearElasticMaterial::computeInterpolation()
//{
//  _h = _OP[_qp]*_OP[_qp]*_OP[_qp]*(6*_OP[_qp]*_OP[_qp] - 15*_OP[_qp] + 10);
//  _dhdn = 30*_OP[_qp]*_OP[_qp]*( _OP[_qp] - 1)*( _OP[_qp] - 1);
//  _d2hdn2 = 60*_OP[_qp]*( _OP[_qp] - 1)*(2.*_OP[_qp] - 1);
//}

void
TwoPhaseLinearElasticMaterial::computeQpElasticityTensor()
{
  _elasticity_tensor[_qp] = _Cijkl_matrix*(1 - _h[_qp]) + _Cijkl_precip*_h[_qp];
  _Jacobian_multiplier[_qp] = _elasticity_tensor[_qp];
}

void
TwoPhaseLinearElasticMaterial::computeQpStrain()
{
  RankTwoTensor grad_tensor(_grad_disp_x[_qp], _grad_disp_y[_qp], _grad_disp_z[_qp]);

  _current_precip_misfit = _percent_precip_misfit*(_precipitate_eigenstrain + _T[_qp]*_precip_misfit_T_coeffs);

  _total_strain[_qp] = 0.5*(grad_tensor + grad_tensor.transpose());
  _misfit_strain[_qp] = (1 - _h[_qp])*(_X[_qp]*_matrix_eigenstrain) + _h[_qp]*(_current_precip_misfit);
  _elastic_strain[_qp] = _total_strain[_qp] - _misfit_strain[_qp];
  _precipitate_misfit_tensor[_qp] = _current_precip_misfit;
}

void
TwoPhaseLinearElasticMaterial::computeQpStress()
{
  _stress[_qp] = _elasticity_tensor[_qp]*_elastic_strain[_qp];
}


void
TwoPhaseLinearElasticMaterial::computeEnergy()
{
  //compute elastic energy density
  RankTwoTensor temp = _elasticity_tensor[_qp]*_elastic_strain[_qp];
  _f_el[_qp] = 0.5*temp.doubleContraction(_elastic_strain[_qp]);


  //compute derivative of elastic energy density
  //break the derivative into two terms, one for each Cijkl type, denoted as A and B

  //there are 3 terms in each derivative part, but the 2nd and 3rd are the same
  //doing the A term
  temp = (-1.*_Cijkl_matrix*_dhdn[_qp])*_elastic_strain[_qp];
  Real first = temp.doubleContraction( _elastic_strain[_qp] );

  temp = (_Cijkl_matrix*(1 - _h[_qp]))*(_matrix_eigenstrain*_X[_qp]*_dhdn[_qp] - _current_precip_misfit*_dhdn[_qp]);
  Real second = temp.doubleContraction( _elastic_strain[_qp] );

  Real dfel_dOP_A = first + 2.*second;

  //doing the B term
  temp = (_Cijkl_precip*_dhdn[_qp])*_elastic_strain[_qp];
  first = temp.doubleContraction( _elastic_strain[_qp] );

  temp = (_Cijkl_precip*(_h[_qp]))*(_matrix_eigenstrain*_X[_qp]*_dhdn[_qp] - _current_precip_misfit*_dhdn[_qp]);
  second = temp.doubleContraction( _elastic_strain[_qp] );

  Real dfel_dOP_B = first +2.*second;

  _dfel_dOP[_qp] = 0.5*(dfel_dOP_A + dfel_dOP_B);



  //compute the 2nd derivative of the elastic energy density
  //it again is broken into two parts A and B as above for each type of Cijkl
  //A term
  temp = (-1.*_Cijkl_matrix*_d2hdn2[_qp])*_elastic_strain[_qp];
  first = temp.doubleContraction( _elastic_strain[_qp] );

  temp = (-1.*_Cijkl_matrix*_dhdn[_qp])*(_matrix_eigenstrain*_X[_qp]*_dhdn[_qp] - _current_precip_misfit*_dhdn[_qp]);
  Real temp2 = temp.doubleContraction( _elastic_strain[_qp]);
  first += 2.*temp2;

  second = temp.doubleContraction( _elastic_strain[_qp]);

  temp = (_Cijkl_matrix*(1 - _h[_qp]))*(_matrix_eigenstrain*_X[_qp]*_d2hdn2[_qp] - _current_precip_misfit*_d2hdn2[_qp]);
  temp2 = temp.doubleContraction( _elastic_strain[_qp] );
  second += temp2;

  temp = (_Cijkl_matrix*(1 - _h[_qp]))*(_matrix_eigenstrain*_X[_qp]*_dhdn[_qp] - _current_precip_misfit*_dhdn[_qp]);
  temp2 = temp.doubleContraction(_matrix_eigenstrain*_X[_qp]*_dhdn[_qp] - _current_precip_misfit*_dhdn[_qp]);

  second += temp2;

  Real d2fel_dOP2_A = first + 2.*second;

  //B term
  temp = (_Cijkl_precip*_d2hdn2[_qp])*_elastic_strain[_qp];
  first = temp.doubleContraction( _elastic_strain[_qp] );

  temp = (_Cijkl_precip*_dhdn[_qp])*(_matrix_eigenstrain*_X[_qp]*_dhdn[_qp] - _current_precip_misfit*_dhdn[_qp]);
  temp2 = temp.doubleContraction( _elastic_strain[_qp]);
  first += 2.*temp2;

  second = temp.doubleContraction( _elastic_strain[_qp]);

  temp = (_Cijkl_precip*(_h[_qp]))*(_matrix_eigenstrain*_X[_qp]*_d2hdn2[_qp] - _current_precip_misfit*_d2hdn2[_qp]);
  temp2 = temp.doubleContraction(_elastic_strain[_qp]);

  second += temp2;

  temp = (_Cijkl_precip*(_h[_qp]))*(_matrix_eigenstrain*_X[_qp]*_dhdn[_qp] - _current_precip_misfit*_dhdn[_qp]);
  temp2 = temp.doubleContraction(_matrix_eigenstrain*_X[_qp]*_dhdn[_qp] - _current_precip_misfit*_dhdn[_qp]);

  second += temp2;

  Real d2fel_dOP2_B = first + 2.*second;

  _d2fel_dOP2[_qp] = 0.5*(d2fel_dOP2_A + d2fel_dOP2_B);



  //compute the terms for X

  temp = (_elasticity_tensor[_qp])*(-1.*(1 - _h[_qp])*_matrix_eigenstrain);

  _dfel_dX[_qp] = temp.doubleContraction(_elastic_strain[_qp]);

  _d2fel_dX2[_qp] = temp.doubleContraction(-1.*(1 - _h[_qp])*_matrix_eigenstrain);

}
