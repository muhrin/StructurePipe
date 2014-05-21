/*
 * KeepTopNTest.cpp
 *
 *  Created on: May 21, 2014
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipetest.h"

#include <spl/common/AtomsFormula.h>
#include <spl/common/Structure.h>

#include <spipe/spipe.h>

using namespace spipe;

namespace splbc = spl::build_cell;
namespace splc = spl::common;
namespace properties = splc::structure_properties;

BOOST_AUTO_TEST_SUITE(KeepTopNTest)

class SeedBlock : public StartBlock
{
public:
  SeedBlock() :
      Block("Seed block")
  {
  }
  virtual void
  start()
  {
    // Create 100 structure with one atom type having increasing energy
    for(int i = 0; i < 100; ++i)
    {
      common::StructureData * const structureData = getEngine()->createData();
      splc::Structure & structure = structureData->setStructure(
          spl::UniquePtr< splc::Structure>::Type(new splc::Structure()));

      structure.newAtom("A");
      structure.setProperty(properties::general::ENTHALPY,
          static_cast< double>(i));

      out(structureData);
    }
    // Create 100 structures with two atom types having increasing energy
    for(int i = -100; i < 0; ++i)
    {
      common::StructureData * const structureData = getEngine()->createData();
      splc::Structure & structure = structureData->setStructure(
          spl::UniquePtr< splc::Structure>::Type(new splc::Structure()));

      structure.newAtom("A");
      structure.newAtom("B");
      structure.setProperty(properties::general::ENTHALPY,
          static_cast< double>(i));

      out(structureData);
    }
  }
};

class KeepChecker : public FinishedSink, public DroppedSink
{
  typedef std::map< splc::AtomsFormula, int> CompositionCount;
  typedef spipe::StructureDataUniquePtr StructureDataPtr;
public:
  virtual
  void
  dropped(StructureDataPtr data)
  {
    splc::AtomsFormula composition = data->getStructure()->getComposition();
    composition.reduce();
    std::map< splc::AtomsFormula, int>::iterator it = myNumDropped.find(
        composition);
    if(it == myNumDropped.end())
      myNumDropped[composition] = 1;
    else
      ++it->second;
  }

  virtual
  void
  finished(StructureDataPtr data)
  {
    splc::AtomsFormula composition = data->getStructure()->getComposition();
    composition.reduce();
    std::map< splc::AtomsFormula, int>::iterator it = myNumFinished.find(
        composition);
    if(it == myNumFinished.end())
      myNumFinished[composition] = 1;
    else
      ++it->second;
  }
  void
  check()
  {
     BOOST_FOREACH(CompositionCount::const_reference c, myNumDropped)
         BOOST_CHECK_EQUAL(c.second, 90);
     BOOST_FOREACH(CompositionCount::const_reference c, myNumFinished)
         BOOST_CHECK_EQUAL(c.second, 10);
  }
private:
  CompositionCount myNumFinished;
  CompositionCount myNumDropped;
};

BOOST_AUTO_TEST_CASE(KeepTopNBlock)
{
  typedef SerialEngine Engine;

  BlockHandle seed(new SeedBlock());
  BlockHandle keepTopN(new blocks::KeepTopN(10));
  seed->connect(keepTopN);

  KeepChecker checker;

  Engine engine;
  engine.setFinishedDataSink(&checker);
  engine.run(seed);

  checker.check();
}

BOOST_AUTO_TEST_SUITE_END()
