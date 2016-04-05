/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  13 December 2012
*
*************************************************************************/

#include "Nucleus.h"

#include "libmesh/libmesh.h"
#include "libmesh/point.h"

Nucleus::Nucleus():
    _start_time(0),
    _end_time(0),
    _orientation(0)
{
  _location.zero();
}

Point
Nucleus::getLocation() const
{
  return _location;
}

Real
Nucleus::getStartTime() const
{
  return _start_time;
}

Real
Nucleus::getEndTime() const
{
  return _end_time;
}

int
Nucleus::getOrientation() const
{
  return _orientation;
}

void
Nucleus::setLocation(Point a)
{
  _location = a;
}

void
Nucleus::setStartTime(Real a)
{
  _start_time = a;
}

void
Nucleus::setEndTime(Real a)
{
  _end_time = a;
}

void
Nucleus::setOrientation(int a)
{
  _orientation = a;
}

void
Nucleus::pack(const std::vector<Nucleus> & to_pack, std::vector<Real> & packed_data)
{
  // Don't repack data if it's already packed - could cause data loss, and that would make me sad.
  if (!packed_data.empty())
    return;

  //get the size of the _local_nucleus and multiply by stride, the # of individual pieces of data for each nucleus
  const unsigned int stride = Nucleus::stride();
  unsigned int packed_size = to_pack.size()*stride;
  packed_data.resize(packed_size);

  unsigned int i=0;
  for (unsigned int j=0; j<to_pack.size(); ++j)
  {
    Point this_location = to_pack[j].getLocation();
    int this_orientation = to_pack[j].getOrientation();

    packed_data[i] = this_location(0); //x-coord of point
    packed_data[i+1] = this_location(1); //y-coord
    packed_data[i+2] = this_location(2); //z-coord
    packed_data[i+3] = to_pack[j].getStartTime();
    packed_data[i+4] = to_pack[j].getEndTime();
    packed_data[i+5] = Real(this_orientation); //need to make sure we won't get any type conversion errors...

   i+=stride;
  }
}

void
Nucleus::unpack(const std::vector<Real> & packed_data, std::vector<Nucleus> & unpacked_data)
{
  const unsigned int stride = Nucleus::stride();
  unsigned int number_nuclei = packed_data.size()/stride;
  std::vector<Nucleus> new_nuclei(number_nuclei);

  Real tol = 1e-5;
  unsigned int i=0;

  for(unsigned int j(0); j<new_nuclei.size(); ++j)
  {
    Point current_point(packed_data[i], packed_data[i+1], packed_data[i+2]);

    new_nuclei[j].setLocation(current_point);
    new_nuclei[j].setStartTime(packed_data[i+3]);
    new_nuclei[j].setEndTime(packed_data[i+4]);

    Real local_orientation = packed_data[i+5];

    //make sure there's no type conversion errors for the orientation
    if(int(local_orientation + tol) == int(local_orientation))
      new_nuclei[j].setOrientation(int(local_orientation));
    else
      new_nuclei[j].setOrientation(int(local_orientation)+1);

    i+=stride;
  }

  std::copy(new_nuclei.begin(), new_nuclei.end(), std::back_inserter(unpacked_data));
  //might want to include some error message in here in case unpacking screws up
}

template<>
void
dataStore(std::ostream & stream, Nucleus & n, void * context)
{
  dataStore(stream, n._location, context);
  dataStore(stream, n._start_time, context);
  dataStore(stream, n._end_time, context);
  dataStore(stream, n._orientation, context);
}

template<>
void
dataLoad(std::istream & stream, Nucleus & n, void * context)
{
  dataLoad(stream, n._location, context);
  dataLoad(stream, n._start_time, context);
  dataLoad(stream, n._end_time, context);
  dataLoad(stream, n._orientation, context);
}
