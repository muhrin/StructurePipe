/*
 * Clone.cpp
 *
 *  Created on: Dec 1, 2013
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/Clone.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

Clone::Clone(const int times) :
    Block("Clone"), myTimes(times), myGiveUniqueNames(true)
{
}

Clone::Clone(const int times, const bool giveUniqueNames) :
    Block("Clone"), myTimes(times), myGiveUniqueNames(giveUniqueNames)
{
}

void
Clone::in(spl::common::Structure * const structure)
{
  for(int i = 0; i < myTimes - 1; ++i)
  {
    spl::common::Structure * const clone = getEngine()->createData();
    *clone = *structure; // Copy
    prepStructure(clone);
    out(clone);
  }

  prepStructure(structure);
  out(structure);
}

void
Clone::prepStructure(spl::common::Structure * const structure) const
{
  if(myGiveUniqueNames)
    structure->setName(
        common::generateStructureName(getEngine()->globalData()));
}

}
}
