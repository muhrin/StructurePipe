/*
 * ClusterDistanceCalculator.h
 *
 *  Created on: Sep 4, 2012
 *      Author: Martin Uhrin
 */

#ifndef CLUSTER_DISTANCE_CALCULATOR_H
#define CLUSTER_DISTANCE_CALCULATOR_H

// INCLUDES ///////////////////////////////////
#include "common/DistanceCalculator.h"

#include "common/Structure.h"

namespace sstbx {
namespace common {

class ClusterDistanceCalculator : public DistanceCalculator
{
public:

  ClusterDistanceCalculator(const Structure & structure):
    DistanceCalculator(structure)
    {}

  virtual inline ::arma::vec3 getVecMinImg(const ::arma::vec3 & a, const ::arma::vec3 & b) const
  { return b - a; }

  virtual void getDistsBetween(
    const ::arma::vec3 & a,
    const ::arma::vec3 & b,
    const double cutoff,
    ::std::vector<double> & outDistances) const
  {
    const double cutoffSq = cutoff * cutoff;
    const ::arma::vec dr = b - a;
    const double distSq = ::arma::dot(dr, dr);

    if(distSq < cutoffSq)
      outDistances.push_back(sqrt(distSq));
  }

  virtual inline void getVecsBetween(
    const ::arma::vec3 & a,
    const ::arma::vec3 & b,
    const double cutoff,
    ::std::vector< ::arma::vec3> & outVectors,
    const size_t maxVectors = DEFAULT_MAX_OUT_VECTORS) const
  {
    const double cutoffSq = cutoff * cutoff;
    const ::arma::vec dr = b - a;

    if(::arma::dot(dr, dr) < cutoffSq)
      outVectors.push_back(dr);
  }

  virtual inline bool isValid() const
  { return myStructure.getUnitCell() == NULL; }

};

}
} // Close the namespace

#endif /* CLUSTER_DISTANCE_CALCULATOR_H */
