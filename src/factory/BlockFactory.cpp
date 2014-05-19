/*
 * BlockFactory.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/factory/BlockFactory.h"

#include <spl/potential/Types.h>
#include <spl/utility/UtilityFwd.h>

// Local includes
#include "spipe/blocks/BuildStructures.h"
#include "spipe/blocks/Clone.h"
#include "spipe/blocks/CutAndPaste.h"
#include "spipe/blocks/FindSymmetryGroup.h"
#include "spipe/blocks/KeepStableCompositions.h"
#include "spipe/blocks/KeepTopN.h"
#include "spipe/blocks/KeepWithinXPercent.h"
#include "spipe/blocks/LoadStructures.h"
#include "spipe/blocks/NiggliReduce.h"
#include "spipe/blocks/GeomOptimise.h"
#include "spipe/blocks/ParamGeomOptimise.h"
#include "spipe/blocks/PasteFragment.h"
#include "spipe/blocks/RemoveDuplicates.h"
#include "spipe/blocks/RunPotentialParamsQueue.h"
#include "spipe/blocks/SearchStoichiometries.h"
#include "spipe/blocks/SeparateAtoms.h"
#include "spipe/blocks/WriteStructures.h"
#include "spipe/common/CommonData.h"
#include "spipe/common/StructureData.h"
#include "spipe/common/SharedData.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace factory {

// Alias for accessing keywords namespace
namespace ssf = spl::factory;
namespace spb = spipe::blocks;
namespace ssbc = spl::build_cell;
namespace ssio = spl::io;
namespace ssp = spl::potential;
namespace ssu = spl::utility;

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::BuildStructures & options) const
{
  // Try to construct a structure generator
  ssbc::IStructureGeneratorPtr generator(
      mySplFactory.createStructureGenerator(options.generator));
  if(!generator.get())
    return false;

  blockOut->reset(new spipe::blocks::BuildStructures(options.num, generator));
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::Clone & options) const
{
  if(options.giveUniqueNames)
    blockOut->reset(
        new spipe::blocks::Clone(options.times, *options.giveUniqueNames));
  else
    blockOut->reset(new spipe::blocks::Clone(options.times));
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::CutAndPaste & options) const
{
  ssf::GenShapeFactory::GenShapePtr genShape;
  if(!mySplFactory.getShapeFactory().createShape(genShape, options.genShape))
    return false;

  spipe::blocks::CutAndPaste::Settings settings;
  settings.paste = options.paste;
  settings.separate = options.separate;
  settings.fixUntouched = options.fixUntouched;

  spl::UniquePtr< spipe::blocks::CutAndPaste>::Type cutAndPaste(
      new spipe::blocks::CutAndPaste(genShape, settings));
  if(options.pairDistances)
    cutAndPaste->setPairDistances(*options.pairDistances);

  blockOut->reset(cutAndPaste.release());

  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::FindSymmetryGroup & options) const
{
  blockOut->reset(new spipe::blocks::FindSymmetryGroup());
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::GeomOptimise & options) const
{
  ssp::IGeomOptimiserPtr optimiser = mySplFactory.createGeometryOptimiser(
      options.optimiser);
  if(!optimiser.get())
    return false;

  const bool potentialIsParameterisable = optimiser->getPotential()
      && optimiser->getPotential()->getParameterisable();

  const ssp::OptimisationSettings optParams =
      mySplFactory.createOptimisationSettings(options);

  spipe::blocks::GeomOptimise::Settings settings;
  settings.failAction = options.failAction;
  settings.writeSummary = options.writeSummary;

  if(potentialIsParameterisable)
    blockOut->reset(
        new spipe::blocks::ParamGeomOptimise(optimiser, optParams, settings));
  else
    blockOut->reset(
        new spipe::blocks::GeomOptimise(optimiser, optParams, settings));

  return true;
}

#ifdef SPL_WITH_CGAL
bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::KeepStableCompositions & options) const
{
  blockOut->reset(new spipe::blocks::KeepStableCompositions(options.writeHull));
  return true;
}
#endif

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::KeepTopN & options) const
{
  blockOut->reset(new spipe::blocks::KeepTopN(options.num));
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::KeepWithinXPercent & options) const
{
  blockOut->reset(new spipe::blocks::KeepWithinXPercent(options.percent));
  return false;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const std::string & toLoad) const
{
  blockOut->reset(new spipe::blocks::LoadStructures(toLoad));
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::NiggliReduce & options) const
{
  blockOut->reset(new spipe::blocks::NiggliReduce());
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    const blocks::PasteFragment & options) const
{
  blockOut->reset(new spipe::blocks::PasteFragment(options.fragment));
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * blockOut,
    const blocks::RemoveDuplicates & options) const
{
  ssu::IStructureComparatorPtr comparator(
      mySplFactory.createStructureComparator(options.comparator));
  if(!comparator.get())
    return false;

  blockOut->reset(new spipe::blocks::RemoveDuplicates(comparator));
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    blocks::RunPotentialParamsQueue & options) const
{
  if(!options.pipe)
    return false;

  blockOut->reset(
      new spipe::blocks::RunPotentialParamsQueue(&options.paramsQueueFile,
          &options.paramsDoneFile, options.pipe));
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    blocks::SearchStoichiometries & options) const
{
  if(!options.pipe)
    return false;

  spipe::blocks::SearchStoichiometries::Options searchOptions;
  searchOptions.atomRanges = options.ranges;
  searchOptions.useSeparateDirectories = options.useSeparateDirs;

  blockOut->reset(
      new spipe::blocks::SearchStoichiometries(searchOptions, options.pipe));

  return true;
}

bool
BlockFactory::createBlock(BlockHandle * const blockOut,
    blocks::SeparateAtoms & options) const
{
  spl::UniquePtr< spipe::blocks::SeparateAtoms>::Type sep(
      new spipe::blocks::SeparateAtoms());
  if(options.pairDistances)
    sep->setPairDistances(*options.pairDistances);
  blockOut->reset(sep.release());
  return true;
}

bool
BlockFactory::createBlock(BlockHandle * blockOut,
    const blocks::WriteStructures & options) const
{
  spl::UniquePtr< spipe::blocks::WriteStructures>::Type writeStructures(
      new spipe::blocks::WriteStructures());

  writeStructures->setFileType(options.format);
  writeStructures->setWriteMulti(options.multiWrite);

  // Transfer ownership
  *blockOut = writeStructures;
  return true;
}

} // namespace common
} // namespace factory
