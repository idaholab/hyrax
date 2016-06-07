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

  const MaterialProperty<RankFourTensor> & _elasticity_tensor;
  const MaterialProperty<RankTwoTensor> & _elastic_strain;
  const MaterialProperty<RankTwoTensor> & _dc_misfit_strain;

//Real _scaling_factor;

};

#endif //AUXDFELDC_H
