/****************************************************************/
/****************************************************************/

#ifndef ELEMENTINTEGRALFREEENERGY_H
#define ELEMENTINTEGRALFREEENERGY_H

#include "ElementIntegralPostprocessor.h"
//#include "MooseVariableInterface.h"

class ElementIntegralFreeEnergy;

template<>
InputParameters validParams<ElementIntegralFreeEnergy>();

class ElementIntegralFreeEnergy :
  public ElementIntegralPostprocessor//,
//  public MooseVariableInterface
{
public:
  ElementIntegralFreeEnergy(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral();

  const MaterialProperty<Real> & _phase1_energy;
  const MaterialProperty<Real> & _phase2_energy;

  const MaterialProperty<Real> & _grad_OP_term;
  const MaterialProperty<Real> & _grad_C_term;

  const MaterialProperty<Real> & _Heaviside;

};

#endif //ELEMENTINTEGRALFREEENERGY_H
