/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  13 February 2014
*
*************************************************************************/

#include "PrecipitateMatrixMisfitMaterial.h"

// libMesh includes
#include "libmesh/quadrature.h"

template<>
InputParameters validParams<PrecipitateMatrixMisfitMaterial>()
{
  InputParameters params = validParams<LinearSingleCrystalPrecipitateMaterial>();
  // matrix material into C_ijkl
  // precipitate material into C_precipitate
  // precipitate misfit into e_precipitate
  // number of precip variants into n_variants
  // order parameter coupled variables into variable_names
  // scaling factor for energy nondimensionalization into scaling_factor
  // temperature dependence of precip misfit coeffs into misfit_temperature_coeffs

  params.addRequiredParam<std::vector<Real> >("e_matrix","Eigenstrain tensor for solute in matrix: e11, e22, e33, e23, e13, e12");
  params.addRequiredCoupledVar("solute_name","coupled variable name of solute in matrix");

 params.addParam<Real>("percent_matrix_misfit", 1, "percent of misfit strain of matrix to be applied, between 0 and 1");
 params.addParam<Real>("precip_conc", 0.6, "concentration of precipitate");
 params.addParam<Real>("precip_OP", 1, "concentration of precipitate");

 return params;
}

PrecipitateMatrixMisfitMaterial::PrecipitateMatrixMisfitMaterial(const InputParameters & parameters) :
    LinearSingleCrystalPrecipitateMaterial(parameters),
    _eigenstrain_matrix_vector(getParam<std::vector<Real> >("e_matrix")),
    _eigenstrain_matrix(),

    _eigenstrain_matrix_MP(declareProperty<RankTwoTensor>("eigenstrain_matrix_MP")),
    _dn_eigenstrain_matrix_MP(declareProperty<std::vector<RankTwoTensor> >("dn_eigenstrain_matrix_MP")),
    _dc_eigenstrain_matrix_MP(declareProperty<RankTwoTensor>("dc_eigenstrain_matrix_MP")),
    _matrix_eigenstrain(declareProperty<RankTwoTensor>("matrix_eigenstrain")),

    _dn_elasticity_tensor(declareProperty<std::vector<RankFourTensor> >("dn_elasticity_tensor")),
    _dc_elasticity_tensor(declareProperty<RankFourTensor>("dc_elasticity_tensor")),
    _dndn_elasticity_tensor(declareProperty<std::vector<RankFourTensor> >("dndn_elasticity_tensor")),
    _dcdc_elasticity_tensor(declareProperty<RankFourTensor>("dcdc_elasticity_tensor")),
    _dcdn_elasticity_tensor(declareProperty<std::vector<RankFourTensor> >("dcdn_elasticity_tensor")),

    _dn_misfit_strain(declareProperty<std::vector<RankTwoTensor> >("dn_misfit_strain")),
    _dc_misfit_strain(declareProperty<RankTwoTensor>("dc_misfit_strain")),
    _dndn_misfit_strain(declareProperty<std::vector<RankTwoTensor> >("dndn_misfit_strain")),
    _dcdc_misfit_strain(declareProperty<RankTwoTensor>("dcdc_misfit_strain")),
    _dcdn_misfit_strain(declareProperty<std::vector<RankTwoTensor> >("dcdn_misfit_strain")),

    _solute(coupledValue("solute_name")),
    _percent_matrix_misfit(getParam<Real>("percent_matrix_misfit"))
{
  //make sure misfit strain application is between 0 and 100%
  if (_percent_matrix_misfit > 1 || _percent_matrix_misfit < 0)
    mooseError("Please give a percent matrix misfit between 0 and 1 (PMMM)");

  // fill in the original tensors.  Don't touch after this!
  _eigenstrain_matrix.fillFromInputVector(_eigenstrain_matrix_vector);
}

void
PrecipitateMatrixMisfitMaterial::computeProperties()
{
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _eigenstrains_MP[_qp].resize(_n_variants);
    _d_eigenstrains_MP[_qp].resize(_n_variants);
    _dn_eigenstrain_matrix_MP[_qp].resize(_n_variants);

    _precipitate_eigenstrain[_qp].resize(_n_variants);

    _dn_elasticity_tensor[_qp].resize(_n_variants);
    _dndn_elasticity_tensor[_qp].resize(_n_variants);
    _dcdn_elasticity_tensor[_qp].resize(_n_variants);

    _dn_misfit_strain[_qp].resize(_n_variants);
    _dcdn_misfit_strain[_qp].resize(_n_variants);
    _dndn_misfit_strain[_qp].resize(_n_variants);
    _dcdn_misfit_strain[_qp].resize(_n_variants);

    computeQpElasticityTensor();
    computeQpEigenstrain();

    computeQpElasticStrain();
    computeQpElasticStress();
  }
}

