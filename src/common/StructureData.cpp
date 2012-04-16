/*
 * StructureData.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "StructureData.h"

#include "common/Structure.h"


// NAMESPACES ////////////////////////////////

namespace spipe { namespace common {

StructureData::StructureData():
myStructure(NULL)
{}

StructureData::~StructureData()
{
	if(myStructure)
		delete myStructure;
}

sstbx::common::Structure * StructureData::getStructure() const
{
	return myStructure;
}

void StructureData::setStructure(sstbx::common::Structure * const structure)
{
	myStructure = structure;
}

}}