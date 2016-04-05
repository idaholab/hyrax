/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  9 December 2013
*
*************************************************************************/


#ifndef SPLITCOUPLEDCHWRES_H
#define SPLITCOUPLEDCHWRES_H

#include "SplitCHWRes.h"

//Forward Declarations
class SplitCoupledCHWRes;

template<>
InputParameters validParams<SplitCoupledCHWRes>();

class SplitCoupledCHWRes : public SplitCHWRes
{
public:
  SplitCoupledCHWRes(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  //duplicating some things from SplitCHWRes...
  const MaterialProperty<Real> & _mob;

  unsigned int _c_var;
  //unsigned int _n_var;
  unsigned int _T_var;

  const VariableValue & _c;
  //const VariableValue & _n;
  const VariableValue & _T;

  unsigned int _n_OP_vars;
  std::vector<unsigned int> _n_var;
  std::vector<const VariableValue *> _OP;
};

#endif //SPLITCOUPLEDCHWRES_H
