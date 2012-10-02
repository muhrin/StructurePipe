/*
 * Types.h
 *
 *  Created on: Aug 30, 2012
 *      Author: Martin Uhrin
 */

#ifndef BUILD_CELL__TYPES_H
#define BUILD_CELL__TYPES_H

// INCLUDES ////////////
#include "SSLib.h"

namespace sstbx {
namespace build_cell {

// FORWARD DECLARES ////////////
class IUnitCellBlueprint;
class RandomUnitCell;
class StructureDescription;

typedef ::sstbx::UniquePtr<StructureDescription>::Type StructureDescriptionPtr;

typedef ::sstbx::UniquePtr<IUnitCellBlueprint>::Type UnitCellBlueprintPtr;
typedef ::sstbx::UniquePtr<const IUnitCellBlueprint>::Type ConstUnitCellBlueprintPtr;

typedef ::sstbx::UniquePtr<RandomUnitCell>::Type RandomUnitCellPtr;

}
}

#endif /* BUILD_CELL__TYPES_H */
