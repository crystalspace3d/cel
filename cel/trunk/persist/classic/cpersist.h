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

#ifndef __CEL_PERSIST_CLASSIC__
#define __CEL_PERSIST_CLASSIC__

#include "iutil/comp.h"
#include "pl/persist.h"

struct iFile;
struct iObjectRegistry;
struct iCelPropertyClass;
struct iCelEntity;
struct iCelDataBuffer;
struct celData;

/**
 * This is the classic persistance layer.
 */
class celPersistClassic : public iCelPersistance
{
private:
  iObjectRegistry* object_reg;

  bool WriteMarker (iFile* f, const char* s);
  bool WriteString (iFile* f, const char* s);
  bool Write (iFile* f, iCelPropertyClass* pc);
  bool Write (iFile* f, iCelEntity* entity);
  bool Write (iFile* f, iCelDataBuffer* db);
  bool Write (iFile* f, celData* data);

public:
  celPersistClassic (iBase* parent);
  virtual ~celPersistClassic ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual iCelEntity* LoadEntity (const char* name);
  virtual bool SaveEntity (iCelEntity* entity, const char* name);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPersistClassic);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_PERSIST_CLASSIC__

