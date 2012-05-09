/*
 * StoichiometrySearch.h
 *
 * Convex hull meta structure pipe block based on concepts developed
 * by Andrew Morris.
 *
 *  Created on: May 4, 2012
 *      Author: Martin Uhrin
 */

#ifndef STOICHIOMETRY_SEARCH_H
#define STOICHIOMETRY_SEARCH_H

// INCLUDES /////////////////////////////////////////////
#include "mspipe/MetaStructurePipe.h"

#include <vector>

// From PipeLib
#include <AbstractSimpleBlock.h>
#include <StartBlock.h>

// From SSTbx
#include <common/AtomSpeciesId.h>


// FORWARD DECLARATIONS ////////////////////////////////////
namespace pipelib
{
  template <class DataTyp, class SharedDataTyp>
  class AbstractSimpleBlock;
}


namespace spipe
{
namespace mspipe
{
namespace blocks
{

class StoichiometrySearch :
  public ::pipelib::AbstractSimpleBlock<mspipe::MspDataTyp, mspipe::MspSharedDataTyp>,
  public ::pipelib::StartBlock<mspipe::MspDataTyp, mspipe::MspSharedDataTyp>
{
public:

  StoichiometrySearch(
    const ::sstbx::common::AtomSpeciesId species1,
    const ::sstbx::common::AtomSpeciesId species2,
    const size_t maxAtoms,
    const double atomsRadius);

	virtual ~StoichiometrySearch();

	// From PipeBlock ////////////////////
	virtual void in(mspipe::MspDataTyp & data);
	// End from PipeBlock ///////////////

  // From StartBlock ///
  virtual void start();
  // End from StartBlock ///

private:

  const ::sstbx::common::AtomSpeciesId  mySpecies1;
  const ::sstbx::common::AtomSpeciesId  mySpecies2;
  const size_t                          myMaxAtoms;
  const double                          myAtomsRadius;



};

}
}
}

#endif /* STOICHIOMETRY_SEARCH_H */
