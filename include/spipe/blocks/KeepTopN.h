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
#ifdef SPIPE_USE_THREAD
#  include <boost/thread/mutex.hpp>
#endif

#include <pipelib/pipelib.h>

#include <spl/common/StructureProperties.h>

#include "spipe/SpTypes.h"

// FORWARD DECLARATIONS ////////////////////////////////////

namespace spipe {
namespace blocks {

class KeepTopN : public Barrier, ::boost::noncopyable
{
  typedef ::spl::utility::Key<double> StructureProperty;

public:
  explicit KeepTopN(const size_t keepTopN);
  KeepTopN(const size_t keepTopN, const StructureProperty & proeprty);
  KeepTopN(const size_t keepTopN, const StructureProperty & proeprty, const bool usePerAtom);


  // From Block /////////////////
  virtual void
  in(common::StructureData * const data);
  // End from Block /////////////

  // From Barrier /////////////////
  virtual size_t
  release();
  virtual bool
  hasData() const;
  // End from Barrier //////////////

private:
  typedef ::std::map< double, StructureDataType *> Structures;

  void
  keep(StructureDataType * const structure, const double energy);

  const size_t myKeepTopN;
  const ::spl::utility::Key<double> myStructureProperty;
  const bool myUsePerAtom;

  Structures myStructures;

#ifdef SPIPE_USE_THREAD
  ::boost::mutex myMutex;
#endif
};

}
}

#endif /* KEEP_TOP_N_H */
