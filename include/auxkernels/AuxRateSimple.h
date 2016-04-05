/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  18 April 2013
*
*************************************************************************/

#ifndef AUXRATESIMPLE_H
#define AUXRATESIMPLE_H

#include "AuxKernel.h"

class AuxRateSimple;

template<>
InputParameters validParams<AuxRateSimple>();

/**
 * AuxRateSimple is designed to work with AuxSupersaturation (following
 * algorithm in J. P. Simmons (2000) paper).  It results in a volumetric
 * nucleation rate (does not take into account mesh volume).
 */

class AuxRateSimple : public AuxKernel
{
public:
  AuxRateSimple(const InputParameters & parameters);

protected:
  /**
   * computeValue()
   * @return returns the nucleation rate (element average value), j_star.
   * Handles adaptivity and different mesh dimensions.
   * j_star = Kn1 * exp(-1*Kn2 / supersaturation)
   */

  virtual Real computeValue();

private:
  const unsigned int _mesh_dimension;

  const VariableValue & _coupled_energy; //free energy change of transformation
  Real _Kn1;                       //First nucleation rate value
  Real _Kn2;                       //Second nucleation rate value

};

#endif //AUXRATESIMPLE_H
