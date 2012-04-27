/*
 * LoadPotStructures.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef LOAD_POT_STRUCTURES_H
#define LOAD_POT_STRUCTURES_H

// INCLUDES /////////////////////////////////////////////
#include "StructurePipe.h"

// From Pipelib
#include <AbstractSimpleStartBlock.h>

#include <boost/filesystem.hpp>

#include <vector>


// FORWARD DECLARATIONS ////////////////////////////////////
namespace sstbx
{
  namespace common
  {
    class Structure;
  }
  namespace potential
  {
    class IParameterisable;
  }
}

namespace spipe { namespace blocks {

class LoadPotStructures : public pipelib::AbstractSimpleStartBlock<StructureDataTyp, SharedDataTyp>
{
public:

  LoadPotStructures(
    const sstbx::potential::IParameterisable &    pot,
    const boost::filesystem::path &               potPath,
    const bool                                    lowestEOnly
    );

  // From Block //
  //virtual void pipelineStarting();
  // End from Block //

  // From IStartBlock //
  virtual void start();
  // End from IStartBlock //

private:

  size_t loadStructures(
    const boost::filesystem::path &               strFolder,
    std::vector<spipe::StructureDataTyp *>  &    structureVec) const;

  const sstbx::potential::IParameterisable &    myPot;
  const boost::filesystem::path                 myPotPath;
  const bool                                    myLowestEOnly;
};


}}

#endif /* LOAD_POT_STRUCTURES_H */
