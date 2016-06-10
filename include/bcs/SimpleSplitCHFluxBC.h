/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*
*  CHiMaD/MOOSE
*
*  10 June 2016
*
*************************************************************************/

#ifndef SIMPLESPLITCHFLUXBC_H
#define SIMPLESPLITCHFLUXBC_H

#include "IntegratedBC.h"

//forward declaration
class SimpleSplitCHFluxBC;

template<>
InputParameters validParams<SimpleSplitCHFluxBC>();

class SimpleSplitCHFluxBC : public IntegratedBC
{
public:
  SimpleSplitCHFluxBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  const RealGradient _flux;

  const MaterialProperty<Real> & _M;

private:

};

#endif //SIMPLESPLITCHFLUXBC_H
