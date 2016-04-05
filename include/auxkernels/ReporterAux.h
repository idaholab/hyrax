/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef REPORTERAUX_H
#define REPORTERAUX_H

#include "AuxKernel.h"


//Forward Declarations
class ReporterAux;

template<>
InputParameters validParams<ReporterAux>();

/**
 * Self auxiliary value
 */
class ReporterAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  ReporterAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  const VariableValue & _coupled_val;                 ///< Coupled variable
};

#endif //REPORTERAUX_H
