/*
 * IInputObject.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef I_INPUT_OBJECT_H
#define I_INPUT_OBJECT_H

// INCLUDES /////////////////////////////////////////////
#include <string>

#include <boost/shared_ptr.hpp>

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////


namespace sstbx
{
namespace factory
{

class IInputObject
{
public:

  typedef ::boost::shared_ptr<IInputObject>       SharedPtrTyp;
  typedef ::boost::shared_ptr<const IInputObject> SharedPtrConstTyp;

  enum Type
  {
    LIST,
    MAP,
    SCALAR,
    SCALAR_VALUE,
    UNKNOWN
  };

  virtual ~IInputObject() {}

  virtual const ::std::string &   getName() const = 0;
  virtual Type                    getType() const = 0;
  virtual size_t                  getSize() const = 0;
  virtual void                    insert(::boost::shared_ptr<IInputObject> & value) = 0;
  virtual ::boost::shared_ptr<IInputObject> clone() const = 0;

  virtual const IInputObject *    operator [](const size_t idx) const;
};

}
}

#endif /* I_INPUT_OBJECT_H */
