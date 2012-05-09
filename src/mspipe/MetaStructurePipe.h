/*
 * MetaStructurePipe.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef META_STRUCTURE_PIPE_H
#define META_STRUCTURE_PIPE_H

// FORWARD DECLARES //////////////
namespace pipelib {
template <class DataType, class PipelineDataType>
class IPipeline;
}

namespace spipe {
namespace mspipe {
class MetaStructurePipeData;
class MetaStructurePipeSharedData;
}
}

namespace spipe
{
namespace mspipe
{

// TYPEDEFS ///////////////////////

typedef MetaStructurePipeData                             MspDataTyp;
typedef MetaStructurePipeSharedData			                  MspSharedDataTyp;
typedef pipelib::IPipeline<MspDataTyp, MspSharedDataTyp>	IPipelineTyp;

}
}

#endif /* META_STRUCTURE_PIPE_H */
