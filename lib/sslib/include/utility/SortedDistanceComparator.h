/*
 * SortedDistanceComparator.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef SORTED_DISTANCE_COMPARATOR_H
#define SORTED_DISTANCE_COMPARATOR_H

// INCLUDES /////////////////////////////////////////////
#include "utility/AbstractComparator.h"

#include "common/AbstractFmidCell.h"

#include <vector>

// FORWARD DECLARATIONS ////////////////////////////////////
namespace sstbx
{
	namespace common
	{
		class Structure;
	}
}

namespace sstbx { namespace utility {

struct SortedDistanceComparisonData
{
	SortedDistanceComparisonData(
    const ::sstbx::common::AbstractFmidCell<> & _cell ,
    const double _maxDist):
		cell(_cell), maxDist(_maxDist) {}
	const ::sstbx::common::AbstractFmidCell<>	cell;
	::std::vector<double>						sortedDistances;
  const double                    maxDist;
};

class SortedDistanceComparator : public AbstractComparator<SortedDistanceComparisonData>
{
public:

	typedef SortedDistanceComparisonData DataTyp;

  // Our compareStructures/areSimilar will hide these in our parent so explicitly
  // bring them back into scope
  using AbstractComparator<SortedDistanceComparisonData>::compareStructures;
  using AbstractComparator<SortedDistanceComparisonData>::areSimilar;

	SortedDistanceComparator(double tolerance = DEFAULT_TOLERANCE);

	virtual double compareStructures(
		const SortedDistanceComparisonData & dist1,
		const SortedDistanceComparisonData & dist2) const;

	virtual bool areSimilar(
		const SortedDistanceComparisonData & dist1,
		const SortedDistanceComparisonData & dist2) const;

	virtual const DataTyp * generateComparisonData(const ::sstbx::common::Structure & str) const;

private:

	static const double DEFAULT_TOLERANCE;

	static const size_t MAX_CELL_MULTIPLES;

	void populateDistanceVector(
		const ::sstbx::common::Structure & str,
		::std::vector<double> & distVec,
		const double cutoff) const;

	double myTolerance;

};

}}

#endif /* SORTED_DISTANCE_COMPARATOR_H */
