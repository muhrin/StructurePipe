/*
 * PotentialGo.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "PotentialGo.h"

#include "common/StructureData.h"
#include "common/UtilityFunctions.h"

// From SSTbx
#include <common/Structure.h>
#include <potential/StandardData.h>
#include <potential/IGeomOptimiser.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

#include <locale>

// NAMESPACES ////////////////////////////////


namespace spipe { namespace blocks {

PotentialGo::PotentialGo(
	sstbx::potential::IGeomOptimiser & optimiser):
Block("Potential geometry optimisation"),
myOptimiser(optimiser)
{
}

void PotentialGo::in(spipe::common::StructureData & data)
{
	sstbx::potential::StandardData<> * optData = NULL;
	myOptimiser.optimise(*data.getStructure(), optData);

	// Copy over information from the optimisation results
	data.enthalpy.reset(optData->totalEnthalpy);
	data.stressMtx.reset(optData->stressMtx);

	delete optData;
	optData = NULL;

	myOutput->in(data);
}

}}