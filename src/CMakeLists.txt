
## spipe/blocks/

set(spipe_Header_Files__blocks
  blocks/EdgeDetect.h
  blocks/LoadPotStructures.h
  blocks/LowestFreeEnergy.h
  blocks/MakeMap.h
  blocks/NiggliReduction.h
  blocks/ParamPotentialGo.h
  blocks/PotentialGo.h
  blocks/PotentialParamSweep.h
  blocks/RandomStructure.h
  blocks/RemoveDuplicates.h
  blocks/WriteStructure.h
)
source_group("Header Files\\blocks" FILES ${spipe_Header_Files__blocks})

set(spipe_Source_Files__blocks
  blocks/LoadPotStructures.cpp
  blocks/LowestFreeEnergy.cpp
  blocks/MakeMap.cpp
  blocks/NiggliReduction.cpp
  blocks/ParamPotentialGo.cpp
  blocks/PotentialGo.cpp
  blocks/PotentialParamSweep.cpp
  blocks/RandomStructure.cpp
  blocks/RemoveDuplicates.cpp
  blocks/WriteStructure.cpp
)
source_group("Source Files\\blocks" FILES ${spipe_Source_Files__blocks})

## spipe/common/

set(spipe_Header_Files__common
  common/FactoryError.h
  common/PipeBuilder.h
  common/PipeFactoryYaml.h
  common/SharedData.h
  common/SsLibFactoryYaml.h
  common/SsLibYamlKeywords.h
  common/StructureData.h
  common/UtilityFunctions.h
  common/YamlCommon.h
  common/YamlKeywords.h
)
source_group("Header Files\\common" FILES ${spipe_Header_Files__common})

set(spipe_Source_Files__common
  common/PipeBuilder.cpp
  common/PipeFactoryYaml.cpp
  common/SsLibFactoryYaml.cpp
  common/StructureData.cpp
  common/UtilityFunctions.cpp
)
source_group("Source Files\\common" FILES ${spipe_Source_Files__common})

## spipe/

set(spipe_Header_Files__
  StructurePipe.h
  StructurePipeConfig.h
)
source_group("Header Files" FILES ${spipe_Header_Files__})

set(spipe_Source_Files__
)
source_group("Source Files" FILES ${spipe_Source_Files__})

set(spipe_Header_Files
  ${spipe_Header_Files__blocks}
  ${spipe_Header_Files__common}
  ${spipe_Header_Files__}
)

set(spipe_Source_Files
  ${spipe_Source_Files__blocks}
  ${spipe_Source_Files__common}
  ${spipe_Source_Files__}
)

set(spipe_Files
  ${spipe_Header_Files}
  ${spipe_Source_Files}
)

#############################
## MetaStructurePipe files ##
#############################


## mspipe/blocks

set(mspipe_Header_Files__blocks
  mspipe/blocks/StoichiometrySearch.h
  mspipe/blocks/ConvexHullGenerator.h
  mspipe/blocks/RunSPipe.h
)
source_group("Header Files\\mspipe\\blocks" FILES ${mspipe_Header_Files__blocks})

set(mspipe_Source_Files__blocks
  mspipe/blocks/StoichiometrySearch.cpp
  mspipe/blocks/ConvexHullGenerator.cpp
  mspipe/blocks/RunSPipe.cpp
)
source_group("Source Files\\mspipe\\blocks" FILES ${mspipe_Source_Files__blocks})

## mspipe/

set(mspipe_Header_Files__
  mspipe/MetaStructurePipe.h
  mspipe/MetaStructurePipeData.h
  mspipe/MetaStructurePipeSharedData.h
)
source_group("Header Files\\mspipe" FILES ${mspipe_Header_Files__})

set(mspipe_Source_Files__
  mspipe/MetaStructurePipe.cpp
  mspipe/MetaStructurePipeData.cpp
)
source_group("Source Files\\mspipe" FILES ${spipe_Source_Files__})

set(mspipe_Header_Files
  ${mspipe_Header_Files__blocks}
  ${mspipe_Header_Files__common}
  ${mspipe_Header_Files__}
  ${spipe_Header_Files__blocks}
  ${spipe_Header_Files__common}
  ${spipe_Header_Files__}
)

set(mspipe_Source_Files
  ${mspipe_Source_Files__blocks}
  ${mspipe_Source_Files__common}
  ${mspipe_Source_Files__}
  ${spipe_Source_Files__blocks}
  ${spipe_Source_Files__common}
  ${spipe_Source_Files__}
)

set(mspipe_Files
  ${mspipe_Header_Files}
  ${mspipe_Source_Files}
)

#########################
## Include directories ##
#########################

include_directories(
  ${CMAKE_SOURCE_DIR}/lib/pipelib/pipelib
  ${CMAKE_SOURCE_DIR}/lib/sslib/include
  ${CMAKE_SOURCE_DIR}/lib/yaml-cpp/include
  ${CMAKE_SOURCE_DIR}/lib/sslib/src # TODO: Remove this and move sslib/src/SSLib* to include
)


include_directories(.)


###################
## StructurePipe ##
###################
add_executable(spipe
  ${spipe_Files}
  StructurePipe.cpp
)

add_dependencies(spipe sslib)

# Libraries we need to link to
  target_link_libraries(spipe
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}	# filesystem depends on this
    ${ARMADILLO_LIBRARIES}
    yaml-cpp
    sslib
  )

##################
## StructureMap ##
##################
add_executable(smap
  ${spipe_Files}
  MakeMap.cpp
)

add_dependencies(smap sslib)

# Libraries we need to link to
  target_link_libraries(smap
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}	# filesystem depends on this
    ${ARMADILLO_LIBRARIES}
    yaml-cpp
    sslib
  )
  

###################
## StructureDiff ##
###################
add_executable(sdiff
  ${spipe_Files}
  StructureDiff.cpp
)

add_dependencies(sdiff sslib)

# Libraries we need to link to
  target_link_libraries(sdiff
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}	# filesystem depends on this
    ${ARMADILLO_LIBRARIES}
    yaml-cpp
    sslib
  )
  


#######################
## MetaStructurePipe ##
#######################
add_executable(mspipe
  ${mspipe_Files}
)

add_dependencies(mspipe spipe)

# Libraries we need to link to
  target_link_libraries(mspipe
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}	# filesystem depends on this
    ${ARMADILLO_LIBRARIES}
    yaml-cpp
    sslib
  )

