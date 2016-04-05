/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  11 October 2013
*
*************************************************************************/

#ifndef AUXTEMPERATURE_H
#define AUXTEMPERATURE_H

#include "AuxKernel.h"

//forward declaration
class AuxTemperature;

template<>
InputParameters validParams<AuxTemperature>();

class AuxTemperature : public AuxKernel
{
public:
  AuxTemperature(const InputParameters & parameters);

protected:

  /**
   * computeValue()
   * @return returns the supersaturation (C-C1), element average value
   */

  virtual Real computeValue();

private:
  Real _T;

};

#endif //AUXTEMPERATURE_H
