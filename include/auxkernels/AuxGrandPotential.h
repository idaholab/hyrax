/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 21 December 2015
*
*************************************************************************/

#ifndef AUXGRANDPOTENTIAL_H
#define AUXGRANDPOTENTIAL_H

#include "AuxKernel.h"

class AuxGrandPotential;

template<>
InputParameters validParams<AuxGrandPotential>();

class AuxGrandPotential : public AuxKernel
{
public:
    AuxGrandPotential(const InputParameters & parameters);

protected:
  virtual Real computeValue();

private:

  const VariableValue & _X;

  const VariableGradient & _grad_X;
  const VariableGradient & _grad_OP;

  const MaterialProperty<Real> & _fbulk;

  const MaterialProperty<Real> & _dfbulk_dc;

  const MaterialProperty<Real> & _kappa_X;
  const MaterialProperty<Real> & _kappa_OP;

  Real _omega_eq;

};

#endif //AUXGRANDPOTENTIAL_H
