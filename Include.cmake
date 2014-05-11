
## spipe/build/
set(spipe_Header_Files__build
  build/BlockLoader.h
)
source_group("Header Files\\build" FILES ${spipe_Header_Files__build})

## spipe/blocks/
set(spipe_Header_Files__blocks
  src/blocks/BuildStructures.h
  src/blocks/Clone.h
  src/blocks/CutAndPaste.h
  src/blocks/FindSymmetryGroup.h
  src/blocks/EdgeDetect.h
  src/blocks/GeomOptimise.h
  src/blocks/KeepStableCompositions.h
  src/blocks/KeepTopN.h
  src/blocks/KeepWithinXPercent.h
  src/blocks/LoadStructures.h
  src/blocks/NiggliReduce.h
  src/blocks/ParamGeomOptimise.h
  src/blocks/PasteFragment.h
  src/blocks/RemoveDuplicates.h
  src/blocks/RunPotentialParamsQueue.h
  src/blocks/SearchStoichiometries.h
  src/blocks/SeparateAtoms.h
  src/blocks/SweepPotentialParams.h
  src/blocks/WriteStructures.h
)
source_group("Header Files\\blocks" FILES ${spipe_Header_Files__blocks})

## spipe/build/
set(spipe_Header_Files__build
  src/build/BlockLoader.h
  src/build/PipeBuilder.h
)
source_group("Header Files\\build" FILES ${spipe_Header_Files__build})

## spipe/common/
set(spipe_Header_Files__common
  src/common/Assert.h
  src/common/CommonData.h
  src/common/GlobalData.h
  src/common/PipeBuilder.h
  src/common/PipeFunctions.h
  src/common/SharedData.h
  src/common/StructureData.h
  src/common/UtilityFunctions.h
)
source_group("Header Files\\common" FILES ${spipe_Header_Files__common})

## spipe/factory/
set(spipe_Header_Files__factory
  src/factory/BlockFactory.h
  src/factory/PipeEngineFactory.h
  src/factory/PipeEngineSchema.h
  src/factory/YamlSchema.h
)
source_group("Header Files\\factory" FILES ${spipe_Header_Files__factory})

## spipe/io/
set(spipe_Header_Files__io
  src/io/BlockLoaderType.h
)
source_group("Header Files\\io" FILES ${spipe_Header_Files__io})

## spipe/utility/
set(spipe_Header_Files__utility
  src/utility/PipeDataInitialisation.h
  src/utility/DataTable.h
  src/utility/DataTableInserters.h
  src/utility/DataTableSupport.h
  src/utility/DataTableWriter.h
  src/utility/IDataTableChangeListener.h
  src/utility/Initialisation.h
)
source_group("Header Files\\utility" FILES ${spipe_Header_Files__utility})

## spipe/
set(spipe_Header_Files__
  src/PipeLibTypes.h
  src/SpTypes.h
  src/StructurePipe.h
  src/StructurePipeConfig.h
)
source_group("Header Files" FILES ${spipe_Header_Files__})

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


