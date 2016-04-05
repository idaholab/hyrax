/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  26 September 2012
*
*************************************************************************/

#include "OneSeed.h"

#include "MooseVariable.h"
#include "SubProblem.h"
#include "MooseMesh.h"
#include "NonlinearSystem.h"
#include "GeneratedMesh.h"

#include <cmath>

#include <ostream>


template<>
InputParameters validParams<OneSeed>()
{
  InputParameters params = validParams <ChangeVariableData>();
  params.addRequiredParam<Real>("radius", "nucleus radius");
  params.addRequiredParam<Real>("dwell_time", "how long the nucleation event is");
  params.addRequiredParam<Real>("seed_value", "Nucleus seed order parameter value");
  params.addParam<Real>("int_width", 0.0, "nucleus interface width");
  params.addRequiredParam<Real>("x_position", "x-poxsition of nucleus center");
  params.addRequiredParam<Real>("y_position", "y-position of nucleus center");
  params.addParam<Real>("z_position", 0.0, "z-position of the nucleus center");
  params.addParam<bool>("restart", false, "if restarting order parameter field at some time or not");
  params.addParam<Real>("restart_time", 0.0, "restart time for order parameter field");

  return params;
}

OneSeed::OneSeed(const InputParameters & parameters) :
    ChangeVariableData(parameters),
    _radius(getParam<Real>("radius")),
    _dwell_time(getParam<Real>("dwell_time")),
    _seed_value(getParam<Real>("seed_value")),
    _int_width(getParam<Real>("int_width")),
    _x_position(getParam<Real>("x_position")),
    _y_position(getParam<Real>("y_position")),
    _z_position(getParam<Real>("z_position")),
    _restart(getParam<bool>("restart")),
    _restart_time(getParam<Real>("restart_time")),
    _location(_x_position, _y_position, _z_position)
{
  if(_dwell_time >= _restart_time && _restart)
    mooseError("please set dwell_time < restart_time (OneSeed Postprocessor)");
}

void
OneSeed::initialize()
{
  // Assumption: We are going to assume that all variables are periodic together
//  _mesh.initPeriodicDistanceForVariable(_nl, _moose_variable[0]->number());
}

void
OneSeed::modifySolutionVector()
{
  if ( (_t <= _dwell_time) || (_t >= _restart_time && _restart) )
  {

    //Grab the mesh information
    MeshBase::const_node_iterator it_end = _mesh.localNodesEnd();
    MeshBase::const_node_iterator it = _mesh.localNodesBegin();

    // node loop to introduce nucleus into order parameter field
    for ( ; it != it_end ; ++it)
    {
      Node *node = *it;

      _subproblem.reinitNode(node, 0);

      // check the node against the nucleation point and see if it lives within distance the nucleus
      Real distance;

      distance = _mesh.minPeriodicDistance(_moose_variable[0]->number(), _location, *node);
      //if( _t < _dwell_time)
      //{
      if(_t >= _restart_time && _restart)
        _moose_variable[0]->setNodalValue(0.0);

      if(distance <=_radius - _int_width/2.0)
      {
        _moose_variable[0]->setNodalValue(_seed_value);
        // _counter++;
      }
      else if(distance < _radius + _int_width/2.0)
      {
        Real interface_position = (distance - _radius + _int_width/2.0)/_int_width;
        Real int_value = _seed_value*(1.0 - std::cos(-interface_position + libMesh::pi/2.0));
        _moose_variable[0]->setNodalValue(int_value);
        //_counter++;
      }
      _moose_variable[0]->insert(_nl.solution());
    }
    if(_t>= _restart_time && _restart)
    {
      _restart = false;
      _console << "set _restart to "<< _restart<<std::endl;
    }

  }
}

Real
OneSeed::getValue()
{
  //return Real(_counter);
  return 0.0;
}
