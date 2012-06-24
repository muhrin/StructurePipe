/*
 * SchemaScalarValueAny.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_SCALAR_VALUE_ANY_H
#define SCHEMA_SCALAR_VALUE_ANY_H

// INCLUDES /////////////////////////////////////////////
#include <string>

#include <boost/shared_ptr.hpp>

#include "factory/IInputObject.h"
#include "factory/ISchemaElementInstance.h"
#include "factory/SchemaScalarValue.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////


namespace sstbx
{
namespace factory
{

class IInputObject;
class SchemaDoc;

class SchemaScalarValueAny : public SchemaScalarValue
{
public:

  typedef SchemaElement::ChildPtr ChildPtr;
  typedef SchemaElement::DefaultValuePointer          DefaultValuePointer;
  typedef SchemaElement::DefaultValueConstPointer     DefaultValueConstPointer;

  static ::boost::shared_ptr<SchemaScalarValueAny> create();

  SchemaScalarValueAny();

  // From ISchemaElementInstance
  virtual const SchemaElement &                getSchemaElement() const;
  virtual bool                                 isRequired() const;
  virtual int                                  getSize() const;
  virtual bool                                 validate(const IInputObject & obj, const SchemaDoc & doc) const;
  // Default entries
  virtual size_t                               numDefaults() const;
  virtual const DefaultValueConstPointer       getDefault(const size_t /*idx*/) const;
  // End from ISchemaElementInstance

  // From SchemaElement
  virtual size_t            numChildren() const;
  virtual const ChildPtr    operator[](const size_t /*idx*/) const;
  virtual ChildPtr          newInstance() const;
  // End from SchemaElement

protected:

};

}
}

#endif /* SCHEMA_SCALAR_VALUE_ANY_H */
