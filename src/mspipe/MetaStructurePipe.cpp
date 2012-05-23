/*
 * MetaStructurePipe.cpp
 *
 *  Created on: May 4, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "mspipe/MetaStructurePipe.h"

#include <armadillo>

// From PipeLib
#include <SingleThreadedPipeline.h>

// From SSTbx
#include <build_cell/DefaultCrystalGenerator.h>
#include <build_cell/RandomCellGenerator.h>
#include <common/AtomSpeciesId.h>
#include <io/ResReaderWriter.h>
#include <io/StructureWriterManager.h>
#include <potential/SimplePairPotential.h>
#include <potential/TpsdGeomOptimiser.h>
#include <utility/SortedDistanceComparator.h>
#include <utility/UniqueStructureSet.h>

// Local includes
#include "StructurePipe.h"
#include "blocks/LowestFreeEnergy.h"
#include "blocks/NiggliReduction.h"
#include "blocks/ParamPotentialGo.h"
#include "blocks/RandomStructure.h"
#include "blocks/RemoveDuplicates.h"
#include "blocks/WriteStructure.h"
#include "common/SharedData.h"
#include "common/StructureData.h"
#include "mspipe/MetaStructurePipeData.h"
#include "mspipe/MetaStructurePipeSharedData.h"
#include "mspipe/blocks/ConvexHullGenerator.h"
#include "mspipe/blocks/StoichiometrySearch.h"
#include "mspipe/blocks/RunSPipe.h"


// NAMESPACES ////////////////////////////////


int main(const int argc, const char * const argv[])
{
  using namespace ::spipe;
  using namespace ::spipe::blocks;

  using ::arma::Mat;
  using ::arma::endr;
  using ::pipelib::SingleThreadedPipeline;
  using ::sstbx::build_cell::DefaultCrystalGenerator;
  using ::sstbx::build_cell::RandomCellGenerator;
  using ::sstbx::potential::SimplePairPotential;
  using ::sstbx::potential::TpsdGeomOptimiser;
  using ::sstbx::utility::SortedDistanceComparator;
  using ::sstbx::utility::UniqueStructureSet;

	// Create the structure pipeline
  SingleThreadedPipeline< ::spipe::StructureDataTyp, ::spipe::SharedDataTyp> pipe =
    SingleThreadedPipeline< ::spipe::StructureDataTyp, ::spipe::SharedDataTyp>();

	// Set up random structure block
	RandomCellGenerator<> cellGenerator;
	DefaultCrystalGenerator crystalGenerator(cellGenerator);
	RandomStructure strBlock(100, crystalGenerator);

	// Set up niggli reduction block
	NiggliReduction niggli;
	NiggliReduction niggli2;

	// Set up geometry optimisation block
	Mat<double> epsilon;
	epsilon.set_size(2, 2);
	epsilon << 1 << 2 << endr
			<< 2 << 2 << endr;

	Mat<double> sigma;
	sigma.set_size(2, 2);
	sigma << 2 << 2 << endr
			<< 2 << 2 << endr;

	Mat<double> beta;
	beta.set_size(2, 2);
	beta << -1 << 1 << endr
			<< 1 << -1 << endr;

  SimplePairPotential<> potential(2, epsilon, sigma, 2.5, beta, 12, 6, SimplePairPotential<>::NONE);
	TpsdGeomOptimiser<double> optimiser(potential);
	TpsdGeomOptimiser<double> optimiser2(potential);

	ParamPotentialGo go(potential, optimiser);

	// Set up the structure comparator
	SortedDistanceComparator comp;
	UniqueStructureSet sset(comp);
	RemoveDuplicates delDuplicates(sset);

	// Set up a writer block
	::sstbx::io::ResReaderWriter resRw;
	::sstbx::io::StructureWriterManager writeManager;
	writeManager.registerWriter(resRw);
	WriteStructure writer(writeManager);
	WriteStructure writer2(writeManager);

	// Lowest free energy filter
	LowestFreeEnergy lowest;

	// Set up the pipeline
	pipe.setStartBlock(strBlock);
	pipe.connect(strBlock, niggli);
	pipe.connect(niggli, go);
	//pipe.connect(go, niggli2);
	//pipe.connect(niggli2, delDuplicates);
	pipe.connect(go, delDuplicates);
	pipe.connect(delDuplicates, writer);
	// Put in a barrier
  ::pipelib::DefaultBarrier<StructureDataTyp, SharedDataTyp> barrier;
	pipe.connect(writer, barrier);
	pipe.connect(barrier, writer2);
	pipe.connect(writer2, lowest);

  using namespace ::spipe::mspipe;
  using namespace ::spipe::mspipe::blocks;

  // Now set up the meta-pipe
  SingleThreadedPipeline<MspDataTyp, MspSharedDataTyp> metaPipe =
    SingleThreadedPipeline<MspDataTyp, MspSharedDataTyp>();

  StoichiometrySearch stoichSearch(::sstbx::common::NA, ::sstbx::common::CL, 16, 15.0);

  RunSPipe runSPipe(pipe);

  ConvexHullGenerator cHull;

  metaPipe.setStartBlock(stoichSearch);
  metaPipe.connect(stoichSearch, runSPipe);
  metaPipe.connect(runSPipe, cHull);

  metaPipe.getSharedData().species1 = ::sstbx::common::NA;
  metaPipe.getSharedData().species2 = ::sstbx::common::CL;


  // Start the thing
  metaPipe.initialise();
  metaPipe.start();



	return 0;
}

