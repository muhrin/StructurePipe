/*
 * Rescale.h
 *
 *  Created on: May 22, 2014
 *      Author: Martin Uhrin
 */

#ifndef SPIPE__BLOCKS__RESCALE_H_
#define SPIPE__BLOCKS__RESCALE_H_

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include "spipe/PipeLibTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace blocks {

class Rescale : public PipeBlock
{
public:
  Rescale(const double scaleFactor);

  virtual void
  in(spl::common::Structure * const data);

private:
  const double myScaleFactor;
};

}
}

#endif /* SPIPE__BLOCKS__RESCALE_H_ */
