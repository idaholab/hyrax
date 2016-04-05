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

  Real computeDHeavisideDOP();
  Real computeD2HeavisideDOP2();
  Real computeDHeavisideDeltaDOP();
  Real computeD2HeavisideDeltaDOP2();

  Real computeDBarrierDOP(Real & SS, Real & QS, Real & SM);
  Real computeD2BarrierDOP2(Real & SS, Real & QS, Real & SM);

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  const MaterialProperty<Real> & _W;                            //Well height
  const MaterialProperty<Real> & _Omega;                        //Molar volume
  const MaterialProperty<Real> & _G_alpha;                      //Gmix_alpha
  const MaterialProperty<Real> & _G_delta;                      //Gmix_delta
  const MaterialProperty<Real> & _dGalpha_dc;
  const MaterialProperty<Real> & _dGdelta_dc;

  unsigned int _n_OP_vars;
  unsigned int _OP_number;

  std::vector<unsigned int> _n_var;
  std::vector<const VariableValue *> _coupled_OP_vars;

  Real _scaling_factor;

  unsigned int _c_var;
  unsigned int _w_var;
  unsigned int _T_var;

  const VariableValue & _c;
  const VariableValue & _w;
  const VariableValue & _T;
};

#endif //ACCOUPLEDCALPHAD_H
