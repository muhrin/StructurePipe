/*
 * SslibElements.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "factory/SslibElements.h"


#include "factory/SchemaList.h"
#include "factory/SchemaMap.h"
#include "factory/SchemaScalar.h"
#include "factory/SchemaObjectUid.h"

// NAMESPACES ////////////////////////////////

namespace sstbx
{
namespace factory
{

SslibElements::SslibElements()
{
  // Cell description
  {
    SchemaMap * const _cellDesc = new SchemaMap("cellDesc", true, "sslib.cellDesc");
    cellDesc.reset(_cellDesc);

    SchemaScalar * const _cellDescVol = new SchemaScalar("vol", true, "sslib.cellDesc.bol");
    cellDescVol.reset(_cellDescVol);

    SchemaList * const _cellDescParams = new SchemaList("params", true, "sslib.cellDesc.params", 6);
    cellDescParams.reset(_cellDescParams);

    _cellDesc->insert(cellDescVol->newInstance());
    _cellDesc->insert(cellDescParams->newInstance());
  }

  // Potentials
  {
    SchemaMap * const _potential = new SchemaMap("potential", true, "sslib.potential");
  }
}

}
}


