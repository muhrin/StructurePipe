/*
 * SortedDistanceComparator.cpp
 *
 *  Created on: Aug 17, 2011
 *      Author: muhrin
 */

// INCLUDES /////////////////////////////////////

#include "utility/SortedDistanceComparator.h"

#include "common/AbstractFmidCell.h"
#include "common/Structure.h"

#include <armadillo>

namespace sstbx { namespace utility {

const size_t SortedDistanceComparator::MAX_CELL_MULTIPLES = 10;
const double SortedDistanceComparator::DEFAULT_TOLERANCE = 1e-3;

SortedDistanceComparator::SortedDistanceComparator(const double tolerance):
myTolerance(tolerance)
{}

double SortedDistanceComparator::compareStructures(
	const SortedDistanceComparisonData & dist1,
	const SortedDistanceComparisonData & dist2) const
{
	using std::vector;

	const size_t maxI = std::min(
		dist1.sortedDistances.size(),
		dist2.sortedDistances.size());
  const double maxDist = std::max(dist1.maxDist, dist2.maxDist);

  double delta = 0.0;
	double rms = 0.0;
  double max = 0.0;
  double sum = 0.0;
  double d1, d2;
	for(size_t i = 0; i < maxI; ++i)
	{
    d1 = dist1.sortedDistances[i];
    d2 = dist2.sortedDistances[i];

    if(d1 < maxDist && d2 < maxDist)
    {
      sum = d1 + d2;
      delta = sum > 0 ? std::abs(d1 - d2) / sum : 0;
      max   = std::max(max, delta);
		  rms   += delta * delta;
    }
	}
	rms = sqrt(rms / maxI);

	return max;
}

bool SortedDistanceComparator::areSimilar(
	const SortedDistanceComparisonData & dist1,
	const SortedDistanceComparisonData & dist2) const
{
	return compareStructures(dist1, dist2) < myTolerance;
}

const SortedDistanceComparisonData *
SortedDistanceComparator::generateComparisonData(const sstbx::common::Structure & str) const
{
	using std::vector;

	typedef arma::vec3	Vec3;
	typedef arma::mat33 Mat33;

  const sstbx::common::AbstractFmidCell<> * const uc = str.getUnitCell();

	// Copy over the unit cell so we can do distance calculations
	SortedDistanceComparisonData * data =
    new SortedDistanceComparisonData(
    *uc,
    1.75 * uc->getLongestVector());

	// Calculate the distances ...
  populateDistanceVector(str, data->sortedDistances, data->maxDist);
	// ... and sort them
	std::sort(data->sortedDistances.begin(), data->sortedDistances.end());

	return data;
}

void SortedDistanceComparator::populateDistanceVector(
	const ::sstbx::common::Structure & str,
	std::vector<double> & distVec,
	const double cutoff) const
{
	typedef arma::Mat<double> Mat;

	Mat pos;
	str.getAtomPositionsDescendent(pos);
	const ::sstbx::common::AbstractFmidCell<> & cell = *str.getUnitCell();

	std::vector<double> distances;
	const size_t numParticles = pos.n_cols;

  /*/
  /* Have to do all i, j pairs including i = j and ij/ji to get all the possible distances.
  /*
  /**/
	for(size_t i = 0; i < numParticles; ++i)
	{
		for(size_t j = 0; j < numParticles; ++j)
		{
			distances.clear();

			cell.getAllDistancesWithinCutoff(pos.col(i), pos.col(j), cutoff, distances, MAX_CELL_MULTIPLES);

			distVec.insert(distVec.end(), distances.begin(), distances.end());
		}
	}
}

}}
