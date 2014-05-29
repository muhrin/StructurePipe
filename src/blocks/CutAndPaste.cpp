/*
 * CutAndPaste.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/CutAndPaste.h"

#include <boost/foreach.hpp>

#include <spl/common/Atom.h>

#include "spipe/common/Assert.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace splbc = spl::build_cell;
namespace splc = spl::common;

CutAndPaste::CutAndPaste(ShapePtr shape, Settings & settings):
    Block("Cut and paste"),
    myShape(shape),
    mySettings(settings)
{
  SPIPE_ASSERT(myShape.get());
}

void
CutAndPaste::setPairDistances(const spl::factory::PairDistances & distances)
{
  myPairDistances = distances;
}

void
CutAndPaste::pipelineInitialised()
{
  mySpeciesDb = getEngine()->sharedData().getSpeciesDatabase();
  BOOST_FOREACH(spl::factory::PairDistances::const_reference dist, myPairDistances)
    mySpeciesDb.setSpeciesPairDistance(dist.first, dist.second);
}

void
CutAndPaste::in(spl::common::Structure * const structure)
{
  std::vector<splc::Atom> cutAtoms;
  for(splc::Structure::AtomIterator it = structure->atomsBegin();
      it != structure->atomsEnd(); /* increment in body */)
  {
    if(myShape->isInShape(it->getPosition()))
    {
      cutAtoms.push_back(*it);
      it = structure->eraseAtom(it);
    }
    else
      ++it;
  }

  if(mySettings.paste)
  {
    const size_t numUntouched = structure->getNumAtoms();
    BOOST_FOREACH(splc::Atom & atom, cutAtoms)
    {
      atom.setPosition(myShape->randomPoint());
      structure->newAtom(atom);
    }

    if(mySettings.separate)
    {
      splbc::SeparationData sepData(*structure, mySpeciesDb);
      if(mySettings.fixUntouched)
      {
        for(size_t i = 0; i < numUntouched; ++i)
          sepData.fixedPoints.insert(i);
      }
      mySeparator.separatePoints(&sepData);
      structure->setAtomPositions(sepData.points);
    }
  }

  out(structure);
}

}
}
