/*
 * SchemaScalarValue.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "factory/SchemaScalarValue.h"

#include "factory/IInputObject.h"

// NAMESPACES ////////////////////////////////

namespace sstbx
{
namespace factory
{

SchemaScalarValue::SchemaScalarValue(const std::string &name):
SchemaElement(IInputObject::SCALAR_VALUE, name)
{}

SchemaScalarValue::SchemaScalarValue(const SchemaScalarValue & toCopy):
SchemaElement(IInputObject::SCALAR_VALUE, toCopy.getName())
{}

const SchemaElement & SchemaScalarValue::getSchemaElement() const
{
  // Scalar values are their own elements
  return *this;
}

bool SchemaScalarValue::isRequired() const
{
  return false;
}

int SchemaScalarValue::getSize() const
{
  return 0;
}

bool SchemaScalarValue::validate(const IInputObject & obj, const SchemaDoc & doc) const
{
  if(obj.getType() != IInputObject::SCALAR_VALUE &&
    obj.getType() != IInputObject::UNKNOWN)
  {
    return false;
  }

  if(obj.getSize() != 0)
  {
    return false;
  }

  if(obj.getName() != getName())
  {
    return false;
  }

  return true;
}

::boost::shared_ptr<ISchemaElementInstance> SchemaScalarValue::clone() const
{
  return ::boost::shared_ptr<ISchemaElementInstance>(new SchemaScalarValue(*this));
}

size_t SchemaScalarValue::numDefaults() const
{
  return 0;
}

const IInputObject::SharedPtrConstTyp SchemaScalarValue::getDefault(const size_t idx) const
{
  return IInputObject::SharedPtrConstTyp();
}

size_t SchemaScalarValue::numChildren() const
{
  return 0;
}

const SchemaScalarValue::ChildPtr SchemaScalarValue::operator[](const size_t idx) const
{
  ISchemaElementInstance::SharedPtrTyp child;

  if(idx < numChildren())
  {
    child = myChildren[idx];
  }

  return child;
}

::boost::shared_ptr<ISchemaElementInstance> SchemaScalarValue::newInstance() const
{
  return ::boost::shared_ptr<ISchemaElementInstance>(new SchemaScalarValue(*this));
}

}
}


