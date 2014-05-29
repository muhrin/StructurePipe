/*
 * Shrink.h
 *
 *  Created on: May 23, 2014
 *      Author: Martin Uhrin
 */

#ifndef SPIPE__BLOCKS__SHRINK_H_
#define SPIPE__BLOCKS__SHRINK_H_

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <spl/potential/IPotential.h>

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////
namespace spipe {
namespace blocks {

class Shrink : public PipeBlock, boost::noncopyable
{
  typedef spl::UniquePtr< spl::potential::IPotential>::Type PotentialPtr;
public:
  static const double DEFAULT_SHRINK_FACTOR;
  static const double DEFAULT_ENERGY_TOL;
  static const double DEFAULT_MIN_VOLUME;

  Shrink(PotentialPtr potential,
      const boost::optional< std::string> & paramsTag);
  Shrink(PotentialPtr potential,
      const boost::optional< std::string> & paramsTag,
      const double shrinkFactor, const double energyTol);

  virtual void
  pipelineInitialising();
  virtual void
  in(StructureDataType * const structureData);
private:
  PotentialPtr myPotential;
  const boost::optional< std::string> myParamsTag;
  const double myShrinkFactor;
  const double myEnergyTol;
};

}
}

#endif /* SPIPE__BLOCKS__SHRINK_H_ */
