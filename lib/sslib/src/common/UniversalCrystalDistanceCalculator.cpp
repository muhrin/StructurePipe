/*
 * UniversalCrystalDistanceCalculator.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES ///////////////
#include "common/UniversalCrystalDistanceCalculator.h"

#include "common/Structure.h"
#include "common/UnitCell.h"


namespace sstbx {
namespace common {

UniversalCrystalDistanceCalculator::UniversalCrystalDistanceCalculator(const Structure & structure):
DistanceCalculator(structure)
{}

::arma::vec3 UniversalCrystalDistanceCalculator::getVecMinImg(const ::arma::vec3 & a, const ::arma::vec3 & b) const
{
  const UnitCell & cell = *myStructure.getUnitCell();

	// Make sure cart1 and 2 are in the unit cell at the origin
  const ::arma::vec3 		aUnit	= cell.wrapVec(a);
  const ::arma::vec3 		bUnit	= cell.wrapVec(b);
  const ::arma::vec3		dR		= bUnit - aUnit;
	double minModDRSq = dot(dR, dR);
	const double minModDR = sqrt(minModDRSq);

	// Get the lattice vectors
  const ::arma::vec3 A(cell.getAVec());
  const ::arma::vec3 B(cell.getBVec());
  const ::arma::vec3 C(cell.getCVec());

	// Maximum multiple of cell vectors we need to go to
	int maxA = (int)ceil(getNumPlaneRepetitionsToBoundSphere(A, B, C, minModDR));
	int maxB = (int)ceil(getNumPlaneRepetitionsToBoundSphere(B, A, C, minModDR));
	int maxC = (int)ceil(getNumPlaneRepetitionsToBoundSphere(C, A, B, minModDR));

  ::arma::vec3 dFrac, dRImg, minDR = dR;
	double modDRSq;
	for(int a = -maxA; a <= maxA; ++a)
	{
		for(int b = -maxB; b <= maxB; ++b)
		{
			for(int c = -maxC; c <= maxC; ++c)
			{
				dFrac[0] = a;
				dFrac[1] = b;
				dFrac[2] = c;

        dRImg = cell.fracToCartInplace(dFrac) + dR;
				
				modDRSq = dot(dRImg, dRImg);
				if(modDRSq < minModDRSq)
				{
					minModDRSq	= modDRSq;
					minDR		= dRImg;
				}
			}
		}
	}

	return minDR;
}

void UniversalCrystalDistanceCalculator::getDistsBetween(
    const ::arma::vec3 & a,
    const ::arma::vec3 & b,
    const double cutoff,
    ::std::vector<double> & outDistances) const
{
  const UnitCell & cell = *myStructure.getUnitCell();

	// Make sure cart1 and 2 are in the unit cell at the origin
  const ::arma::vec3 		aUnit	= cell.wrapVec(a);
  const ::arma::vec3 		bUnit	= cell.wrapVec(b);
  const ::arma::vec3		dR		= bUnit - aUnit;
	double minModDRSq = dot(dR, dR);
	const double minModDR = sqrt(minModDRSq);

	// Get the lattice vectors
  const ::arma::vec3 A(cell.getAVec());
  const ::arma::vec3 B(cell.getBVec());
  const ::arma::vec3 C(cell.getCVec());

	// Maximum multiple of cell vectors we need to go to
	int maxA = (int)ceil(getNumPlaneRepetitionsToBoundSphere(A, B, C, minModDR));
	int maxB = (int)ceil(getNumPlaneRepetitionsToBoundSphere(B, A, C, minModDR));
	int maxC = (int)ceil(getNumPlaneRepetitionsToBoundSphere(C, A, B, minModDR));

  ::arma::vec3 dFrac, dRImg;
  double dRDistSq;
	for(int a = -maxA; a <= maxA; ++a)
	{
		for(int b = -maxB; b <= maxB; ++b)
		{
			for(int c = -maxC; c <= maxC; ++c)
			{
				dFrac[0] = a;
				dFrac[1] = b;
				dFrac[2] = c;

        dRImg = cell.fracToCartInplace(dFrac) + dR;
        dRDistSq = ::arma::dot(dRImg, dRImg);

				if(dRDistSq < minModDRSq)
				{
          outDistances.push_back(sqrt(dRDistSq));
				}
			}
		}
	}
}

void UniversalCrystalDistanceCalculator::getVecsBetween(
  const ::arma::vec3 & a,
  const ::arma::vec3 & b,
  const double cutoff,
  ::std::vector< ::arma::vec3> & outVectors,
  const size_t maxVectors) const
{
  const UnitCell & cell = *myStructure.getUnitCell();

	// Make sure cart1 and 2 are in the unit cell at the origin
  const ::arma::vec3 		aUnit	= cell.wrapVec(a);
  const ::arma::vec3 		bUnit	= cell.wrapVec(b);
  const ::arma::vec3		dR		= bUnit - aUnit;
	double minModDRSq = dot(dR, dR);
	const double minModDR = sqrt(minModDRSq);

	// Get the lattice vectors
  const ::arma::vec3 A(cell.getAVec());
  const ::arma::vec3 B(cell.getBVec());
  const ::arma::vec3 C(cell.getCVec());

	// Maximum multiple of cell vectors we need to go to
	int maxA = (int)ceil(getNumPlaneRepetitionsToBoundSphere(A, B, C, minModDR));
	int maxB = (int)ceil(getNumPlaneRepetitionsToBoundSphere(B, A, C, minModDR));
	int maxC = (int)ceil(getNumPlaneRepetitionsToBoundSphere(C, A, B, minModDR));

  ::arma::vec3 dFrac, dRImg;
	for(int a = -maxA; a <= maxA; ++a)
	{
		for(int b = -maxB; b <= maxB; ++b)
		{
			for(int c = -maxC; c <= maxC; ++c)
			{
				dFrac[0] = a;
				dFrac[1] = b;
				dFrac[2] = c;

        dRImg = cell.fracToCartInplace(dFrac) + dR;
				
				if(dot(dRImg, dRImg) < minModDRSq)
				{
          outVectors.push_back(dRImg);
				}
			}
		}
	}
}

bool UniversalCrystalDistanceCalculator::isValid() const
{
  return myStructure.getUnitCell() != NULL;
}

double UniversalCrystalDistanceCalculator::getNumPlaneRepetitionsToBoundSphere(
  const ::arma::vec3 & boundDir,
  const ::arma::vec3 & planeVecA,
  const ::arma::vec3 & planeVecB,
	const double radius) const
{
	// The vector normal to the plane
  const ::arma::vec3 normal = ::arma::cross(planeVecA, planeVecB);
  const double unitCellVolume = myStructure.getUnitCell()->getVolume();

  return radius / unitCellVolume * ::std::sqrt(::arma::dot(normal, normal));
}

}
}
