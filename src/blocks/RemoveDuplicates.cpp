/*
 * RemoveDuplicates.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/RemoveDuplicates.h"

#include <map>

#include <boost/foreach.hpp>

#include <spl/common/Structure.h>

#include "spipe/StructurePipe.h"
#include "spipe/common/StructureData.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace ssc = spl::common;
namespace ssu = spl::utility;
namespace structure_properties = ssc::structure_properties;

RemoveDuplicates::RemoveDuplicates(ssu::IStructureComparatorPtr comparator) :
    Block("Remove duplicates"), myStructureSet(comparator)
{
}

RemoveDuplicates::RemoveDuplicates(
    const spl::utility::IStructureComparator & comparator) :
    Block("Remove duplicates"), myStructureSet(comparator)
{
}

void
RemoveDuplicates::in(spl::common::Structure * const structure)
{
#ifdef SPIPE_USE_BOOST_THREAD
  boost::lock_guard< boost::mutex> guard(myMutex);
#endif

  const StructureSet::insert_return_type result = myStructureSet.insert(
      structure);
  spl::utility::HeterogeneousMap & properties = structure->properties();

  if(result.second)
  {
    // Inserted
    properties[structure_properties::searching::TIMES_FOUND] = 1;
  }
  else
  {
    // The structure is not unique so discard it
    drop(structure);

    // Up the 'times found' counter on the original structure
    spl::common::Structure * const originalStructure = *result.first;
    unsigned int * const timesFound = properties.find(
        structure_properties::searching::TIMES_FOUND);
    if(timesFound)
      *timesFound += 1;
    else
      properties[structure_properties::searching::TIMES_FOUND] = 1;
  }
}

void
RemoveDuplicates::pipelineFinishing()
{
  myStructureSet.clear();
}

size_t
RemoveDuplicates::release()
{
  const size_t numUnique = myStructureSet.size();

  BOOST_FOREACH(spl::common::Structure * const structure, myStructureSet)
    out(structure);

  myStructureSet.clear();

  return numUnique;
}

bool
RemoveDuplicates::hasData() const
{
  return !myStructureSet.empty();
}

}
}
