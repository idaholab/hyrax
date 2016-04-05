/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  2 October 2013
*
*************************************************************************/

#ifndef CHLARRYSPLIT_H
#define CHLARRYSPLIT_H

#include "SplitCHCRes.h"

class CHLarrySplit;

template<>
InputParameters validParams<CHLarrySplit>();

class CHLarrySplit : public SplitCHCRes
{
public:
  CHLarrySplit(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);

private:

  Real _W;
};

#endif //CHLARRY_H
