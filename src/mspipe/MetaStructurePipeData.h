/*
 * MetaStructurePipeData.h
 *
 *
 *  Created on: May 4, 2011
 *      Author: Martin Uhrin
 */

#ifndef META_STRUCTURE_PIPE_DATA_H
#define META_STRUCTURE_PIPE_DATA_H

// INCLUDES /////////////////////////////////////////////

#include <vector>

#include <boost/optional.hpp>

#include <armadillo>

// Local includes 
#include "StructurePipe.h"

// FORWARD DECLARATIONS ////////////////////////////////////
namespace sstbx
{
  namespace build_cell
  {
    template <typename FloatType>
    class RandomCellDescription;
    class StructureDescription;
  }
	namespace common
	{
		class Structure;
	}
}

namespace spipe
{
namespace mspipe
{

class MetaStructurePipeData
{
public:
	MetaStructurePipeData();
	~MetaStructurePipeData();

  ::std::vector<spipe::StructureDataTyp *>  resultStructures;

  ::sstbx::build_cell::StructureDescription * structureDescription;

  ::sstbx::build_cell::RandomCellDescription<double> * cellDescription;

private:

};


}
}

#endif /* META_STRUCTURE_PIPE_DATA_H */
