/*
 * SchemaScalar.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "factory/SchemaScalar.h"

#include <boost/foreach.hpp>

// NAMESPACES ////////////////////////////////

namespace sstbx
{
namespace factory
{


// INSTANCE IMPLEMENTATION ///////////////////

void SchemaScalar::Instance::setRequired(const bool required)
{
  myRequired = required;
}

void SchemaScalar::Instance::setDefault(const DefaultValueConstPointer & defaultVal)
{
  // TODO: Validate default being set somehow...
  myDefault = defaultVal;
}

const SchemaScalar & SchemaScalar::Instance::getSchemaElement() const
{
  return myElement;
}

bool SchemaScalar::Instance::isRequired() const
{
  return myRequired;
}

int SchemaScalar::Instance::getSize() const
{
  // Scalar is always size 1
  return 1;
}

bool SchemaScalar::Instance::validate(const sstbx::factory::IInputObject &obj, const sstbx::factory::SchemaDoc &doc) const
{
  if(obj.getType() != IInputObject::SCALAR &&
    obj.getType() != IInputObject::UNKNOWN)
  {
    return false;
  }

  if(obj.getSize() != 1)
  {
    return false;
  }

  const IInputObject * const child = obj[0];
  
  bool valid = false;
  BOOST_FOREACH(const ::boost::shared_ptr<ISchemaElementInstance> & childObj, myElement.getChildren())
  {
    if(childObj->validate(obj, doc))
    {
      valid = true;
      break;
    }
  }

  return valid;
}

size_t SchemaScalar::Instance::numDefaults() const
{
  return myDefault.get() ? 1 : 0;
}

const SchemaScalar::DefaultValueConstPointer
SchemaScalar::Instance::getDefault(const size_t idx) const
{
  if(idx == 0)
    return myDefault;

  return DefaultValueConstPointer();
}

SchemaScalar::Instance::Instance(
  const SchemaScalar & element,
  const bool required,
  const DefaultValueConstPointer & _default):
myElement(element),
myRequired(required),
myDefault(_default)
{}

SchemaScalar::Instance::Instance(const Instance & toCopy):
myElement(toCopy.myElement),
myRequired(toCopy.isRequired()),
myDefault(toCopy.myDefault)
{

}

// END INSTANCE IMPLEMENTATION //////////////////
  
SchemaScalar::SchemaScalar(
  const ::std::string &                   name,
  const bool                              required,
  const SchemaObjectUid &                 uid,
  const DefaultValueConstPointer &        _default
):
SchemaElement(IInputObject::SCALAR, name, uid),
myRequired(required),
myDefault(_default)
{}

size_t SchemaScalar::numChildren() const
{
  return myChildren.size();
}

const ISchemaElementInstance::SharedPtrTyp SchemaScalar::operator[](const size_t idx) const
{
  ISchemaElementInstance::SharedPtrTyp child;

  if(idx < numChildren())
  {
    child = myChildren[idx];
  }

  return child;
}


::boost::shared_ptr<ISchemaElementInstance> SchemaScalar::newInstance() const
{
  // TODO: Set required value from element local value
  return ::boost::shared_ptr<ISchemaElementInstance>(new Instance(*this, isRequired(), myDefault));
}

void SchemaScalar::insert(const ChildValuePtr & scalarValue)
{
  // TODO: Check if we already have this value!
  myChildren.push_back(scalarValue);
}

const SchemaScalar::ChildrenContainer & SchemaScalar::getChildren() const
{
  return myChildren;
}

bool SchemaScalar::isRequired() const
{
  return myRequired;
}

const SchemaScalar::DefaultValueConstPointer SchemaScalar::getDefault() const
{
  return myDefault;
}

}
}


