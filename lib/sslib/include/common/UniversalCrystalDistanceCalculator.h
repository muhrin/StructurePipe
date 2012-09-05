/*
 * UniversalCrystalDistanceCalculator.h
 *
 *  Created on: Sep 4, 2012
 *      Author: Martin Uhrin
 */

#ifndef UNIVERSAL_CRYSTAL_DISTANCE_CALCULATOR_H
#define UNIVERSAL_CRYSTAL_DISTANCE_CALCULATOR_H

// INCLUDES ///////////////////////////////////
#include "common/DistanceCalculator.h"

namespace sstbx {
namespace common {

class UniversalCrystalDistanceCalculator : public DistanceCalculator
{
public:

  UniversalCrystalDistanceCalculator(const Structure & structure);

  virtual ::arma::vec3 getVecMinImg(const ::arma::vec3 & a, const ::arma::vec3 & b) const;

  virtual void getDistsBetween(
    const ::arma::vec3 & a,
    const ::arma::vec3 & b,
    const double cutoff,
    ::std::vector<double> & outDistances) const;

  virtual void getVecsBetween(
    const ::arma::vec3 & a,
    const ::arma::vec3 & b,
    const double cutoff,
    ::std::vector< ::arma::vec3> & outVectors,
    const size_t maxVectors = DistanceCalculator::DEFAULT_MAX_OUT_VECTORS) const;

  virtual bool isValid() const;


private:

	double getNumPlaneRepetitionsToBoundSphere(
    const ::arma::vec3 & boundDir,
    const ::arma::vec3 & planeVecA,
    const ::arma::vec3 & planeVecB,
		const double radius) const;

};

}
} // Close the namespace

#endif /* UNIVERSAL_CRYSTAL_DISTANCE_CALCULATOR_H */
