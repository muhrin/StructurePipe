/*
 * LoadStructures.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/LoadStructures.h"

#include <iterator>
#include <sstream>
#include <vector>

#include <boost/foreach.hpp>

#include <spl/common/AtomSpeciesDatabase.h>
#include <spl/common/Constants.h>
#include <spl/common/Structure.h>
#include <spl/io/IoFunctions.h>
#include <spl/io/ResourceLocator.h>
#include <spl/utility/UtilFunctions.h>

// Local includes
#include "spipe/common/PipeFunctions.h"
#include "spipe/common/SharedData.h"
#include "spipe/common/StructureData.h"
#include "spipe/common/UtilityFunctions.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace fs = boost::filesystem;
namespace ssbc = spl::build_cell;
namespace ssc = spl::common;
namespace ssio = spl::io;
namespace ssu = spl::utility;

LoadStructures::LoadStructures(const std::string & seedStructures) :
    Block("Load structures"), mySeedStructuresString(seedStructures)
{
}

void
LoadStructures::pipelineInitialising()
{
  if(myStructures.empty())
    loadStructures();
}

void
LoadStructures::start()
{
  using spipe::common::StructureData;

  BOOST_FOREACH(const spl::common::Structure & str, myStructures)
  {
    StructureData * const data = getEngine()->createData();
    ssc::Structure & structure = data->setStructure(
        UniquePtr< ssc::Structure>::Type(new ssc::Structure(str)));

    // Set up the structure name if needed
    if(structure.getName().empty())
      structure.setName(
          common::generateStructureName(getEngine()->globalData()));

    // Send it on its way
    out(data);
  }
}

int
LoadStructures::processEntry(const std::string & entry)
{
  const EntryType type = entryType(entry);

  if(type == FILE_PATH || type == FOLDER_PATH)
  {
    ssio::ResourceLocator loc;
    loc.set(entry);
    getEngine()->globalData().getStructureIo().readStructures(myStructures,
        loc);
  }
  else if(type == WILDCARD_PATH)
    return processWildcardEntry(entry);

  return -1;
}

int
LoadStructures::processWildcardEntry(const std::string & entry)
{
  std::vector< fs::path> entryPaths;

  if(!ssio::getWildcardPaths(entry, entryPaths))
    return -1;

  int numOut = 0;
  BOOST_FOREACH(const fs::path & entryPath, entryPaths)
  {
    if(fs::is_regular_file(entryPath))
    {
      getEngine()->globalData().getStructureIo().readStructures(myStructures,
          ssio::ResourceLocator(entryPath));
      ++numOut;
    }
  }
  return numOut;
}

LoadStructures::EntryType
LoadStructures::entryType(const std::string & entry) const
{
  if(entry.find('*') != std::string::npos)
    return WILDCARD_PATH;

  ssio::ResourceLocator entryLocator;
  if(!entryLocator.set(entry))
    return UNKNOWN;

  if(fs::exists(entryLocator.path()))
  {
    if(fs::is_regular_file(entryLocator.path()))
      return FILE_PATH;
    else if(fs::is_directory(entryLocator.path()))
      return FOLDER_PATH;
  }

  return UNKNOWN;
}

void
LoadStructures::loadStructures()
{
  SPIPE_ASSERT(getEngine());

  myStructures.clear();
  std::string entry;
  std::stringstream stream(mySeedStructuresString);
  while(getline(stream, entry))
    processEntry(entry);
}

}
}
