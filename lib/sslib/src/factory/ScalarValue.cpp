/*
 * ScalarValue.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "factory/ScalarValue.h"

#include "factory/IInputObject.h"

// NAMESPACES ////////////////////////////////

namespace sstbx
{
namespace factory
{

ScalarValue::ScalarValue(const ScalarValue & toCopy):
AbstractInputObject(IInputObject::SCALAR, toCopy.getName())
{}

ScalarValue::ScalarValue(const ::std::string & name):
AbstractInputObject(IInputObject::SCALAR_VALUE, name)
{}

void ScalarValue::insert(::boost::shared_ptr<IInputObject> & /*value*/)
{
  // TODO: Throw exception
}

size_t ScalarValue::getSize() const
{
  // A scalar value cannot contain any input objects therefore its size is always 0
  return 0;
}

::boost::shared_ptr<IInputObject> ScalarValue::clone() const
{
  return ::boost::shared_ptr<IInputObject>(new ScalarValue(*this));
}

}
}


