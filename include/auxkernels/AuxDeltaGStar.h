/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  25 March 2013
*
*************************************************************************/

#ifndef AUXDELTAGSTAR_H
#define AUXDELTAGSTAR_H

#include "AuxKernel.h"

class AuxDeltaGStar;

template<>
InputParameters validParams<AuxDeltaGStar>();

/**
 *  AuxDeltaGStar handles the activation energy (Delta G*) of a critical nucleus.
 */

class AuxDeltaGStar : public AuxKernel
{
public:
  AuxDeltaGStar(const InputParameters & parameters);

protected:
  /**
   * computeValue()
   * @return returns the activation energy (element average value), Delta G*.
   * Handles adaptivity and different mesh dimensions.
   */

  virtual Real computeValue();

private:
  const unsigned int _mesh_dimension;

  const VariableValue & _coupled_energy; ///< area/volume free energy change of transformation

  Real _gamma;
};

#endif //AUXDELTAGSTAR_H
