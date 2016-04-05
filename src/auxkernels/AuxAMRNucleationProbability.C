/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  20 March 2015
*
*************************************************************************/

#include "AuxAMRNucleationProbability.h"
#include "MooseMesh.h"

/**
 *  AuxAMRNucleationProbability handles the nucleation probability (P_nm)
 *  calculation in the concurrent nucleation and growth algorithm first proposed
 *  by J.P. Simmons (2000).  Returns the nucleation probability for an adaptive
 *  mesh in 2 or 3 dimensions.  Note that code this REQUIRES a volumetric nucleation rate.
 *
 *  This code operates in 2D or 3D.  If in 2D, it calculates the element volume as a slice
 *  through a 3D volume, with some user-specified thickness times the element area.
 */

template<>
InputParameters validParams<AuxAMRNucleationProbability>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled_aux_var","volumetric nucleation rate");
  params.addRequiredCoupledVar("coupled_variable", "coupled order parameter variable");
  params.addParam<Real>("OP_threshold", 0.01, "Threshold value for determining existence within 2nd phase");
  params.addParam<Real>("2D_mesh_height", -1, "height of 2D mesh to perform 3D rate calculation, ensure correct dimensionality");

  return params;
}

AuxAMRNucleationProbability::AuxAMRNucleationProbability(const InputParameters & parameters)
  : AuxKernel(parameters),
    _coupled_nuc_rate(coupledValue("coupled_aux_var")),
    _coupled_OP(coupledValue("coupled_variable")),
    _OP_threshold(getParam<Real>("OP_threshold")),
    _2D_mesh_height(getParam<Real>("2D_mesh_height")),
    _mesh_dimension(_mesh.dimension())
{
  if(_mesh_dimension == 2 && _2D_mesh_height < 0)
    mooseError("Please supply a mesh height (AuxAMRNucleationProbability)");

  if(_mesh_dimension == 1)
    mooseError("Only works in 2 and 3 dimensions (AuxAMRNucleationProbability)");
}

Real
AuxAMRNucleationProbability::computeValue()
{
  //don't nucleate in an existing particle
  if ( _coupled_OP[_qp] > _OP_threshold )
    return -1;

  /*get mesh area in 2D or volume in 3D - this will have whatever dimension or
  nondimensionalized scale you supply in input file*/

  Real vol;
  if (_mesh_dimension == 2)
    vol = _current_elem_volume*_2D_mesh_height;
  else vol = _current_elem_volume;

  /*Make sure the dimensions of nucleation rate calculation match up with the rest of
  the simulation! Note that if using MeshSolutionModify executioner, mesh refinement
  is applied AFTER the values are computed. Therefore, nucleation probability with AMR
  is going to be useless output.*/

  return 1.0 - exp(-1.0*_coupled_nuc_rate[_qp]*_dt*vol);
}

