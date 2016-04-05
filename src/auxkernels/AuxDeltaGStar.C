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

  // handling the dimension of the problem here and making sure we get the correct
  // (areal or volume) density
   if (_mesh_dimension == 2)
     alpha = libMesh::pi;
   else if (_mesh_dimension == 3)
     alpha = (16*libMesh::pi)/3;
   else
    mooseError("honky, your problem dimesion must be 2 or 3 (AuxDeltaGStar");

   kn2 = alpha*std::pow(_gamma, (int)_mesh_dimension);

  return kn2/std::pow(_coupled_energy[_qp], (int)_mesh_dimension-1);
}

