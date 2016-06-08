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

private:

  const MaterialProperty<Real> & _fbulk;

};

#endif //AUXBULKENERGYCALPHAD_H
