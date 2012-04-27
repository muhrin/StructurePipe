/*
 * ISructureReader.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef I_STRUCTURE_READER_H
#define I_STRUCTURE_READER_H

// INCLUDES /////////////////////////////////////////////

#include <boost/filesystem.hpp>

// FORWARD DECLARATIONS ////////////////////////////////////
namespace sstbx
{
	namespace common
	{
		class AtomSpeciesDatabase;
		class Structure;
	}
	namespace io
	{
		struct AdditionalData;
	}
}

namespace sstbx { namespace io {

class IStructureReader
{
public:

	virtual ~IStructureReader() {}

	/**
	/* Read a structure from disk.  Additional data may be read if avilable
	/* and will be written out by any readers that support it.
	/* WARNING: This method may rely on an atom species database in which case the
	/* default database will be used which is NOT thread safe.
	/**/
	virtual void readStructure(
		sstbx::common::Structure &      str,
		const boost::filesystem::path & path,
		AdditionalData * const          data) const = 0;

	/**
	/* Read a structure from disk.  Additional data may be read if avilable
	/* and will be written out by any writers that support it.
	/* The user can supply their own species database, however it is up to them
	/* to make sure that the implementation is thread safe if necessary.
	/**/
	virtual void readStructure(
		sstbx::common::Structure &            str,
		const ::boost::filesystem::path &     filepath,
		const ::sstbx::common::AtomSpeciesDatabase & speciesDb,
		AdditionalData * const                data = NULL) const = 0;

	virtual ::std::vector<std::string> getSupportedFileExtensions() const = 0;
};

}}

#endif /* I_STRUCTURE_READER_H */
