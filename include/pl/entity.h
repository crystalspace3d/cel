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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PL_ENTITY__
#define __CEL_PL_ENTITY__

#include "cstypes.h"
#include "csutil/scf.h"

struct iPropertyClassList;
struct iCelBlEntity;

SCF_VERSION (iCelEntity, 0, 0, 1);

/**
 * This is an entity in the CEL layer at the PL (physical layer) side.
 */
struct iCelEntity : public iBase
{
  /**
   * Get the name of this entity.
   */
  virtual const char* GetName () const = 0;

  /**
   * Set the name of this entity.
   */
  virtual void SetName (const char* n) = 0;

  /**
   * Get the list of property classes.
   */
  virtual iPropertyClassList* GetPropertyClassList () = 0;

  /**
   * Set the behaviour layer entity associated with this entity.
   */
  virtual void SetBlEntity (iCelBlEntity* bl_entity) = 0;

  /**
   * Get the behaviour layer entity associated with this entity.
   */
  virtual iCelBlEntity* GetBlEntity () = 0;
};

#endif // __CEL_PL_ENTITY__

