
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
    sslib
  )
  


