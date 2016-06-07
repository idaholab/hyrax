/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  6 February 2013
*
*************************************************************************/

#ifndef AUXCHEMELASTIC_H
#define AUXCHEMELASTIC_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

//forward declaration
class AuxChemElastic;

template<>
InputParameters validParams<AuxChemElastic>();

class AuxChemElastic : public AuxKernel
{
public:
  AuxChemElastic(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  virtual Real computeEnergy(const Real & conserved, const Real & nonconserved, bool matrix);
  virtual Real computeDifferential(const Real & coupled_conserved, const Real & nonconserved);

  virtual Real computeFchem(const Real & conserved, const Real & nonconserved);
  virtual Real computeSelfElasticEnergy(bool matrix);
  virtual Real computeInteractionElasticEnergy(bool matrix);

  virtual Real computeDfchemDcons(const Real & coupled_conserved, const Real & coupled_nonconserved);
  virtual Real computeDselfDcons();
  virtual Real computeDintDcons();

  virtual Real computeDfchemDnoncons(const Real & coupled_conserved, const Real & coupled_nonconserved);
  virtual Real computeDselfDnoncons();
  virtual Real computeDintDnoncons();

  const VariableValue & _coupled_cons;
  const VariableValue & _coupled_noncons;

  Real _precip_conserved;
  Real _precip_nonconserved;

  /*MaterialProperty<Real> & _a1;
  const MaterialProperty<Real> & _a2;
  const MaterialProperty<Real> & _a3;
  const MaterialProperty<Real> & _a4;
  const MaterialProperty<Real> & _c1;
  const MaterialProperty<Real> & _c2; */

  // unsigned int _n_variants;
  unsigned int _noncons_var_num;

  const MaterialProperty<std::vector<RankTwoTensor> > & _eigenstrains_rotated_MP;
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;

  const MaterialProperty<std::vector<RankTwoTensor> > & _precipitate_eigenstrain_rotated;
  const MaterialProperty<RankFourTensor> & _precipitate_elasticity;

  const MaterialProperty<RankTwoTensor> & _local_strain;

  const MaterialProperty<std::vector<RankTwoTensor> > & _d_eigenstrains_rotated_MP;
};

#endif //AUXCHEMELASTIC_H
