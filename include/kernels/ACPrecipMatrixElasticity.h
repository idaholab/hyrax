/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  14 February 2014
*
*************************************************************************/

#ifndef ACPRECIPMATRIXELASTICITY_H
#define ACPRECIPMATRIXELASTICITY_H

#include "ACBulk.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

class ACPrecipMatrixElasticity;

template<>
InputParameters validParams<ACPrecipMatrixElasticity>();

class ACPrecipMatrixElasticity : public ACBulk<Real>
{
public:

  ACPrecipMatrixElasticity(const InputParameters & parameters);

protected:

  /**
   * computeDFDOP()
   * @return returns the partial(elastic free energy/order parameter)
   */
  virtual Real computeDFDOP(PFFunctionType type);

  // system elasticity tensor, varies in space
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;
  const MaterialProperty<std::vector<RankFourTensor> > & _dn_elasticity_tensor;
  const MaterialProperty<std::vector<RankFourTensor> > & _dndn_elasticity_tensor;

  const MaterialProperty<RankTwoTensor> & _elastic_strain;
  const MaterialProperty<std::vector<RankTwoTensor> > & _dn_misfit_strain;
  const MaterialProperty<std::vector<RankTwoTensor> > & _dndn_misfit_strain;

  // orientation variant number for this kernel (1 to n)
  unsigned int _OP_number;

  Real _scaling_factor;

private:

};

#endif //ACPRECIPMATRIXELASTICITY_H
