/*
 * RandomStructure.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "blocks/RandomStructure.h"

#include "common/StructureData.h"
#include "common/UtilityFunctions.h"

// From SSTbx
#include <build_cell/DefaultCrystalGenerator.h>
#include <build_cell/RandomCellGenerator.h>
#include <common/AbstractFmidCell.h>
#include <common/Structure.h>

// From PipelineLib
#include <IPipeline.h>

// NAMESPACES ////////////////////////////////


namespace spipe { namespace blocks {

RandomStructure::RandomStructure(
	const size_t numToGenerate,
	const ::sstbx::build_cell::IStructureGenerator & structureGenerator):
pipelib::Block<StructureDataTyp, SharedDataTyp>("Random structures"),
myNumToGenerate(numToGenerate),
myStructureGenerator(structureGenerator)
{
}

void RandomStructure::start()
{
	using ::spipe::common::StructureData;
	for(size_t i = 0; i < myNumToGenerate; ++i)
	{
		StructureData & data = myPipeline->newData();

		// Build up the name
		std::stringstream ss;
		ss << ::spipe::common::generateUniqueName() << "-" << i;
		data.name.reset(ss.str());

		in(data);
	}
}

void RandomStructure::in(::spipe::common::StructureData & data)
{
	// TODO: Check if the structure was created successfully, otherwise delete!
	// Create the random structure
	::sstbx::common::Structure * str = myStructureGenerator.generateStructure();

	if(str)
	{
		data.setStructure(str);

		// Build up the name
		if(!data.name)
		{
			std::stringstream ss;
			ss << ::spipe::common::generateUniqueName();
			data.name.reset(ss.str());
		}

		// Send it down the pipe
		myOutput->in(data);
	}
	else
	{
		myPipeline->dropData(data);
	}
}

}}
