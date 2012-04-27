/*
 * Minsep.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: Martin Uhrin
 */

#include "build_cell/Minsep.h"

#include "build_cell/ConstraintDescriptionId.h"

namespace sstbx { namespace build_cell {

Minsep::Minsep(const double minsep, const bool inherits):
myMinsep(minsep),
AtomConstraintDescription(MINSEP, inherits)
{}

double Minsep::getMinsep() const
{
	return myMinsep;
}


}}
