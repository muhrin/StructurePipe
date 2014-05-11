/*
 * Clone.h
 *
 *
 *  Created on: Dec 11, 2013
 *      Author: Martin Uhrin
 */

#ifndef CLONE_H
#define CLONE_H

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <boost/noncopyable.hpp>

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace blocks {

class Clone : public PipeBlock, ::boost::noncopyable
{
public:
  Clone(const int times);
  Clone(const int times, const bool giveUniqueNames);

  // From Block /////////////////
  virtual void
  in(common::StructureData * const data);
  // End from Block /////////////

private:
  void
  prepStructure(spl::common::Structure * const structure) const;

  const int myTimes;
  const bool myGiveUniqueNames;
};

}
}

#endif /* CLONE_H */
