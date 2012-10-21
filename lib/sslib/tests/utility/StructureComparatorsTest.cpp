/*
 * DistanceCalculatorsTest.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "sslibtest.h"

#include <boost/filesystem.hpp>

#include <common/Structure.h>
#include <io/ResReaderWriter.h>
#include <utility/DistanceMatrixComparator.h>
#include <utility/SortedDistanceComparator.h>
#include <utility/SortedDistanceComparatorEx.h>

namespace fs = ::boost::filesystem;
//namespace ssbc = ::sstbx::build_cell;
namespace ssc = ::sstbx::common;
namespace ssu = ::sstbx::utility;


BOOST_AUTO_TEST_CASE(StructureComparatorsTest)
{
  // SETTINGS ////////////////
  const fs::path referenceStructuresPath("similarStructures");

  BOOST_REQUIRE(fs::exists(referenceStructuresPath));

  BOOST_REQUIRE(fs::is_directory(referenceStructuresPath));


}
