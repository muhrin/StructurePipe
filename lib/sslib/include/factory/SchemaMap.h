/*
 * SchemaMap.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_MAP_H
#define SCHEMA_MAP_H

// INCLUDES /////////////////////////////////////////////
#include <vector>

#include "factory/IInputObject.h"
#include "factory/ISchemaElementInstance.h"
#include "factory/SchemaElement.h"
#include "factory/SchemaObjectUid.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace sstbx
{
namespace factory
{

class IInputObject;
class SchemaDoc;

class SchemaMap : public SchemaElement
{
public:

  typedef SchemaElement::ChildPtr ChildPtr;

  class Instance : public ISchemaElementInstance
  {
  public:

    // From ISchemaElementInstance ////////////////////
    virtual const SchemaMap &                       getSchemaElement() const;
    virtual bool                                    isRequired() const;
    virtual int                                     getSize() const;
    virtual bool                                    validate(const IInputObject & obj, const SchemaDoc & doc) const;
    virtual ISchemaElementInstance::SharedPtrTyp    clone() const;
    virtual size_t                                  numDefaults() const;
    virtual const IInputObject::SharedPtrConstTyp   getDefault(const size_t idx) const;
    // End from ISchemaElementInstance /////////////////

  private:

    typedef ::std::vector<IInputObject::SharedPtrConstTyp> DefaultsContainer;

    Instance(const SchemaMap & element, const bool required);
    Instance(const Instance & toCopy);

    const SchemaMap &     myElement;
    bool                  myRequired;
    DefaultsContainer     myDefaults;

    friend class SchemaMap;
  };

  SchemaMap(
    const ::std::string &     name,
    const bool                required     = true,
    const SchemaObjectUid &   uid          = SchemaObjectUid()
  );

  // From SchemaElement ////////////////////
  virtual size_t                numChildren() const;
  virtual const ChildPtr        operator[](const size_t idx) const;
  virtual ChildPtr              newInstance() const;
  // End from SchemaElement ////////////////

  void insert(const ChildPtr & child);
  const ISchemaElementInstance::SharedPtrTyp operator [](const ::std::string & name) const;

protected:

  typedef ::std::vector<ISchemaElementInstance::SharedPtrTyp>       ChildrenContainer;

  const ChildrenContainer & getChildren() const;
  bool                      isRequired() const;

  const bool                myRequired;
  ChildrenContainer         myChildren;

};

}
}

#endif /* SCHEMA_MAP_H */
