/*
 * MetaStructurePipeSharedData.h
 *
 *
 *  Created on: May 4, 2011
 *      Author: Martin Uhrin
 */

#ifndef META_STRUCTURE_PIPE_SHARED_DATA_H
#define META_STRUCTURE_PIPE_SHARED_DATA_H

// INCLUDES /////////////////////////////////////////////

#include <boost/optional.hpp>

#include <armadillo>

#include <common/AtomSpeciesId.h>

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace mspipe {

class MetaStructurePipeSharedData
{
public:

  ::sstbx::common::AtomSpeciesId species1;
  ::sstbx::common::AtomSpeciesId species2;

private:

};


}
}

#endif /* META_STRUCTURE_PIPE_SHARED_DATA_H */
