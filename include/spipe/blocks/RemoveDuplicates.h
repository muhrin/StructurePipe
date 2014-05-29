/*
 * RemoveDuplicates.h
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef REMOVE_DUPLICATES_H
#define REMOVE_DUPLICATES_H

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <map>

#include <boost/noncopyable.hpp>
#ifdef SPIPE_USE_BOOST_THREAD
#  include <boost/thread/mutex.hpp>
#endif

#include <pipelib/pipelib.h>

#include <spl/utility/UniqueStructureSet.h>
#include <spl/utility/UtilityFwd.h>

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spl {
namespace common {
class Structure;
}
}

namespace spipe {
namespace blocks {

class RemoveDuplicates : public Barrier, ::boost::noncopyable
{
public:
  RemoveDuplicates(spl::utility::IStructureComparatorPtr comparator);
  RemoveDuplicates(const spl::utility::IStructureComparator & comparator);

  virtual void
  in(spl::common::Structure * const data);

  // From Block /////////////////////////
  virtual void
  pipelineFinishing();
  // End from Block ///////////////////

  // From Barrier ////////////////////////
  virtual size_t
  release();
  virtual bool
  hasData() const;
  // End from Barrier ///////////////////

private:
  typedef spl::utility::UniqueStructureSet< > StructureSet;

  StructureSet myStructureSet;

#ifdef SPIPE_USE_BOOST_THREAD
  ::boost::mutex myMutex;
#endif
};

}
}

#endif /* REMOVE_DUPLICATES_H */
