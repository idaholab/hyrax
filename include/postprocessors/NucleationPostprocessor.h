/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  10 July 2012
*
*************************************************************************/

#ifndef NUCLEATIONPOSTPROCESSOR_H
#define NUCLEATIONPOSTPROCESSOR_H

#include "ChangeVariableData.h"
#include "MooseRandom.h"

//forward declarations
class NucleationPostprocessor;
class GeneratedMesh;

template<>
InputParameters validParams<NucleationPostprocessor>();

class NucleationPostprocessor : public ChangeVariableData
{
public:
  NucleationPostprocessor(const InputParameters & parameters);

  virtual void initialize();

  virtual void modifySolutionVector();

  virtual Real getValue();

protected:
  void searchForNucleationEvents();

  void changeValues();

  // Real minPeriodicDistance(Point p, Point q);

  /// A reference to the coupled variable
  MooseVariable & _coupled;

private:

  Real _radius;
  Real _dwell_time;
  Real _seed_value;
  Real _int_width;

  int _counter;

  /**
   * Global data used for calculating when the solution needs to be modified
   */
  std::vector<Real> _start_times;
  std::vector<Real> _end_times;
  std::vector<int> _orientation_type;
  std::vector<Node *> _nucleation_locations;

  /**
   * The following vectors are filled by each local process and concatenated
   * onto the "global" arrays at the end of each step
   */
  std::vector<Real> _local_start_times;
  std::vector<Real> _local_end_times;
  std::vector<int> _local_orientation_type;
  // Node ids used to retieve nucleation location information
  std::vector<unsigned int> _local_node_ids;

  /// The Moose stateful random number generator
  MooseRandom _mrand;

  /// The index of the phase orientation generator (we will use a high index that isn't used by the node generators)
  const unsigned int _phase_gen_index;
};

#endif //NUCLEATIONPOSTPROCESSOR_H
