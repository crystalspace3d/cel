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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_CELLESGES__
#define __CEL_CELLESGES__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csgeom/vector2.h"

struct iLedge : public virtual iBase
{
  SCF_INTERFACE (iLedge, 0, 0, 1);

  virtual void AddPoint (const csVector2 &p) = 0;
  virtual void SetYPosition (float y) = 0;
  virtual float GetYPosition () const = 0;
  virtual size_t GetPointCount () const = 0;
  virtual const csVector2 &GetPoint (size_t i) const = 0;
};

struct iLedgeGroup : public virtual iBase
{
  SCF_INTERFACE (iLedgeGroup, 0, 0, 1);

  /**
   * Query the underlying iObject
   */
  virtual iObject* QueryObject () = 0;
  virtual iLedge* CreateLedge () = 0;

  virtual size_t GetCount () const = 0;
  virtual iLedge* Get (size_t i) = 0;
};

#endif
