/*
 * ScalarValue.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCALAR_VALUE_H
#define SCALAR_VALUE_H

// INCLUDES /////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "factory/AbstractInputObject.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////


namespace sstbx
{
namespace factory
{

class ScalarValue : public AbstractInputObject
{
public:

  ScalarValue(const ::std::string & name);

  virtual void    insert(::boost::shared_ptr<IInputObject> & /*value*/);
  virtual size_t  getSize() const;
  virtual ::boost::shared_ptr<IInputObject> clone() const;

protected:

  ScalarValue(const ScalarValue & toCopy);
};

}
}

#endif /* SCALAR_VALUE_H */
