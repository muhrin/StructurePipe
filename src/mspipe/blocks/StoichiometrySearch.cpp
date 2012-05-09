/*
 * StoichiometrySearch.cpp
 *
 *  Created on: May 4, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "mspipe/blocks/StoichiometrySearch.h"

#include <boost/optional.hpp>

// From Pipelib
#include <IPipeline.h>

// From SSTbx
#include <build_cell/AtomsDescription.h>
#include <build_cell/Minsep.h> // TODO: TEMPORARY
#include <build_cell/RandomCellDescription.h>
#include <build_cell/StructureDescription.h>

// Local includes
#include "mspipe/MetaStructurePipe.h"
#include "mspipe/MetaStructurePipeData.h"

// NAMESPACES ////////////////////////////////


namespace spipe {
namespace mspipe {
namespace blocks {

StoichiometrySearch::StoichiometrySearch(
  const ::sstbx::common::AtomSpeciesId species1,
  const ::sstbx::common::AtomSpeciesId species2,
  const size_t maxAtoms,
  const double atomsRadius):
pipelib::Block<mspipe::MspDataTyp, mspipe::MspSharedDataTyp>("Convex hull"),
mySpecies1(species1),
mySpecies2(species2),
myMaxAtoms(maxAtoms),
myAtomsRadius(atomsRadius)
{}

StoichiometrySearch::~StoichiometrySearch()
{}

void StoichiometrySearch::in(mspipe::MspDataTyp & data)
{
  // TODO: Implement, needs a copy constructor on MspDataTyp
}

void StoichiometrySearch::start()
{
  for(size_t i = 0; i <= myMaxAtoms; ++i)
  {
    mspipe::MspDataTyp & data = myPipeline->newData();

    data.structureDescription = new ::sstbx::build_cell::StructureDescription();

    data.structureDescription->addChild(
      new ::sstbx::build_cell::AtomsDescription(mySpecies1, i));
    data.structureDescription->addChild(
      new ::sstbx::build_cell::AtomsDescription(mySpecies2, myMaxAtoms - i));

    data.structureDescription->addAtomConstraint(new ::sstbx::build_cell::Minsep(1.5));

    data.cellDescription = new ::sstbx::build_cell::RandomCellDescription<double>();

    data.cellDescription->myVolume.reset(myMaxAtoms * myAtomsRadius);

	  myOutput->in(data);
  }
}

}
}
}

