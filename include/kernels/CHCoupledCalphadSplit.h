/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*
*  19 November 2013
*
*************************************************************************/

#ifndef CHCOUPLEDCALPHADSPLIT_H
#define CHCOUPLEDCALPHADSPLIT_H

#include "SplitCHCRes.h"

class CHCoupledCalphadSplit;

template<>
InputParameters validParams<CHCoupledCalphadSplit>();

class CHCoupledCalphadSplit : public SplitCHCRes
{
public:
  CHCoupledCalphadSplit(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  Real computeHeaviside();
  Real computeDHeaviside(unsigned int i);
  Real computeHeavisideDelta();
  Real computeDHeavisideDelta(unsigned int i);


private:
  const MaterialProperty<Real> & _W;                            //Well height
  const MaterialProperty<Real> & _Omega;                        //Molar volume

  const MaterialProperty<Real> & _dGalpha_dc;
  const MaterialProperty<Real> & _d2Galpha_dc2;

  const MaterialProperty<Real> & _dGdelta_dc;
  const MaterialProperty<Real> & _d2Gdelta_dc2;

  Real _scaling_factor;

  // unsigned int _n_var;
  unsigned int _w_var;
  unsigned int _T_var;

  //const VariableValue & _n;
  const VariableValue & _w;
  const VariableValue & _T;

  unsigned int _n_OP_vars;
  std::vector<unsigned int> _n_var;
  std::vector<const VariableValue *> _OP;
};

#endif //CHCOUPLEDCALPHADSPLIT_H
