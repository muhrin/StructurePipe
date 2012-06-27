/*
 * SimplePairPotentialData.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef SIMPLE_PAIR_POTENTIAL_DATA_H
#define SIMPLE_PAIR_POTENTIAL_DATA_H

// INCLUDES /////////////////////////////////////////////

#include <vector>

#include "common/AtomSpeciesId.h"
#include "common/Structure.h"
#include "potential/SimplePairPotential.h"
#include "potential/StandardData.h"

// FORWARD DECLARATIONS ////////////////////////////////////


// DEFINITION //////////////////////////////////////////////

namespace sstbx
{
namespace potential
{

template <typename FloatType = double>
struct SimplePairPotentialData : public StandardData<FloatType>
{
  // TODO: Sort this
  // should be: typedef typename SimplePairPotential<FloatType>::SpeciesList SpeciesList;
  typedef typename ::std::vector< typename ::sstbx::common::AtomSpeciesId::Value>  SpeciesList;

	SimplePairPotentialData(
    const sstbx::common::Structure & structure,
    const SpeciesList &              speciesList);

	std::vector<int>				species;

};

// IMPLEMENTATION //////////////////////////////////////////

template <typename FloatType>
SimplePairPotentialData<FloatType>::SimplePairPotentialData(
  const sstbx::common::Structure & structure,
  const typename SimplePairPotentialData<FloatType>::SpeciesList & speciesList):
StandardData<FloatType>(structure)
{
  using sstbx::common::AtomSpeciesId;

	// Get the atom species
  std::vector<AtomSpeciesId::Value> strSpecies;
	structure.getAtomSpeciesDescendent(strSpecies);

	// Now populate our species vector
  const size_t numAtoms = strSpecies.size();
	species.resize(numAtoms);

  bool found;
  const size_t numSpecies = speciesList.size();
  AtomSpeciesId::Value currentSpecies;
  for(size_t i = 0; i < numAtoms; ++i)
  {
    currentSpecies = strSpecies[i];
    found = false;
    for(size_t j = 0; j < numSpecies; ++i)
    {
      if(currentSpecies == speciesList[i])
      {
        found = true;
        species[i] = j;
        break;
      }
    }
    if(!found)
    {
      species[i] = SimplePairPotential<FloatType>::IGNORE_ATOM;
    }
  }

}

}
}

#endif /* SIMPLE_PAIR_POTENTIAL_DATA_H */
