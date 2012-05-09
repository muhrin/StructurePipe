/*
 * RunSPipe.h
 *
 *
 *  Created on: May 4, 2012
 *      Author: Martin Uhrin
 */

#ifndef RUN_S_PIPE_H
#define RUN_S_PIPE_H

// INCLUDES /////////////////////////////////////////////
#include "mspipe/MetaStructurePipe.h"

#include <vector>

// Pipelib
#include <AbstractSimpleBlock.h>
#include <IDataSink.h>

// Local includes
#include "StructurePipe.h"


// FORWARD DECLARATIONS ////////////////////////////////////
namespace pipelib {
template <class DataTyp, class SharedDataTyp>
class AbstractSimpleBlock;
}


namespace spipe
{
namespace mspipe
{
namespace blocks
{

class RunSPipe :
  public pipelib::AbstractSimpleBlock<spipe::mspipe::MspDataTyp, spipe::mspipe::MspSharedDataTyp>,
  public pipelib::IDataSink<StructureDataTyp>
{
public:

  RunSPipe(spipe::IPipelineTyp & structurePipe);
	virtual ~RunSPipe();

  // From Block /////////////////////////////////
	virtual void pipelineInitialising();
	virtual void pipelineInitialised();
  // End from Block /////////////////////////////

	// From PipeBlock ////////////////////
  virtual void in(spipe::mspipe::MspDataTyp & data);
	// End from PipeBlock ///////////////

  // From IDataSink ///////////////////
  virtual void in(spipe::StructureDataTyp * const data);
  // End from IDataSink ///////////////

private:

  void copyMetadata(mspipe::MspDataTyp & data);

  /** The pipeline to run when this block is reached */
  spipe::IPipelineTyp &       myStructurePipe;
  
  /** The structure data buffer of structures that have finished
  /* their path through the structure pipe 
  /**/
  std::vector<spipe::StructureDataTyp *>  myStrBuffer;
};

}
}
}

#endif /* RUN_S_PIPE_H */
