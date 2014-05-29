/*
 * KeepStableCompositions.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/KeepStableCompositions.h"

#ifdef SPL_WITH_CGAL

#include <boost/iterator/transform_iterator.hpp>
#include <boost/thread/lock_guard.hpp>

#include <spl/analysis/GnuplotConvexHullPlotter.h>
#include <spl/common/Structure.h>

#include <pipelib/pipelib.h>

// NAMESPACES ////////////////////////////////
namespace spla = spl::analysis;
namespace splc = spl::common;
namespace splu = spl::utility;

namespace spipe {
namespace blocks {

KeepStableCompositions::KeepStableCompositions() :
    Block("Keep stable compositions"), myWriteHull(false)
{
}

KeepStableCompositions::KeepStableCompositions(const bool writeHull) :
    Block("Keep stable compositions"), myWriteHull(writeHull)
{
}

void
KeepStableCompositions::pipelineInitialised()
{
  if(myWriteHull)
  {
    spla::GnuplotConvexHullPlotter::Settings settings;
    settings.outputDir = workingDir();
    myOutputter.reset(new spla::GnuplotConvexHullPlotter(settings));
  }
}

void
KeepStableCompositions::in(spl::common::Structure * const structure)
{
#ifdef SPIPE_USE_BOOST_THREAD
  boost::lock_guard< boost::mutex> lock(myMutex);
#endif
  myStructureStore.insert(structure);
  myConvexHullStructures.reset();
}

void
KeepStableCompositions::pipelineFinishing()
{
  myStructureStore.clear();
  myConvexHullStructures.reset();
}

size_t
KeepStableCompositions::release()
{
  if(!myConvexHullStructures.get())
  {
    const size_t numReleased = myStructureStore.size();
    BOOST_FOREACH(spl::common::Structure * const str, myStructureStore)
      out(str);
    myStructureStore.clear();
    return numReleased;
  }
  else if(myConvexHullStructures->numStable() == 0)
    return 0;

  if(myOutputter.get())
    myOutputter->outputHull(myConvexHullStructures->getHull(),
        *myConvexHullStructures);

  size_t numStable = 0;
  StructureStore::iterator it;
  while(!myStructureStore.empty())
  {
    it = myStructureStore.begin();
    if(myConvexHullStructures->getStability(**it)
        == spla::ConvexHullStructures::Stability::STABLE)
    {
      out(*it);
      ++numStable;
    }
    else
      drop(*it);

    myStructureStore.erase(it);
  }

  return numStable;
}

bool
KeepStableCompositions::hasData() const
{
  if(myStructureStore.empty())
    return false;

  const spla::ConvexHullStructures::EndpointLabels endpoints =
      spla::ConvexHullStructures::generateEndpoints(myStructureStore.begin(),
          myStructureStore.end());

  if(endpoints.size() < 2)
    return true; // We're going to release all the structures in this case

  myConvexHullStructures.reset(new spla::ConvexHullStructures(endpoints));
  myConvexHullStructures->insert(myStructureStore.begin(),
      myStructureStore.end());

  return myConvexHullStructures->numStable() != 0;
}

}
}

#endif // SPL_WITH_CGAL
