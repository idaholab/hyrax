/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  11 June 2013
*
*************************************************************************/

#ifndef CHCOUPLEDCALPHAD_H
#define CHCOUPLEDCALPHAD_H

#include "CHBulk.h"

/**
 * CHCoupledCalphad handles the conserved order parameter (atomic fraction)
 * evolved using the Cahn-Hilliard equation coupled to an arbitrary number of
 * non-conserved structural order parameters for the Zr-delta hydride system.
 * It uses free energy equations taken from A.T. Dinsdale 15 (1991) CALPHAD 317,
 * N. Dupin et al, J. Nuc. Mater. 275 (1999) 287 and J.D. Cox et al., CODATA Key
 * Values for Thermodynamics, Hemisphere Publishing Corporation, 1989.
 */

//Forward declarations
class CHCoupledCalphad;

template<>
InputParameters validParams<CHCoupledCalphad>();

class CHCoupledCalphad : public CHBulk<Real>
{
public:
  CHCoupledCalphad(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

  Real computeHeaviside();
  void computeDHeaviside();

  RealGradient computeGradConservedTerm();
  RealGradient computeGradNonconservedTerm();

private:
  const MaterialProperty<Real> & _W;                            //Well height
  const MaterialProperty<Real> & _Omega;                        //Molar volume
  const MaterialProperty<Real> & _dGalpha_dc;
  const MaterialProperty<Real> & _d2Galpha_dc2;
  const MaterialProperty<Real> & _d3Galpha_dc3;
  const MaterialProperty<Real> & _dGdelta_dc;
  const MaterialProperty<Real> & _d2Gdelta_dc2;
  const MaterialProperty<Real> & _d3Gdelta_dc3;

  unsigned int _n_OP_variables;
  std::vector<const VariableValue *> _OP;
  std::vector<const VariableGradient *> _grad_OP;

  Real _Heaviside;
  std::vector<Real> _dHeaviside;
};

#endif //CHCOUPLEDCALPHAD_H
