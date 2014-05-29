/*
 * FindSymmetryGroup.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef FIND_SYMMETRY_GROUP_H
#define FIND_SYMMETRY_GROUP_H

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <boost/noncopyable.hpp>

#include <pipelib/pipelib.h>

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace blocks {

class FindSymmetryGroup : public PipeBlock, boost::noncopyable
{
public:
  FindSymmetryGroup();

  // From PipeBlock //////////////////////
  virtual void
  in(spl::common::Structure * const structure);
  // End from PipeBlock /////////////////
};

}
}

#endif /* FIND_SYMMETRY_GROUP_H */
