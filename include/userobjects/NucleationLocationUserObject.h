/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  7 December 2012
*
*  NucleationLocationUserObject really needs to be computed as a post-aux,
*  timestep begin user object.  Currently not sure how to ensure this.
*
*************************************************************************/

#ifndef NUCLEATIONLOCATIONUSEROBJECT_H
#define NUCLEATIONLOCATIONUSEROBJECT_H

#include "ElementUserObject.h"
#include "MooseRandom.h"
#include "Nucleus.h"

//Forward declarations
class NucleationLocationUserObject;

template<>
InputParameters validParams<NucleationLocationUserObject>();

class NucleationLocationUserObject : public ElementUserObject
{
public:
  NucleationLocationUserObject(const InputParameters & parameters);
  virtual ~NucleationLocationUserObject() {}

  virtual void initialize();
  virtual void execute();
  virtual void finalize();
  virtual void threadJoin(const UserObject &a);

  bool elementWasHit(const Elem * elem) const;
  bool hasNewNucleus() const { return _has_new_nucleus; }

  const std::vector<Nucleus> & getNuclei() const { return _nuclei; }

protected:
  bool closeToBoundary() const;

  //local data that's concatenated onto the "global" array at the end of each step
  std::vector<Nucleus> _local_nucleus;


private:

  MooseMesh & _mesh;
  // const VariableValue & _coupled_probability;
  std::vector<const VariableValue *> _coupled_probability;

  unsigned int _n_coupled_aux;
  Real _dwell_time;
  int _num_orientations;
  Real _boundary_width;
  int _random_seed;
  //int _counter;
  //make restartable
  int & _counter;

   /// The index of the phase orientation generator (we will use a high index that isn't used by the node generators)
  const unsigned int _phase_gen_index;

  //global data
  //std::vector<Nucleus> _nuclei;
  //make restartable
  std::vector<Nucleus> & _nuclei;

  // The Moose stateful random number generator
  MooseRandom _mrand;

  //  const unsigned int _stride;  //this is the stride length for packing and unpacking nucleus data
   std::vector<Real> _packed_data;

  //check to see if new nucleus has been found
  //unsigned int _old_nucleus_list_size;
  //make restartable
  unsigned int & _old_nucleus_list_size;

  bool _has_new_nucleus;

  //for whipping the damned probability algorithm into shape
  int _master_random;
  int _slave_random;
  int _granddad;
};

#endif //NUCLEATIONLOCATIONUSEROBJECT_H
