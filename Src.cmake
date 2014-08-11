
## spipe/blocks/
set(spipe_Source_Files__blocks
  src/blocks/AutoScale.cpp
  src/blocks/BuildStructures.cpp
  src/blocks/Clone.cpp
  src/blocks/CutAndPaste.cpp
  src/blocks/FindSymmetryGroup.cpp
  src/blocks/GeomOptimise.cpp
  src/blocks/KeepStableCompositions.cpp
  src/blocks/KeepTopN.cpp
  src/blocks/KeepWithinXPercent.cpp
  src/blocks/LoadStructures.cpp
  src/blocks/NiggliReduce.cpp
  src/blocks/PasteFragment.cpp
  src/blocks/RemoveDuplicates.cpp
  src/blocks/Rescale.cpp
  src/blocks/RunPotentialParamsQueue.cpp
  src/blocks/SearchStoichiometries.cpp
  src/blocks/SeparateAtoms.cpp
  src/blocks/WriteStructures.cpp
)
source_group("Source Files\\blocks" FILES ${spipe_Source_Files__blocks})

## spipe/build
set(spipe_Source_Files__build
  src/build/BlockLoader.cpp
  src/build/PipeBuilder.cpp
)
source_group("Source Files\\build" FILES ${spipe_Source_Files__build})

## spipe/common
set(spipe_Source_Files__common
  src/common/CommonData.cpp
  src/common/GlobalData.cpp
  src/common/PipeFunctions.cpp
  src/common/SharedData.cpp
  src/common/StructureData.cpp
  src/common/UtilityFunctions.cpp
)
source_group("Source Files\\common" FILES ${spipe_Source_Files__common})

## spipe/factory
set(spipe_Source_Files__factory
  src/factory/BlockFactory.cpp
  src/factory/PipeEngineFactory.cpp
)
source_group("Source Files\\factory" FILES ${spipe_Source_Files__factory})

## spipe/io/
set(spipe_Source_Files__io
  src/io/BlockLoaderType.cpp
)
source_group("Source Files\\io" FILES ${spipe_Source_Files__io})

## spipe/utility/
set(spipe_Source_Files__utility
  src/utility/DataTable.cpp
  src/utility/DataTableInserters.cpp
  src/utility/DataTableSupport.cpp
  src/utility/DataTableWriter.cpp
  src/utility/Initialisation.cpp
)
source_group("Source Files\\utility" FILES ${spipe_Source_Files__utility})

## spipe/
set(spipe_Source_Files__
)
source_group("Source Files" FILES ${spipe_Source_Files__})

set(spipe_Source_Files
  ${spipe_Source_Files__build}
  ${spipe_Source_Files__blocks}
  ${spipe_Source_Files__build}
  ${spipe_Source_Files__common}
  ${spipe_Source_Files__factory}
  ${spipe_Source_Files__io}
  ${spipe_Source_Files__utility}
  ${spipe_Source_Files__}
)


