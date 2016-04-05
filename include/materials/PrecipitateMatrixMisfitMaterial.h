/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  13 February 2014
*
*************************************************************************/

#ifndef PRECIPITATEMATRIXMISFITMATERIAL_H
#define PRECIPITATEMATRIXMISFITMATERIAL_H

#include "LinearSingleCrystalPrecipitateMaterial.h"

//forward declaration
class PrecipitateMatrixMisfitMaterial;

template<>
InputParameters validParams<PrecipitateMatrixMisfitMaterial>();

class PrecipitateMatrixMisfitMaterial : public LinearSingleCrystalPrecipitateMaterial
{
public:
  PrecipitateMatrixMisfitMaterial(const InputParameters & parameters);

protected:
  virtual void computeProperties();

  virtual void computeQpElasticityTensor();
  virtual void computeQpEigenstrain();
  virtual void computeQpPrecipitateEigenstrain();
  virtual void computeQpMatrixEigenstrain();

  virtual void computeQpElasticStrain();
  virtual void computeQpMisfitStrain();
  //virtual void computeQpElasticStress();
  //virtual void computeQpStrain();
  //virtual void computeQpStress();

//  virtual Real computeHeaviside();
//  virtual Real computeDHeaviside(unsigned int i);
//  virtual Real computeD2Heaviside(unsigned int i);

  std::vector<Real> _eigenstrain_matrix_vector;
  RankTwoTensor _eigenstrain_matrix;

  MaterialProperty<RankTwoTensor> & _eigenstrain_matrix_MP;
  MaterialProperty<std::vector<RankTwoTensor> > & _dn_eigenstrain_matrix_MP;
  MaterialProperty<RankTwoTensor> & _dc_eigenstrain_matrix_MP;

  MaterialProperty<RankTwoTensor> & _matrix_eigenstrain;  //lambda_ijkl*%factor as material property

  MaterialProperty<std::vector<ElasticityTensorR4> > & _dn_elasticity_tensor;
  MaterialProperty<ElasticityTensorR4> & _dc_elasticity_tensor;
  MaterialProperty<std::vector<ElasticityTensorR4> > & _dndn_elasticity_tensor;
  MaterialProperty<ElasticityTensorR4> & _dcdc_elasticity_tensor;
  MaterialProperty<std::vector<ElasticityTensorR4> > & _dcdn_elasticity_tensor;

  MaterialProperty<std::vector<RankTwoTensor> > & _dn_misfit_strain;
  MaterialProperty<RankTwoTensor> & _dc_misfit_strain;
  MaterialProperty<std::vector<RankTwoTensor> > & _dndn_misfit_strain;
  MaterialProperty<RankTwoTensor> & _dcdc_misfit_strain;
  MaterialProperty<std::vector<RankTwoTensor> > & _dcdn_misfit_strain;

  const VariableValue & _solute;

  Real _percent_matrix_misfit;
  RankTwoTensor _current_matrix_misfit;

  Real _precip_conc;
  Real _precip_OP;

private:
};

#endif //PRECIPITATEMATRIXMISFITMATERIAL_H

