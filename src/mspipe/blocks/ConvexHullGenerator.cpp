/*
 * ConvexHullGenerator.cpp
 *
 *  Created on: May 4, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "mspipe/blocks/ConvexHullGenerator.h"

#include <boost/foreach.hpp>

// From PipeLib
#include <IPipeline.h>

// From SSTbx
#include <common/Structure.h>

// Local includes
#include "common/StructureData.h"
#include "mspipe/MetaStructurePipeData.h"
#include "mspipe/MetaStructurePipeSharedData.h"

// NAMESPACES ////////////////////////////////


namespace spipe {
namespace mspipe {
namespace blocks {

ConvexHullGenerator::ConvexHullGenerator():
Block<spipe::mspipe::MspDataTyp, spipe::mspipe::MspSharedDataTyp>("Convex hull generator")
{}


ConvexHullGenerator::~ConvexHullGenerator()
{
}

void ConvexHullGenerator::pipelineStarting()
{
  myHullStream.open("convex_hull.dat");
}

void ConvexHullGenerator::pipelineEnding()
{
  if(myHullStream.is_open())
    myHullStream.close();
}

void ConvexHullGenerator::in(mspipe::MspDataTyp & data)
{
  spipe::mspipe::MspSharedDataTyp & sharedDat = myPipeline->getSharedData();
  BOOST_FOREACH(const spipe::StructureDataTyp * strDat, data.resultStructures)
  {
    const ::sstbx::common::Structure * const str = strDat->getStructure();
    if(str)
    {
      const size_t s1 = str->getNumAtomsOfSpeciesDescendent(sharedDat.species1);
      const size_t s2 = str->getNumAtomsOfSpeciesDescendent(sharedDat.species2);

      if(myHullStream.is_open())
      {
        myHullStream << (double)s1/(double)(s1 + s2) << " " << *strDat->enthalpy << std::endl;
      }
    }
  }
  
}


}
}
}

