/*
 * RemoveDuplicates.h
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef REMOVE_DUPLICATES_H
#define REMOVE_DUPLICATES_H

// INCLUDES /////////////////////////////////////////////

#include <map>

#include <pipelib/pipelib.h>

#include "PipeLibTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace sstbx {
namespace common {
class Structure;
}
namespace utility {
class UniqueStructureSet;
}
}

namespace spipe {
namespace blocks {

class RemoveDuplicates : public pipelib::PipeBlock<StructureDataTyp, SharedDataTyp, SharedDataTyp>
{
public:
	RemoveDuplicates(sstbx::utility::UniqueStructureSet & structureSet);

	virtual void in(::spipe::common::StructureData & data);

  // From Block /////////////////////////
	virtual void pipelineFinishing();
  // End from Block ///////////////////

private:
	typedef ::std::vector<StructureDataHandle> StructureDataHandles;

	sstbx::utility::UniqueStructureSet & myStructureSet;

	StructureDataHandles	myStructureDataHandles;
};

}
}

#endif /* REMOVE_DUPLICATES_H */
