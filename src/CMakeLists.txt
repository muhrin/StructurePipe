set(spipe_Header_Files__blocks
  blocks/EdgeDetect.h
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

set(spipe_Header_Files__common
  common/SharedData.h
  common/StructureData.h
  common/UtilityFunctions.h
)
source_group("Header Files\\common" FILES ${spipe_Header_Files__common})

set(spipe_Source_Files__common
  common/StructureData.cpp
  common/UtilityFunctions.cpp
)
source_group("Source Files\\common" FILES ${spipe_Source_Files__common})

set(spipe_Header_Files__
  StructurePipe.h
  StructurePipeConfig.h
)
source_group("Header Files" FILES ${spipe_Header_Files__})

set(spipe_Source_Files__
  StructurePipe.cpp
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


include_directories(
  ${CMAKE_SOURCE_DIR}/lib/pipelib/pipelib
  ${CMAKE_SOURCE_DIR}/lib/sslib/include
)


include_directories(.)
add_executable(spipe ${spipe_Files})

add_dependencies(spipe sslib)


# Libraries we need to link to
  target_link_libraries(spipe
    ${Boost_FILESYSTEM_LIBRARY_RELEASE}
    ${Boost_SYSTEM_LIBRARY_RELEASE}	# filesystem depends on this
    sslib
  )
