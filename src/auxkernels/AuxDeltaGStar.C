/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  25 March 2013
*
*************************************************************************/

#include "AuxDeltaGStar.h"
#include "MooseMesh.h"

#include <cmath>

registerMooseObject("HyraxApp", AuxDeltaGStar);

template<>
InputParameters validParams<AuxDeltaGStar>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled_aux_var","coupled auxiliary variable for free energy change");
  // I'd have line in input file, coupled_aux_var = supersaturation, or chem_elastic, or whatever
  params.addRequiredParam<Real>("gamma", "Surface energy");

  return params;
}

AuxDeltaGStar::AuxDeltaGStar(const InputParameters & parameters)
  : AuxKernel(parameters),
    _mesh_dimension(_mesh.dimension()),
    _coupled_energy(coupledValue("coupled_aux_var")),
    _gamma(getParam<Real>("gamma"))
{
}

Real
AuxDeltaGStar::computeValue()
{
  Real kn2;
  Real alpha;

  //computing this as if it is 3D
  alpha = (16*libMesh::pi)/3;

  kn2 = alpha*_gamma*_gamma*_gamma;

  return kn2/(_coupled_energy[_qp]*_coupled_energy[_qp]);
}
