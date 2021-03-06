/*
 * FindSymmetryGroup.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/FindSymmetryGroup.h"

#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/scoped_array.hpp>

#include <armadillo>

#include <spl/analysis/SpaceGroup.h>
#include <spl/common/AtomSpeciesId.h>
#include <spl/common/Structure.h>
#include <spl/common/UnitCell.h>

#include "spipe/common/StructureData.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {
namespace common = spipe::common;
namespace ssc = spl::common;
namespace ssa = spl::analysis;
namespace structure_properties = ssc::structure_properties;

FindSymmetryGroup::FindSymmetryGroup() :
    Block("Determine space group")
{
}

void
FindSymmetryGroup::in(spl::common::Structure * const structure)
{
  if(structure->getUnitCell())
  {
    ssa::space_group::SpacegroupInfo sgInfo;
    ssa::space_group::getSpacegroupInfo(sgInfo, *structure);

    structure->properties()[structure_properties::general::SPACEGROUP_NUMBER] =
        sgInfo.number;
    structure->properties()[structure_properties::general::SPACEGROUP_SYMBOL] =
        sgInfo.iucSymbol;
  }

  out(structure);
}

}
}
