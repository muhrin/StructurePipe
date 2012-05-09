/*
 * MetaStructurePipeData.cpp
 *
 *  Created on: May 4, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "mspipe/MetaStructurePipeData.h"

#include <boost/foreach.hpp>

// From SSLib
#include <build_cell/StructureDescription.h>

// Local includes
#include "common/StructureData.h"

// NAMESPACES ////////////////////////////////

namespace spipe
{
namespace mspipe
{

MetaStructurePipeData::MetaStructurePipeData():
structureDescription(NULL),
cellDescription(NULL)
{}

MetaStructurePipeData::~MetaStructurePipeData()
{
  // Delete any structure data that we are holding
  BOOST_FOREACH(spipe::StructureDataTyp * const str, resultStructures)
  {
    delete str;
  }
  if(structureDescription)
    delete structureDescription;

  if(cellDescription)
    delete cellDescription;
}

}
}

