/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  8 March 2012
*
*************************************************************************/

#ifndef ACBULKPOLYCOUPLED_H
#define ACBULKPOLYCOUPLED_H

#include "ACBulkCoupled.h"

// Forward Declaration
class ACBulkPolyCoupled;

template<>
InputParameters validParams<ACBulkPolyCoupled>();

/**
 * ACBulkPolyCoupled couples the bulk Allen-Cahn equation term with order parameter eta
 * to the conserved field variable term from the Cahn-Hilliard equation.  It also couples
 * with multiple order parameters.  It uses the PFMobilityLandau materials class.
 */

class ACBulkPolyCoupled : public ACBulkCoupled
{
public:
  ACBulkPolyCoupled(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);

  unsigned int _n_OP_vars;
  unsigned int _OP_number;

  const MaterialProperty<Real> & _a5;
  const MaterialProperty<Real> & _a6;
  const MaterialProperty<Real> & _a7;

  std::vector<const VariableValue *> _coupled_OP_vars;
};


#endif //ACBULKPOLYCOUPLED_H
