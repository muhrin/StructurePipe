/*
 * AutoScale.h
 *
 *  Created on: May 23, 2014
 *      Author: Martin Uhrin
 */

#ifndef SPIPE__BLOCKS__AUTO_SCALE_H_
#define SPIPE__BLOCKS__AUTO_SCALE_H_

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////
namespace spipe {
namespace blocks {

class AutoScale : public PipeBlock, boost::noncopyable
{
public:
  static const double DEFAULT_PACKING_FACTOR;

  AutoScale();
  AutoScale(const double packingFactor);

  virtual void
  in(StructureDataType * const structureData);

private:
  double
  naturalVolume(const std::vector< std::string> & particles) const;

  const double myPackingFactor;
};

}
}

#endif /* SPIPE__BLOCKS__AUTO_SCALE_H_ */
