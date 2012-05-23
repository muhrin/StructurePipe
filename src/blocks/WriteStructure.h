/*
 * WriteStructure.h
 * Write structures out to file.
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef WRITE_STRUCTURE_H
#define WRITE_STRUCTURE_H

// INCLUDES /////////////////////////////////////////////

#include "AbstractSimpleBlock.h"

// FORWARD DECLARATIONS ////////////////////////////////////


namespace sstbx
{
namespace io
{
struct AdditionalData;
class StructureWriterManager;
}
}


namespace spipe
{
namespace blocks
{


class WriteStructure : public pipelib::AbstractSimpleBlock<StructureDataTyp, SharedDataTyp>
{
public:
	WriteStructure(const ::sstbx::io::StructureWriterManager & writerManager);

	virtual void in(spipe::common::StructureData & data);


private:
	const ::sstbx::io::StructureWriterManager & myWriterManager;
};

}
}


#endif /* WRITE_STRUCTURE_H */
