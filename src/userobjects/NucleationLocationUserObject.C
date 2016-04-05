/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  7 December 2012
*
*************************************************************************/

#include "NucleationLocationUserObject.h"

#include "MooseVariable.h"
#include "SubProblem.h"
#include "MooseMesh.h"
#include "NonlinearSystem.h"
#include "GeneratedMesh.h"
#include "libmesh/parallel.h"

#include <ostream>

template<>
InputParameters validParams<NucleationLocationUserObject>()
{
  InputParameters params = validParams<ElementUserObject>();

  params.addRequiredCoupledVar("coupled_aux_vars", "coupled elemental auxiliary variables");
  params.addRequiredParam<int>("n_coupled_aux", "# of coupled aux variables");
  params.addRequiredParam<Real>("dwell_time", "How long nucleation event is");

  MultiMooseEnum execute_options(SetupInterface::getExecuteOptions());
  execute_options = "timestep_begin";
  params.set<MultiMooseEnum>("execute_on") = execute_options;

  params.addRequiredParam<int>("num_orientations", "# of orientation variants");
  params.addParam<Real>("boundary_width", 0.0, "the distance from mesh boundary to not nucleate");
  params.addParam<int>("random_seed", 0.0, "the random number seed for Bernoulli trial");

  return params;
}

NucleationLocationUserObject::NucleationLocationUserObject(const InputParameters & parameters) :
    ElementUserObject(parameters),
    _mesh(_subproblem.mesh()),
    //_coupled_probability(coupledValue("coupled_aux")),
    _n_coupled_aux(getParam<int>("n_coupled_aux")),
    _dwell_time(getParam<Real>("dwell_time")),
    _num_orientations(getParam<int>("num_orientations")),
    _boundary_width(getParam<Real>("boundary_width")),
    _random_seed(getParam<int>("random_seed")),
    // _counter(0),
    //make restartable
    _counter(declareRestartableData<int>("counter", 0)),
    _phase_gen_index(std::numeric_limits<unsigned int>::max()),
    // _nuclei(0),
    //make restartable
    _nuclei(declareRestartableData<std::vector<Nucleus> >("nuclei")),
    //_old_nucleus_list_size(0),
    //make restartable
    _old_nucleus_list_size(declareRestartableData<unsigned int>("old_nucleus_list_size", 0)),
    _has_new_nucleus(false),
    _master_random(-1000),
    _slave_random(-100)
{
    if(_n_coupled_aux != coupledComponents("coupled_aux_vars"))
    mooseError("Please specify the correct # of coupled probabilities (NucleationLocationUserObject).");

  _coupled_probability.resize(_n_coupled_aux);

  for(unsigned int i=0; i<_n_coupled_aux; i++)
    _coupled_probability[i] = &coupledValue("coupled_aux_vars", i);
}

void
NucleationLocationUserObject::initialize()
{
   _counter++;
   _old_nucleus_list_size = _nuclei.size();
   _has_new_nucleus = false;

   //reinitialize the local vectors to size 0 with no data
   _local_nucleus.clear();
   _packed_data.clear();

   _mrand.seed(_master_random, _random_seed + _counter);
   _mrand.seed(_slave_random, _random_seed + 2*_counter);
   _granddad = _mrand.randl(_master_random);
}

