/*
 * SharedData.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef SHARED_DATA_H
#define SHARED_DATA_H

// INCLUDES /////////////////////////////////////////////

#include <boost/optional.hpp>

#include <armadillo>

// FORWARD DECLARATIONS ////////////////////////////////////
namespace sstbx
{
  namespace build_cell
  {
    template <typename FloatType>
    class RandomCellDescription;
    class StructureDescription;
  }
}

namespace spipe
{
namespace common
{

class SharedData
{
public:

  SharedData():
      structureDescription(NULL),
      cellDescription(NULL){}
  //~SharedData();

	/** The current parameterised potential parameters */
	::boost::optional< ::arma::vec>			potentialParams;

	/** Potential sweep starting values */
	::boost::optional< ::arma::vec>			potSweepFrom;
	/** Potential sweep deltas */
	::boost::optional< ::arma::vec>			potSweepStep;
	/** Potential sweep number of steps to make */
	::boost::optional< ::arma::Col<unsigned int> >	potSweepNSteps;

  const sstbx::build_cell::StructureDescription * structureDescription;

  const sstbx::build_cell::RandomCellDescription<double> * cellDescription;

};


}
}

#endif /* SHARED_DATA_H */
