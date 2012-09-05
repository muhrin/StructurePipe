/*
 * DistanceCalculator.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES ///////////////
#include "common/DistanceCalculator.h"

#include <boost/assert.hpp>


namespace sstbx {
namespace common {

const size_t DEFAULT_MAX_OUT_VECTORS = 10000;

DistanceCalculator::DistanceCalculator(const Structure & structure):
myStructure(structure)
{}

}
}
