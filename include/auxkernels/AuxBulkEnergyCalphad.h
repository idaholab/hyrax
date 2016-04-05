/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  9 January 2014
*
*************************************************************************/

#ifndef AUXBULKENERGYCALPHAD_H
#define AUXBULKENERGYCALPHAD_H

#include "AuxKernel.h"
#include "CalphadAB1CD1.h"
#include "CalphadAB1CD2.h"

class AuxBulkEnergyCalphad;

template<>
InputParameters validParams<AuxBulkEnergyCalphad>();

class AuxBulkEnergyCalphad : public AuxKernel
{
public:
  AuxBulkEnergyCalphad(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  Real computeHeaviside();
  Real computeBarrier();

private:

  const VariableValue & _C;
  std::vector<const VariableValue *> _OP;
  unsigned int _n_OP_variables;

  Real _scaling_factor;

  const MaterialProperty<Real> & _W;                            //Well height
  const MaterialProperty<Real> & _Omega;                        //Molar volume
  const MaterialProperty<Real> & _G_alpha;                      //Gmix_alpha
  const MaterialProperty<Real> & _G_delta;                      //Gmix_delta

};

#endif //AUXBULKENERGYCALPHAD_H
