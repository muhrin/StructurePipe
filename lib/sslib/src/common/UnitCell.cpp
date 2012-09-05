/*
 * UnitCell.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES /////////////////////////////////////
#include "common/UnitCell.h"

#include <boost/assert.hpp>

#include "common/Constants.h"
#include "common/Structure.h"
#include "utility/StableComparison.h"

namespace sstbx {
namespace common {

UnitCell::UnitCell(const double a, const double b, const double c, const double alpha, const double beta, const double gamma)
{
  init(a, b, c, alpha, beta, gamma);
}

UnitCell::UnitCell(const double (&latticeParams)[6])
{
  init(latticeParams[A], latticeParams[B], latticeParams[C],
    latticeParams[ALPHA], latticeParams[BETA], latticeParams[GAMMA]);
}

const double (&UnitCell::getLatticeParams() const)[6]
{
  return myLatticeParams;
}

::arma::vec3 UnitCell::getAVec() const
{
  return myOrthoMtx.col(X);
}

::arma::vec3 UnitCell::getBVec() const
{
  return myOrthoMtx.col(Y);
}

::arma::vec3 UnitCell::getCVec() const
{
  return myOrthoMtx.col(Z);
}

double UnitCell::getLongestCellVectorLength() const
{
  return ::std::max(myLatticeParams[A], ::std::max(myLatticeParams[B], myLatticeParams[C]));
}

const ::arma::mat33 & UnitCell::getOrthoMtx() const
{
  return myOrthoMtx;
}

const void UnitCell::setOrthoMtx(const ::arma::mat33 & orthoMtx)
{
	myOrthoMtx = orthoMtx;
	init(orthoMtx);
  if(myStructure)
    myStructure->unitCellChanged();
}

double UnitCell::getVolume() const
{
  return myVolume;
}

double UnitCell::setVolume(const double volume)
{
	const double scale = pow(volume / getVolume(), 1.0 / 3.0);
	init(scale * myOrthoMtx);
  return scale;
}

double UnitCell::getNormVolume() const
{
  using std::sqrt;

  // First normalise the lattice vectors
  ::arma::vec3 a(myOrthoMtx.col(0));
  ::arma::vec3 b(myOrthoMtx.col(1));
  ::arma::vec3 c(myOrthoMtx.col(2));

  a /= sqrt(::arma::dot(a, a));
  b /= sqrt(::arma::dot(b, b));
  c /= sqrt(::arma::dot(c, c));

  // Now calculate abs value of the the triple product
  return std::abs(dot(arma::cross(a, b), c));
}

UnitCell::LatticeSystem::Value UnitCell::getLatticeSystem() const
{
  namespace comp = utility::StableComp;

  if(comp::eq(myLatticeParams[ALPHA], myLatticeParams[BETA]) &&
    comp::eq(myLatticeParams[BETA], myLatticeParams[GAMMA]))
  {
    if(comp::eq(myLatticeParams[ALPHA], 90.0))
    {
      if(comp::eq(myLatticeParams[A], myLatticeParams[B]))
      {
        if(comp::eq(myLatticeParams[A], myLatticeParams[C]))
        {
          return LatticeSystem::CUBIC;
        }
        else
        {
          return LatticeSystem::TETRAGONAL;
        }
      } // if(a == b)
      else if(comp::eq(myLatticeParams[A], myLatticeParams[C]))
      {
        return LatticeSystem::TETRAGONAL;
      } // if(a == c)
      else
      {
        return LatticeSystem::ORTHORHOMBIC;
      } // if(a == b), elseif(a == c), else
    } // if(alpha == 90)
    else
      return LatticeSystem::RHOMBOHEDRAL;
  } // if(alpha == beta == gamma)
  else
  {
    if(comp::eq(myLatticeParams[ALPHA], 90.0))
    {
      if(comp::eq(myLatticeParams[BETA], 90.0) || comp::eq(myLatticeParams[GAMMA], 90.0))
      {
        return LatticeSystem::MONOCLINIC;
      }
      else
      {
        // Technically, this is triclinic but it probably indicates that there is a better
        // centering for the system
        return LatticeSystem::TRICLINIC;
      }
    } // if(alpha == 90)
    else if(comp::eq(myLatticeParams[BETA], 90.0))
    {
      if(comp::eq(myLatticeParams[GAMMA], 90.0))
      {
        return LatticeSystem::MONOCLINIC;
      }
      else
      {
        // Technically, this is triclinic but it probably indicates that there is a better
        // centering for the system
        return LatticeSystem::TRICLINIC;
      }
    } // if(alpha == 90), else if(beta == 90.0)
    else if(comp::eq(myLatticeParams[GAMMA], 90.0))
    {
      return LatticeSystem::MONOCLINIC;
    } // if(alpha == 90), else if(beta == 90), else if(gamma == 90)
    else
    {
      return LatticeSystem::TRICLINIC;
    }
  }

  return LatticeSystem::TRICLINIC;
}

::arma::vec3 UnitCell::wrapVec(const ::arma::vec3 & cart) const
{
  ::arma::vec3 toWrap(cart);
  cartToFracInplace(toWrap); // Fractionalise it
  wrapVecFracInplace(toWrap); // Wrap it
  return fracToCartInplace(toWrap); // Orthogonalise it back
}

::arma::vec3 & UnitCell::wrapVecInplace(::arma::vec3 & cart) const
{
  cartToFracInplace(cart);
  wrapVecFracInplace(cart);
  fracToCartInplace(cart);
  return cart;
}

::arma::vec3 UnitCell::wrapVecFrac(const ::arma::vec3 & frac) const
{
  ::arma::vec3 toWrap(frac);
  wrapVecFracInplace(toWrap);
  return toWrap;
}

::arma::vec3 & UnitCell::wrapVecFracInplace(::arma::vec3 & frac) const
{
  frac[X] -= floor(frac[X]);
  frac[Y] -= floor(frac[Y]);
  frac[Z] -= floor(frac[Z]);

  return frac;
}

::arma::vec3 UnitCell::randomPoint() const
{
  ::arma::vec3 rand;
  rand.randu();
  return fracToCartInplace(rand);
}

void UnitCell::setStructure(const Structure * const structure)
{
  myStructure = structure;
}

void UnitCell::init(
	const double a, const double b, const double c,
	const double alpha, const double beta, const double gamma)
{
  // Sanity checks on parameters
  BOOST_ASSERT_MSG(alpha+beta+gamma <= 360.0, "Non-physical lattice parameters supplied - required that alpha+beta+gamma <= 360.0");
  BOOST_ASSERT_MSG(abs(alpha-beta) <= gamma, "Non-physical lattice parameters supplied - require that abs(alpha-beta) < gamma");
  BOOST_ASSERT_MSG(abs(beta-gamma) <= alpha, "Non-physical lattice parameters supplied - require that abs(beta-gamma) < alpha");
  BOOST_ASSERT_MSG(abs(gamma-alpha) <= beta, "Non-physical lattice parameters supplied - require that abs(gamma-alpha) < beta");

	myLatticeParams[0] = a;
	myLatticeParams[1] = b;
	myLatticeParams[2] = c;
	myLatticeParams[3] = alpha;
	myLatticeParams[4] = beta;
	myLatticeParams[5] = gamma;
	
	initOrthoAndFracMatrices();
	initRest();
}


void UnitCell::init(const ::arma::mat33 & orthoMtx)
{
	myOrthoMtx	= orthoMtx;
	myFracMtx	= arma::inv(orthoMtx);

	initLatticeParams();
	initRest();
}


void UnitCell::initOrthoAndFracMatrices()
{
  const double alphaRad = Constants::DEG_TO_RAD * myLatticeParams[3];
  const double betaRad = Constants::DEG_TO_RAD * myLatticeParams[4];
  const double gammaRad = Constants::DEG_TO_RAD * myLatticeParams[5];

	myOrthoMtx.fill(0);
	// A - col 0
	myOrthoMtx.at(0, 0) = myLatticeParams[0];
	// B - col 1
	myOrthoMtx.at(0, 1) = myLatticeParams[1] * cos(gammaRad);
	myOrthoMtx.at(1, 1) = myLatticeParams[1] * sin(gammaRad);
	// C - col 2
	myOrthoMtx.at(0, 2) = myLatticeParams[2] * cos(betaRad);
	myOrthoMtx.at(1, 2) = myLatticeParams[2] * (cos(alphaRad) - cos(betaRad) * cos(gammaRad)) / sin(gammaRad);
	myOrthoMtx.at(2, 2) = sqrt(
    myLatticeParams[2] * myLatticeParams[2] -
		myOrthoMtx(0, 2) * myOrthoMtx(0, 2) -
		myOrthoMtx(1, 2) * myOrthoMtx(1, 2)
  );

	myFracMtx = inv(myOrthoMtx);
}


void UnitCell::initLatticeParams()
{
	using namespace arma;

	// Get the lattice vectors
  const ::arma::vec3 a = myOrthoMtx.col(0);
	const ::arma::vec3 b = myOrthoMtx.col(1);
	const ::arma::vec3 c = myOrthoMtx.col(2);

	myLatticeParams[0] = std::sqrt(dot(a, a));
	myLatticeParams[1] = std::sqrt(dot(b, b));
	myLatticeParams[2] = std::sqrt(dot(c, c));
	myLatticeParams[3] = acos(dot(b, c) / (myLatticeParams[1] * myLatticeParams[2])) * Constants::RAD_TO_DEG;
	myLatticeParams[4] = acos(dot(a, c) / (myLatticeParams[0] * myLatticeParams[2])) * Constants::RAD_TO_DEG;
	myLatticeParams[5] = acos(dot(a, b) / (myLatticeParams[0] * myLatticeParams[1])) * Constants::RAD_TO_DEG;
}


void UnitCell::initRest()
{
	myVolume = std::fabs(
		dot(myOrthoMtx.col(0),
		cross(myOrthoMtx.col(1), myOrthoMtx.col(2))));
}

}
}
