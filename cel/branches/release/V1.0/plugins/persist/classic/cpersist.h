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

/**
 * This is the classic persistence layer.
 */
class celPersistClassic : public iCelPersistence
{
private:
  bool WriteMarker (const char* s);
  bool Write (const char* s);
  bool Write (iCelDataBuffer* db);
  bool Write (celData* data);
  bool Write (uint32 v);
  bool Write (int32 v);
  bool Write (uint16 v);
  bool Write (int16 v);
  bool Write (uint8 v);
  bool Write (int8 v);
  bool Write (float f);
  bool Write (iCelPropertyClass* pc, bool savelocal);
  bool Write (iCelEntity* entity, bool savelocal);
  bool WriteFirstPass (iCelEntity* entity);

  bool ReadMarker (char* marker);
  bool CheckMarker (const char* comp);
  bool Read (int8& b);
  bool Read (uint8& ub);
  bool Read (int16& w);
  bool Read (uint16& uw);
  bool Read (int32& l);
  bool Read (uint32& ul);
  bool Read (float& f);
  bool Read (char*& str);
  bool Read (csString& str);
  bool Read (celData* cd);
  bool Read (csRef<iCelDataBuffer>& db);
  bool Read (iCelEntity* entity, iCelPropertyClass*& pc);
  bool Read (iCelEntity*& entity);
  bool ReadFirstPass (iCelEntity* entity);

  bool Report (const char* msg, ...);
  bool ReportTrunc ();
  bool ReportWrite ();

public:
  SCF_DECLARE_IBASE;
  csRef<iVFS> vfs;
  iFile* file;
  iCelLocalEntitySet* set;
  csHash<size_t, csPtrKey<iCelEntity> > entities_map;
  
  celPersistClassic (iBase* parent);
  virtual ~celPersistClassic ();
  bool Initialize (iObjectRegistry* object_reg);

  virtual bool Load (iCelLocalEntitySet* set, iFile* file);
  virtual bool Load (iCelLocalEntitySet* set, const char* name);
  virtual bool Save (iCelLocalEntitySet* set, iFile* file);
  virtual bool Save (iCelLocalEntitySet* set, const char* name);

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

