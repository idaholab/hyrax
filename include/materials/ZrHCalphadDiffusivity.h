/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  13 February 2014
*
*************************************************************************/

#ifndef ZRHCALPHADDIFFUSIVITY_H
#define ZRHCALPHADDIFFUSIVITY_H

#include "ZrHCalphad.h"

//forward declaration
class ZrHCalphadDiffusivity;

template<>
InputParameters validParams<ZrHCalphad>();

class ZrHCalphadDiffusivity : public ZrHCalphad
{
public:
  ZrHCalphadDiffusivity(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();
  virtual Real computeHeaviside();
  virtual Real computeHeavisideDelta();

private:
  //Diffusion coefficient information
  Real _H_Zr_D0;
  Real _H_ZrH2_D0;
  Real _H_Zr_Q0;
  Real _H_ZrH2_Q0;
  Real _R;
  Real _k;
  Real _mobility_CH_scaling;

  const MaterialProperty<Real> & _d2Galpha_dc2;
  const MaterialProperty<Real> & _d2Gdelta_dc2;

  MaterialProperty<Real> & _D_alpha;
  MaterialProperty<Real> & _D_delta;

  const VariableValue & _c;
  const VariableValue & _OP;

  MaterialProperty<Real> & _L1Q;
  Real _Q_transport;
  const MaterialProperty<Real> & _d2Galpha_dcdT;

};

#endif //ZRHCALPHADDIFFUSIVITY_H
