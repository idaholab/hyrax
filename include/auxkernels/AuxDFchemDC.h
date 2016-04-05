/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 25 April 2015
*
*************************************************************************/

#ifndef AUXDFCHEMDC_H
#define AUXDFCHEMDC_H

#include "AuxKernel.h"

class AuxDFchemDC;

template<>
InputParameters validParams<AuxDFchemDC>();

class AuxDFchemDC : public AuxKernel
{
public:
    AuxDFchemDC(const InputParameters & parameters);

protected:
  virtual Real computeValue();

private:

  const VariableValue & _X;
  const VariableValue & _OP;

  const MaterialProperty<Real> & _Omega;
  const MaterialProperty<Real> & _dGalpha_dc;
  const MaterialProperty<Real> & _dGdelta_dc;

//Real _scaling_factor;

};

#endif //AUXELASTICENERGY_H
