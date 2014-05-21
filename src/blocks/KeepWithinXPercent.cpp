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

namespace ssc = spl::common;
namespace structure_properties = ssc::structure_properties;

KeepWithinXPercent::KeepWithinXPercent(const double percent) :
    Block("Keep within X percent"), myKeepPercent(percent), myStructureProperty(
        structure_properties::general::ENTHALPY), myUsePerAtom(true)
{
}

KeepWithinXPercent::KeepWithinXPercent(const double percent,
    const StructureProperty & property) :
    Block("Keep within X percent"), myKeepPercent(percent), myStructureProperty(
        property), myUsePerAtom(true)
{
}

KeepWithinXPercent::KeepWithinXPercent(const double percent,
    const StructureProperty & property, const bool usePerAtom) :
    Block("Keep within X percent"), myKeepPercent(percent), myStructureProperty(
        property), myUsePerAtom(usePerAtom)
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
    localValue /= static_cast< double>(structure->getNumAtoms());

  keep(data, localValue);
}

size_t
KeepWithinXPercent::release()
{
  size_t numReleased = 0;
  BOOST_FOREACH(StructuresByComposition::reference order, myStructures)
  {
    BOOST_FOREACH(StructureOrder::reference structure, order.second)
      out(structure.second);
    numReleased += order.second.size();
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
KeepWithinXPercent::keep(StructureDataType * const structure,
    const double energy)
{
  using std::make_pair;

#ifdef SPIPE_USE_BOOST_THREAD
  boost::lock_guard< boost::mutex> guard(myMutex);
#endif

  spl::common::AtomsFormula composition =
      structure->getStructure()->getComposition();
  composition.reduce();
  StructureOrder & order = myStructures[composition];

  // Check if we have any structures yet
  bool kept = true;
  if(order.empty())
    order.insert(order.begin(), make_pair(energy, structure));
  else
  {
    if(energy < order.begin()->first)
      newLowest(structure, energy, &order);
    else if(energy < getCutoff(order))
      order.insert(make_pair(energy, structure));
    else
      kept = false;
  }

  if(!kept)
    drop(structure);
}

void
KeepWithinXPercent::newLowest(StructureDataType * const structure,
    const double energy, StructureOrder * const order)
{
  // WARNING: This must be called from keep(...) or another method that locks a mutex
  // to make sure the operations in this method remain thread safe as it doesn't
  // use a lock itself
  SPIPE_ASSERT(energy < order->begin()->first);

  order->insert(order->begin(), std::make_pair(energy, structure));
  const double cutoff = getCutoff(*order);

  // Go from the end erasing all entries greater than the new cutoff
  StructureOrder::reverse_iterator it;

  // Erasing using reverse iterator is a bit annoying
  // see: http://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
  while((it = order->rbegin())->first > cutoff)
  {
    drop(it->second);
    order->erase((++it).base());
  }
}

double
KeepWithinXPercent::getCutoff(const StructureOrder & order) const
{
  if(order.empty())
    return std::numeric_limits< double>::max();

  return order.begin()->first * (1.0 - myKeepPercent);
}

}
}
