/*
 * Scalar.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCALAR_H
#define SCALAR_H

// INCLUDES /////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "factory/AbstractInputObject.h"
#include "factory/ScalarValue.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////


namespace sstbx
{
namespace factory
{

class ScalarValue;

class Scalar : public AbstractInputObject
{
public:

  explicit Scalar(const ::std::string & name);

  virtual size_t                  getSize() const;
  virtual void                    insert(::boost::shared_ptr<IInputObject> & value);
  virtual const IInputObject *    operator [](const size_t idx) const;

  virtual ::boost::shared_ptr<IInputObject> clone() const;

private:

  // Copy constructor
  Scalar(const Scalar & toCopy);

  ::boost::shared_ptr<IInputObject>     myValue;

};

}
}

#endif /* SCALAR_H */
