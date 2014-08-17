/*
 * DataTableInserters.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/utility/DataTableInserters.h"

#include <vector>

#include <spl/common/Structure.h>
#include <spl/io/ResourceLocator.h>

#include "spipe/common/StructureData.h"
#include "spipe/common/UtilityFunctions.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace utility {

namespace splc = spl::common;
namespace splio = spl::io;
namespace structure_properties = spl::common::structure_properties;

bool
insertStructureInfoAndPotentialParams(const utility::DataTable::Key & key,
    const StructureDataType & structure,
    const boost::filesystem::path & pathsRelativeTo, DataTable & table)
{
  if(const std::vector< std::string> * const params =
      structure.properties().find(common::GlobalKeys::POTENTIAL_PARAMS))
  {
    // Update the table with the current parameters
    for(size_t i = 0; i < params->size(); ++i)
    {
      table.insert(key, "p_" + boost::lexical_cast< std::string>(i),
          (*params)[i]);
    }
  }

  if(const double * const internalEnergy = structure.properties().find(
      structure_properties::general::ENERGY_INTERNAL))
  {
    const double energy = *internalEnergy;
    table.insert(key, "energy", common::toString(energy));

    const size_t numAtoms = structure.getNumAtoms();
    table.insert(key, "energy/atom", common::toString(energy / numAtoms));
  }

  const splio::ResourceLocator locator = common::getRelativeSavePath(structure,
      pathsRelativeTo);
  if(!locator.empty())
    table.insert(key, "lowest_path", locator.string());

  insertStructureProperty(key, "sg",
      structure_properties::general::SPACEGROUP_NUMBER, structure, table);

  return true;
}

}
}
