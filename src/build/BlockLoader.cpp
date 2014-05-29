/*
 * BlockLoader.cpp
 *
 *  Created on: Dec 13, 2013
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipe/build/BlockLoader.h"

#include <sstream>

#include <boost/foreach.hpp>

#include <spl/SSLibConfig.h>

#include "spipe/factory/YamlSchema.h"

// NAMESPACES ////////////////////////////////

namespace spipe {
namespace build {

BlockLoader::BlockLoader()
{
  namespace blocks = spipe::factory::blocks;

  // TODO: Move this element
  registerFactoryBlockCreator< blocks::BuildStructuresSchema>(
      "generateStructures");
  registerFactoryBlockCreator< blocks::CloneSchema>("clone");
  registerFactoryBlockCreator< blocks::CutAndPasteSchema>("cutAndPaste");
  registerFactoryBlockCreator< blocks::FindSymmetryGroupSchema>("findSymmetry");
  registerFactoryBlockCreator< blocks::GeomOptimiseSchema>("geomOptimise");
#ifdef SPL_WITH_CGAL
  registerFactoryBlockCreator< blocks::KeepStableCompositionsSchema>(
      "keepStableCompositions");
#endif
  registerFactoryBlockCreator< blocks::KeepTopNSchema>("keepTopN");
  registerFactoryBlockCreator< blocks::KeepWithinXPercentSchema>(
      "keepWithinXPercent");
  registerFactoryBlockCreator< blocks::LoadStructures>("loadStructures");
  registerFactoryBlockCreator< blocks::PasteFragmentSchema>("pasteFragment");
  registerFactoryBlockCreator< blocks::NiggliReduceSchema>("niggliReduce");
  registerFactoryBlockCreator< blocks::RemoveDuplicatesSchema>(
      "removeDuplicates");
  registerFactoryBlockCreator< blocks::RescaleSchema>("rescale");
  registerFactoryBlockCreator< blocks::RunPotentialParamsQueueSchema>(
      "runPotentialParamsQueue");
  registerFactoryBlockCreator< blocks::SearchStoichiometriesSchema>(
      "searchStoichiometries");
  registerFactoryBlockCreator< blocks::SeparateAtomsType>("separateAtoms");
  registerFactoryBlockCreator< blocks::ShrinkSchema>("shrink");
  registerFactoryBlockCreator< blocks::WriteStructuresSchema>(
      "writeStructures");
}

BlockHandle
BlockLoader::get(const ::std::string & name)
{
  const BlocksMap::const_iterator it = myBlocks.find(name);
  if(it == myBlocks.end())
    return BlockHandle();

  return it->second;
}

size_t
BlockLoader::load(const YAML::Node & node, std::ostream * const errStream)
{
  using YAML::Node;

  // Format for map entries that are blocks is:
  //
  // blockType [name]: [block info]
  //
  // if [name] is not present then the name is assumed to be
  // whatever blockType is

  if(!node.IsMap())
  {
    if(errStream)
      *errStream << "Error, YAML node supplied to load blocks is not a map.\n";
    return 0;
  }

  size_t numLoaded = 0;
  BlocksDirectory::const_iterator creator;
  std::string temp;
  for(Node::const_iterator it = node.begin(), end = node.end(); it != end; ++it)
  {
    // Find out what type of block to create
    std::stringstream ss(it->first.Scalar());
    ss >> temp;
    creator = myBlocksDirectory.find(temp);
    if(creator != myBlocksDirectory.end())
    {
      // Find out the name (if present)
      if(ss.good())
        ss >> temp;

      // Try to create the block
      BlockHandle handle = creator->second->createBlock(it->second, errStream);
      if(handle)
      {
        insert(temp, handle);
        ++numLoaded;
      }
    }
  }
  return numLoaded;
}

void
BlockLoader::insert(const std::string & name, BlockHandle block)
{
  myBlocks[name] = block;
}

void
BlockLoader::registerCreator(const std::string & blockName, CreatorPtr creator)
{
  myBlocksDirectory.insert(blockName, creator);
}

}
}
