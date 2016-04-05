/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  16 December 2013
*
*************************************************************************/

#ifndef INITIALSOLUTIONADAPTIVEDT_H
#define INITIALSOLUTIONADAPTIVEDT_H

#include "SolutionTimeAdaptiveDT.h"

class InitialSolutionAdaptiveDT;

template<>
InputParameters validParams<InitialSolutionAdaptiveDT>();

class InitialSolutionAdaptiveDT : public SolutionTimeAdaptiveDT
{
public:
  InitialSolutionAdaptiveDT(const InputParameters & parameters);

protected:
  virtual Real computeInitialDT();
  virtual Real computeDT();

  Real _initial_dt;
  bool _has_initial_dt;
  unsigned int _n_initial_steps;

  int & _total_t_step;

private:

};

#endif //INITIALSOLUTIONADAPTIVEDT_H
