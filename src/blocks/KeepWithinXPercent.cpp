/*
 * KeepWithinXPercent.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/KeepWithinXPercent.h"

#include <spl/common/Structure.h>

#include <pipelib/pipelib.h>

#include "spipe/common/Assert.h"
#include "spipe/common/StructureData.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace ssc = ::spl::common;
namespace structure_properties = ssc::structure_properties;

KeepWithinXPercent::KeepWithinXPercent(const double percent) :
    Block("Keep within X percent"), myKeepPercent(percent),
    myStructureProperty(structure_properties::general::ENTHALPY),
    myUsePerAtom(false)
{
}

KeepWithinXPercent::KeepWithinXPercent(const double percent,
    const StructureProperty & property) :
    Block("Keep within X percent"), myKeepPercent(percent),
    myStructureProperty(property),
    myUsePerAtom(false)
{
}

KeepWithinXPercent::KeepWithinXPercent(const double percent,
    const StructureProperty & property, const bool usePerAtom) :
    Block("Keep within X percent"), myKeepPercent(percent),
    myStructureProperty(property),
    myUsePerAtom(usePerAtom)
{
}

void
KeepWithinXPercent::in(spipe::common::StructureData * const data)
{
  const ssc::Structure * const structure = data->getStructure();
  const double * const value = structure->getProperty(myStructureProperty);

  // Let anything that doesn't have the property through
  if(value == NULL)
  {
    out(data);
    return;
  }

  double localValue = *value;
  if(myUsePerAtom)
    localValue /= static_cast<double>(structure->getNumAtoms());

  keep(data, localValue);
}

size_t
KeepWithinXPercent::release()
{
  const size_t numReleased = myStructures.size();
  BOOST_FOREACH(Structures::reference structurePair, myStructures)
  {
    out(structurePair.second);
  }
  myStructures.clear();
  return numReleased;
}

bool
KeepWithinXPercent::hasData() const
{
  return !myStructures.empty();
}

void
KeepWithinXPercent::keep(StructureDataType * const structure, const double energy)
{
  using ::std::make_pair;

#ifdef SPIPE_USE_THREAD
  boost::lock_guard<boost::mutex> guard(myMutex);
#endif

  // Check if we have any structures yet
  bool kept = true;
  if(myStructures.empty())
    myStructures.insert(make_pair(energy, structure));
  else
  {
    if(energy < myStructures.begin()->first)
      newLowest(structure, energy);
    else if(energy < getCutoff())
      myStructures.insert(make_pair(energy, structure));
    else
      kept = false;
  }

  if(!kept)
    drop(structure);
}

void
KeepWithinXPercent::newLowest(StructureDataType * const structure, const double energy)
{
  // WARNING: This must be called from keep(...) or another method that locks a mutex
  // to make sure the operations in this method remain thread safe as it doesn't
  // use a lock itself
  SPIPE_ASSERT(energy < myStructures.begin()->first);

  myStructures.insert(myStructures.begin(), ::std::make_pair(energy, structure));
  const double cutoff = getCutoff();

  // Go from the end erasing all entries greater than the new cutoff
  Structures::reverse_iterator it;

  // Erasing using reverse iterator is a bit annoying
  // see: http://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
  while((it = myStructures.rbegin())->first > cutoff)
  {
    drop(it->second);
    myStructures.erase((++it).base());
  }
}

double
KeepWithinXPercent::getCutoff() const
{
  return myStructures.begin()->first * (1.0 - myKeepPercent);
}

}
}
