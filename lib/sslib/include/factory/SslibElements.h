/*
 * SslibElements.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SSLIB_ELEMENTS_H
#define SSLIB_ELEMENTS_H

// INCLUDES /////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

// DEFINES //////////////////////////////////////////////

namespace sstbx
{
namespace factory
{

class SchemaElement;

// FORWARD DECLARATIONS ////////////////////////////////////

class SslibElements
{
public:

  typedef ::boost::shared_ptr<SchemaElement>  ElementPtr;

  SslibElements();

  ElementPtr    cellDesc;
  ElementPtr    cellDescParams;
  ElementPtr    cellDescVol;

  ElementPtr    potential;

};
}
}

#endif /* SSLIB_ELEMENTS_h */

