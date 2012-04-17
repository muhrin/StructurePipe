/*
 * PotentialParamSweep.h
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef POTENTIAL_PARAM_SWEEP_H
#define POTENTIAL_PARAM_SWEEP_H

// INCLUDES /////////////////////////////////////////////
#include "StructurePipe.h"

// From SSTbx
#include <utility/MultiIdx.h>

// From PipeLib
#include <AbstractSimpleStartBlock.h>
#include <IDataSink.h>

#include <armadillo>

#include <vector>

// FORWARD DECLARATIONS ////////////////////////////////////
template <class DataType, class GlobalDataType>
class IPipeline;


namespace spipe { namespace blocks {

class PotentialParamSweep :
	public pipelib::AbstractSimpleStartBlock<StructureDataTyp, SharedDataTyp>,
	public pipelib::IDataSink<StructureDataTyp>
{
public:
	PotentialParamSweep(
		const ::arma::vec	&		from,
		const ::arma::vec	&		step,
		const ::arma::Col<unsigned int> & nSteps,
		IPipelineTyp &				sweepPipeline);

	// From Block /////////////////////////////////
	virtual void pipelineInitialising();
	virtual void pipelineInitialised();
	virtual void start();
	virtual void in(StructureDataTyp * const data);
	// End from Block //////////////////////////////

private:

	size_t								myNumParams;
	const ::arma::vec					myFrom;
	const ::arma::vec					myStep;
	const ::arma::Col<unsigned int>			myNSteps;
	::sstbx::utility::MultiIdx<size_t>	myStepExtents;

	IPipelineTyp & mySweepPipeline;

	/** Buffer to store structure that have finished their path through the sub pipeline. */
	::std::vector<StructureDataTyp *>			myBuffer;

};

}}

#endif /* POTENTIAL_PARAM_SWEEP_H */
