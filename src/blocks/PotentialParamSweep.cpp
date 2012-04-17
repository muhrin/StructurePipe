/*
 * PotentialParamSweep.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "PotentialParamSweep.h"

#include "common/SharedData.h"
#include "common/StructureData.h"

// From SSTbx
#include <utility/Loops.h>

// From PipelineLib
#include <IPipeline.h>

#include <boost/foreach.hpp>

// NAMESPACES ////////////////////////////////


namespace spipe { namespace blocks {

PotentialParamSweep::PotentialParamSweep(
	const ::arma::vec	&		from,
	const ::arma::vec	&		step,
	const ::arma::Col<unsigned int> & nSteps,
	IPipelineTyp &				sweepPipeline):
pipelib::Block<StructureDataTyp, SharedDataTyp>("Potential param sweep"),
myFrom(from),
myStep(step),
myNSteps(nSteps),
mySweepPipeline(sweepPipeline),
myStepExtents(nSteps.n_rows)
{
	SP_ASSERT((myFrom.n_rows == myStep.n_rows) && (myFrom.n_rows == myNSteps.n_rows));

	myNumParams		= myNSteps.n_rows;
	for(size_t i = 0; i < myNumParams; ++i)
	{
		myStepExtents[i] = myNSteps(i);
	}
}

void PotentialParamSweep::pipelineInitialising()
{
	// Set the parameters in the shared data
	SharedDataTyp & sharedDat = myPipeline->getSharedData();
	sharedDat.potSweepFrom.reset(myFrom);
	sharedDat.potSweepStep.reset(myStep);
	sharedDat.potSweepNSteps.reset(myNSteps);
}

void PotentialParamSweep::pipelineInitialised()
{
	// Set outselves to collect any finished data from the sweep pipeline
	mySweepPipeline.setFinishedDataSink(*this);

	// Initialise the sweep pipeline
	mySweepPipeline.initialise();
}

void PotentialParamSweep::start()
{
	using ::sstbx::utility::Loops;

	for(Loops<size_t> loops(myStepExtents); !loops.isAtEnd(); ++loops)
	{
		// Load the current potential parameters into the pipeline data
		SharedDataTyp & pipelineDat = mySweepPipeline.getSharedData();

		::arma::vec params(myNumParams);
		for(size_t i = 0; i < myNumParams; ++i)
		{
			params(i) = myFrom(i) + (*loops)[i] * myStep(i);
		}
		pipelineDat.potentialParams.reset(params);

		// Start the sweep pipeline
		mySweepPipeline.start();

		// Send any finished structure data down my pipe
		BOOST_FOREACH(StructureDataTyp * const sweepPipeData, myBuffer)
		{
			myOutput->in(*sweepPipeData);
		}
		myBuffer.clear();
	}
}

void  PotentialParamSweep::in(StructureDataTyp * const data)
{
	SP_ASSERT(data);

	// Copy over the parameters into the structure data
	data->potentialParams = mySweepPipeline.getSharedData().potentialParams;

	// Register the data with our pipeline to transfer ownership
	myPipeline->registerNewData(data);

	// Save it in the buffer for sending down the pipe
	myBuffer.push_back(data);
}

}}

