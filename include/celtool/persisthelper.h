/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA.
*/

#ifndef __CEL_CELTOOL_PERSISTHELPER__
#define __CEL_CELTOOL_PERSISTHELPER__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/util.h"
#include "csutil/array.h"
#include "csutil/set.h"

#include "physicallayer/persist.h"
#include "physicallayer/propclas.h"

/**
 * Standard implementation of iCelLocalEntitySet. This
 * version uses the name of the entity as a unique identifier for
 * finding external entities. So it can only be used if non-local entities
 * have unique names.
 */
class celStandardLocalEntitySet : public iCelLocalEntitySet
{
private:
  iCelPlLayer* pl;

  csArray<iCelEntity*> local_entities;
  csSet<iCelEntity*> local_entities_set;

public:
  celStandardLocalEntitySet (iCelPlLayer* pl);
  virtual ~celStandardLocalEntitySet ();

  SCF_DECLARE_IBASE;

  virtual size_t GetEntityCount () const
  {
    return local_entities.Length ();
  }
  virtual iCelEntity* GetEntity (size_t idx) const
  {
    return local_entities[idx];
  }
  virtual void AddEntity (iCelEntity* entity)
  {
    local_entities.Push (entity);
    local_entities_set.Add (entity);
  }
  virtual bool IsLocal (iCelEntity* entity)
  {
    return local_entities_set.In (entity);
  }
  virtual bool IsLocal (iCelPropertyClass* pc)
  {
    return local_entities_set.In (pc->GetEntity ());
  }
  virtual csPtr<iCelDataBuffer> SaveExternalEntity (iCelEntity* entity);
  virtual iCelEntity* FindExternalEntity (iCelDataBuffer* databuf);
  virtual csPtr<iCelDataBuffer> SaveExternalPC (iCelPropertyClass* pc);
  virtual iCelPropertyClass* FindExternalPC (iCelDataBuffer* databuf);
};

#endif // __CEL_CELTOOL_PERSISTHELPER__

