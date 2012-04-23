/*
 * UtilityFunctions.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: muhrin
 */

// INCLUDES //////////////////////////////////
#include "UtilityFunctions.h"

// From SSTbx
#include <io/AdditionalData.h>

// From local
#include "common/StructureData.h"

// NAMESPACES ////////////////////////////////

namespace spipe { namespace common {


ProcessId getProcessId()
{
	return NS_BOOST_IPC_DETAIL::get_current_process_id();
}

std::string generateUniqueName()
{
	// Use boost as portable way to get the process id
	const ProcessId	processId	= getProcessId();
	const time_t	currTime	= time(NULL);

	// Build up the name
	std::stringstream ss;	//create a stringstream
	ss << processId << "-" << currTime;

	return ss.str();
}

void generateIoDataFromStructure(
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

void generateStructureDataFromIo(
  const sstbx::io::AdditionalData & ioData,
  spipe::common::StructureData & strData)
{
	if(ioData.enthalpy)
	{
		strData.enthalpy.reset(*ioData.enthalpy);
	}
	if(ioData.name)
	{
		strData.name.reset(*ioData.name);
	}
	if(ioData.pressure)
	{
		strData.pressure.reset(*ioData.pressure);
	}
	if(ioData.spaceGroup)
	{
		strData.spaceGroup.reset(*ioData.spaceGroup);
	}
	if(ioData.timesFound)
	{
		strData.timesFound.reset(*ioData.timesFound);
	}
}

}}
