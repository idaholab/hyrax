/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  16 December 2013
*
*************************************************************************/

#include "InitialSolutionAdaptiveDT.h"
#include <iostream>

template<>
InputParameters validParams<InitialSolutionAdaptiveDT>()
{
  InputParameters params = validParams<SolutionTimeAdaptiveDT>();

  params.addParam<Real>("initial_dt", -1, "initial tiny timestep size to start simulation");
  params.addParam<bool>("has_initial_dt",false, "boolean for if you have tiny initial timestep");
  params.addParam<unsigned int>("n_initial_steps", 1, "how many initial tiny timesteps to take");

  return params;
}

InitialSolutionAdaptiveDT::InitialSolutionAdaptiveDT(const InputParameters & parameters) :
    SolutionTimeAdaptiveDT(parameters),
    _initial_dt(getParam<Real>("initial_dt")),
    _has_initial_dt(getParam<bool>("has_initial_dt")),
    _n_initial_steps(getParam<unsigned int>("n_initial_steps")),
    _total_t_step(declareRestartableData("total_t_step", -1))
{
}

Real
InitialSolutionAdaptiveDT::computeInitialDT()
{

  if(_total_t_step < 1)
    _total_t_step += 1;

  if( _t_step == _total_t_step)
  {
    if(_has_initial_dt)
      return _initial_dt;
    else
      return getParam<Real>("dt");

 }
  else
    return SolutionTimeAdaptiveDT::computeDT();
}


Real
InitialSolutionAdaptiveDT::computeDT()
{
  _total_t_step += 1;

  if (_total_t_step == static_cast<int>(_n_initial_steps) + 1 && _has_initial_dt)
      return getParam<Real>("dt");
    else
      return SolutionTimeAdaptiveDT::computeDT();

}
