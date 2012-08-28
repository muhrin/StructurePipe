/*
 * AtomExtruder.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: Martin Uhrin
 */

#include "build_cell/AtomExtruder.h"

#include "common/Atom.h"
#include "common/AbstractFmidCell.h"
#include "common/Structure.h"
#include "common/Types.h"

namespace sstbx {
namespace build_cell {

bool AtomExtruder::extrudeAtoms(common::Structure & structure) const
{
  const ::std::vector<common::AtomPtr> & atoms = structure.getAtoms();

  ::std::vector<common::AtomPtr> atomsWithRadii;

  for(size_t i = 0; i < atoms.size(); ++i)
  {
    if(atoms[i]->getRadius() != 0.0)
    {
      atomsWithRadii.push_back(atoms[i]);
    }
  }

  // Calculate seaparation matrix
  ::arma::mat sepMtx(atomsWithRadii.size(), atomsWithRadii.size());


  double radius1;
  for(size_t row = 0; row < atomsWithRadii.size(); ++row)
  {
    radius1 = atomsWithRadii[row]->getRadius();
    for(size_t col = col; col < atomsWithRadii.size(); ++col)
    {
      sepMtx(row, col) = radius1 + atomsWithRadii[col]->getRadius();
      sepMtx(row, col) *= sepMtx(row, col);
    }
  }

  return extrudeAtoms(*structure.getUnitCell(), atomsWithRadii, sepMtx);
}

bool AtomExtruder::extrudeAtoms(
  const common::AbstractFmidCell & cell,
  ::std::vector<common::AtomPtr> & atoms,
  const ::arma::mat & sepMtx,
  const size_t maxIterations) const
{
  bool success = false;
  ::arma::vec posI, posJ;
  for(size_t iters = 0; iters < maxIterations; ++iters)
  {
    for(size_t i = 0; i < atoms.size() - 1; ++i)
    {
      posI = atoms[i]->getPosition();
      for(size_t j = i + 1; j < atoms.size(); ++j)
      {
        posJ = atoms[j]->getPosition();
        cell.getDistanceSqMinimumImg(posI, posJ);
      }
    }
  }
  return success;
}

}
}
