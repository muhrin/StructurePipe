/*
 * StructurePipe.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef STRUCTURE_PIPE_H
#define STRUCTURE_PIPE_H

#ifndef NULL
#define NULL 0
#endif

#define DEBUG 1

// Include the configuration header file
#include "StructurePipeConfig.h"

// Comment this out if YAML should not be used
#define SP_USE_YAML

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
namespace pipelib
{
template <class DataType, class PipelineDataType>
class Block;
template <class DataType, class PipelineDataType>
class IPipeline;
template <class DataType, class PipelineDataType>
class PipeBlock;
template <class DataType, class PipelineDataType>
class StartBlock;
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

typedef spipe::common::StructureData			                    StructureDataTyp;
typedef spipe::common::SharedData					                    SharedDataTyp;
typedef pipelib::IPipeline<StructureDataTyp, SharedDataTyp>  	IPipelineTyp;
typedef pipelib::Block<StructureDataTyp, SharedDataTyp>       SpBlockTyp;
typedef pipelib::PipeBlock<StructureDataTyp, SharedDataTyp>   SpPipeBlockTyp;
typedef pipelib::StartBlock<StructureDataTyp, SharedDataTyp>  SpStartBlockTyp;

}

#endif /* STRUCTURE_PIPE_H */
