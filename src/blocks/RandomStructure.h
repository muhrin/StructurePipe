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
		class ICrystalStructureGenerator;
    template <typename FloatType>
    class RandomCellDescription;
    class StructureDescription;
	}
}

namespace spipe
{
namespace blocks {

class RandomStructure :
	public pipelib::AbstractSimpleStartBlock<StructureDataTyp, SharedDataTyp>
{
public:
	RandomStructure(
		const size_t numToGenerate,
		const ::sstbx::build_cell::ICrystalStructureGenerator &           structureGenerator,
    const ::sstbx::build_cell::StructureDescription * const           structureDescription = NULL,
    const ::sstbx::build_cell::RandomCellDescription<double> * const  cellDescription = NULL);

  // From StartBlock ///
	virtual void start();
  // End from StartBlock


  // From PipeBlock //
	virtual void in(::spipe::common::StructureData & data);
  // End from PipeBlock

private:

  virtual void initDescriptions();

	const size_t myNumToGenerate;

  /** Should the block use the structure description found in shared data */
  const bool  myUseSharedDataStructureDesc;

  /** Should the block use the cell description found in shared data */
  const bool  myUseSharedDataCellDesc;

  const ::sstbx::build_cell::StructureDescription * myStructureDescription;

  const ::sstbx::build_cell::RandomCellDescription<double> * myCellDescription;

	const ::sstbx::build_cell::ICrystalStructureGenerator &	myStructureGenerator;

};


}
}

#endif /* RANDOM_STRUCTURE_H */
