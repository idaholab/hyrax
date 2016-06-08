/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 25 April 2015
*
*************************************************************************/

#ifndef AUXDFELDC_H
#define AUXDFELDC_H

#include "AuxKernel.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

class AuxDFelDC;

template<>
InputParameters validParams<AuxDFelDC>();

class AuxDFelDC : public AuxKernel
{
public:
    AuxDFelDC(const InputParameters & parameters);

protected:
  virtual Real computeValue();

private:

  const MaterialProperty<Real> & _dfel_dc;

};

#endif //AUXDFELDC_H
