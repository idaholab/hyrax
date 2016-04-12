/******************************************************
 *
 *   Welcome to HYRAX!
 *
 *   Developer: Andrea Jokisaari
 *
 *   5 April 2016
 *
 *****************************************************/

#ifndef TWOPHASELINEARELASTICMATERIAL_H
#define TWOPHASELINEARELASTICMATERIAL_H

#include "Material.h"
#include "RankTwoTensor.h"
#include "ElasticityTensorR4.h"

//forward declarations
class TwoPhaseLinearElasticMaterial;

/**
 * TwoPhaseLinearElasticMaterial handles anisotropic, single-crystal
 * material elastic constants. It handles a single crystal of matrix with a
 * precipitate phase of any elastic constants.  Both the matrix and precipitate can
 * have a misfit strain.  The model assumes a coherent precipitate. A simple factor is
 * included for the precipitate's misfit strain to reduce it uniformly (a model of
 * misfit strain relaxation via dislocation formation, for example).
 */

template<>
InputParameters validParams<TwoPhaseLinearElasticMaterial>();

class TwoPhaseLinearElasticMaterial : public Material
{
public:
  TwoPhaseLinearElasticMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();
  virtual void computeInterpolation();

  virtual void computeQpElasticityTensor();
  virtual void computeQpStrain();
  virtual void computeQpStress();

  virtual void computeEnergy();

  const VariableValue & _OP;
  const VariableValue & _X;
  const VariableValue & _T;

  const VariableGradient & _grad_disp_x;
  const VariableGradient & _grad_disp_y;
  const VariableGradient & _grad_disp_z;

  Real _h;
  Real _dhdn;
  Real _d2hdn2;

  MaterialProperty<RankTwoTensor> & _stress;

  MaterialProperty<RankTwoTensor> & _total_strain;
  MaterialProperty<RankTwoTensor> & _elastic_strain;
  MaterialProperty<RankTwoTensor> & _misfit_strain;

  MaterialProperty<ElasticityTensorR4> & _elasticity_tensor;
  MaterialProperty<ElasticityTensorR4> & _Jacobian_multiplier;

  MaterialProperty<Real> & _f_el;
  MaterialProperty<Real> & _dfel_dOP;
  MaterialProperty<Real> & _d2fel_dOP2;
  MaterialProperty<Real> & _dfel_dX;
  MaterialProperty<Real> & _d2fel_dX2;

  ElasticityTensorR4 _Cijkl_matrix;
  ElasticityTensorR4 _Cijkl_precip;

  std::vector<Real> _precipitate_eigenstrain_vector;
  std::vector<Real> _matrix_eigenstrain_vector;
  std::vector<Real> _precip_misfit_T_coeffs_vector;

  RankTwoTensor _precipitate_eigenstrain;
  RankTwoTensor _matrix_eigenstrain;

  RankTwoTensor _precip_misfit_T_coeffs;
  Real _percent_precip_misfit;
  RankTwoTensor _current_precip_misfit;

private:
};

#endif //TWOPHASELINEARELASTICMATERIAL_H
