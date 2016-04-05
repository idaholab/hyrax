/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 21 December 2015
*
*************************************************************************/

#ifndef AUXCANONICALENSEMBLE_H
#define AUXCANONICALENSEMBLE_H

#include "AuxKernel.h"

class AuxCanonicalEnsemble;

template<>
InputParameters validParams<AuxCanonicalEnsemble>();

class AuxCanonicalEnsemble : public AuxKernel
{
public:
    AuxCanonicalEnsemble(const InputParameters & parameters);

protected:
  virtual Real computeValue();

private:

  const VariableValue & _X;
  const VariableValue & _OP;

  const VariableGradient & _grad_X;
  const VariableGradient & _grad_OP;
  
  const MaterialProperty<Real> & _molar_vol;

  const MaterialProperty<Real> & _Galpha;
  const MaterialProperty<Real> & _Gdelta;
  
  const MaterialProperty<Real> & _dGalpha_dc;
  const MaterialProperty<Real> & _dGdelta_dc;

  const MaterialProperty<Real> & _kappa_X;
  const MaterialProperty<Real> & _kappa_OP;

  
  Real _omega_eq;
  
  Real _length_scale_factor;
  Real _energy_scale_factor;

//Real _scaling_factor;

};

#endif //AUXCANONICALENSEMBLE_H
