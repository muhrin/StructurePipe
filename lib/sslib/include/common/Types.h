/*
 * Types.h
 *
 *
 *  Created on: Aug 17, 2011
 *      Author: Martin Uhrin
 */

#ifndef TYPES_H
#define TYPES_H

// INCLUDES /////////////////////////////////////////////

// DEFINES ////////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace sstbx
{
namespace common
{

class Atom;

typedef ::boost::shared_ptr<Atom> AtomPtr;
typedef ::boost::shared_ptr<const Atom> AtomConstPtr;

}
}

#endif /* TYPES_H */
