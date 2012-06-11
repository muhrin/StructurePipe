/*
 * SchemaDoc.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "factory/SchemaDoc.h"
#include "factory/SchemaMap.h"

#include "factory/SchemaObjectUid.h"

// NAMESPACES ////////////////////////////////

namespace sstbx
{
namespace factory
{

SchemaDoc::SchemaDoc()
{
  //::boost::shared_ptr<SchemaMap
}

SchemaDoc::SchemaDoc(ElementInstPointer & root):
myRoot(root)
{

}

void SchemaDoc::registerElement(const SchemaElement::SharedPtrTyp & obj)
{
  myElements.insert(ElementsContainer::value_type(obj->getUid(), obj));
}


}
}


