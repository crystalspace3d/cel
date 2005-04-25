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

#ifndef __CEL_PERSIST_CLASSIC__
#define __CEL_PERSIST_CLASSIC__

#include "csutil/hash.h"
#include "csutil/refarr.h"
#include "csutil/set.h"
#include "iutil/comp.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"

class celPersistClassic;
struct iFile;
struct iVFS;
struct iObjectRegistry;
struct iCelPropertyClass;
struct iCelEntity;
struct iCelDataBuffer;
struct iCelPlLayer;
struct iCelBlLayer;
struct celData;

#define CEL_PERSIST_BACKWARD_COMPAT
#ifdef CEL_PERSIST_BACKWARD_COMPAT
#  define CEL_PERSIST_PARENT_ROOT iCelPersistance
#  define CEL_PERSIST_CONTEXT_PARENT_ROOT iCelPersistanceContext
#else
#  define CEL_PERSIST_PARENT_ROOT iCelPersistence
#  define CEL_PERSIST_CONTEXT_PARENT_ROOT iCelPersistenceContext
#endif

class celPersistClassicContext : public CEL_PERSIST_CONTEXT_PARENT_ROOT
{
private:
  iObjectRegistry* object_reg;
  csRef<iCelPlLayer> pl;
  csRef<iFile> file;
  bool performmapping;
  csSet<iCelEntity*> entities;
  csSet<iCelPropertyClass*> pclasses;
  csHash<iCelEntity*> read_entities;
  csHash<uint*> read_ids;
  csRefArray<iCelEntity> temprefs;

  bool WriteMarker (const char* s);
  bool WriteID (iCelEntity* entity);
  bool Write (const char* s);
  bool Write (iCelPropertyClass* pc);
  bool Write (iCelEntity* entity);
  bool Write (iCelDataBuffer* db);
  bool Write (celData* data);
  bool Write (uint32 v);
  bool Write (uint16 v);
  bool Write (int16 v);
  bool Write (int32 v);
  bool Write (uint8 v);
  bool Write (int8 v);
  bool Write (float f);

  iCelEntity* FindEntity (uint id);
  iCelEntity* FindOrCreateEntity (uint id);
  bool ReadMarker (char* marker);
  bool CheckMarker (const char* marker);
  bool Read (int8& b);
  bool Read (uint8& ub);
  bool Read (int16& w);
  bool Read (uint16& uw);
  bool Read (int32& l);
  bool Read (uint32& ul);
  bool Read (float& f);
  bool Read (char*& str);
  bool Read (iCelPropertyClass*& pc);
  bool Read (iCelEntity*& entity);
  bool Read (iCelDataBuffer*& db);
  bool Read (celData* cd);

  void Report (const char* msg, ...);

public:
  SCF_DECLARE_IBASE;
  
  celPersistClassicContext();
  virtual ~celPersistClassicContext();
  bool Initialize (iObjectRegistry* object_reg, iFile* file, int mode,
      bool performmapping);

  virtual csPtr<iCelEntity> LoadEntity();
  virtual bool SaveEntity(iCelEntity* entity);

  virtual uint GetMappedID(iCelEntity* entitiy);
  virtual iCelEntity* GetMappedEntity(uint id);

  void Clear();
  void RemoveEntity(iCelEntity* entity);

  void ClearTempRefs();

  struct RemoveCallback : public iCelEntityRemoveCallback
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPersistClassicContext);
    virtual void RemoveEntity(iCelEntity* entity)
    {
      scfParent->RemoveEntity (entity);
    }
  } scfiCelEntityRemoveCallback;

  friend class celPersistClassic;
};

/**
 * This is the classic persistence layer.
 */
class celPersistClassic : public CEL_PERSIST_PARENT_ROOT
{
public:
  SCF_DECLARE_IBASE;
  csRef<iVFS> vfs;
  
  celPersistClassic (iBase* parent);
  virtual ~celPersistClassic ();
  bool Initialize (iObjectRegistry* object_reg);

  virtual csPtr<iCelEntity> LoadEntity (const char* name);
  virtual bool SaveEntity (iCelEntity* entity, const char* name);

  virtual iCelPersistenceContext* CreateContext(iBase* data, int mode, 
      bool performmapping);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPersistClassic);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
protected:
  iObjectRegistry* object_reg;
};

#endif // __CEL_PERSIST_CLASSIC__

