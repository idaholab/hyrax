/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 9 January 2014
*
*************************************************************************/

#ifndef AUXELASTICENERGY_H
#define AUXELASTICENERGY_H

#include "AuxKernel.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

class AuxElasticEnergy;

template<>
InputParameters validParams<AuxElasticEnergy>();

class AuxElasticEnergy : public AuxKernel
{
public:
    AuxElasticEnergy(const InputParameters & parameters);

protected:
  virtual Real computeValue();

private:

  const MaterialProperty<Real> & _fel;

};

#endif //AUXELASTICENERGY_H
