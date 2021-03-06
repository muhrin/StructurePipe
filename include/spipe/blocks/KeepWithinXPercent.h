/*
 * KeepWithinXPercent.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef KEEP_WITHIN_X_PERCENT_H
#define KEEP_WITHIN_X_PERCENT_H

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

class KeepWithinXPercent : public Barrier, boost::noncopyable
{
  typedef spl::utility::Key< double> StructureProperty;

public:
  explicit
  KeepWithinXPercent(const double percent);
  KeepWithinXPercent(const double percent, const StructureProperty & property);
  KeepWithinXPercent(const double percent, const StructureProperty & property,
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
  void
  newLowest(StructureDataType * const structure, const double energy,
      StructureOrder * const order);
  double
  getCutoff(const StructureOrder & order) const;

  const double myKeepPercent;
  const StructureProperty myStructureProperty;
  const bool myUsePerAtom;

  StructuresByComposition myStructures;

#ifdef SPIPE_USE_BOOST_THREAD
  boost::mutex myMutex;
#endif
};

}
}

#endif /* KEEP_WITHIN_X_PERCENT_H */
