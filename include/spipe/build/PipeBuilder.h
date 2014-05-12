/*
 * PipeBuilder.h
 *
 *
 *  Created on: Jan 12, 2014
 *      Author: Martin Uhrin
 */

#ifndef PIPE_BUILDER_H
#define PIPE_BUILDER_H

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <yaml-cpp/yaml.h>

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////
namespace spipe {
namespace build {

BlockHandle
buildPipe(const YAML::Node & node);

}
}

#endif /* PIPE_BUILDER_H */
