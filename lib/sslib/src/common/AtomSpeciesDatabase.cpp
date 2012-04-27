/*
 * AtomSpciesDatabase.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "common/AtomSpeciesDatabase.h"

#include "common/AtomSpeciesId.h"

// NAMESPACES ////////////////////////////////

namespace sstbx { namespace common {

// Declare the singleton instance
AtomSpeciesDatabase * AtomSpeciesDatabase::myInstance = NULL;

AtomSpeciesDatabase::AtomSpeciesDatabase()
{
	setAll(H, "H", "Hydrogen");
	setAll(NA, "Na", "Sodium");
	setAll(CL, "Cl", "Chlorine");
}

AtomSpeciesDatabase::~AtomSpeciesDatabase()
{
	if(myInstance)
		delete myInstance;
}

const ::std::string * AtomSpeciesDatabase::getName(const AtomSpeciesId id) const
{
	SpeciesString::const_iterator it = myNames.find(id);
	if(it == myNames.end())
		return NULL;
	return &it->second;
}

void AtomSpeciesDatabase::setName(const AtomSpeciesId id, const ::std::string & name)
{
	myNames[id] = name;
}

const ::std::string * AtomSpeciesDatabase::getSymbol(const AtomSpeciesId id) const
{
	SpeciesString::const_iterator it = mySymbols.find(id);
	if(it == mySymbols.end())
		return NULL;
	return &it->second;
}

void AtomSpeciesDatabase::setSymbol(const AtomSpeciesId id, const ::std::string & symbol)
{
	mySymbols[id] = symbol;
}

const AtomSpeciesId AtomSpeciesDatabase::getIdFromSymbol(const std::string & symbol) const
{
  AtomSpeciesId id = DUMMY;
  for(SpeciesString::const_iterator it = mySymbols.begin(), end = mySymbols.end();
    it != end; ++it)
  {
    if(it->second == symbol)
    {
      id = it->first;
      break;
    }
  }
  return id;
}

void AtomSpeciesDatabase::setAll(
	const AtomSpeciesId id,
	const ::std::string & symbol,
	const ::std::string & name)
{
	setSymbol(id, symbol);
	setName(id, name);
}

AtomSpeciesDatabase & AtomSpeciesDatabase::inst()
{
	if(!myInstance)
	{
		myInstance = new AtomSpeciesDatabase();
	}
	return *myInstance;
}

}}
