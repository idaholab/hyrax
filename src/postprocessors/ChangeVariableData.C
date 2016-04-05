/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  7 July 2012
*
*************************************************************************/

#include "ChangeVariableData.h"
#include "MooseVariable.h"
#include "SubProblem.h"
#include "MooseMesh.h"
#include "NonlinearSystem.h"
#include "FEProblem.h"

template<>
InputParameters validParams<ChangeVariableData>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addRequiredParam<std::vector<NonlinearVariableName> >("variables", "The variable(s) we want to change");
  // params.addRequiredParam<std::string>("coupled_aux", "The aux variable that we want to couple in");
  // We really want to only run this at the end of each timestep, so we'll force that here

  MultiMooseEnum execute_options(SetupInterface::getExecuteOptions());
  execute_options = "timestep";
  params.set<MultiMooseEnum>("execute_on") = execute_options;

  return params;
}

ChangeVariableData::ChangeVariableData(const InputParameters & parameters) :
    GeneralPostprocessor(parameters),
    _mesh(_subproblem.mesh()),
    _nl(static_cast<FEProblem &>(_subproblem).getNonlinearSystem())//,
//    _coupled(_subproblem.getVariable(0, getParam<std::string>("coupled_aux")))
{
  std::vector<NonlinearVariableName> vars = getParam<std::vector<NonlinearVariableName> >("variables");
  _moose_variable.resize(vars.size());

  // initialize our vector of variable pointers
  for (unsigned int i=0; i<vars.size(); ++i)
    _moose_variable[i] = &_subproblem.getVariable(0, vars[i]);
}

void
ChangeVariableData::initialize()
{
}

void
ChangeVariableData::execute()
{
  modifySolutionVector();

  _nl.solution().close();
  _nl.sys().update();
}

Real
ChangeVariableData::getValue()
{
  return 0.0;
}

void
ChangeVariableData::threadJoin(const UserObject & /*y*/)
{
}
