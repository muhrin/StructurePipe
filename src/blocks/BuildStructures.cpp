/*
 * BuildStructures.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/BuildStructures.h"

#include <cmath>

#include <boost/optional.hpp>

#include <spl/SSLib.h>
#include <spl/build_cell/AtomsDescription.h>
#include <spl/build_cell/GenerationOutcome.h>
#include <spl/build_cell/GenerationSettings.h>
#include <spl/build_cell/StructureGenerator.h>
#include <spl/common/Constants.h>
#include <spl/common/Structure.h>
#include <spl/common/Types.h>
#include <spl/utility/UtilFunctions.h>

// Local includes
#include "spipe/common/PipeFunctions.h"
#include "spipe/common/UtilityFunctions.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace ssbc = spl::build_cell;
namespace ssc = spl::common;
namespace ssu = spl::utility;

const int BuildStructures::DEFAULT_MAX_ATTEMPTS = 1000;

void
BuildStructures::start()
{
  using spipe::common::StructureData;

  int numToGenerate = myFixedNumGenerate ? myNumToGenerate : 100;

  float totalAtomsGenerated = 0.0;
  for(int i = 0; i < numToGenerate; ++i)
  {
    bool generatedStructure = false;
    for(int attempt = 0; !generatedStructure && attempt < myMaxAttempts;
        ++attempt)
    {
      // Create the random structure
      ssc::StructurePtr str = generateStructure();

      if(str.get() && str->getNumAtoms() != 0)
      {
        generatedStructure = true;

        spl::common::Structure * const structure = getEngine()->createData();
        *structure = *str;
        structure->setName(generateStructureName(i));

        if(!myFixedNumGenerate)
        {
          totalAtomsGenerated += static_cast< float>(str->getNumAtoms());
          numToGenerate = static_cast< int>(std::ceil(
              myAtomsMultiplierGenerate * totalAtomsGenerated
                  / static_cast< float>(i)));
        }

        // Send it down the pipe
        out(structure);
      }
    }
  }
}

std::string
BuildStructures::generateStructureName(const size_t structureNum) const
{
  // Build up the name
  std::stringstream ss;
  ss << common::generateStructureName(getEngine()->globalData()) << "-"
      << structureNum;
  return ss.str();
}

ssc::StructurePtr
BuildStructures::generateStructure() const
{
  ssc::StructurePtr str;
  const ssbc::GenerationSettings * const settings =
      getEngine()->globalData().objectsStore.find(
          common::GlobalKeys::GENERATION_SETTINGS);

  if(settings)
    myStructureGenerator->generateStructure(str,
        getEngine()->sharedData().getSpeciesDatabase(), *settings);
  else
    myStructureGenerator->generateStructure(str,
        getEngine()->sharedData().getSpeciesDatabase());
  return str;
}

}
}
