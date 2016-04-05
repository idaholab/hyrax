/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  7 July 2012
*
*************************************************************************/

#ifndef CHANGEVARIABLEDATA_H
#define CHANGEVARIABLEDATA_H

#include "GeneralPostprocessor.h"

//Forward Declarations
class ChangeVariableData;
class MooseMesh;
class MooseVariable;
class NonlinearSystem;

template<>
InputParameters validParams<ChangeVariableData>();

class ChangeVariableData : public GeneralPostprocessor
{
public:
  ChangeVariableData(const InputParameters & parameters);

  virtual void initialize();
  virtual void execute();

  virtual Real getValue();
  virtual void threadJoin(const UserObject & y);

  /**
   * Modifies the solution vector of the system; be careful. If you want
   * to change the values to some specific value, this only works with
   * linear Lagrange elements.
   */
  virtual void modifySolutionVector() = 0;

protected:
  /// A reference to the mesh
  MooseMesh & _mesh;

  /// The reference to the variable we want to retrieve in the solution vector
  std::vector<MooseVariable *> _moose_variable;

  /// A reference to the nonlinear system
  NonlinearSystem & _nl;

  /// A reference to the coupled variable
  //MooseVariable & _coupled;

private:

};

#endif // CHANGEVARIABLEDATA_H
