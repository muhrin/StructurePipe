/*
 * UtilityFunctions.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: muhrin
 */

// INCLUDES //////////////////////////////////
#include "UtilityFunctions.h"

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

}}