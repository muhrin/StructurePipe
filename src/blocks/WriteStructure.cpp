/*
 * WriteStructure.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "StructurePipe.h"

#include "blocks/WriteStructure.h"

#include "common/StructureData.h"
#include "common/UtilityFunctions.h"

// From SSTbx
#include <io/AdditionalData.h>
#include <io/StructureWriterManager.h>

#include <boost/filesystem.hpp>

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
	generateIoData(data, ioData);
	
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

void WriteStructure::generateIoData(
	const ::spipe::common::StructureData & strData,
	sstbx::io::AdditionalData & ioData)
{
	if(strData.enthalpy)
	{
		ioData.enthalpy.reset(*strData.enthalpy);
	}
	if(strData.name)
	{
		ioData.name.reset(*strData.name);
	}
	if(strData.pressure)
	{
		ioData.pressure.reset(*strData.pressure);
	}
	if(strData.spaceGroup)
	{
		ioData.spaceGroup.reset(*strData.spaceGroup);
	}
	if(strData.timesFound)
	{
		ioData.timesFound.reset(*strData.timesFound);
	}
}

}}

