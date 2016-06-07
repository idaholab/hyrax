/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  12 January 2012
*
*************************************************************************/

#include "LinearSingleCrystalPrecipitateMaterial.h"

// libMesh includes
#include "libmesh/quadrature.h"

/**
 * LinearSingleCrystalPrecipitateMaterial handles anisotropic,
 * single-crystal material elastic constants.  It handles a single
 * crystal of matrix with an arbitrary number of orientation variants of
 * a coherent precipitate.
 *
 * The matrix material goes into C_ijkl, while the precipitate info goes into
 * C_ijkl_precipitate
 */

template<>
InputParameters validParams<LinearSingleCrystalPrecipitateMaterial>()
{
  InputParameters params = validParams<TensorMechanicsMaterial>();
  // matrix material into C_ijkl
  params.addRequiredParam<std::vector<Real> >("C_precipitate", "Stiffness tensor for precipitate");
  params.addRequiredParam<std::vector<Real> >("e_precipitate","Eigenstrain tensor for precipitate: e11, e22, e33, e23, e13, e12");
  params.addRequiredParam<int>("n_variants","# of orientation variants for precipitate in single crystal");

  params.addCoupledVar("temperature", 0.0, "Temperature in Kelvin");
  params.addRequiredCoupledVar("variable_names", "Array of coupled variable names");

  params.addParam<Real>("scaling_factor", 1, "free energy scaling factor for nondimensionalization");

  params.addParam<std::vector<Real> >("misfit_temperature_coeffs", "misfit strain temperature coefficients, e11, e22, e33, e33, e23, e13, e12");
  params.addParam<Real>("percent_precip_misfit", 1, "percent of misfit strain of precip to be applied, between 0 and 1");

  return params;
}

LinearSingleCrystalPrecipitateMaterial::LinearSingleCrystalPrecipitateMaterial(const InputParameters & parameters) :
    TensorMechanicsMaterial(parameters),
    _eigenstrain_vector(getParam<std::vector<Real> >("e_precipitate")),
    _scaling_factor(getParam<Real>("scaling_factor")),

    _Cijkl_precipitate(getParam<std::vector<Real> >("C_precipitate"), (RankFourTensor::FillMethod)(int)getParam<MooseEnum>("fill_method")),

    _eigenstrain(),
    _T(coupledValue("temperature")),
    _n_variants(getParam<int>("n_variants")),
    _eigenstrains_rotated(),

    _local_strain(declareProperty<RankTwoTensor >("local_strain")),
    _misfit_strain(declareProperty<RankTwoTensor >("misfit_strain")),

    _eigenstrains_MP(declareProperty<std::vector<RankTwoTensor> >("eigenstrains_MP")),
    _Cijkl_MP(declareProperty<RankFourTensor>("Cijkl_MP")),
    _Cijkl_precipitates_MP(declareProperty<RankFourTensor>("Cijkl_precipitates_MP")),
    _d_eigenstrains_MP(declareProperty<std::vector<RankTwoTensor> >("d_eigenstrains_MP")),
    _precipitate_eigenstrain(declareProperty<std::vector<RankTwoTensor> >("precipitate_eigenstrain")),
    _misfit_T_coeffs_vector(getParam<std::vector<Real> >("misfit_temperature_coeffs")),
    _percent_precip_misfit(getParam<Real>("percent_precip_misfit"))

{
  // check to make sure the input file is all set up right
  if(_n_variants != coupledComponents("variable_names"))
    mooseError("Please match the number of orientation variants with coupled order parameters (LSXPM).");

  // make sure you don't do something stupid with the temperature coeffcients in the input file
  if (_misfit_T_coeffs_vector.size() != 6)
  {
    mooseWarning("No misfit temperature coefficients (LSXPM) specified.");
    _misfit_T_coeffs_vector.assign(6, 0);
  }

  //make sure your misfit strain application is between 0 and 100%
  if (_percent_precip_misfit > 1 || _percent_precip_misfit < 0)
    mooseError("Please give a percent precip misfit between 0 and 1 (LSXPM)");

  // size vectors appropriately
  _OP.resize(_n_variants);
  _eigenstrains_rotated.resize(_n_variants);

  // populate with data
  for(unsigned int i=0; i < _n_variants; i++)
    _OP[i] = &coupledValue("variable_names", i);

  // fill in the original tensors.  Don't touch after this!
  _eigenstrain.fillFromInputVector(_eigenstrain_vector);
  _misfit_T_coeffs.fillFromInputVector(_misfit_T_coeffs_vector);
}

