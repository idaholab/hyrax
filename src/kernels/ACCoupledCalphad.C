/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  13 June 2013
*
*************************************************************************/

#include "ACCoupledCalphad.h"

template<>
InputParameters validParams<ACCoupledCalphad>()
{
  InputParameters params = ACBulk<Real>::validParams();
  // params.addParam<Real>("scaling_factor", 1, "free energy scaling factor for nondimensionalization");

  params.addRequiredCoupledVar("c", "concentration");
  params.addRequiredCoupledVar("w", "chemical potential");
//  params.addRequiredCoupledVar("T", "temperature");

  return params;
}

ACCoupledCalphad::ACCoupledCalphad(const InputParameters & parameters) :
    ACBulk<Real>(parameters),
    _W(getMaterialProperty<Real>("well_height")),
    _Omega(getMaterialProperty<Real>("molar_volume")),
    _dfbulk_dn(getMaterialProperty<Real>("df_bulk_dOP")),
    _d2fbulk_dn2(getMaterialProperty<Real>("d2f_bulk_dOP2")),
    _d2fbulk_dcdn(getMaterialProperty<Real>("d2f_bulk_dcdOP")),

//    _G_alpha(getMaterialProperty<Real>("G_AB1CD1")),
//    _G_delta(getMaterialProperty<Real>("G_AB1CD2")),
//    _dGalpha_dc(getMaterialProperty<Real>("dGAB1CD1_dc")),
//    _dGdelta_dc(getMaterialProperty<Real>("dGAB1CD2_dc")),
//    _dhdn(getMaterialProperty<Real>("dinterpolation_dOP")),
    //   _d2hdn2(getMaterialProperty<Real>("d2interpolation_dOP2")),
    // _dhdn(getMaterialProperty<Real>("dbarrier_dOP")),
    //_d2hdn2(getMaterialProperty<Real>("d2barrier_dOP2")),

    _c_var(coupled("c")),
    _w_var(coupled("w")),
    _c(coupledValue("c")),
    _w(coupledValue("w"))
{
}

Real
ACCoupledCalphad::computeDFDOP(PFFunctionType type)
{
  switch (type)
  {
    case Residual:

      return _dfbulk_dn[_qp];

  case Jacobian:

    return  _phi[_j][_qp]*_d2fbulk_dn2[_qp];
  }

  mooseError("Invalid type passed in");
}

Real
ACCoupledCalphad::computeQpOffDiagJacobian(unsigned int jvar)
{

  if (jvar == _c_var)
    return _L[_qp]*_test[_i][_qp]*_phi[_j][_qp]*_d2fbulk_dcdn[_qp];

  else if (jvar == _w_var)
    return 0;

//  else if (jvar == _T_var)
    //return _L[_qp]*_test[_i][_qp]*_phi[_j][_qp]*(d/dT of df/dn);
//    return 0;

  else
    return 0;
    // mooseError("Screwed up ACCoupledCalphad::computeQpOffDiagJacobian()");
}
