/*
 * SchemaScalar.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_SCALAR_VALUE_H
#define SCHEMA_SCALAR_VALUE_H

// INCLUDES /////////////////////////////////////////////
#include <string>

#include <boost/shared_ptr.hpp>

#include "factory/IInputObject.h"
#include "factory/ISchemaElementInstance.h"
#include "factory/SchemaElement.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////


namespace sstbx
{
namespace factory
{

class IInputObject;
class SchemaDoc;

class SchemaScalarValue : public ISchemaElementInstance, public SchemaElement
{
public:

  typedef SchemaElement::ChildPtr ChildPtr;
  typedef SchemaElement::DefaultValuePointer          DefaultValuePointer;
  typedef SchemaElement::DefaultValueConstPointer     DefaultValueConstPointer;

  explicit SchemaScalarValue(const ::std::string & name);

  // From ISchemaElementInstance
  virtual const SchemaElement &                getSchemaElement() const;
  virtual bool                                 isRequired() const;
  virtual int                                  getSize() const;
  virtual bool                                 validate(const IInputObject & obj, const SchemaDoc & doc) const;
  virtual ::boost::shared_ptr<ISchemaElementInstance> clone() const;
  // Default entries
  virtual size_t                               numDefaults() const;
  virtual const DefaultValueConstPointer       getDefault(const size_t idx) const;
  // End from ISchemaElementInstance

  // From SchemaElement
  virtual size_t            numChildren() const;
  virtual const ChildPtr    operator[](const size_t idx) const;
  virtual ChildPtr          newInstance() const;
  // End from SchemaElement

protected:

  typedef ::std::vector<ChildPtr> ChildrenContainer;

  ChildrenContainer   myChildren;

  SchemaScalarValue(const SchemaScalarValue & toCopy);

};

}
}

#endif /* SCHEMA_SCALAR_VALUE_H */
