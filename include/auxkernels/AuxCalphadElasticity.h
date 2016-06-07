/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  22 November 2014
*
*************************************************************************/

#ifndef AUXCALPHADELASTICITY_H
#define AUXCALPHADELASTICITY_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"
#include "ElasticityTensorR4.h"

#include "CalphadAB1CD1.h"
#include "CalphadAB1CD2.h"

#include "ElementIntegralVariablePostprocessor.h"

//forward declaration
class AuxCalphadElasticity;

template<>
InputParameters validParams<AuxCalphadElasticity>();

class AuxCalphadElasticity : public AuxKernel
{
public:
  AuxCalphadElasticity(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  virtual Real computeChemMatrixEnergy();
  virtual Real computeChemPrecipEnergy();
  virtual Real computeDifferential();
  virtual Real computeElasticEnergy();

  Real _precip_cons;
  Real _precip_noncons;
  Real _self_energy;

  const MaterialProperty<Real> & _G_alpha;
  const MaterialProperty<Real> & _G_delta;
  const MaterialProperty<Real> & _G_alpha_precip;
  const MaterialProperty<Real> & _G_delta_precip;
  const MaterialProperty<Real> & _dG_alpha;
  const MaterialProperty<Real> & _dG_delta;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _precipitate_eigenstrain;

  const MaterialProperty<Real> & _Omega;
  const MaterialProperty<Real> & _W;

  const VariableValue & _OP;
  const VariableValue & _X;
  bool _use_elastic_energy;

private:
};

#endif //AUXCALPHADELASTICITY_H
