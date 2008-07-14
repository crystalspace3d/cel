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

#ifndef __CEL_PF_GRAB__
#define __CEL_PF_GRAB__

#include "cstypes.h"
#include "csutil/scf.h"

struct iPcGrab : public virtual iBase
{
  SCF_INTERFACE (iPcGrab, 0, 0, 1);

  enum GrabState
  {
    DISABLED = 0,
    SEARCHING,
    HANG,
    SHIMMY_LEFT,
    SHIMMY_RIGHT
  };
  virtual void SetState (GrabState state) = 0;
  virtual GrabState GetState () const = 0;

  virtual void SetShimmyTime (float time) = 0;
  virtual float GetShimmyTime () const = 0;
  virtual void SetShimmyDistance (float dist) = 0;
  virtual float GetShimmyDistance () const = 0;
  virtual void SetShimmyInitialVelocity (float vel) = 0;
  virtual float GetShimmyInitialVelocity () const = 0;
};

#endif // __CEL_PF_GRAB__
