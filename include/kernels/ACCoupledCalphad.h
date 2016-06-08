/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  12 June 2013
*
*************************************************************************/

#ifndef ACCOUPLEDCALPHAD_H
#define ACCOUPLEDCALPHAD_H

#include "ACBulk.h"

//Forward declarations
class ACCoupledCalphad;

template<>
InputParameters validParams<ACCoupledCalphad>();

class ACCoupledCalphad : public ACBulk<Real>
{
public:
  ACCoupledCalphad(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  const MaterialProperty<Real> & _W;                            //Well height
  const MaterialProperty<Real> & _Omega;                        //Molar volume
//  const MaterialProperty<Real> & _G_alpha;                      //Gmix_alpha
//  const MaterialProperty<Real> & _G_delta;                      //Gmix_delta

  const MaterialProperty<Real> & _dfbulk_dn;
  const MaterialProperty<Real> & _d2fbulk_dn2;
  const MaterialProperty<Real> & _d2fbulk_dcdn;

  /*
  const MaterialProperty<Real> & _dGalpha_dc;
  const MaterialProperty<Real> & _dGdelta_dc;

  const MaterialProperty<Real> & _dhdn;
  const MaterialProperty<Real> & _d2hdn2;
  const MaterialProperty<Real> & _dgdn;
  const MaterialProperty<Real> & _d2gdn2;
  */

  unsigned int _c_var;
  unsigned int _w_var;

  const VariableValue & _c;
  const VariableValue & _w;
};

#endif //ACCOUPLEDCALPHAD_H
