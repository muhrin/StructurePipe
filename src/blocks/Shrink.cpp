/*
 * Shrink.cpp
 *
 *  Created on: Jan 23, 2014
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/Shrink.h"

#include <spl/potential/IPotentialEvaluator.h>
#include <spl/utility/StableComparison.h>

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

const double Shrink::DEFAULT_SHRINK_FACTOR = 0.99;
const double Shrink::DEFAULT_ENERGY_TOL = 1e-10;
const double Shrink::DEFAULT_MIN_VOLUME = 1e-3;

Shrink::Shrink(PotentialPtr potential,
    const boost::optional< std::string> & paramsTag) :
    Block("Shrink"), myPotential(potential), myParamsTag(paramsTag), myShrinkFactor(
        DEFAULT_SHRINK_FACTOR), myEnergyTol(DEFAULT_ENERGY_TOL)
{
}

Shrink::Shrink(PotentialPtr potential,
    const boost::optional< std::string> & paramsTag, const double shrinkFactor,
    const double energyTol) :
    Block("Shrink"), myParamsTag(paramsTag), myPotential(potential), myShrinkFactor(
        shrinkFactor), myEnergyTol(energyTol)
{
}

void
Shrink::pipelineInitialising()
{
  if(myParamsTag && myPotential->getParameterisable())
  {
    // Parameterise the potential with the global parameters (if set)
    getEngine()->globalData().updateParameterisable(*myParamsTag,
        myPotential->getParameterisable());
  }
}

void
Shrink::in(StructureDataType * const structure)
{
  namespace stable = spl::utility::stable;

  if(spl::common::UnitCell * const unitCell = structure->getUnitCell())
  {
    spl::potential::IPotential::EvaluatorPtr evaluator =
        myPotential->createEvaluator(*structure);
    evaluator->evalPotential();

    const double origVolume = unitCell->getVolume();

    // Keep making the unit cell smaller until the energy goes non-zero
    while(stable::eq(evaluator->getData().internalEnergy, 0.0,
        DEFAULT_ENERGY_TOL))
    {
      unitCell->setVolume(myShrinkFactor * unitCell->getVolume());
      if(unitCell->getVolume() < DEFAULT_MIN_VOLUME)
      {
        // Couldn't get the potential to be non-zero so reset the structure back
        // to the original volume
        unitCell->setVolume(origVolume);
        break;
      }
      evaluator->evalPotential();
    }
  }

  out(structure);
}

}
}

