/*
 * InputObject.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "factory/Scalar.h"

#include "factory/IInputObject.h"

// NAMESPACES ////////////////////////////////

namespace sstbx
{
namespace factory
{

Scalar::Scalar(const Scalar & toCopy):
AbstractInputObject(IInputObject::SCALAR, toCopy.getName())
{
  // TODO: Insert scalar value from toCopy
}

Scalar::Scalar(const ::std::string & name):
AbstractInputObject(IInputObject::SCALAR, name)
{}

size_t Scalar::getSize() const
{
  return 1;
}

void Scalar::insert(::boost::shared_ptr<IInputObject> & value)
{
  if(value->getType() != IInputObject::SCALAR_VALUE)
  {
    // TODO: Throw exception
    return;
  }
  myValue = value;
}

const IInputObject * Scalar::operator [](const size_t idx) const
{
  return myValue.get();
}

::boost::shared_ptr<IInputObject> Scalar::clone() const
{
  return ::boost::shared_ptr<IInputObject>(new Scalar(*this));
}

}
}