void
NucleationLocationUserObject::execute()
{
  // here, we just look at an individual element

  /**
   * generate random # based on element id and timestep. random number needs to
   * be different for each element, each timestep.  take steps of n_elem
   */
  unsigned int elem_id = _current_elem->id();

  unsigned int grandgirl = _mrand.randl(_slave_random);

  // we are driving the random seed for each element_id state off of TWO random numbers
  _mrand.seed(elem_id, elem_id + grandgirl + _granddad);

  //_mrand.seed(elem_id, elem_id + _random_seed + (_counter * _mesh.nElem()));
  Real random_number;

  if(!closeToBoundary())
  {
    for(unsigned int i(0); i<_n_coupled_aux; ++i)
    {
      random_number = _mrand.rand(elem_id);

      //test for nucleation
      if (((*_coupled_probability[i])[0] > 0) && (random_number < (*_coupled_probability[i])[0]))
      {
        // get the centroid of the element as the center of the nucleus
        Point nucleus_center = _current_elem->centroid();

        Nucleus current_nucleus;
        current_nucleus.setLocation(nucleus_center);
        current_nucleus.setStartTime(_t);
        current_nucleus.setEndTime(_t+_dwell_time);

        // if(_n_coupled_aux != _num_orientations)
        // {
        //  _mrand.seed(_phase_gen_index, elem_id + _random_seed + (_counter * _mesh.nElem()));
        //  int r_num = _mrand.randl(_phase_gen_index);

          /**
           * randl supplies some integer random number, we want to be between 1 and n coupled
           * vars, so modulo size()
           */
        //  r_num = r_num%_num_orientations;
        //  current_nucleus.setOrientation(r_num);
        // }
        //else
        //{
          current_nucleus.setOrientation(i);
          //}
          // this runs the tiny chance that there might be 2 or 3 nucleation events on the same element.
        _local_nucleus.push_back(current_nucleus);

      }
    }
  }
}

void
NucleationLocationUserObject::finalize()
{
  //pack up the data so it can be communicated using allgather

  Nucleus::pack(_local_nucleus, _packed_data);

  // Gather all the shared data onto each processor
  _communicator.allgather(_packed_data, false);

  //unpack all the data into the "global" variable
  Nucleus::unpack(_packed_data, _nuclei);

  _console << "NLUO::finalize nucleus size = "<<_nuclei.size()<<std::endl;

  //see if a new nucleus has been found
  if(_nuclei.size() > _old_nucleus_list_size)
    _has_new_nucleus = true;
  else
    _has_new_nucleus = false;

  _console << "NLUO::finalize has new nucleus = "<<_has_new_nucleus<<std::endl;
}

void
NucleationLocationUserObject::threadJoin(const UserObject &a)
{
  const NucleationLocationUserObject & nluo = dynamic_cast<const NucleationLocationUserObject &>(a);

  std::vector<Nucleus> nluo_local_nucleus = nluo._local_nucleus;

  //stick the two pieces of data together like peanut butter and jelly
  std::copy(nluo_local_nucleus.begin(), nluo_local_nucleus.end(), std::back_inserter(_local_nucleus));
}

bool
NucleationLocationUserObject::elementWasHit(const Elem * elem) const
{
  bool was_hit = false;
  //Elem * parent_elem(NULL);

  for(unsigned int i(0); i<_nuclei.size(); i++)
  {
    // parent_elem = _nuclei[i].getOriginalElement();
    //if( parent_elem->is_ancestor_of(elem)
    was_hit = elem->contains_point(_nuclei[i].getLocation());
    if(was_hit)
      break;
    //   }
  }
  return was_hit;
}

bool
NucleationLocationUserObject::closeToBoundary() const
{
  //here, we see if we are within some distance of a boundary on a mesh.
  //NB: this is only going to work on a regular orthogonal mesh.
  //returns true if too close to boundary.
  Point centroid = _current_elem->centroid();

  Real position_fraction[3];
  Real boundary_fraction[3];

  // it'd be really cool if we could do this, but darn you, protected info!
  // if(!_mesh._regular_orthogonal_mesh)
  //  return false;

  for(int i(0); i<3; ++i)
  {
    position_fraction[i] = centroid(i)/ _mesh.dimensionWidth(i);
    boundary_fraction[i] = _boundary_width/_mesh.dimensionWidth(i);
  }

  for(int i(0); i<3; ++i)
  {
    if(position_fraction[i] >= 0.5)
    {
      if(boundary_fraction[i] >= 1.0 - position_fraction[i])
        return true;
    }
    else
    {
      if(boundary_fraction[i] >= position_fraction[i])
        return true;
    }
  }

  return false;
}
