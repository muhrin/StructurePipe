/*
 * ShrinkTest.cpp
 *
 *  Created on: May 23, 2014
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "spipetest.h"

#include <spl/common/Structure.h>
#include <spl/potential/LennardJones.h>

#include <spipe/blocks/Shrink.h>

using namespace spipe;

BOOST_AUTO_TEST_SUITE(ShrinkTest)

BOOST_AUTO_TEST_CASE(BasicShrinkTest)
{
  typedef spl::potential::LennardJones LennardJones;
  typedef spl::UniquePtr< LennardJones>::Type LjPtr;

  // SETTINGS //

  LennardJones lj;
  lj.addInteraction(spl::SpeciesPair("A"), 1, 1, 12, 6, 2.5);

  spl::common::Structure structure;
  structure.setUnitCell(spl::common::UnitCell(10, 10, 10, 90, 90, 90));
  structure.newAtom("A");
  structure.newAtom("A").setPosition(5.0, 0.0, 0.0);

  blocks::Shrink shrink(
      spl::UniquePtr< spl::potential::IPotential>::Type(
          LjPtr(new LennardJones(lj))), boost::optional< std::string>());

  LennardJones::DataType ljData(2);
  lj.evaluate(structure, ljData);
  BOOST_REQUIRE_EQUAL(ljData.internalEnergy, 0.0);

  shrink.in(&structure);
  // Check that the potential is now non-zero
  BOOST_CHECK_LT(ljData.internalEnergy, 0.0);
}

BOOST_AUTO_TEST_SUITE_END()
