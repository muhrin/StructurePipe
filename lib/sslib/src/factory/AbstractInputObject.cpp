/*
 * AbstractInputObject.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "factory/AbstractInputObject.h"

// NAMESPACES ////////////////////////////////

namespace sstbx
{
namespace factory
{

AbstractInputObject::AbstractInputObject(
  const IInputObject::Type        type,
  const ::std::string &           name
):
myType(type),
myName(name)
{}

const ::std::string & AbstractInputObject::getName() const
{
  return myName;
}

IInputObject::Type AbstractInputObject::getType() const
{
  return myType;
}



}
}


