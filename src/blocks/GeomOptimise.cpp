/*
 * GeomOptimise.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/GeomOptimise.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>

#include <spl/build_cell/AtomsDescription.h>
#include <spl/common/Structure.h>
#include <spl/potential/PotentialData.h>
#include <spl/potential/GeomOptimiser.h>
#include <spl/potential/IPotential.h>

#include "spipe/common/PipeFunctions.h"
#include "spipe/common/StructureData.h"
#include "spipe/common/SharedData.h"
#include "spipe/common/UtilityFunctions.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace ssbc = ::spl::build_cell;
namespace ssc = ::spl::common;
namespace potential = spl::potential;
namespace structure_properties = ssc::structure_properties;

GeomOptimise::Settings::Settings() :
    writeSummary(false), failAction(factory::FailAction::CONTINUE)
{
}

GeomOptimise::GeomOptimise(spl::potential::IGeomOptimiserPtr optimiser) :
    Block("Geometry optimise"), mySettings(), myOptimisationSettings(), myOptimiser(
        optimiser)
{
}

GeomOptimise::GeomOptimise(spl::potential::IGeomOptimiserPtr optimiser,
    const Settings & settings) :
    Block("Geometry optimise"), mySettings(settings), myOptimisationSettings(), myOptimiser(
        optimiser)
{
}

GeomOptimise::GeomOptimise(spl::potential::IGeomOptimiserPtr optimiser,
    const ::spl::potential::OptimisationSettings & optimisationParams) :
    Block("Potential geometry optimisation"), mySettings(), myOptimisationSettings(
        optimisationParams), myOptimiser(optimiser)
{
}

GeomOptimise::GeomOptimise(spl::potential::IGeomOptimiserPtr optimiser,
    const ::spl::potential::OptimisationSettings & optimisationParams,
    const Settings & settings) :
    Block("Potential geometry optimisation"), mySettings(settings), myOptimisationSettings(
        optimisationParams), myOptimiser(optimiser)
{
}

void
GeomOptimise::pipelineInitialising()
{
  if(mySettings.writeSummary)
  {
    myTableSupport.setFilename(
        common::getOutputFileStem(getEngine()->sharedData(),
            getEngine()->globalData()) + ".geomopt");
    myTableSupport.registerEngine(getEngine());
  }

  if(potential::IPotential * const potential = myOptimiser->getPotential())
  {
    if(potential::IParameterisable * const parameterisable =
        potential->getParameterisable())
    {
      // Parameterise the potential with the global parameters (if set)
      if(mySettings.paramsTag)
        getEngine()->globalData().updateParameterisable(*mySettings.paramsTag,
            parameterisable);

      if(mySettings.writeSummary)
      {
        const PotentialParams & potentialParams = parameterisable->getParams();
        if(!myPotentialParams.empty())
        {
          // Add a note to the table with the current parameter string
          getTableSupport().getTable().addTableNote(
              "params: " + boost::join(potentialParams, " "));
        }
      }
    }

    potential->updateSpeciesDb(&getEngine()->sharedData().getSpeciesDatabase());
  }
}

void
GeomOptimise::in(spl::common::Structure * const structure)
{
  // Is the optimiser using a parameterisable potential?
  if(myOptimiser->getPotential()
      && myOptimiser->getPotential()->getParameterisable())
  {
    // Add the potential parameters to the structure data
    structure->properties()[common::GlobalKeys::POTENTIAL_PARAMS] =
        myOptimiser->getPotential()->getParameterisable()->getParams();
  }

  potential::OptimisationData optData;
  const potential::OptimisationOutcome outcome = myOptimiser->optimise(
      *structure, optData, myOptimisationSettings);
  if(outcome.isSuccess()
      || mySettings.failAction == factory::FailAction::CONTINUE)
  {
    // Update our data table with the structure data
    if(mySettings.writeSummary)
      updateTable(*structure, optData);
    out(structure);
  }
  else
    drop(structure);
}

potential::GeomOptimiser &
GeomOptimise::getOptimiser()
{
  return *myOptimiser;
}

::spipe::utility::DataTableSupport &
GeomOptimise::getTableSupport()
{
  return myTableSupport;
}

void
GeomOptimise::updateTable(const ssc::Structure & structure,
    const ::spl::potential::OptimisationData & optimisationData)
{
  utility::DataTable & table = myTableSupport.getTable();
  const std::string & strName = structure.getName();

  if(const double * const internalEnergy = structure.properties().find(
      structure_properties::general::ENERGY_INTERNAL))
  {
    table.insert(strName, "energy", common::toString(*internalEnergy));
    table.insert(strName, "energy/atom",
        common::toString(
            *internalEnergy / static_cast< double>(structure.getNumAtoms())));
    if(optimisationData.numIters)
      table.insert(strName, "iters",
          boost::lexical_cast< std::string>(*optimisationData.numIters));
  }
}

}
}

