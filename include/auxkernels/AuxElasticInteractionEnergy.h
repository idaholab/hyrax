/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
* 5 December 2014
*
*************************************************************************/

#ifndef AUXELASTICINTERACTIONENERGY_H
#define AUXELASTICINTERACTIONENERGY_H

#include "AuxKernel.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

class AuxElasticInteractionEnergy;

template<>
InputParameters validParams<AuxElasticInteractionEnergy>();

class AuxElasticInteractionEnergy : public AuxKernel
{
public:
    AuxElasticInteractionEnergy(const InputParameters & parameters);

protected:
  virtual Real computeValue();

private:

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<std::vector<RankTwoTensor> > & _precipitate_eigenstrain;

  unsigned int _OP_number;

  //Real _scaling_factor;

};

#endif //AUXELASTICINTERACTIONENERGY_H
