/*
 * StructureData.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef STRUCTURE_DATA_H
#define STRUCTURE_DATA_H

// INCLUDES /////////////////////////////////////////////

#include <boost/optional.hpp>

#include <armadillo>

// FORWARD DECLARATIONS ////////////////////////////////////
namespace sstbx
{
	namespace common
	{
		class Structure;
	}
}

namespace spipe { namespace common {

class StructureData
{
public:
	StructureData();
	~StructureData();

	sstbx::common::Structure * getStructure() const;
	void setStructure(sstbx::common::Structure * const structure);

	::boost::optional<double>				enthalpy;
	::boost::optional<std::string>		name;
	::boost::optional<std::string>		group;
	::boost::optional<arma::vec>			potentialParams;
	::boost::optional<double>				pressure;
	::boost::optional<std::string>		spaceGroup;
	::boost::optional<arma::Mat<double> > stressMtx;
	::boost::optional<size_t>				timesFound;

private:

	::sstbx::common::Structure * myStructure;
};


}}

#endif /* STRUCTURE_DATA_H */
