/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  6 December 2012
*
*************************************************************************/

#include "NucleusIntroductionSolutionModifier.h"
#include "NucleationLocationUserObject.h"

// MOOSE includes
#include "NonlinearSystem.h"

#include <ostream>

template<>
InputParameters validParams<NucleusIntroductionSolutionModifier>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addRequiredParam<UserObjectName>("nucleation_userobject", "The name of the UserObject to use for nucleation event locations");
  params.addRequiredParam<std::vector<VariableName> >("variables", "The name of the variable(s) to be modified");
  //arams.addRequiredCoupledVar("OP_var_names", "array of coupled OP variable names");
  params.addRequiredParam<int>("num_vars", "The # of variables to modify");
  params.addRequiredParam<Real>("seed_value", "The nucleus order parameter value");
  params.addRequiredParam<Real>("radius", "nucleus radius");
  params.addParam<Real>("int_width", 0.0, "interface width of nucleus");

  return params;
}

NucleusIntroductionSolutionModifier::NucleusIntroductionSolutionModifier(const InputParameters & parameters) :
    GeneralUserObject(parameters),
    _nucleation_userobject(getUserObject<NucleationLocationUserObject>("nucleation_userobject")),
    _mesh(_subproblem.mesh()),
    _nl(static_cast<FEProblem &>(_subproblem).getNonlinearSystem()),
    _radius(getParam<Real>("radius")),
    _seed_value(getParam<Real>("seed_value")),
    _int_width(getParam<Real>("int_width")),
    _num_vars(getParam<int>("num_vars"))
{
  std::vector<VariableName> vars = getParam<std::vector<VariableName> >("variables");
  _moose_variable.resize(vars.size());

  if(vars.size() != _num_vars)
    mooseError("Double check the # of variables you want to modify (NucleusIntroductionSolutionModifier).");

  // initialize our vector of variable pointers
  for (unsigned int i=0; i<_num_vars; ++i)
  {
    _moose_variable[i] = &_subproblem.getVariable(0, vars[i]);
  }
}

void
NucleusIntroductionSolutionModifier::initialize()
{
  // Assumption: We are going to assume that all variables are periodic together
//  _mesh.initPeriodicDistanceForVariable(_nl, _moose_variable[0]->number());
}

void
NucleusIntroductionSolutionModifier::execute()
{
  const std::vector<Nucleus> & nuclei = _nucleation_userobject.getNuclei();

  MeshBase::const_node_iterator it_end = _mesh.localNodesEnd();
  MeshBase::const_node_iterator it = _mesh.localNodesBegin();

  // node loop to introduce nuclei into order parameter field
  for ( ; it != it_end ; ++it)
  {
    Node *node = *it;

    _subproblem.reinitNode(node, 0);

    // check the node against each nucleation point and see if it lives
    // within distance of a nucleus
    Real distance;
    for(unsigned int j(0); j<nuclei.size(); j++)
    {
      distance = _mesh.minPeriodicDistance(_moose_variable[0]->number(), nuclei[j].getLocation(), *node);

      if( _t >= nuclei[j].getStartTime() &&
          _t < nuclei[j].getEndTime() )
      {
        int orientation = nuclei[j].getOrientation();
        if(distance <=_radius - _int_width/2.0)
        {
          _moose_variable[orientation]->setNodalValue(_seed_value);
        }
        else if(distance < _radius + _int_width/2.0)
        {
          Real interface_position = (distance - _radius + _int_width/2.0)/_int_width;
          Real int_value = _seed_value*(1.0 - std::cos(-interface_position + libMesh::pi/2.0));
          _moose_variable[orientation]->setNodalValue(int_value);
        }

        // Not sure if we need this, but probably :)
        _moose_variable[orientation]->insert(_nl.solution());
      }
    }
  }
  _nl.solution().close();
  _nl.sys().update();
}

