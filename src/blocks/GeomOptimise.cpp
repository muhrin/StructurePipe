/*
 * GeomOptimise.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "blocks/GeomOptimise.h"

#include <boost/lexical_cast.hpp>

#include <spl/build_cell/AtomsDescription.h>
#include <spl/common/Structure.h>
#include <spl/potential/PotentialData.h>
#include <spl/potential/GeomOptimiser.h>
#include <spl/potential/IPotential.h>

#include "common/PipeFunctions.h"
#include "common/StructureData.h"
#include "common/SharedData.h"
#include "common/UtilityFunctions.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace ssbc = ::spl::build_cell;
namespace ssc = ::spl::common;
namespace ssp = ::spl::potential;
namespace structure_properties = ssc::structure_properties;

GeomOptimise::Settings::Settings():
    writeSummary(false), failAction(factory::FailAction::CONTINUE)
{
}

GeomOptimise::GeomOptimise(spl::potential::IGeomOptimiserPtr optimiser) :
    Block("Geometry optimise"), mySettings(), myOptimisationParams(), myOptimiser(
        optimiser)
{
}

GeomOptimise::GeomOptimise(spl::potential::IGeomOptimiserPtr optimiser,
    const Settings & settings) :
    Block("Geometry optimise"), mySettings(settings), myOptimisationParams(), myOptimiser(
        optimiser)
{
}

GeomOptimise::GeomOptimise(spl::potential::IGeomOptimiserPtr optimiser,
    const ::spl::potential::OptimisationSettings & optimisationParams) :
    Block("Potential geometry optimisation"), mySettings(), myOptimisationParams(
        optimisationParams), myOptimiser(optimiser)
{
}

GeomOptimise::GeomOptimise(spl::potential::IGeomOptimiserPtr optimiser,
    const ::spl::potential::OptimisationSettings & optimisationParams,
    const Settings & settings) :
    Block("Potential geometry optimisation"), mySettings(settings), myOptimisationParams(
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
}

void
GeomOptimise::in(common::StructureData * const data)
{
  ssc::Structure * const structure = data->getStructure();
  ssp::OptimisationData optData;
  const ssp::OptimisationOutcome outcome = myOptimiser->optimise(*structure,
      optData, myOptimisationParams);
  if(outcome.isSuccess()
      || mySettings.failAction == factory::FailAction::CONTINUE)
  {
    // Update our data table with the structure data
    if(mySettings.writeSummary)
      updateTable(*structure, optData);
    out(data);
  }
  else
    drop(data);
}

ssp::GeomOptimiser &
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
  const ::std::string & strName = structure.getName();

  const double * const internalEnergy = structure.getProperty(
      structure_properties::general::ENERGY_INTERNAL);
  if(internalEnergy)
  {
    table.insert(strName, "energy", common::toString(*internalEnergy));
    table.insert(strName, "energy/atom",
        common::toString(
            *internalEnergy / static_cast< double>(structure.getNumAtoms())));
    if(optimisationData.numIters)
      table.insert(strName, "iters",
          ::boost::lexical_cast< ::std::string>(*optimisationData.numIters));
  }
}

}
}

