/*
 * WriteStructure.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "StructurePipe.h"

#include <boost/filesystem.hpp>

// From SSTbx
#include <io/AdditionalData.h>
#include <io/StructureWriterManager.h>

// From local
#include "blocks/WriteStructure.h"
#include "common/StructureData.h"
#include "common/UtilityFunctions.h"


// NAMESPACES ////////////////////////////////


namespace spipe { namespace blocks {

WriteStructure::WriteStructure(const ::sstbx::io::StructureWriterManager & writerManager):
pipelib::Block<StructureDataTyp, SharedDataTyp>("Write structures"),
myWriterManager(writerManager)
{}

void WriteStructure::in(::spipe::common::StructureData & data)
{
	using ::boost::filesystem::path;

	// Check if the structure has a name already, otherwise give it one
	if(!data.name)
	{
		data.name.reset(::spipe::common::generateUniqueName());
	}

	// Create additional data to go with the structure
	sstbx::io::AdditionalData ioData;
	generateIoDataFromStructure(data, ioData);
	
	// Create the path to store the structure
	path p(*data.name + ".res");
	if(data.group)
	{
		path folder(*data.group);
		p = folder / p;
	}
	
	myWriterManager.writeStructure(*data.getStructure(), p, &ioData);

	myOutput->in(data);
}

}}

