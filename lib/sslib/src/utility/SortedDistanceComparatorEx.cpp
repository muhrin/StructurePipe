/*
 * SortedDistanceComparatorEx.cpp
 *
 *  Created on: Aug 17, 2011
 *      Author: muhrin
 */

// INCLUDES /////////////////////////////////////

#include "utility/SortedDistanceComparatorEx.h"

#include <memory>

#include <boost/scoped_ptr.hpp>

#include <armadillo>

#include "common/DistanceCalculator.h"
#include "common/Structure.h"
#include "common/UnitCell.h"
#include "utility/GenericBufferedComparator.h"


namespace sstbx {
namespace utility {

const size_t SortedDistanceComparatorEx::MAX_CELL_MULTIPLES   = 10;
const double SortedDistanceComparatorEx::DEFAULT_TOLERANCE    = 1e-3;

SortedDistanceComparisonData::SortedDistanceComparisonData(const common::Structure & structure, const double _maxDist):
cutoff(_maxDist)
{
  const common::StructurePtr primitive = structure.getPrimitiveCopy();
  const common::DistanceCalculator & distCalc = primitive->getDistanceCalculator();

  common::UnitCell * const unitCell = primitive->getUnitCell();
  unitCell->niggliReduce();

  const size_t numAtoms = primitive->getNumAtoms();

  primitive->getAtomSpecies(species);
  ::std::set<common::AtomSpeciesId::Value> speciesSet(species.begin(), species.end());
  species.clear();
  ::std::copy(speciesSet.begin(), speciesSet.end(), species.begin());
  const size_t numSpecies = species.size();

  // First calculation all the 'lattice distances'
  distCalc.getDistsBetween(::arma::vec3(), ::arma::vec3(), cutoff, latticeDistances);

	// Calculate the distances ...
  common::AtomSpeciesId::Value specI, specJ;
  DistancesVecPtr distVecIJ;
  for(size_t i = 0; i < numAtoms - 1; ++i)
  {
    const common::Atom & atomI = primitive->getAtom(i);
    specI = atomI.getSpecies();
    DistancesMap & iDistMap = speciesDistancesMap[specI];

    // Now to all the others
    for(size_t j = i + 1; j < numAtoms; ++j)
    {
      const common::Atom & atomJ = primitive->getAtom(j);
      specJ = atomJ.getSpecies();
      distVecIJ = iDistMap[specJ];

      distCalc.getDistsBetween(atomI, atomJ, cutoff, *distVecIJ);
    }
  }

	// ... and sort them
  ::std::sort(latticeDistances.begin(), latticeDistances.end());
  for(size_t i = 0; i < numSpecies - 1; ++i)
  {
    DistancesMap & iDistMap = speciesDistancesMap[specI];
    for(size_t j = i; j < numSpecies; ++j)
    {
      specJ = species[j];
      distVecIJ = iDistMap[specJ];
      ::std::sort(distVecIJ->begin(), distVecIJ->end());
    }
  }
}

void SortedDistanceComparisonData::initSpeciesDistancesMap()
{
  const size_t numSpecies = species.size();
  DistancesVecPtr distVec;
  common::AtomSpeciesId::Value specI, specJ;
  for(size_t i = 0; i < numSpecies - 1; ++i)
  {
    specI = species[i];
    DistancesMap & distMap = speciesDistancesMap[specI];
    for(size_t j = i; j < numSpecies; ++j)
    {
      specJ = species[j];
      distVec = distMap[specJ];
      distVec.reset(new ::std::vector<double>());
      speciesDistancesMap[specJ][specI] = distVec;
    }
  }
}

SortedDistanceComparatorEx::SortedDistanceComparatorEx(const double tolerance):
myTolerance(tolerance)
{}

double SortedDistanceComparatorEx::compareStructures(
	const sstbx::common::Structure & str1,
	const sstbx::common::Structure & str2) const
{
  ::std::auto_ptr<const SortedDistanceComparatorEx::DataTyp> comp1(generateComparisonData(str1));
  ::std::auto_ptr<const SortedDistanceComparatorEx::DataTyp> comp2(generateComparisonData(str2));

  return compareStructures(str1, *comp1, str2, *comp2);
}

bool SortedDistanceComparatorEx::areSimilar(
	const sstbx::common::Structure & str1,
	const sstbx::common::Structure & str2) const
{
  ::std::auto_ptr<const SortedDistanceComparatorEx::DataTyp> comp1(generateComparisonData(str1));
  ::std::auto_ptr<const SortedDistanceComparatorEx::DataTyp> comp2(generateComparisonData(str2));

  return areSimilar(str1, *comp1, str2, *comp2);
}

double SortedDistanceComparatorEx::compareStructures(
    const common::Structure & str1,
		const SortedDistanceComparisonData & dist1,
    const common::Structure & str2,
		const SortedDistanceComparisonData & dist2) const
{
  typedef ::std::vector<double> DistancesVec;

  const size_t numSpecies = dist1.species.size();

  if(numSpecies != dist1.species.size())
  {
    // Species mismatch!
    return ::std::numeric_limits<double>::max();
  }

  double max = ::std::numeric_limits<double>::min();
  double rms = 0.0;

  // Do lattice distances
  calcProperties(
    dist1.latticeDistances,
    dist2.latticeDistances,
    rms,
    max);

  common::AtomSpeciesId::Value specI, specJ;
  for(size_t i = 0; i < numSpecies - 1; ++i)
  {
    specI = dist1.species[i];
    
    // Do others
    const SortedDistanceComparisonData::DistancesMap & distMapI1 =
      dist1.speciesDistancesMap(specI);
    const SortedDistanceComparisonData::DistancesMap & distMapI2 =
      dist2.speciesDistancesMap(specI);
    for(size_t j = i; j < numSpecies; ++j)
    {
      specJ = dist1.species[j];
      calcProperties(*distMapI1(specJ), *distMapI2(specJ), rms, max);
    }
  }

  return max;
}

bool SortedDistanceComparatorEx::areSimilar(
    const common::Structure & str1,
		const SortedDistanceComparisonData & dist1,
    const common::Structure & str2,
		const SortedDistanceComparisonData & dist2) const
{
	return compareStructures(str1, dist1, str2, dist2) < myTolerance;
}

::std::auto_ptr<SortedDistanceComparisonData>
SortedDistanceComparatorEx::generateComparisonData(const sstbx::common::Structure & str) const
{
  const common::DistanceCalculator & distCalc = str.getDistanceCalculator();

  const common::UnitCell * const unitCell = str.getUnitCell();

  double maxDist = 0.0;
  if(unitCell)
  {
    ::arma::vec3 diag = unitCell->getLongestDiagonal();
    double longestDiag = sqrt(::arma::dot(diag, diag));
    maxDist = 1.75 * longestDiag;
  }

	// Copy over the unit cell so we can do distance calculations
	::std::auto_ptr<SortedDistanceComparisonData> data(new SortedDistanceComparisonData(str, maxDist));

	return data;
}

::boost::shared_ptr<SortedDistanceComparatorEx::BufferedTyp> SortedDistanceComparatorEx::generateBuffered() const
{
  return ::boost::shared_ptr<IBufferedComparator>(
    new GenericBufferedComparator<SortedDistanceComparatorEx>(*this)
  );
}

void SortedDistanceComparatorEx::calcProperties(
  const SortedDistanceComparatorEx::DistancesVec & dist1,
  const SortedDistanceComparatorEx::DistancesVec & dist2,
  double & rms,
  double & max) const
{
  const size_t maxIdx = ::std::min(dist1.size(), dist2.size());

  double d1, d2, sum, delta;
  for(size_t i = 0; i < maxIdx; ++i)
  {
    d1 = dist1[i];
    d2 = dist2[i];

    sum = d1 + d2;
    delta = sum > 0 ? std::abs(d1 - d2) / sum : 0;
    max   = std::max(max, delta);
    rms   += delta * delta;
  }
}

}
}
