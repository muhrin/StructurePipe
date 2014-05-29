/*
 * NiggliReduce.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/NiggliReduce.h"

#include <spl/common/Structure.h>
#include <spl/common/UnitCell.h>

#include "spipe/common/StructureData.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace ssc = ::spl::common;

NiggliReduce::NiggliReduce() :
    Block("Niggli reduce")
{
}

void
NiggliReduce::in(spl::common::Structure * const structure)
{
  ssc::UnitCell * const cell = structure->getUnitCell();
  if(cell)
    cell->niggliReduce();

  out(structure);
}

}
}

