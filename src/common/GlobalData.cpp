/*
 * GlobalData.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/common/GlobalData.h"

#include <spl/utility/UtilFunctions.h>

#include "spipe/common/UtilityFunctions.h"
#include "spipe/utility/PipeDataInitialisation.h"

// NAMESPACES ////////////////////////////////
namespace ssc = ::spl::common;
namespace ssio = ::spl::io;
namespace ssu = ::spl::utility;

namespace spipe {
namespace common {

GlobalData::GlobalData()
{
  utility::initStructureRwManDefault(myStructureIoManager);
}

const ::std::string &
GlobalData::getSeedName() const
{
  return mySeedName;
}

void
GlobalData::setSeedName(const ::std::string & seedName)
{
  mySeedName = seedName;
}

ssio::StructureReadWriteManager &
GlobalData::getStructureIo()
{
  return myStructureIoManager;
}

const ::boost::filesystem::path &
GlobalData::getWorkingDir() const
{
  return myWorkingDir;
}

void
GlobalData::setWorkingDir(const ::boost::filesystem::path & workingDir)
{
  myWorkingDir = workingDir;
}

void
GlobalData::setParameters(const std::string & tag, const Parameters & params)
{
  if(params.empty())
  {
    std::map< std::string, Parameters>::iterator it = myParameters.find(
        tag);
    if(it != myParameters.end())
      myParameters.erase(it);
  }
  else
    myParameters[tag] = params;
}

bool
GlobalData::updateParameterisable(const std::string & tag,
    spl::potential::IParameterisable * const parameterisable) const
{
  if(tag.empty())
    return false;

  std::map< std::string, Parameters>::const_iterator it = myParameters.find(
      tag);
  if(it == myParameters.end())
    return false;

  return parameterisable->setParams(it->second, NULL);
}

}
}