void
PrecipitateMatrixMisfitMaterial::computeQpElasticityTensor()
{
  //going to need to put temperature dependence in here

  Real inverse = 1/_scaling_factor;
  RankFourTensor zeros;
  zeros.zero();

  //scale these so everything is scaled
  _Cijkl_MP[_qp] = _Cijkl/inverse;
  _Cijkl_precipitates_MP[_qp] = _Cijkl_precipitate/inverse;

  // Real sum_OP = 0;
  for (unsigned int i=0; i<_n_variants; i++)
  {
    //  (_dn_elasticity_tensor[_qp])[i] = ( (_Cijkl_precipitate - _Cijkl)*2*(*_OP[i])[_qp] )/inverse;
    //  (_dndn_elasticity_tensor[_qp])[i] = ( (_Cijkl_precipitate - _Cijkl)*2 )/inverse;
    //  (_dcdn_elasticity_tensor[_qp])[i] = zeros;
    // sum_OP += ( (*_OP[i])[_qp] )*( (*_OP[i])[_qp] );

    (_dn_elasticity_tensor[_qp])[i] = zeros;
    (_dndn_elasticity_tensor[_qp])[i] = zeros;
    (_dcdn_elasticity_tensor[_qp])[i] = zeros;
  }

  // _elasticity_tensor[_qp] = (_Cijkl + (_Cijkl_precipitate - _Cijkl)*sum_OP)/inverse;
  // _dc_elasticity_tensor[_qp] = zeros;
  // _dcdc_elasticity_tensor[_qp] = zeros;
  // _Jacobian_mult[_qp] = (_Cijkl + (_Cijkl_precipitate - _Cijkl)*sum_OP)/inverse;

  _elasticity_tensor[_qp] = _Cijkl/inverse;
  _dc_elasticity_tensor[_qp] = zeros;
  _dcdc_elasticity_tensor[_qp] = zeros;
  _Jacobian_mult[_qp] = _Cijkl/inverse;


}

void
PrecipitateMatrixMisfitMaterial::computeQpEigenstrain()
{
  computeQpPrecipitateEigenstrain();
  computeQpMatrixEigenstrain();
}

void
PrecipitateMatrixMisfitMaterial::computeQpPrecipitateEigenstrain()
{
  LinearSingleCrystalPrecipitateMaterial::computeQpEigenstrain();
}

void
PrecipitateMatrixMisfitMaterial::computeQpMatrixEigenstrain()
{
  Real interpolation_value(0);
  //RankTwoTensor current_misfit;
  Real solute = _solute[_qp];

  // make sure you don't go outside the [0,1] interval for concentration
  if (solute < 0)
   solute = 0;

  _current_matrix_misfit = _eigenstrain_matrix*_percent_matrix_misfit;

  for (unsigned int i=0; i<_n_variants; i++)
  {

   interpolation_value =+ (*_OP[i])[_qp]*(*_OP[i])[_qp];

   //this should have a negative actually unless it's accounted for elsewhere:
   // it is in computeQpMisfitStrain
   (_dn_eigenstrain_matrix_MP[_qp])[i] = _current_matrix_misfit*2*solute*(*_OP[i])[_qp];
  }

  _eigenstrain_matrix_MP[_qp] = _current_matrix_misfit*solute*(1 - interpolation_value);

  _dc_eigenstrain_matrix_MP[_qp] = _current_matrix_misfit*(1 - interpolation_value);

  //I don't think this is used anywhere
  _matrix_eigenstrain[_qp] = _current_matrix_misfit;
}

void
PrecipitateMatrixMisfitMaterial::computeQpElasticStrain()
{
  // compute the elastic strain: e_el = e_local - e_misfit

  //compute the total strain
  computeQpStrain();

  //compute the overall misfit strain
  computeQpMisfitStrain();

  _elastic_strain[_qp] = _local_strain[_qp] - _misfit_strain[_qp];
}

void
PrecipitateMatrixMisfitMaterial::computeQpMisfitStrain()
{
  //sum up the misfit strains for the orientation variants
  RankTwoTensor sum_precipitate_strains;
  RankTwoTensor zeros;
  sum_precipitate_strains.zero();
  zeros.zero();

  Real OP_sum = 0;

  Real solute = _solute[_qp];

  if (solute < 0)
   solute = 0;

  for(unsigned int i=0; i<_n_variants; i++)
  {
    OP_sum += (*_OP[i])[_qp]*(*_OP[i])[_qp];

    sum_precipitate_strains += (_eigenstrains_MP[_qp])[i];

    //negative on 2nd term because of the formulation for it
    (_dn_misfit_strain[_qp])[i] = (_d_eigenstrains_MP[_qp])[i] - (_dn_eigenstrain_matrix_MP[_qp])[i];

    //Signs here are correct for the rest of the formulation?
    (_dcdn_misfit_strain[_qp])[i] = ( _current_matrix_misfit* (*_OP[i])[_qp] )*-2;
    (_dndn_misfit_strain[_qp])[i] = -2*solute*_current_matrix_misfit + 2*_eigenstrains_rotated[i];
  }

  _misfit_strain[_qp] = sum_precipitate_strains + _eigenstrain_matrix_MP[_qp];
  _dc_misfit_strain[_qp] = _current_matrix_misfit*(1 - OP_sum);
  _dcdc_misfit_strain[_qp] = zeros;
}

/*
Real
PrecipitateMatrixMisfitMaterial::computeHeaviside()
{
  Real heaviside_first(0);
  Real heaviside_second(0);

  Real OP;

  //may need to put some checking in here so that OP fixed between 0 and 1
  for(unsigned int i=0; i<_n_variants; i++)
  {
    OP = (*_OP[i])[_qp];
    // if (OP < 0)
    //  OP = 0;
    //if (OP > 1)
    //  OP = 1;

    heaviside_first += std::pow(OP, 2);
    heaviside_second += std::pow(OP, 3);
  }

  return 3*heaviside_first - 2*heaviside_second;
}

Real
PrecipitateMatrixMisfitMaterial::computeDHeaviside(unsigned int i)
{
  Real OP = (*_OP[i])[_qp];
  //if (OP < 0)
  //  OP = 0;
  //if (OP > 1)
  //  OP = 1;

  return 6*OP*(1 - OP);
}

Real
PrecipitateMatrixMisfitMaterial::computeD2Heaviside(unsigned int i)
{
  Real OP = (*_OP[i])[_qp];
  //if (OP < 0)
  //  OP = 0;
  //if (OP > 1)
  //  OP = 1;

  return 6*(1 - OP);
}
*/
