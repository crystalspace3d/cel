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

#include "csutil/hashmap.h"
#include "iutil/comp.h"
#include "pl/persist.h"
#include "pl/datatype.h"

class csHashSet;
struct iFile;
struct iObjectRegistry;
struct iCelPropertyClass;
struct iCelEntity;
struct iCelDataBuffer;
struct iCelPlLayer;
struct iCelBlLayer;
struct celData;

/**
 * This is the classic persistance layer.
 */
class celPersistClassic : public iCelPersistance
{
private:
  iObjectRegistry* object_reg;
  iCelPlLayer* pl;
  iCelBlLayer* bl;
  csHashSet entities;
  csHashSet pclasses;
  csHashMap read_entities;

  bool WriteMarker (iFile* f, const char* s);
  bool Write (iFile* f, const char* s);
  bool Write (iFile* f, iCelPropertyClass* pc);
  bool Write (iFile* f, iCelEntity* entity);
  bool Write (iFile* f, iCelDataBuffer* db);
  bool Write (iFile* f, celData* data);

  iCelEntity* FindOrCreateEntity (const char* name);
  bool ReadMarker (char*& data, size_t& remaining, char* marker);
  bool Read (char*& data, size_t& remaining, int8& b);
  bool Read (char*& data, size_t& remaining, uint8& ub);
  bool Read (char*& data, size_t& remaining, int16& w);
  bool Read (char*& data, size_t& remaining, uint16& uw);
  bool Read (char*& data, size_t& remaining, int32& l);
  bool Read (char*& data, size_t& remaining, uint32& ul);
  bool Read (char*& data, size_t& remaining, float& f);
  bool Read (char*& data, size_t& remaining, char*& str);
  bool Read (char*& data, size_t& remaining, iCelPropertyClass*& pc);
  bool Read (char*& data, size_t& remaining, iCelEntity*& entity);
  bool Read (char*& data, size_t& remaining, iCelDataBuffer*& db);
  bool Read (char*& data, size_t& remaining, celData* cd);

  void Report (const char* msg, ...);

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

