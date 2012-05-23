/*
 * StructurePipe.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "StructurePipe.h"

#include <yaml-cpp/yaml.h>

// From PipeLib
#include "IPipeline.h"

// Local includes
#include "common/PipeBuilder.h"
#include "common/PipeFactoryYaml.h"

// MACROS ////////////////////////////////////

// NAMESPACES ////////////////////////////////


int main(const int argc, const char * const argv[])
{
  namespace sp = ::spipe;
  namespace spcom = ::spipe::common;

  if(argc != 2)
  {
    ::std::cout << "Usage: " << argv[0] << " [yaml pipline in file]";
    return 1;
  }

  try
  {
    YAML::Node config = YAML::LoadFile(argv[1]);

    spcom::PipeBuilder builder;

    sp::IPipelineTyp * pipe = builder.buildPipeFromYaml(config);

    if(pipe)
    {
      pipe->initialise();
      pipe->start();
    }
  }
  catch(const YAML::Exception & e)
  {
    ::std::cerr << "Error parsing YAML file\n" << e.what();
  }

	return 0;
}

