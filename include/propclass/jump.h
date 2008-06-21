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

#ifndef __CEL_PF_JUMP__
#define __CEL_PF_JUMP__

#include "cstypes.h"
#include "csutil/scf.h"

struct iPcJump : public virtual iBase
{
  SCF_INTERFACE (iPcJump, 0, 0, 1);

  virtual void Jump () = 0;
  virtual void DoubleJump () = 0;
  virtual void Freeze (bool frozen) = 0;
  virtual void Glide () = 0;

  virtual bool IsJumping () const = 0;
  virtual bool IsFrozen () = 0;

  virtual void SetJumpSpeed (float spd) = 0;
  virtual float GetJumpSpeed () const = 0;
  virtual void SetDoubleJumpSpeed (float spd) = 0;
  virtual float GetDoubleJumpSpeed () const = 0;
  virtual void SetDoubleJumpSensitivity (float sens) = 0;
  virtual float GetDoubleJumpSensitivity () const = 0;
  virtual void SetGlideSpeed (float spd) = 0;
  virtual float GetGlideSpeed () const = 0;
  virtual void SetGlideSensitivity (float sens) = 0;
  virtual float GetGlideSensitivity () const = 0;
  /*virtual void SetGravity (float grav) = 0;
  virtual float GetGravity () const = 0;
  virtual void SetGlideGravity (float grav) = 0;
  virtual float GetGlideGravity () const = 0;*/
};

#endif // __CEL_PF_JUMP__

