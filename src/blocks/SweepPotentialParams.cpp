/*
 * SweepPotentialParams.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/blocks/SweepPotentialParams.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include <spl/io/BoostFilesystem.h>
#include <spl/utility/MultiIdxRange.h>
#include <spl/utility/UtilFunctions.h>

#include "spipe/common/Assert.h"
#include "spipe/common/PipeFunctions.h"
#include "spipe/common/SharedData.h"
#include "spipe/common/StructureData.h"
#include "spipe/common/UtilityFunctions.h"
#include "spipe/utility/DataTableInserters.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace blocks {

namespace fs = ::boost::filesystem;
namespace ssc = ::spl::common;
namespace ssio = ::spl::io;
namespace ssu = ::spl::utility;
namespace structure_properties = ssc::structure_properties;
typedef common::GlobalKeys Keys;

const ::std::string SweepPotentialParams::POTPARAMS_FILE_EXTENSION = "potparams";

SweepPotentialParams::SweepPotentialParams(
    const common::ParamRange & paramRange, BlockHandle & sweepPipeline) :
    Block("Potential param sweep"), myParamRange(paramRange), myStepExtents(
        paramRange.nSteps.size()), mySweepPipeline(sweepPipeline), mySubpipeEngine(
    NULL)
{
  SPIPE_ASSERT(myParamRange.from.size() == myParamRange.step.size());
  SPIPE_ASSERT(myParamRange.from.size() == myParamRange.nSteps.size());

  myNumParams = myParamRange.nSteps.size();
  for(size_t i = 0; i < myNumParams; ++i)
  {
    myStepExtents[i] = myParamRange.nSteps[i] + 1;
  }
}

void
SweepPotentialParams::pipelineInitialising()
{
  myTableSupport.setFilename(
      outputFileStemPath().string() + "." + POTPARAMS_FILE_EXTENSION);
  myTableSupport.registerEngine(getEngine());
}

void
SweepPotentialParams::start()
{
  const ssu::MultiIdxRange< int> stepsRange(ParamSpaceIdx(myStepExtents.dims()),
      myStepExtents);

  const fs::path workingDir = this->workingDir();

  PotentialParams params(myNumParams);
  BOOST_FOREACH(const ParamSpaceIdx & stepsIdx, stepsRange)
  {
    ::spipe::SharedDataType & sweepSharedData = mySubpipeEngine->sharedData();

    // Load the current potential parameters into the pipeline data
    for(size_t i = 0; i < myNumParams; ++i)
      params[i] = myParamRange.from[i]
          + static_cast< double>(stepsIdx[i]) * myParamRange.step[i];

    // Store the potential parameters in global memory
    getEngine()->globalData().objectsStore[common::GlobalKeys::POTENTIAL_PARAMS] =
        params;

    const fs::path & sweepPath = workingDir
        / common::generateParamDirName(params,
            getEngine()->globalData().getSeedName());
    sweepSharedData.setWorkingDir(sweepPath);

    // Run the sweep pipeline
    mySubpipeEngine->run();

    // Send any finished structure data down my pipe
    releaseBufferedStructures(sweepPath.string());
  }
}

void
SweepPotentialParams::engineAttached(EngineSetup * setup)
{
  mySubpipeEngine = setup->createEngine();
  mySubpipeEngine->attach(mySweepPipeline);
  // Set outselves to collect any finished data from the sweep pipeline
  mySubpipeEngine->setFinishedDataSink(this);
}

void
SweepPotentialParams::engineDetached()
{
  if(mySubpipeEngine)
  {
    mySubpipeEngine->setFinishedDataSink(NULL);
    mySubpipeEngine->detach();
    mySubpipeEngine = NULL;
  }
}

void
SweepPotentialParams::finished(StructureDataUniquePtr data)
{
  // Copy over the parameters into the structure data
  const ::spipe::common::ObjectData< const PotentialParams> result =
      ::spipe::common::getObjectConst(
          ::spipe::common::GlobalKeys::POTENTIAL_PARAMS,
          mySubpipeEngine->sharedData(), mySubpipeEngine->globalData());

  if(result.first != common::DataLocation::NONE)
    data->objectsStore[common::GlobalKeys::POTENTIAL_PARAMS] = *result.second;

  // Register the data with our pipeline to transfer ownership
  // Save it in the buffer for sending down the pipe
  myBuffer.push_back(getEngine()->registerData(data));
}

void
SweepPotentialParams::releaseBufferedStructures(
    const ::spipe::utility::DataTable::Key & key)
{
  // Send any finished structure data down my pipe
  BOOST_FOREACH(StructureDataType * const sweepStrData, myBuffer)
  {
    updateTable(key, *sweepStrData);

    out(sweepStrData);
  }
  myBuffer.clear();
}

void
SweepPotentialParams::updateTable(const utility::DataTable::Key & key,
    const StructureDataType & structureData)
{
  utility::insertStructureInfoAndPotentialParams(key, structureData,
      workingDir(), myTableSupport.getTable());
}

}
}

