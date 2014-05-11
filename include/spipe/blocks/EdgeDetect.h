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
#include "spipe/StructurePipe.h"

#include <map>
#include <vector>

#include <boost/noncopyable.hpp>

#include <armadillo>

#include <spl/utility/EdgeMap.h>
#include <spl/utility/IStructureComparator.h>
#include <spl/utility/MultiArray.h>
#include <spl/utility/MultiIdx.h>

#include <pipelib/pipelib.h>

// Local includes
#include "spipe/SpTypes.h"
#include "spipe/common/StructureData.h"
#include "spipe/common/SharedData.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace blocks {

class EdgeDetect : public SpPipeBlockTyp, ::boost::noncopyable
{
public:

  EdgeDetect(const ::spl::utility::IStructureComparator & comparator);
  virtual
  ~EdgeDetect();

  // From Block ////////////////////////
  virtual void
  pipelineStarting();
  virtual void
  pipelineFinishing();
  // End from Block //////////////////////

  // From PipeBlock ////////////////////
  virtual void
  in(spipe::common::StructureData & data);
  // End from PipeBlock ///////////////

private:

  /** An array keeping track of structure data that we're still waiting on to finish edge detection */
  typedef ::spl::utility::MultiArray< spipe::common::StructureData *> OutputData;

  typedef ::std::vector< spl::utility::FinishedEdgePair> FinishedEdgeContainer;

  struct EdgeMapper
  {
    EdgeMapper(const ::spl::utility::MultiIdx< size_t> extents,
        const ::spl::utility::IStructureComparator & comparator, const ::std::string & filePath);
    ~EdgeMapper();

    size_t nParams;
    ::spl::utility::EdgeMap * edgeMap;
    OutputData * outputData;
    ::std::ofstream outStream;
  };

  void
  writeOutput(const FinishedEdgeContainer & finishedEdges);

  const ::spl::utility::IStructureComparator & myComparator;
  EdgeMapper * myCurrentEdgeMapper;
};

// IMPLEMENTATION /////////////////////////////////

// EdgeMapper implementation //
EdgeDetect::EdgeMapper::EdgeMapper(const ::spl::utility::MultiIdx< size_t> extents,
    const ::spl::utility::IStructureComparator & comparator, const ::std::string & filePath) :
    nParams(extents.dims()), edgeMap(new ::spl::utility::EdgeMap(comparator, extents)), outputData(
        new OutputData(extents, NULL))
{
  // Open the output file in append mode
  outStream.open(filePath.c_str(), ::std::ios::app);
}

EdgeDetect::EdgeMapper::~EdgeMapper()
{
  if(edgeMap)
    delete edgeMap;
  if(outputData)
    delete outputData;
  if(outStream.is_open())
    outStream.close();
}

// EdgeDetect implementation //

EdgeDetect::EdgeDetect(const ::spl::utility::IStructureComparator & comparator) :
    pipelib::Block< StructureDataTyp, SharedDataType>("Edge detect"), myComparator(comparator), myCurrentEdgeMapper(
        NULL)
{
}

EdgeDetect::~EdgeDetect()
{
  if(myCurrentEdgeMapper)
    delete myCurrentEdgeMapper;
}

void
EdgeDetect::pipelineStarting()
{
  using ::spl::utility::EdgeMap;
  using ::spl::utility::MultiArray;
  using ::spl::utility::MultiIdx;

  ::spipe::SharedDataType & sData = myPipeline->getSharedData();

  // From out the number of dimensions if possible
  if(sData.potSweepNSteps)
  {
    const ::arma::Col< unsigned int> & steps = *sData.potSweepNSteps;

    // Get the number of steps for each parameter (can be 0 for some parameters)
    MultiIdx< size_t> extents(steps.n_rows);
    for(size_t i = 0; i < steps.n_rows; ++i)
    {
      extents[i] = steps(i);
    }

    myCurrentEdgeMapper = new EdgeMapper(extents, myComparator, "edgedetect.map");
  }
}

void
EdgeDetect::pipelineFinishing()
{
  // Clean up the edge data
  if(myCurrentEdgeMapper)
  {
    delete myCurrentEdgeMapper;
    myCurrentEdgeMapper = NULL;
  }
}

void
EdgeDetect::in(spipe::common::StructureData & data)
{
  using arma::Col;
  using arma::vec;
  using ::spl::utility::MultiIdx;
  using ::spl::utility::EdgeMap;

  const ::spipe::common::SharedData & sData = myPipeline->getSharedData();

  // Find out where in parameter space this structure is
  const vec relativeParams = *data.potentialParams - *sData.potSweepFrom;

  MultiIdx< size_t> pos(myCurrentEdgeMapper->nParams, 0);
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
  out(data);
}

void
EdgeDetect::writeOutput(const FinishedEdgeContainer & finishedEdges)
{
  using ::spl::utility::FinishedEdgePair;

  const ::arma::vec & from = *myPipeline->getSharedData().potSweepFrom;
  const ::arma::vec & step = *myPipeline->getSharedData().potSweepStep;
  //::arma::vec params(myCurrentEdgeMapper->nParams);

  // Go through them all and write to output file
  BOOST_FOREACH(const FinishedEdgePair & edge, finishedEdges)
  {
    ::spipe::common::StructureData * const data = (*myCurrentEdgeMapper->outputData)[edge.first];
    const arma::vec & params = *data->potentialParams;

    ::std::ofstream & out = myCurrentEdgeMapper->outStream;

    //for(size_t i = 0; i < myCurrentEdgeMapper->nParams; ++i)
    //{
    //	params(i) = from(i) + edge.first[i] * step(i);
    //}

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

}
}

#endif /* EDGE_DETECT_H */
