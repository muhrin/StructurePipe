/*
 * StructureBuilder.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: Martin Uhrin
 */

#include "build_cell/StructureBuilder.h"

#include <boost/foreach.hpp>

#include "build_cell/AtomsDescription.h"
#include "build_cell/AtomGroupDescription.h"
#include "build_cell/Minsep.h"
#include "build_cell/StructureDescription.h"

#include "common/Atom.h"
#include "common/AtomGroup.h"
#include "common/Structure.h"


namespace sstbx { namespace build_cell {

	StructureBuilder::StructureBuilder(const StructureDescription * structureDescription, ::sstbx::common::Structure * const structure):
myStructureDescription(structureDescription),
myStructure(structure)
{
	// Build the structure from the description
	buildAtomGroup(structureDescription, *myStructure);
}

::sstbx::common::Structure * StructureBuilder::getStructure() const
{
	return myStructure;
}

const AtomsDescription * StructureBuilder::getAtomsDescription(
	const ::sstbx::common::Atom * const atom) const
{
	this->myAtomsMap.find(atom);
	AtomMap::const_iterator it = myAtomsMap.find(atom);

	const AtomsDescription * desc = NULL;
	
	if(it != myAtomsMap.end())
	{
		desc = it->second;
	}

	return desc;
}

const AtomGroupDescription * StructureBuilder::getAtomGroupDescription(
	const ::sstbx::common::AtomGroup * const group) const
{
	AtomGroupMap::const_iterator it = myAtomGroupsMap.find(group);

	const AtomGroupDescription * desc = NULL;
	
	if(it != myAtomGroupsMap.end())
	{
		desc = it->second;
	}

	return desc;
}

void StructureBuilder::buildAtomGroup(
	const AtomGroupDescription * groupDescription,
	::sstbx::common::AtomGroup & group)
{
	using ::std::vector;
	using ::sstbx::common::Atom;
	using ::sstbx::common::AtomGroup;

	// First create the atoms
  const AtomGroupDescription::AtomsContainer & atoms =
    groupDescription->getChildAtoms();

  BOOST_FOREACH(const AtomsDescription & desc, atoms)
	{
		for(size_t i = 0; i < desc.getCount(); ++i)
		{
			Atom * const a = new Atom(desc.getSpecies());
			group.insertAtom(a);
			// Add it to the map
			myAtomsMap[a] = &desc;
		}
	}

	// And now any child groups
  const AtomGroupDescription::GroupsContainer & groups =
    groupDescription->getChildGroups();

  BOOST_FOREACH(const AtomGroupDescription & desc, groups)
  {
		AtomGroup * childGroup = new AtomGroup();
		group.insertGroup(childGroup);
		// Add it to the map
		myAtomGroupsMap[childGroup] =  &desc;
		buildAtomGroup(&desc, *childGroup);
	}
}

}}
