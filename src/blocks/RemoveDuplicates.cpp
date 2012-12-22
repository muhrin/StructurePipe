/*
 * RemoveDuplicates.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "StructurePipe.h"

#include "blocks/RemoveDuplicates.h"

#include "common/StructureData.h"

// From SSTbx
#include <common/Structure.h>
#include <utility/UniqueStructureSet.h>

#include <boost/foreach.hpp>

#include <map>

// NAMESPACES ////////////////////////////////


namespace spipe {
namespace blocks {

namespace ssu = ::sstbx::utility;

RemoveDuplicates::RemoveDuplicates(ssu::UniqueStructureSet & structureSet):
pipelib::Block<StructureDataTyp, SharedDataTyp, SharedDataTyp>("Remove duplicates"),
myStructureSet(structureSet)
{}

void RemoveDuplicates::in(::spipe::common::StructureData & data)
{
  const std::pair<ssu::UniqueStructureSet::iterator, bool> result = myStructureSet.insert(data.getStructure());

	if(result.second)
	{
		// Flag the data to say that we will want to use it again
    myStructureDataHandles.push_back(getRunner()->createDataHandle(data));
		data.timesFound.reset(1);

		out(data);
	}
	else
	{
		// The structure is not unique so discard it
		getRunner()->dropData(data);

		// Up the 'times found' counter on the original structure
    // TODO: FINISH!

		//::spipe::common::StructureData & origStrData = *it->second;
		//if(origStrData.timesFound)
		//	origStrData.timesFound.reset(*origStrData.timesFound + 1);
		//else
		//	origStrData.timesFound.reset(1);


	}
}

void RemoveDuplicates::pipelineFinishing()
{
	// Make sure we clean up any data we are holding on to
  BOOST_FOREACH(const StructureDataHandle & handle, myStructureDataHandles)
	{
		getRunner()->releaseDataHandle(handle);
	}
	myStructureDataHandles.clear();
	myStructureSet.clear();
}

}
}
