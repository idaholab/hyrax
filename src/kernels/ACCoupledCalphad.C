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
    _G_alpha(getMaterialProperty<Real>("G_AB1CD1")),
    _G_delta(getMaterialProperty<Real>("G_AB1CD2")),
    _dGalpha_dc(getMaterialProperty<Real>("dGAB1CD1_dc")),
    _dGdelta_dc(getMaterialProperty<Real>("dGAB1CD2_dc")),
    _c_var(coupled("c")),
    _w_var(coupled("w")),
    _c(coupledValue("c")),
    _w(coupledValue("w"))
{
}

Real
ACCoupledCalphad::computeDFDOP(PFFunctionType type)
{

  Real dgdn, dHeavisidedn, d2gdn2, d2Heavisidedn2;

  switch (type)
  {
    case Residual:

      dgdn = computeDBarrierDOP();

      dHeavisidedn = computeDHeavisideDOP();

      return ( ( _G_delta[_qp] - _G_alpha[_qp])*dHeavisidedn + _W[_qp]*dgdn ) / _Omega[_qp];

  case Jacobian:

    d2gdn2 = computeD2BarrierDOP2();

    d2Heavisidedn2 = computeD2HeavisideDOP2();

    return  (  _G_delta[_qp]*d2Heavisidedn2*_phi[_j][_qp] - _G_alpha[_qp]*d2Heavisidedn2*_phi[_j][_qp]
               + _W[_qp]*d2gdn2*_phi[_j][_qp] ) / _Omega[_qp];
  }
  mooseError("Invalid type passed in");
}

Real
ACCoupledCalphad::computeQpOffDiagJacobian(unsigned int jvar)
{

  if (jvar == _c_var)
    return _L[_qp]*_test[_i][_qp]*_phi[_j][_qp]*
      (( computeDHeavisideDOP()*_dGdelta_dc[_qp]
                         - computeDHeavisideDOP()*_dGalpha_dc[_qp] )/_Omega[_qp] );

  else if (jvar == _w_var)
    return 0;

//  else if (jvar == _T_var)
    //return _L[_qp]*_test[_i][_qp]*_phi[_j][_qp]*(d/dT of df/dn);
//    return 0;

  else
    return 0;
    // mooseError("Screwed up ACCoupledCalphad::computeQpOffDiagJacobian()");
}

Real
ACCoupledCalphad::computeDHeavisideDOP()
{
  Real OP =_u[_qp];

  return 30*OP*OP*OP*OP - 60*OP*OP*OP + 30*OP*OP;
}

Real
ACCoupledCalphad::computeD2HeavisideDOP2()
{
  Real OP = _u[_qp];

  return 120*OP*OP*OP - 180*OP*OP + 60*OP;
}

Real
ACCoupledCalphad::computeDBarrierDOP()
{
  return 2*_u[_qp] - 6*_u[_qp]*_u[_qp] + 4*_u[_qp]*_u[_qp]*_u[_qp];
}

Real
ACCoupledCalphad::computeD2BarrierDOP2()
{
  return 2 - 12*_u[_qp] + 12*_u[_qp]*_u[_qp];
}
