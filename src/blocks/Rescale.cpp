/*
 * Rescale.cpp
 *
 *  Created on: May 22, 2014
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/Rescale.h"

#include <spl/build_cell/Utility.h>
#include <spl/common/Structure.h>

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

Rescale::Rescale(const double scaleFactor) :
    Block("Rescale"), myScaleFactor(scaleFactor)
{
}

void
Rescale::in(spl::common::Structure * const structure)
{
  if(const spl::common::UnitCell * const unitCell = structure->getUnitCell())
  {
    const spl::common::AtomsFormula & composition = structure->getComposition();
    const double naturalVolume = spl::build_cell::naturalVolume(composition,
        getEngine()->sharedData().getSpeciesDatabase(), true);
    structure->scale(myScaleFactor * naturalVolume / unitCell->getVolume());
  }

  out(structure);
}

}
}
