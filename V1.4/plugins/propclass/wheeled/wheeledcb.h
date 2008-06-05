/*
  Crystal Space Entity Layer
  Copyright (C) 2006 by Jorrit Tyberghein

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
  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef __CEL_PF_WHEELEDCB__
#define __CEL_PF_WHEELEDCB__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "wheeled.h"

struct WheeledCollisionCallback : public scfImplementation1<
      WheeledCollisionCallback, iDynamicsCollisionCallback>
{
  celPcWheeled* parent;
  size_t index;
  WheeledCollisionCallback (celPcWheeled* parent) :
    scfImplementationType (this)
  {
    WheeledCollisionCallback::parent = parent;
    index = 0;
  }
  virtual ~WheeledCollisionCallback ()
  {
  }

  virtual void Execute (iRigidBody *thisbody, iRigidBody *otherbody,
      const csVector3& pos, const csVector3& normal, float depth)
  {
    parent->WheelCollision (thisbody, otherbody, pos, normal, depth, index);
  }

  virtual void SetIndex (int index)
  {
    WheeledCollisionCallback::index = index;
  }
};
#endif // __CEL_PF_WHEELEDCB__

