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

private:
  //Diffusion coefficient information
  Real _H_Zr_D0;
  Real _H_ZrH2_D0;
  Real _H_Zr_Q0;
  Real _H_ZrH2_Q0;
  Real _R;
  Real _k;
//  Real _mobility_CH_scaling;

  const MaterialProperty<Real> & _Galpha;
  const MaterialProperty<Real> & _Gdelta;
  const MaterialProperty<Real> & _dGalpha_dc;
  const MaterialProperty<Real> & _dGdelta_dc;
  const MaterialProperty<Real> & _d2Galpha_dc2;
  const MaterialProperty<Real> & _d2Gdelta_dc2;

  MaterialProperty<Real> & _D_alpha;
  MaterialProperty<Real> & _D_delta;

  MaterialProperty<Real> & _fbulk;
  MaterialProperty<Real> & _dfbulkdc;
  MaterialProperty<Real> & _d2fbulkdc2;
  MaterialProperty<Real> & _dfbulkdOP;
  MaterialProperty<Real> & _d2fbulkdOP2;

  MaterialProperty<Real> & _d2fbulkdcdOP;

  MaterialProperty<Real> & _h;
  MaterialProperty<Real> & _dhdOP;
  MaterialProperty<Real> & _d2hdOP2;

  MaterialProperty<Real> & _g;
  MaterialProperty<Real> & _dgdOP;
  MaterialProperty<Real> & _d2gdOP2;

  const VariableValue & _c;
  const VariableValue & _OP;
  const VariableValue & _temperature;


  //MaterialProperty<Real> & _L1Q;
  //Real _Q_transport;
  //const MaterialProperty<Real> & _d2Galpha_dcdT;

};

#endif //ZRHCALPHADDIFFUSIVITY_H
