/*
 * KeepTopN.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/KeepTopN.h"

#include <spl/common/Structure.h>

#include <pipelib/pipelib.h>

#include "spipe/common/StructureData.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace ssc = ::spl::common;
namespace structure_properties = ssc::structure_properties;

KeepTopN::KeepTopN(const size_t keepTopN) :
    Block("Keep top N"), myKeepTopN(keepTopN), myStructureProperty(
        structure_properties::general::ENTHALPY), myUsePerAtom(true)
{
}

KeepTopN::KeepTopN(const size_t keepTopN, const StructureProperty & property) :
    Block("Keep top N"), myKeepTopN(keepTopN), myStructureProperty(property), myUsePerAtom(
        true)
{
}

KeepTopN::KeepTopN(const size_t keepTopN, const StructureProperty & property,
    const bool usePerAtom) :
    Block("Keep top N"), myKeepTopN(keepTopN), myStructureProperty(property), myUsePerAtom(
        usePerAtom)
{
}

void
KeepTopN::in(spl::common::Structure * const structure)
{
  const double * const value = structure->getProperty(myStructureProperty);

  // Let anything that doesn't have the property through
  if(value == NULL)
  {
    out(structure);
    return;
  }

  double localValue = *value;
  if(myUsePerAtom)
    localValue /= static_cast< double>(structure->getNumAtoms());

  keep(structure, localValue);
}

size_t
KeepTopN::release()
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
KeepTopN::hasData() const
{
  return !myStructures.empty();
}

void
KeepTopN::keep(spl::common::Structure * const structure, const double energy)
{
#ifdef SPIPE_USE_BOOST_THREAD
  boost::lock_guard< boost::mutex> guard(myMutex);
#endif

  spl::common::AtomsFormula composition = structure->getComposition();
  composition.reduce();
  StructureOrder & order = myStructures[composition];

  order.insert(std::make_pair(energy, structure));
  if(order.size() > myKeepTopN)
  {
    // We have gone over by one, remove the last
    StructureOrder::reverse_iterator last = order.rbegin();
    drop(last->second);
    // Erasing using reverse iterator is a bit annoying
    // see: http://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
    order.erase((++last).base());
  }
}

}
}
