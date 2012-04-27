/*
 * AbstractConstraintDescription.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: Martin Uhrin
 */

// INCLUEDES /////////////
#include "build_cell/AbstractConstraintDescription.h"

#include "build_cell/ConstraintDescriptionId.h"


namespace sstbx { namespace build_cell {

AbstractConstraintDescription::AbstractConstraintDescription(
	const ConstraintDescriptionId type,
	const bool inherits):
myType(type),
myInherits(inherits)
{
}

bool AbstractConstraintDescription::inherits() const
{
	return myInherits;
}

ConstraintDescriptionId AbstractConstraintDescription::getType() const
{
	return myType;
}

}}