void
LinearSingleCrystalPrecipitateMaterial::computeProperties()
{
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    // resize all the material properties vectors.  Don't forget this.
    _eigenstrains_MP[_qp].resize(_n_variants);
    _d_eigenstrains_MP[_qp].resize(_n_variants);
    _precipitate_eigenstrain[_qp].resize(_n_variants);

    computeQpElasticityTensor();
    computeQpEigenstrain();
    computeQpElasticStrain();
    computeQpElasticStress();
  }
}

void
LinearSingleCrystalPrecipitateMaterial::computeQpElasticityTensor()
{
  Real inverse = 1/_scaling_factor;

  _elasticity_tensor[_qp] = _Cijkl/inverse;
  _Jacobian_mult[_qp] = _Cijkl/inverse;
  _Cijkl_MP[_qp] = _Cijkl/inverse;
  _Cijkl_precipitates_MP[_qp] = _Cijkl/inverse;
}

void
LinearSingleCrystalPrecipitateMaterial::computeQpEigenstrain()
{
  Real interpolation_value(0.0);
  Real d_interp_value(0.0);

  // calculate the current misfit strain for the first orientation
  _current_precip_misfit = (_eigenstrain + _misfit_T_coeffs * _T[_qp]) * _percent_precip_misfit;

  // calculate the rotations and fill in
  // fill in the first variant without rotation
  _eigenstrains_rotated[0] = _current_precip_misfit;

  // rotate all the things, in radians
  Real rotation_angle_base = 2.0*libMesh::pi/Real(_n_variants);
  Real rotation_angle = rotation_angle_base;

  for(unsigned int i=1; i<_n_variants; i++)
  {
    _eigenstrains_rotated[i] = _current_precip_misfit.rotateXyPlane(rotation_angle);

    // increment the rotation angle for the next go-round
    rotation_angle = rotation_angle + rotation_angle_base;
  }

  //fill in the material properties
  for (unsigned int i=0; i<_n_variants; i++)
  {

    interpolation_value = (*_OP[i])[_qp] * (*_OP[i])[_qp];
    d_interp_value = 2.0 * (*_OP[i])[_qp];

    (_eigenstrains_MP[_qp])[i] = _eigenstrains_rotated[i] * interpolation_value;
    (_d_eigenstrains_MP[_qp])[i] = _eigenstrains_rotated[i] * d_interp_value;
    (_precipitate_eigenstrain[_qp])[i] = _eigenstrains_rotated[i];
  }
}

void
 LinearSingleCrystalPrecipitateMaterial::computeQpElasticStrain()
 {
   // compute the elastic strain: e_el = e_local - e_misfit

   computeQpStrain();

   // sum up the misfit strains for the orientation variants
   RankTwoTensor sum_precipitate_strains;
   sum_precipitate_strains.zero();

   for(unsigned int i=0; i<_n_variants; i++)
     sum_precipitate_strains += (_eigenstrains_MP[_qp])[i];

   _misfit_strain[_qp] = sum_precipitate_strains;

   _elastic_strain[_qp] = _local_strain[_qp] - _misfit_strain[_qp];
}

void
LinearSingleCrystalPrecipitateMaterial::computeQpElasticStress()
{
  computeQpStress();
}

void
LinearSingleCrystalPrecipitateMaterial::computeQpStrain()
{
  //strain = (grad_disp + grad_disp^T)/2
  RankTwoTensor grad_tensor(_grad_disp_x[_qp],_grad_disp_y[_qp],_grad_disp_z[_qp]);

  _local_strain[_qp] = (grad_tensor + grad_tensor.transpose())/2.0;
}

void
LinearSingleCrystalPrecipitateMaterial::computeQpStress()
{
  // stress = C * e
  _stress[_qp] = _elasticity_tensor[_qp]*_elastic_strain[_qp];
}
