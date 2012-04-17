/*
 * EdgeDetect.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef EDGE_DETECT_H
#define EDGE_DETECT_H

// INCLUDES /////////////////////////////////////////////
#include "StructurePipe.h"

#include "common/SharedData.h"

// From SSTbx
#include <utility/EdgeMap.h>
#include <utility/IStructureComparator.h>
#include <utility/Loops.h>
#include <utility/MultiArray.h>
#include <utility/MultiIdx.h>

// From Pipelib
#include <AbstractSimpleBlock.h>
#include <IPipeline.h>

#include <armadillo>

#include <map>
#include <vector>

// FORWARD DECLARATIONS ////////////////////////////////////


namespace spipe { namespace blocks {

template <class CompDatTyp>
class EdgeDetect : public pipelib::AbstractSimpleBlock<StructureDataTyp, SharedDataTyp>
{
public:

	EdgeDetect(const ::sstbx::utility::IStructureComparator<CompDatTyp> & comparator);
	virtual ~EdgeDetect();

	// From Block ////////////////////////
	virtual void pipelineStarting();
	virtual void pipelineFinishing();
	// End from Block //////////////////////

	// From PipeBlock ////////////////////
	virtual void in(spipe::common::StructureData & data);
	// End from PipeBlock ///////////////

private:

	/** An array keeping track of structure data that we're still waiting on to finish edge detection */
	typedef ::sstbx::utility::MultiArray<spipe::common::StructureData *> OutputData;

	typedef ::std::vector<sstbx::utility::FinishedEdgePair> FinishedEdgeContainer;

	struct EdgeMapper
	{
		EdgeMapper(
			const ::sstbx::utility::MultiIdx<size_t>					extents,
			const ::sstbx::utility::IStructureComparator<CompDatTyp> &	comparator,
			const ::std::string &										filePath);
		~EdgeMapper();

		size_t											nParams;
		::sstbx::utility::EdgeMap<CompDatTyp> *			edgeMap;
		OutputData *									outputData;
		::std::ofstream									outStream;
	};

	void writeOutput(const FinishedEdgeContainer & finishedEdges);

	const ::sstbx::utility::IStructureComparator<CompDatTyp> &	myComparator;
	EdgeMapper *												myCurrentEdgeMapper;
};

// IMPLEMENTATION /////////////////////////////////

// EdgeMapper implementation //
template <class CompDatTyp>
EdgeDetect<CompDatTyp>::EdgeMapper::EdgeMapper(
	const ::sstbx::utility::MultiIdx<size_t>					extents,
	const ::sstbx::utility::IStructureComparator<CompDatTyp> &	comparator,
	const ::std::string &										filePath):
nParams(extents.dims),
edgeMap(new ::sstbx::utility::EdgeMap<CompDatTyp>(comparator, extents)),
outputData(new OutputData(extents))
{
	// Open the output file in append mode
	outStream.open(filePath.c_str(), ::std::ios::app);
}

template <class CompDatTyp>
EdgeDetect<CompDatTyp>::EdgeMapper::~EdgeMapper()
{
	if(edgeMap)
		delete edgeMap;
	if(outputData)
		delete outputData;
	if(outStream.is_open())
		outStream.close();
}

// EdgeDetect implementation //

template <class CompDatTyp>
EdgeDetect<CompDatTyp>::EdgeDetect(const ::sstbx::utility::IStructureComparator<CompDatTyp> & comparator):
pipelib::Block<StructureDataTyp, SharedDataTyp>("Edge detect"),
myComparator(comparator),
myCurrentEdgeMapper(NULL)
{
}

template <class CompDatTyp>
EdgeDetect<CompDatTyp>::~EdgeDetect()
{
	if(myCurrentEdgeMapper)
		delete myCurrentEdgeMapper;
}

template <class CompDatTyp>
void EdgeDetect<CompDatTyp>::pipelineStarting()
{
	using ::sstbx::utility::EdgeMap;
	using ::sstbx::utility::MultiArray;
	using ::sstbx::utility::MultiIdx;

	::spipe::SharedDataTyp & sData = myPipeline->getSharedData();

	// From out the number of dimensions if possible
	if(sData.potSweepNSteps)
	{
		const ::arma::Col<unsigned int> & steps = *sData.potSweepNSteps;

		// Get the number of steps for each parameter (can be 0 for some parameters)
		MultiIdx<size_t> extents(steps.n_rows);
		for(size_t i = 0; i < steps.n_rows; ++i)
		{
			extents[i] = steps(i);
		}

		myCurrentEdgeMapper = new EdgeMapper(extents, myComparator, "test.map");
	}
}

template <class CompDatTyp>
void EdgeDetect<CompDatTyp>::pipelineFinishing()
{
	// Clean up the edge data
	if(myCurrentEdgeMapper)
	{
		delete myCurrentEdgeMapper;
		myCurrentEdgeMapper = NULL;
	}
}

template <class CompDatTyp>
void EdgeDetect<CompDatTyp>::in(spipe::common::StructureData & data)
{
	using arma::Col;
	using arma::vec;
	using ::sstbx::utility::MultiIdx;
	using ::sstbx::utility::EdgeMap;


	const ::spipe::common::SharedData & sData = myPipeline->getSharedData();

	// Find out where in parameter space this structure is
	const vec relativeParams = *data.potentialParams - *sData.potSweepFrom;

	MultiIdx<size_t> pos(myCurrentEdgeMapper->nParams, 0);
	for(size_t i = 0; i < myCurrentEdgeMapper->nParams; ++i)
	{
		if((*sData.potSweepStep)(i) > 0)
			pos[i] = ::std::floor(relativeParams(i) / (*sData.potSweepStep)(i) + 0.5);
	}

	// Retain a handle to this structure data untill the edge data has been computed
	(*myCurrentEdgeMapper->outputData)[pos] = &data;
	myPipeline->flagData(*this, data);

	FinishedEdgeContainer finishedEdges;
	// Check if any edges have fininshed being calculated
	if(myCurrentEdgeMapper->edgeMap->update(*data.getStructure(), pos, &finishedEdges))
	{
		writeOutput(finishedEdges);
	}

	// Send the structure on its way
	myOutput->in(data);
}

template <class CompDatTyp>
void EdgeDetect<CompDatTyp>::writeOutput(
	const FinishedEdgeContainer & finishedEdges)
{
	using ::sstbx::utility::FinishedEdgePair;

	const ::arma::vec & from = *myPipeline->getSharedData().potSweepFrom;
	const ::arma::vec & step = *myPipeline->getSharedData().potSweepStep;
	::arma::vec params(myCurrentEdgeMapper->nParams);

	// Go through them all and write to output file
	BOOST_FOREACH(const FinishedEdgePair & edge, finishedEdges)
	{
		::spipe::common::StructureData * const data = (*myCurrentEdgeMapper->outputData)[edge.first];

		::std::ofstream & out = myCurrentEdgeMapper->outStream;

		for(size_t i = 0; i < myCurrentEdgeMapper->nParams; ++i)
		{
			params(i) = from(i) + edge.first[i] * step(i);
		}

		if(out.is_open())
		{
			if(data->group)
				out << *data->group << "/";

			if(data->name)
				out << *data->name;
			
			for(size_t i = 0; i < params.n_rows; ++i)
			{
				out << " " << params(i);
			}
				
			out << " " << edge.second << std::endl;
		}

		// We're done with this data
		myPipeline->unflagData(*this, *data);
		(*myCurrentEdgeMapper->outputData)[edge.first] = NULL;
	}
}

}}

#endif /* EDGE_DETECT_H */
