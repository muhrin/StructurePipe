/*
 * LoadSeedStructuresTest.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipetest.h"

#include <iterator>
#include <map>
#include <set>
#include <vector>

#include <boost/foreach.hpp>

#include <pipelib/pipelib.h>

#include <spl/build_cell/AtomsDescription.h>
#include <spl/build_cell/StructureBuilder.h>
#include <spl/common/AtomSpeciesDatabase.h>
#include <spl/common/Structure.h>

#include <spipe/SpTypes.h>
#include <spipe/StructurePipe.h>
#include <spipe/common/SharedData.h>
#include <spipe/common/StructureData.h>
#include <spipe/blocks/BuildStructures.h>
#include <spipe/blocks/SearchStoichiometries.h>

namespace splbc = spl::build_cell;
namespace ssc = spl::common;
namespace blocks = spipe::blocks;

typedef spipe::blocks::SearchStoichiometries::CountRange Range;
typedef spipe::blocks::SearchStoichiometries::AtomRanges AtomRanges;

class StoichiometrySink : public spipe::FinishedSink
{
  typedef spipe::StructureDataUniquePtr StructureDataPtr;
  typedef std::set< size_t> CountSet;
  typedef std::map< std::string, CountSet> AtomCounts;
public:
  StoichiometrySink(const AtomRanges & atomRanges)
  {
    // Populate with the individual counts e.g. 2, 3, 4, ..., 17
    BOOST_FOREACH(AtomRanges::const_reference range, atomRanges)
    {
      for(int i = range.second.min(); i <= range.second.max(); ++i)
        myAtomCounts[range.first].insert(static_cast< size_t>(i));
    }
  }

  virtual void
  finished(StructureDataPtr structure)
  {
    typedef std::set< ssc::AtomSpeciesId::Value> Species;

    // Tick off this structure species count
    Species species;
    structure->getAtomSpecies(
        std::insert_iterator< Species>(species, species.begin()));

    AtomCounts::iterator it;
    CountSet::iterator countIt;
    BOOST_FOREACH(const ssc::AtomSpeciesId::Value & s, species)
    {
      it = myAtomCounts.find(s);

      // Check that we were expecting this species in the structure
      BOOST_REQUIRE(it != myAtomCounts.end());

      countIt = it->second.find(structure->getNumAtomsOfSpecies(s));
      BOOST_REQUIRE(countIt != it->second.end());
    }
  }

  void
  doFinishedCheck() const
  {
  }

private:
  AtomCounts myAtomCounts;
};

BOOST_AUTO_TEST_CASE(StoichiometrySearchTest)
{
  typedef spipe::SerialEngine Engine;

  // SETTINGS ////
  blocks::SearchStoichiometries::Options searchOptions;
  searchOptions.atomRanges["Na"] = Range(1, 7);
  searchOptions.atomRanges["Cl"] = Range(4, 13);

  splbc::StructureBuilderPtr builder(new splbc::StructureBuilder);
  builder->insertAtoms(splbc::AtomsDescription("Na"));
  builder->insertAtoms(splbc::AtomsDescription("Cl"));

  spipe::BlockHandle buildStructures(new blocks::BuildStructures(1, builder));
  spipe::BlockHandle searchStoichiometries(
      new blocks::SearchStoichiometries(searchOptions, buildStructures));

  Engine engine;
  StoichiometrySink sink(searchOptions.atomRanges);

  engine.setFinishedDataSink(&sink);
  engine.run(searchStoichiometries);

  sink.doFinishedCheck();
}

