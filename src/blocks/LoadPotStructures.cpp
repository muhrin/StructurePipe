/*
 * LoadPotStructures.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "blocks/LoadPotStructures.h"

#include <vector>

#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

// From SSTbx
#include <common/Structure.h>
#include <io/AdditionalData.h>
#include <io/ResReaderWriter.h>
#include <potential/IParameterisable.h>

// From PipeLib
#include <IPipeline.h>

// Local includes
#include "common/StructureData.h"
#include "common/UtilityFunctions.h"

// NAMESPACES ////////////////////////////////


namespace spipe { namespace blocks {

LoadPotStructures::LoadPotStructures(
  const sstbx::potential::IParameterisable &        pot,
  const boost::filesystem::path & potPath,
  const bool                      lowestEOnly):
pipelib::Block<StructureDataTyp, SharedDataTyp>("Load potential structures"),
myPot(pot),
myPotPath(potPath),
myLowestEOnly(lowestEOnly)
{}

void LoadPotStructures::start()
{
  using boost::filesystem::ifstream;
  using boost::filesystem::path;
  using boost::tokenizer;
  using std::getline;
  using std::string;

	typedef boost::tokenizer<boost::char_separator<char> > Tok;
	const boost::char_separator<char> sep(" \t");

  ifstream potFile(myPotPath);

  // Storage for loaded structures 
  std::vector<spipe::StructureDataTyp *> structures;

  if(potFile.is_open())
  {
	  // Load structures
    string line;

    size_t totalLoaded = 0; // The number of structures loaded in total
    size_t nLoaded = 0; // The number of structures loaded each time

    for(getline(potFile, line); potFile.good(); getline(potFile, line))
    {
      Tok tok(line, sep);
      Tok::iterator it = tok.begin();
      if(it != tok.end())
      {
        // Find the folder that this is referring to
        path strFolder(*it);

        // Load the structure(s) we're after
        nLoaded = loadStructures(strFolder, structures);
        if(nLoaded > 0)
        {
          std::pair<arma::vec, bool> result = myPot.getParamsFromString(line.substr(it->length()));
          // Was it possible to get the parameter string?
          if(result.second)
          {
            for(size_t i = 0; i < nLoaded; ++i)
            {
              StructureDataTyp * const strDat = structures[i];

              strDat->potentialParams.reset(result.first);
              strDat->group.reset(strFolder.string());

              // Send the structure down the pipe
              myPipeline->registerNewData(strDat);
              myOutput->in(*strDat);
            }
          } // end if(result.second)
        } // end if(nLoaded > 0)
        totalLoaded += nLoaded;
        structures.clear();
      } // end if token != end
    }
  } // end if opened pot file
}

size_t LoadPotStructures::loadStructures(
    const boost::filesystem::path &             strFolder,
    std::vector<spipe::StructureDataTyp *>  &  structureVec) const
{
  using namespace boost::filesystem;

  if(!is_directory(strFolder))
    return 0;

  sstbx::io::ResReaderWriter resReader;

  // Keep track of the lowest energy
  StructureDataTyp * lowest = NULL;
  size_t numLoaded = 0;

  const directory_iterator end = directory_iterator();
  for(directory_iterator dirIt = directory_iterator(strFolder);
    dirIt != end; ++dirIt)
  {
    // Get all .res files
    const path & dirEntry = dirIt->path();
    if(dirEntry.extension() == ".res")
    {
      StructureDataTyp * const strDat = new StructureDataTyp();
      strDat->setStructure(new sstbx::common::Structure());
      sstbx::io::AdditionalData data;

      //std::cout << "Got res file: " << dirEntry.string() << std::endl;

      // Try loading
      resReader.readStructure(*strDat->getStructure(), dirEntry, &data);

      generateStructureDataFromIo(data, *strDat);

      if(myLowestEOnly)
      {
        if(lowest)
        {
          if(lowest->enthalpy && strDat->enthalpy)
          {
            if(*lowest->enthalpy > *strDat->enthalpy)
            {
              delete lowest;
              lowest = strDat;
            }
          }
        }
        else
        {
          lowest = strDat;
          numLoaded = 1;
        }
      }
      else
      {
        structureVec.push_back(strDat);
      }
    }
  }

  if(myLowestEOnly)
  {
    structureVec.push_back(lowest);
  }


  return numLoaded;
}

}}

