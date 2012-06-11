/*
 * AbstractInputObject.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef ABSTRACT_INPUT_OBJECT_H
#define ABSTRACT_INPUT_OBJECT_H

// INCLUDES /////////////////////////////////////////////
#include <string>
#include <memory>

#include <boost/variant.hpp>

#include "factory/IInputObject.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////


namespace sstbx
{
namespace factory
{

class AbstractInputObject : public IInputObject
{
public:

  virtual const ::std::string &   getName() const;
  virtual Type                    getType() const;

protected:

  AbstractInputObject(
    const Type             type,
    const ::std::string &  name
  );

private:

  const Type                     myType;
  const ::std::string            myName;

};

}
}

#endif /* ABSTRACT_INPUT_OBJECT_H */
