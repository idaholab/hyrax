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

  const MaterialProperty<Real> & _dfbulk_dc;

};

#endif //AUXELASTICENERGY_H
