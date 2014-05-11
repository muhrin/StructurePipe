/*
 * SearchStoichiometries.h
 *
 *
 *  Created on: May 4, 2012
 *      Author: Martin Uhrin
 */

#ifndef SEARCH_STOICHIOMETRIES_H
#define SEARCH_STOICHIOMETRIES_H

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <vector>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <pipelib/pipelib.h>

#include <spl/common/AtomSpeciesId.h>
#include <spl/io/BoostFilesystem.h>
#include <spl/utility/MultiIdxRange.h>
#include <spl/utility/Range.h>

// Local includes
#include "spipe/SpTypes.h"
#include "spipe/utility/DataTable.h"
#include "spipe/utility/DataTableSupport.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace common {
class DataTableWriter;
}

namespace blocks {

class SearchStoichiometries : public StartBlock,
    public FinishedSink,
    boost::noncopyable
{
public:
  typedef spl::utility::Range< int> CountRange;
  typedef std::map< std::string, CountRange> AtomRanges;

  struct Options
  {
    static const size_t UNLIMITED_ATOMS;
    Options() :
        atomRanges(), maxAtoms(UNLIMITED_ATOMS), useSeparateDirectories(false)
    {
    }
    AtomRanges atomRanges;
    size_t maxAtoms;
    bool useSeparateDirectories;
  };

  SearchStoichiometries(const Options & options, BlockHandle & subpipe);

  // From Block ////////
  virtual void
  pipelineInitialising();
  // End from Block ////

  // From StartBlock ///
  virtual void
  start();
  // End from StartBlock ///

  // From IDataSink /////////////////////////////
  virtual void
  finished(StructureDataUniquePtr data);
  // End from IDataSink /////////////////////////

private:
  typedef spipe::StructureDataType StructureDataTyp;
  typedef boost::scoped_ptr< spipe::utility::DataTableWriter> TableWriterPtr;

  // From Block ////////
  virtual void
  engineAttached(Engine::SetupType * const setup);
  virtual void
  engineDetached();
  // End from Block ////

  spl::utility::MultiIdxRange< unsigned int>
  getStoichRange();

  void
  releaseBufferedStructures(const utility::DataTable::Key & key);

  void
  updateTable(const utility::DataTable::Key & key,
      const spl::utility::MultiIdx< unsigned int> & currentIdx);

  const Options myOptions;

  BlockHandle mySubpipe;
  Engine * mySubpipeEngine;

  // Use this to write out our table data
  spipe::utility::DataTableSupport myTableSupport;
  boost::filesystem::path myOutputPath;

  /** Buffer to store structure that have finished their path through the sub pipeline. */
  std::vector< StructureDataTyp *> myBuffer;
};

}
}

#endif /* SEARCH_STOICHIOMETRIES_H */
