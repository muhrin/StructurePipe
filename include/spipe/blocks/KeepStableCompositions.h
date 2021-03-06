/*
 * KeepStableCompositions.h
 *
 *
 *  Created on: Nov 21, 2013
 *      Author: Martin Uhrin
 */

#ifndef KEEP_STABLE_COMPOSITIONS_H
#define KEEP_STABLE_COMPOSITIONS_H

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <spl/SSLib.h>

#ifdef SPL_USE_CGAL

#include <set>

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#ifdef SPIPE_USE_BOOST_THREAD
#  include <boost/thread/mutex.hpp>
#endif

#include <pipelib/pipelib.h>

#include <spl/analysis/ConvexHullOutputter.h>
#include <spl/analysis/ConvexHullStructures.h>

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace blocks {

class KeepStableCompositions : public Barrier, boost::noncopyable
{
public:
  KeepStableCompositions();
  KeepStableCompositions(const bool writeHull);

  // From Block /////////////////
  virtual void
  pipelineInitialised();
  virtual void
  in(spl::common::Structure * const data);
  virtual void
  pipelineFinishing();
  // End from Block /////////////

  // From Barrier /////////////////
  virtual size_t
  release();
  virtual bool
  hasData() const;
  // End from Barrier //////////////

private:
  typedef std::set< spl::common::Structure *> StructureStore;

  const bool myWriteHull;
  spl::UniquePtr< spl::analysis::ConvexHullOutputter>::Type myOutputter;
  StructureStore myStructureStore;
  mutable boost::scoped_ptr< spl::analysis::ConvexHullStructures> myConvexHullStructures;

#ifdef SPIPE_USE_BOOST_THREAD
  boost::mutex myMutex;
#endif
};

}
}

#endif // SPL_USE_CGAL
#endif /* KEEP_STABLE_COMPOSITIONS_H */
