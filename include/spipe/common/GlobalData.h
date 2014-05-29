/*
 * GlobalData.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <map>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include <spl/io/StructureReadWriteManager.h>
#include <spl/potential/IParameterisable.h>
#include <spl/utility/HeterogeneousMap.h>

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace common {

class GlobalData
{
public:
  typedef std::vector< std::string> Parameters;

  GlobalData();

  const std::string &
  getSeedName() const;
  void
  setSeedName(const std::string & seedName);

  spl::utility::HeterogeneousMap objectsStore;

  spl::io::StructureReadWriteManager &
  getStructureIo();

  const boost::filesystem::path &
  getWorkingDir() const;
  void
  setWorkingDir(const boost::filesystem::path & workingDir);

  void
  setParameters(const std::string & tag, const Parameters & params);
  bool
  updateParameterisable(const std::string & tag,
      spl::potential::IParameterisable * const parameterisable) const;

private:
  boost::filesystem::path myWorkingDir;
  std::string mySeedName;
  spl::io::StructureReadWriteManager myStructureIoManager;
  std::map< std::string, Parameters> myParameters;
};

}
}

#endif /* GLOBAL_DATA_H */
