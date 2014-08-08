
## spipe/blocks/
set(spipe_Header_Files__blocks
  include/spipe/blocks/blocks.h
  include/spipe/blocks/BuildStructures.h
  include/spipe/blocks/Clone.h
  include/spipe/blocks/CutAndPaste.h
  include/spipe/blocks/FindSymmetryGroup.h
  include/spipe/blocks/EdgeDetect.h
  include/spipe/blocks/GeomOptimise.h
  include/spipe/blocks/KeepStableCompositions.h
  include/spipe/blocks/KeepTopN.h
  include/spipe/blocks/KeepWithinXPercent.h
  include/spipe/blocks/LoadStructures.h
  include/spipe/blocks/NiggliReduce.h
  include/spipe/blocks/PasteFragment.h
  include/spipe/blocks/RemoveDuplicates.h
  include/spipe/blocks/Rescale.h
  include/spipe/blocks/RunPotentialParamsQueue.h
  include/spipe/blocks/SearchStoichiometries.h
  include/spipe/blocks/SeparateAtoms.h
  include/spipe/blocks/Shrink.h
  include/spipe/blocks/WriteStructures.h
)
source_group("Header Files\\blocks" FILES ${spipe_Header_Files__blocks})

## spipe/build/
set(spipe_Header_Files__build
  include/spipe/build/BlockLoader.h
  include/spipe/build/PipeBuilder.h
)
source_group("Header Files\\build" FILES ${spipe_Header_Files__build})

## spipe/common/
set(spipe_Header_Files__common
  include/spipe/common/Assert.h
  include/spipe/common/CommonData.h
  include/spipe/common/GlobalData.h
  include/spipe/common/PipeFunctions.h
  include/spipe/common/SharedData.h
  include/spipe/common/StructureData.h
  include/spipe/common/UtilityFunctions.h
)
source_group("Header Files\\common" FILES ${spipe_Header_Files__common})

## spipe/factory/
set(spipe_Header_Files__factory
  include/spipe/factory/factory.h
  include/spipe/factory/BlockFactory.h
  include/spipe/factory/PipeEngineFactory.h
  include/spipe/factory/PipeEngineSchema.h
  include/spipe/factory/YamlSchema.h
)
source_group("Header Files\\factory" FILES ${spipe_Header_Files__factory})

## spipe/io/
set(spipe_Header_Files__io
  include/spipe/io/BlockLoaderType.h
)
source_group("Header Files\\io" FILES ${spipe_Header_Files__io})

## spipe/utility/
set(spipe_Header_Files__utility
  include/spipe/utility/DataTable.h
  include/spipe/utility/DataTableInserters.h
  include/spipe/utility/DataTableSupport.h
  include/spipe/utility/DataTableWriter.h
  include/spipe/utility/IDataTableChangeListener.h
  include/spipe/utility/Initialisation.h
)
source_group("Header Files\\utility" FILES ${spipe_Header_Files__utility})

## spipe/
set(spipe_Header_Files__
  include/spipe/spipe.h
  include/spipe/PipeLibTypes.h
  include/spipe/SpTypes.h
  include/spipe/StructurePipe.h
  include/spipe/StructurePipeConfig.h
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


