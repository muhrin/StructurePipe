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


// TODO: Make TpsdGeomOptimiser have interface superclass that is non generic
// so that this block does not need to be generic itself.

namespace spipe { namespace blocks {


class WriteStructure : public pipelib::AbstractSimpleBlock<StructureDataTyp, SharedDataTyp>
{
public:
	WriteStructure(const ::sstbx::io::StructureWriterManager & writerManager);

	virtual void in(spipe::common::StructureData & data);


protected:

	void generateIoData(
		const ::spipe::common::StructureData & strData,
		sstbx::io::AdditionalData & ioData);

private:
	const ::sstbx::io::StructureWriterManager & myWriterManager;
};

}}


#endif /* WRITE_STRUCTURE_H */
