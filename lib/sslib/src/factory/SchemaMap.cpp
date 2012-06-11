/*
 * SchemaMap.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "factory/SchemaMap.h"

#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/shared_ptr.hpp>

#include "factory/SchemaDoc.h"

// NAMESPACES ////////////////////////////////

namespace sstbx
{
namespace factory
{

// Instance implementation ///////////////////

const SchemaMap & SchemaMap::Instance::getSchemaElement() const
{
  return myElement;
}

int SchemaMap::Instance::getSize() const
{
  return ISchemaElementInstance::SIZE_UNDEFINED;
}

bool SchemaMap::Instance::validate(const sstbx::factory::IInputObject &obj, const sstbx::factory::SchemaDoc &doc) const
{
  // TODO: WRITE!
  if(obj.getType() != IInputObject::MAP &&
    obj.getType() != IInputObject::UNKNOWN)
  {
    return false;
  }

  const IInputObject * childObj = NULL;
  bool allValid = true;
  for(size_t i = 0; i < obj.getSize() && allValid; ++i)
  {
    // First check for a name match
    childObj = obj[i];
    const ISchemaElementInstance::SharedPtrTyp childElInst = myElement[obj.getName()];
    
    if(childElInst.get())
    {
      // Now validate against that element
      allValid &= childElInst->validate(*childObj, doc); 
    }
    else
    {
      allValid = false;
      break;
    }
  }

  return allValid;
}

ISchemaElementInstance::SharedPtrTyp SchemaMap::Instance::clone() const
{
  return SharedPtrTyp(new Instance(*this));
}

size_t SchemaMap::Instance::numDefaults() const
{
  return myDefaults.size();
}

const IInputObject::SharedPtrConstTyp SchemaMap::Instance::getDefault(const size_t idx) const
{
  IInputObject::SharedPtrConstTyp ptr;

  if(idx < myDefaults.size())
  {
    ptr = myDefaults[idx];
  }

  return ptr;
}


SchemaMap::Instance::Instance(const SchemaMap & element, const bool required):
myElement(element),
myRequired(required)
{}

SchemaMap::Instance::Instance(const Instance & toCopy):
myElement(toCopy.getSchemaElement()),
myRequired(toCopy.isRequired())
{
  // TOOD: Implement copying of default values!!
}

// End Instance implementation ///////////////

SchemaMap::SchemaMap(
    const ::std::string &     name,
    const bool                required,
    const SchemaObjectUid &   uid
):
SchemaElement(IInputObject::LIST, name, uid),
myRequired(required)
{
}

size_t SchemaMap::numChildren() const
{
  return myChildren.size();
}

const SchemaElement::ChildPtr SchemaMap::operator[](const size_t idx) const
{
  ChildPtr ptr;

  if(idx < numChildren())
  {
    ptr = myChildren[idx];
  }

  return ptr;
}

ISchemaElementInstance::SharedPtrTyp SchemaMap::newInstance() const
{
  return ISchemaElementInstance::SharedPtrTyp(new Instance(*this, isRequired()));
}

void SchemaMap::insert(const SchemaMap::ChildPtr & child)
{
  // TODO: Check for entries with the same name!

  myChildren.push_back(child);
}

const ISchemaElementInstance::SharedPtrTyp SchemaMap::operator [](const ::std::string & name) const
{
  ISchemaElementInstance::SharedPtrTyp elementInst;

  bool found = false;
  BOOST_FOREACH(elementInst, myChildren)
  {
    if(elementInst->getSchemaElement().getName() == name)
    {
      found = true;
      break;
    }
  }

  return found ? elementInst : ISchemaElementInstance::SharedPtrTyp();
}

bool SchemaMap::isRequired() const
{
  return myRequired;
}

const SchemaMap::ChildrenContainer & SchemaMap::getChildren() const
{
  return myChildren;
}

}
}


