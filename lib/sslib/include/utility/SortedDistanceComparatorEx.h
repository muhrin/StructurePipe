/*
 * SortedDistanceComparatorEx.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef SORTED_DISTANCE_COMPARATOR_EX_H
#define SORTED_DISTANCE_COMPARATOR_EX_H

// INCLUDES /////////////////////////////////////////////
#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "common/AtomSpeciesId.h"
#include "utility/IStructureComparator.h"

// FORWARD DECLARATIONS ////////////////////////////////////
namespace sstbx {
namespace common {
class Structure;
}
}

namespace sstbx {
namespace utility {

struct SortedDistanceComparisonData
{
  typedef ::std::vector<double> DistancesVec;;
  typedef ::boost::shared_ptr<DistancesVec> DistancesVecPtr;

  template <typename Key, typename Type>
  class MapEx : public ::std::map<Key, Type>
  {
  public:

    const Type & operator ()(const Key & key) const
    {
      const_iterator it = find(key);

      if(it == end())
      {
        throw ::std::runtime_error("Attempting to access value with key that does not exist");
      }
      return it->second;
    }
  };

  typedef MapEx<common::AtomSpeciesId::Value, DistancesVecPtr> DistancesMap;
  typedef MapEx<common::AtomSpeciesId::Value, DistancesMap> SpeciesDistancesMap;

  SortedDistanceComparisonData(const common::Structure & structure, const double _maxDist);

  ::std::vector<common::AtomSpeciesId::Value> species;
  DistancesVec              latticeDistances;
  SpeciesDistancesMap       speciesDistancesMap;
  const double              cutoff;

private:

  void initSpeciesDistancesMap();

	void initDistance(const ::sstbx::common::Structure & str) const;
};

class SortedDistanceComparatorEx : public IStructureComparator
{
  typedef ::std::vector<double> DistancesVec;
public:

	typedef SortedDistanceComparisonData DataTyp;
  typedef IBufferedComparator   BufferedTyp;

	SortedDistanceComparatorEx(double tolerance = DEFAULT_TOLERANCE);

  // From IStructureComparator ////////////////

	virtual double compareStructures(
		const sstbx::common::Structure & str1,
		const sstbx::common::Structure & str2) const;

	virtual bool areSimilar(
		const sstbx::common::Structure & str1,
		const sstbx::common::Structure & str2) const;

  virtual ::boost::shared_ptr<BufferedTyp> generateBuffered() const;

  // End from IStructureComparator /////////////

  // Methods needed to conform to expectations laid out by GenericBufferedComparator ///
	double compareStructures(
    const common::Structure & str1,
		const SortedDistanceComparisonData & dist1,
    const common::Structure & str2,
		const SortedDistanceComparisonData & dist2) const;

	bool areSimilar(
    const common::Structure & str1,
		const SortedDistanceComparisonData & dist1,
    const common::Structure & str2,
		const SortedDistanceComparisonData & dist2) const;

  ::std::auto_ptr<SortedDistanceComparisonData>
    generateComparisonData(const ::sstbx::common::Structure & str) const;
  // End conformation methods //////////////
	

private:

	static const double DEFAULT_TOLERANCE;

	static const size_t MAX_CELL_MULTIPLES;

  void calcProperties(
    const DistancesVec & dist1,
    const DistancesVec & dist2,
    double & rms,
    double & max) const;

	double myTolerance;

};

}
}

#endif /* SORTED_DISTANCE_COMPARATOR_EX_H */
