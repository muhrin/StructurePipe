/*
 * RandomStructure.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef RANDOM_STRUCTURE_H
#define RANDOM_STRUCTURE_H

// INCLUDES /////////////////////////////////////////////
#include "StructurePipe.h"

// From SSTbx


// From PipelineLib
#include <AbstractSimpleStartBlock.h>

// FORWARD DECLARATIONS ////////////////////////////////////

namespace sstbx
{
	namespace build_cell
	{
		class IStructureGenerator;
	}
}

namespace spipe { namespace blocks {

class RandomStructure :
	public pipelib::AbstractSimpleStartBlock<StructureDataTyp, SharedDataTyp>
{
public:
	RandomStructure(
		const size_t numToGenerate,
		const ::sstbx::build_cell::IStructureGenerator & structureGenerator);

	virtual void start();

	virtual void in(::spipe::common::StructureData & data);

private:

	const size_t myNumToGenerate;

	const ::sstbx::build_cell::IStructureGenerator &	myStructureGenerator;

};


}}

#endif /* RANDOM_STRUCTURE_H */
