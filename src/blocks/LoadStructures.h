/*
 * LoadStructures.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef LOAD_STRUCTURES_H
#define LOAD_STRUCTURES_H

// INCLUDES /////////////////////////////////////////////
#include "StructurePipe.h"

#include <queue>
#include <string>

#include <boost/filesystem.hpp>

#include <spl/SSLib.h>
#include <spl/common/Types.h>
#include <spl/io/ResReaderWriter.h>

#include <pipelib/pipelib.h>

#include "SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////
namespace spl {
namespace common {
class AtomSpeciesDatabase;
class Structure;
}
namespace io {
class ResourceLocator;
}
}

namespace spipe {
namespace blocks {

class LoadStructures : public StartBlock, ::boost::noncopyable
{
public:
  static const double ATOMIC_VOLUME_MULTIPLIER;

  // seedStructures - Can be:
  // - path to single structure
  // - path to folder containing structures
  // - wildcard path to structures or folders containig structures
  // - a list containing a mixture of any of the above
  LoadStructures(const ::std::string & seedStructures, const bool tryToScaleVolumes = false);

  // From StartBlock ///
  virtual void
  pipelineInitialising();
  virtual void
  start();
  // End from StartBlock

private:
  typedef ::spl::common::StructurePtr StructurePtr;
  typedef ::spl::io::StructuresContainer StructuresContainer;

  enum EntryType
  {
    UNKNOWN, FILE_PATH, FOLDER_PATH, WILDCARD_PATH
  };

  int
  processEntry(const ::std::string & entry);
  int
  processWildcardEntry(const ::std::string & entry);
  EntryType
  entryType(const ::std::string & entry) const;

  double
  getTotalAtomicVolume(const ::spl::common::Structure & structure) const;

  const ::std::string mySeedStructuresString;
  ::std::queue< ::spl::io::ResourceLocator> myStructureLocations;
  const bool myTryToScaleVolumes;
};

}
}

#endif /* LOAD_SEED_STRUCTURES_H */
