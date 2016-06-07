/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  27 December 2013
*
*************************************************************************/

#ifndef AUXGUOENERGY_H
#define AUXGUOENERGY_H

#include "AuxChemElastic.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

//forward declaration
class AuxGuoEnergy;

template<>
InputParameters validParams<AuxGuoEnergy>();

class AuxGuoEnergy : public AuxChemElastic
{
public:
  AuxGuoEnergy(const InputParameters & parameters);

protected:
  // virtual Real computeValue();

  //virtual Real computeEnergy(Real & conserved, Real & nonconserved, bool matrix);
  //virtual Real computeDifferential(Real & coupled_conserved,Real & nonconserved);

  virtual Real computeFchem(Real & conserved, Real & nonconserved);
  //virtual Real computeSelfElasticEnergy(bool matrix);
  //virtual Real computeInteractionElasticEnergy(bool matrix);

  virtual Real computeDfchemDcons(Real & coupled_conserved, Real & coupled_nonconserved);
  virtual Real computeDselfDcons();
  virtual Real computeDintDcons();

  virtual Real computeDfchemDnoncons(Real & coupled_conserved, Real & coupled_nonconserved);
  //virtual Real computeDselfDnoncons();
  //virtual Real computeDintDnoncons();

  /*
  const VariableValue & _coupled_cons;
  const VariableValue & _coupled_noncons;

  Real _precip_conserved;
  Real _precip_nonconserved;
  */


  // unsigned int _n_variants;
  /*
  unsigned int _noncons_var_num;

  const MaterialProperty<std::vector<RankTwoTensor> > & _eigenstrains_rotated_MP;
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;

  const MaterialProperty<std::vector<RankTwoTensor> > & _precipitate_eigenstrain_rotated;
  const MaterialProperty<RankFourTensor> & _precipitate_elasticity;

  const MaterialProperty<RankTwoTensor> & _local_strain;

  const MaterialProperty<std::vector<RankTwoTensor> > & _d_eigenstrains_rotated_MP;
  */

private:
  const MaterialProperty<Real> & _a1;
  const MaterialProperty<Real> & _a2;
  const MaterialProperty<Real> & _a3;
  const MaterialProperty<Real> & _a4;
  const MaterialProperty<Real> & _c1;
  const MaterialProperty<Real> & _c2;
};

#endif //AUXGUOENERGY_H
