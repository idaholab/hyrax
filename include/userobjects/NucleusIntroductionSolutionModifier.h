/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  6 December 2012
*
*************************************************************************/

#ifndef NUCLEUSINTRODUCTIONSOLUTIONMODIFIER_H
#define NUCLEUSINTRODUCTIONSOLUTIONMODIFIER_H

#include "GeneralUserObject.h"
#include "MooseMesh.h"

//Forward declarations
class NucleationLocationUserObject;
class NucleusIntroductionSolutionModifier;

template<>
InputParameters validParams<NucleusIntroductionSolutionModifier>();

class NucleusIntroductionSolutionModifier : public GeneralUserObject
{
public:
  NucleusIntroductionSolutionModifier(const InputParameters & parameters);
  virtual ~NucleusIntroductionSolutionModifier() {}

  virtual void initialize();
  virtual void execute();
  virtual void finalize() {}

protected:
  const NucleationLocationUserObject & _nucleation_userobject;
  MooseMesh & _mesh;
  std::vector<MooseVariable *> _moose_variable;
  NonlinearSystem & _nl;

private:
Real _radius;
  Real _seed_value;
  Real _int_width;

  unsigned int _num_vars;
};

#endif //NUCLEUSINTRODUCTIONSOLUTIONMODIFIER_H
