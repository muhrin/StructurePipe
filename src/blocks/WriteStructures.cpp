/*
 * WriteStructures.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/WriteStructures.h"

#include <spl/common/Structure.h>
#include <spl/io/StructureReadWriteManager.h>
#include <spl/io/BoostFilesystem.h>
#include <spl/utility/UtilFunctions.h>

// From local
#include "spipe/common/PipeFunctions.h"
#include "spipe/common/SharedData.h"
#include "spipe/common/StructureData.h"
#include "spipe/common/UtilityFunctions.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace fs = boost::filesystem;
namespace ssc = spl::common;
namespace ssio = spl::io;
namespace ssu = spl::utility;

const bool WriteStructures::WRITE_MULTI_DEFAULT = true;
const std::string WriteStructures::FORMAT_DEFAULT = "res";

WriteStructures::WriteStructures() :
    Block("Write structures"), myState(State::DISABLED), myWriteMultiStructure(
        WRITE_MULTI_DEFAULT), myMultiStructureFromPath(false)
{
}

bool
WriteStructures::getWriteMulti() const
{
  return myWriteMultiStructure;
}

void
WriteStructures::setWriteMulti(const bool writeMulti)
{
  myWriteMultiStructure = true;
}

const std::string &
WriteStructures::getFileType() const
{
  return myFileType;
}

void
WriteStructures::setFileType(const std::string & extension)
{
  myFileType = extension;
}

void
WriteStructures::pipelineStarting()
{
  const ssio::StructureReadWriteManager & rwMan =
      getEngine()->globalData().getStructureIo();

  // Do we want to use a custom writer and does it exist?
  if(!myFileType.empty() && rwMan.getWriter(myFileType))
    myState = State::USE_CUSTOM_WRITER;
  else if(rwMan.getDefaultWriter()) // Try the default
    myState = State::USE_DEFAULT_WRITER;
  else
  {
    // TODO: Log error
    myState = State::DISABLED;
  }
}

void
WriteStructures::in(StructureDataType * const structure)
{
  const ssio::StructureReadWriteManager & rwMan =
      getEngine()->globalData().getStructureIo();

  if(myState != State::DISABLED)
  {
    const ssio::IStructureWriter * writer = NULL;

    if(myState == State::USE_CUSTOM_WRITER)
      writer = rwMan.getWriter(myFileType);
    else
      writer = rwMan.getDefaultWriter();

    if(writer)
    {
      const ssio::ResourceLocator saveLocation = generateLocator(*structure,
          *writer);

      bool writeSuccessful;
      if(myState == State::USE_CUSTOM_WRITER)
        writeSuccessful = rwMan.writeStructure(*structure, saveLocation,
            myFileType);
      else
        writeSuccessful = rwMan.writeStructure(*structure, saveLocation);

      if(!writeSuccessful)
      {
        // TODO: Produce error
      }
    }
    else
    {
      // TODO: Error couldn't find writer
    }
  }
  out(structure);
}

ssio::ResourceLocator
WriteStructures::generateLocator(ssc::Structure & structure,
    const ssio::IStructureWriter & writer) const
{
  // Check if the structure has a name already, otherwise give it one
  if(structure.getName().empty())
    structure.setName(common::generateStructureName(getEngine()->globalData()));

  // Create the path to store the structure
  fs::path p = workingDir();

  // Should all the structures be stored in one file or separate files?
  if(useMultiStructure(writer))
  {
    if(!myMultiStructureFromPath)
      p /= common::getOutputFileStem(getEngine()->sharedData(),
          getEngine()->globalData());
  }
  else
    p /= fs::path(structure.getName());

  // Make sure to add on the extension if we're using a custom file type
  if(myState == State::USE_CUSTOM_WRITER)
    p = p.string() + "." + myFileType;

  return ssio::ResourceLocator(p, structure.getName());
}

bool
WriteStructures::useMultiStructure(const ssio::IStructureWriter & writer) const
{
  if(myWriteMultiStructure && writer.multiStructureSupport())
    return true;

  return false;
}

}
}

