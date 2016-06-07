/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  27 December 2013
*
*************************************************************************/

#ifndef AUXCALPHADENERGY_H
#define AUXCALPHADENERGY_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

#include "CalphadAB1CD1.h"
#include "CalphadAB1CD2.h"

//forward declaration
class AuxCalphadEnergy;

template<>
InputParameters validParams<AuxCalphadEnergy>();

class AuxCalphadEnergy : public AuxKernel
{
public:
  AuxCalphadEnergy(const InputParameters & parameters);

protected:
   virtual Real computeValue();

  //virtual Real computeEnergy(Real & conserved, Real & nonconserved, bool matrix);
  virtual Real computeDifferential();
  virtual Real computeMatrixEnergy();
  virtual Real computePrecipEnergy();

  virtual void computeHeaviside();
  virtual void computeBarrier();

  virtual void computeDHeaviside();
  virtual void computeDBarrier();

  Real _precip_cons;
  Real _precip_noncons;

  const MaterialProperty<Real> & _G_alpha;
  const MaterialProperty<Real> & _G_delta;
  const MaterialProperty<Real> & _G_alpha_precip;
  const MaterialProperty<Real> & _G_delta_precip;
  const MaterialProperty<Real> & _dG_alpha;
  const MaterialProperty<Real> & _dG_delta;

  const MaterialProperty<std::vector<RankTwoTensor> > & _dn_misfit_strain;
  const MaterialProperty<RankTwoTensor> & _dc_misfit_strain;
  const MaterialProperty<RankTwoTensor> & _elastic_strain;
  const MaterialProperty<RankTwoTensor> & _local_strain;

  const MaterialProperty<RankFourTensor> & _elasticity_tensor;
  const MaterialProperty<RankFourTensor> & _Cijkl_MP;  //matrix
  const MaterialProperty<RankFourTensor> & _Cijkl_precipitate_MP; //precipitate


  const MaterialProperty<std::vector<RankTwoTensor> > & _precipitate_eigenstrain;
  const MaterialProperty<RankTwoTensor> & _matrix_eigenstrain;

  const MaterialProperty<Real> & _Omega;
  const MaterialProperty<Real> & _W;

  unsigned int _OP_number;
  unsigned int _n_OP_vars;
  std::vector<const VariableValue *> _OP;

  const VariableValue & _X;

  Real _H;
  Real _g;
  Real _dH_dOP;
  Real _dg_dOP;

private:
};

#endif //AUXCALPHADENERGY_H
