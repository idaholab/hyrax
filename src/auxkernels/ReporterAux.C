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

#include "ReporterAux.h"

template<>
InputParameters validParams<ReporterAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled", "Coupled Value to report");
  return params;
}

ReporterAux::ReporterAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_val(coupledValue("coupled"))
{
}


Real
ReporterAux::computeValue()
{
  return _coupled_val[_qp];
}
