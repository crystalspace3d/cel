/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
  
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_MOVE__
#define __CEL_PF_MOVE__

#include "cstypes.h"
#include "csutil/scf.h"

struct iSector;
struct iPcMesh;
struct iPcMovableConstraint;
class csVector3;

/**
 * Property ID used when the movable position changes (for use with
 * property change callbacks).
 */
#define CEL_PCMOVABLE_PROPERTY_POSITION 1

#define CEL_MOVE_FAIL 0
#define CEL_MOVE_SUCCEED 1
#define CEL_MOVE_PARTIAL 2

/**
 * Control moving an iPcMesh.
 */
struct iPcMovable : public virtual iBase
{
  SCF_INTERFACE (iPcMovable, 0, 0, 1);

  /**
   * Set mesh to move. If not set a default mesh will be found
   * from the parent entity.
   */
  virtual void SetMesh (iPcMesh* mesh) = 0;

  /**
   * Get the current mesh on which we're working.
   */
  virtual iPcMesh* GetMesh () = 0;

  /**
   * Move object while checking constraints. This routine will ignore
   * the previous position and will just check if it is possible
   * to move to the given position.
   * Returns:
   * <ul>
   * <li>CEL_MOVE_FAIL: if no movement was possible.
   * <li>CEL_MOVE_SUCCEED: if movement was possible.
   * </ul>
   */
  virtual int Move (iSector* sector, const csVector3& pos) = 0;

  /**
   * Relative move. Check constraints too. This function will correctly
   * update the current sector if a portal is traversed.
   * Returns:
   * <ul>
   * <li>CEL_MOVE_FAIL: if no movement was possible.
   * <li>CEL_MOVE_SUCCEED: if movement was possible.
   * <li>CEL_MOVE_PARTIAL: if object could move partially.
   * </ul>
   */
  virtual int Move (const csVector3& relpos) = 0;

  /**
   * Add a constraint.
   */
  virtual void AddConstraint (iPcMovableConstraint* constraint) = 0;

  /**
   * Remove a constraint.
   */
  virtual void RemoveConstraint (iPcMovableConstraint* constraint) = 0;

  /**
   * Remove all constraints.
   */
  virtual void RemoveAllConstraints () = 0;
};

/**
 * A constraint for a movable.
 */
struct iPcMovableConstraint : public virtual iBase
{
  SCF_INTERFACE (iPcMovableConstraint, 0, 0, 1);

  /**
   * Check if an object can move.
   * Returns:
   * - CEL_MOVE_FAIL: if no movement is possible.
   * - CEL_MOVE_SUCCEED: if movement is possible.
   * - CEL_MOVE_PARTIAL: if object can move partially.
   *
   * 'pos' will contain the final position that this constraint
   * considered valid.
   * Note! If 'from' and 'to' are equal then it is only checked
   * if the object can be put on the specified position. In that
   * case this routine will never return CEL_MOVE_PARTIAL.
   */
  virtual int CheckMove (iSector* sector, const csVector3& from,
  	const csVector3& to, csVector3& pos) = 0;
};

#endif // __CEL_PF_MOVE__

