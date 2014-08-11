/*
 * AutoScale.cpp
 *
 *  Created on: Jan 23, 2014
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/AutoScale.h"

#include <spl/math/RunningStats.h>
#include <spl/common/Constants.h>
#include <spl/potential/IPotentialEvaluator.h>
#include <spl/utility/StableComparison.h>

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

const double AutoScale::DEFAULT_PACKING_FACTOR = 0.3;

AutoScale::AutoScale() :
    Block("AutoScale"), myPackingFactor(DEFAULT_PACKING_FACTOR)
{
}

AutoScale::AutoScale(const double packingFactor) :
    Block("AutoScale"), myPackingFactor(packingFactor)
{
}

void
AutoScale::in(StructureDataType * const structure)
{
  if(spl::common::UnitCell * const unitCell = structure->getUnitCell())
  {
    std::vector< std::string> species;
    structure->getAtomSpecies(std::back_inserter(species));
    unitCell->setVolume(naturalVolume(species) / myPackingFactor);
  }

  out(structure);
}

double
AutoScale::naturalVolume(const std::vector< std::string> & particles) const
{
  typedef std::map< std::string, spl::math::RunningStats> Radii;

  const spl::common::AtomSpeciesDatabase & db =
      getEngine()->sharedData().getSpeciesDatabase();

  const std::set< std::string> species(particles.begin(), particles.end());
  const std::vector< std::string> unique(species.begin(), species.end());

  Radii radii;
  for(size_t i = 0; i < unique.size(); ++i)
  {
    for(size_t j = i; j < unique.size(); ++j)
    {
      const boost::optional< double> & dist = db.getSpeciesPairDistance(
          spl::SpeciesPair(unique[i], unique[j]));
      if(dist)
      {
        radii[unique[i]].insert(0.5 * *dist);
        radii[unique[j]].insert(0.5 * *dist);
      }
    }
  }

  double vol = 0.0;
  BOOST_FOREACH(const std::string & specie, particles)
  {
    const Radii::const_iterator & it = radii.find(specie);
    if(it != radii.end())
      vol += (4.0 / 3.0) * spl::common::constants::PI * it->second.mean();
  }

  return vol;
}

}
}

