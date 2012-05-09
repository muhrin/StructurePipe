/*
 * RunSPipe.cpp
 *
 *  Created on: May 4, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "mspipe/blocks/RunSPipe.h"

#include <iostream> // TODO: TEMP

#include <boost/foreach.hpp>

// From Pipelib
#include <IPipeline.h>

// Local includes
#include "common/StructureData.h"
#include "common/SharedData.h"
#include "mspipe/MetaStructurePipe.h"
#include "mspipe/MetaStructurePipeData.h"

// NAMESPACES ////////////////////////////////


namespace spipe {
namespace mspipe {
namespace blocks {

RunSPipe::RunSPipe(spipe::IPipelineTyp & structurePipe):
pipelib::Block<mspipe::MspDataTyp, mspipe::MspSharedDataTyp>("Run structure pipe"),
myStructurePipe(structurePipe)
{}

RunSPipe::~RunSPipe()
{
  // If there is any data in the structure buffer that means is
  // was not passed on to the metadata properly and we should
  // delete it
  if(!myStrBuffer.empty())
  {
    BOOST_FOREACH(spipe::StructureDataTyp * const strDat, myStrBuffer)
    {
      delete strDat;
    }
  }
}

void RunSPipe::pipelineInitialising()
{}

void RunSPipe::pipelineInitialised()
{
	// Set outselves to collect any finished data from the structure pipeline
  myStructurePipe.setFinishedDataSink(*this);

	// Initialise the structure pipeline
	myStructurePipe.initialise();
}

void RunSPipe::in(mspipe::MspDataTyp & data)
{

  copyMetadata(data);

  myStructurePipe.start();

  // Now copy over any structures that finished their path through
  // the pipeline
  data.resultStructures.insert(
    data.resultStructures.end(),
    myStrBuffer.begin(),
    myStrBuffer.end());
  myStrBuffer.clear();

  // Continue down the pipe
	myOutput->in(data);
}

void RunSPipe::in(spipe::StructureDataTyp * const strDat)
{
  SP_ASSERT(strDat);

	// Save it in the buffer for sending down the pipe
	myStrBuffer.push_back(strDat);

  std::cout << "Energy: " << *strDat->enthalpy << std::endl;
}

void RunSPipe::copyMetadata(mspipe::MspDataTyp & data)
{
  spipe::SharedDataTyp & sharedDat = myStructurePipe.getSharedData();
  if(data.structureDescription)
    sharedDat.structureDescription = data.structureDescription;

  if(data.cellDescription)
    sharedDat.cellDescription = data.cellDescription;
}

}
}
}

