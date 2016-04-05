/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  18 November 2013
*
*************************************************************************/

#ifndef CHCOUPLEDSPLIT_H
#define CHCOUPLEDSPLIT_H

#include "SplitCHCRes.h"

class CHCoupledSplit;

template<>
InputParameters validParams<CHCoupledSplit>();

class CHCoupledSplit : public SplitCHCRes
{
public:
  CHCoupledSplit(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);

private:

  //Real _W;

  const MaterialProperty<Real> & _a1;  ///< Landau polynomial parameters (see Guo, 2008)
  const MaterialProperty<Real> & _a2;
  const MaterialProperty<Real> & _c1;  ///< position-ish of 1st energy well in c-space (terminal solid solubility)

  const VariableValue & _coupled_OP;  ///< Allen-Cahn equation variable (order parameter, probably)
//  const VariableGradient & _coupled_OP_grad;  ///< gradient of AC variable
};

#endif //CHCOUPLEDSPLIT_H
