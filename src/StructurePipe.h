/*
 * StructurePipe.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef STRUCTURE_PIPE_H
#define STRUCTURE_PIPE_H

// Include the configuration header file
#include "StructurePipeConfig.h"

#define NULL 0

#define DEBUG 1

#if !DEBUG
	#define SP_ASSERT(x)
#else
	#define SP_ASSERT(x) \
	if (! (x)) \
	{ \
		std::cout << "ERROR!! Assert " << #x << " failed\n"; \
		std::cout << " on line " << __LINE__  << "\n"; \
		std::cout << " in file " << __FILE__ << "\n";  \
	}
#endif
		// Create a breakpoint
		//__asm { \
		//	int 3 \
		//} \


// FORWARD DECLARES //////////////
namespace pipelib {
	template <class DataType, class PipelineDataType>
	class IPipeline;
}

namespace spipe
{
	namespace common
	{
		class SharedData;
		class StructureData;
	}
}

namespace spipe
{

	// TYPEDEFS ///////////////////////

	typedef spipe::common::StructureData			StructureDataTyp;
	typedef spipe::common::SharedData					SharedDataTyp;
	typedef pipelib::IPipeline<StructureDataTyp, SharedDataTyp>	IPipelineTyp;

}

#endif /* STRUCTURE_PIPE_H */
