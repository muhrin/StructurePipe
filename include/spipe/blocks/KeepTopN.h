/*
 * KeepTopN.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef KEEP_TOP_N_H
#define KEEP_TOP_N_H

// INCLUDES /////////////////////////////////////////////
#include "spipe/StructurePipe.h"

#include <map>

#include <boost/noncopyable.hpp>
#ifdef SPIPE_USE_BOOST_THREAD
#  include <boost/thread/mutex.hpp>
#endif

#include <pipelib/pipelib.h>

#include <spl/common/AtomsFormula.h>
#include <spl/common/StructureProperties.h>

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace blocks {

class KeepTopN : public Barrier, boost::noncopyable
{
  typedef spl::utility::Key< double> StructureProperty;

public:
  explicit
  KeepTopN(const size_t keepTopN);
  KeepTopN(const size_t keepTopN, const StructureProperty & property);
  KeepTopN(const size_t keepTopN, const StructureProperty & property,
      const bool usePerAtom);

  // From Block /////////////////
  virtual void
  in(spl::common::Structure * const data);
  // End from Block /////////////

  // From Barrier /////////////////
  virtual size_t
  release();
  virtual bool
  hasData() const;
  // End from Barrier //////////////

private:
  typedef std::multimap< double, spl::common::Structure *> StructureOrder;
  typedef std::map< spl::common::AtomsFormula, StructureOrder> StructuresByComposition;

  void
  keep(spl::common::Structure * const structure, const double energy);

  const size_t myKeepTopN;
  const spl::utility::Key< double> myStructureProperty;
  const bool myUsePerAtom;

  StructuresByComposition myStructures;

#ifdef SPIPE_USE_BOOST_THREAD
  boost::mutex myMutex;
#endif
};

}
}

#endif /* KEEP_TOP_N_H */
