/*
 * ConvexHullGenerator.h
 *
 *
 *  Created on: May 4, 2012
 *      Author: Martin Uhrin
 */

#ifndef CONVEX_HULL_GENERATOR_H
#define CONVEX_HULL_GENERATOR_H

// INCLUDES /////////////////////////////////////////////
#include "mspipe/MetaStructurePipe.h"

#include <fstream>

// Pipelib
#include <AbstractSimpleBlock.h>


// FORWARD DECLARATIONS ////////////////////////////////////


namespace spipe
{
namespace mspipe
{
namespace blocks
{

class ConvexHullGenerator :
  public pipelib::AbstractSimpleBlock<spipe::mspipe::MspDataTyp, spipe::mspipe::MspSharedDataTyp>
{
public:

  ConvexHullGenerator();
	virtual ~ConvexHullGenerator();

  // From Block //
  virtual void pipelineStarting();
  virtual void pipelineEnding();
  // End from Block //


	// From PipeBlock ////////////////////
  virtual void in(spipe::mspipe::MspDataTyp & data);
	// End from PipeBlock ///////////////

private:


  std::ofstream   myHullStream;
};

}
}
}

#endif /* CONVEX_HULL_GENERATOR_H */
