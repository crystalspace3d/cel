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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA.
*/

#ifndef __CEL_PL_PERSIST__
#define __CEL_PL_PERSIST__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/util.h"

#include "physicallayer/datatype.h"

struct iCelPropertyClass;
struct iCelEntity;
struct iFile;
struct celData;
struct iCelPersistenceContext;

SCF_VERSION (iCelDataBuffer, 0, 0, 1);

enum
{
    CEL_PERSIST_MODE_READ = 1,
    CEL_PERSIST_MODE_WRITE = 2
};

/**
 * This interface describes persistable data.
 */
struct iCelDataBuffer : public iBase
{
  /**
   * Get a serial number for this data. This can be used
   * to check validity of the data (i.e. to compare version
   * numbers).
   */
  virtual long GetSerialNumber () const = 0;

  /**
   * Get the number of data entries.
   */
  virtual size_t GetDataCount () const = 0;

  /**
   * Reset the internal read counter.
   */
  virtual void Reset () = 0;

  /**
   * Get the next data entry. See Reset().
   */
  virtual celData* GetData () = 0;

  /**
   * Get a specific data entry.
   */
  virtual celData* GetData (size_t idx) = 0;

  /**
   * Add a data entry.
   */
  virtual celData* AddData () = 0;

  /// Convenience methods to add specific types of data.
  virtual void Add (bool v) { AddData ()->Set (v); }
  virtual void Add (int8 v) { AddData ()->Set (v); }
  virtual void Add (int16 v) { AddData ()->Set (v); }
  virtual void Add (int32 v) { AddData ()->Set (v); }
  virtual void Add (uint8 v) { AddData ()->Set (v); }
  virtual void Add (uint16 v) { AddData ()->Set (v); }
  virtual void Add (uint32 v) { AddData ()->Set (v); }
  virtual void Add (float v) { AddData ()->Set (v); }
  virtual void Add (const csVector2& v) { AddData ()->Set (v); }
  virtual void Add (const csVector3& v) { AddData ()->Set (v); }
  virtual void Add (const csColor& v) { AddData ()->Set (v); }
  virtual void Add (const char* v) { AddData ()->Set (v); }
  virtual void Add (iCelPropertyClass* v) { AddData ()->Set (v); }
  virtual void Add (iCelEntity* v) { AddData ()->Set (v); }
  virtual void AddAction (const char* v) { AddData ()->SetAction (v); }
  virtual void AddIBase (iBase* v) { AddData ()->SetIBase (v); }

  /// Convenience methods to get specific types of data.
  virtual bool GetBool ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_BOOL) ? cd->value.bo : false;
  }
  virtual int8 GetInt8 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_BYTE) ? cd->value.b : 0;
  }
  virtual uint8 GetUInt8 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_UBYTE) ? cd->value.ub : 0;
  }
  virtual int16 GetInt16 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_WORD) ? cd->value.w : 0;
  }
  virtual uint16 GetUInt16 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_UWORD) ? cd->value.uw : 0;
  }
  virtual int32 GetInt32 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_LONG) ? cd->value.l : 0;
  }
  virtual uint32 GetUInt32 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_ULONG) ? cd->value.ul : 0;
  }
  virtual float GetFloat ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_FLOAT) ? cd->value.f : 0.0f;
  }
  virtual iString* GetString ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_STRING) ? cd->value.s : (iString*)0;
  }
  virtual void GetVector2 (csVector2& v)
  {
    celData* cd = GetData ();
    if (cd && cd->type == CEL_DATA_VECTOR2)
    { v.x = cd->value.v.x; v.y = cd->value.v.y; }
    else v.x = v.y = 0;
  }
  virtual void GetVector3 (csVector3& v)
  {
    celData* cd = GetData ();
    if (cd && cd->type == CEL_DATA_VECTOR3)
    { v.x = cd->value.v.x; v.y = cd->value.v.y; v.z = cd->value.v.z; }
    else v.x = v.y = v.z = 0;
  }
  virtual void GetColor (csColor& v)
  {
    celData* cd = GetData ();
    if (cd && cd->type == CEL_DATA_COLOR)
    {
      v.red = cd->value.col.red;
      v.green = cd->value.col.green;
      v.blue = cd->value.col.blue;
    }
    else v.red = v.green= v.blue = 0;
  }
  virtual iCelPropertyClass* GetPC ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_PCLASS)
    	? cd->value.pc
	: (iCelPropertyClass*)0;
  }
  virtual iCelEntity* GetEntity ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_ENTITY)
    	? cd->value.ent
	: (iCelEntity*)0;
  }
  virtual iBase* GetIBase ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_IBASE) ? cd->value.ibase : (iBase*)0;
  }
};

SCF_VERSION (iCelPersistence, 0, 0, 1);

/**
 * This interface describes a module that can
 * persist CEL data.
 */
struct iCelPersistence : public iBase
{
  /**
   * Load an entity from the persistent data including
   * all property classes. 'name' is a name relevant for
   * the given type of persistence. It can be a filename
   * for example (VFS).
   */
  virtual csPtr<iCelEntity> LoadEntity (const char* name) = 0;

  /**
   * Save an entity to persistent data.
   */
  virtual bool SaveEntity (iCelEntity* entity, const char* name) = 0;

  /**
   * Create an persistence context
   */
  virtual iCelPersistenceContext* CreateContext(iBase* data, int mode,
	  bool performmapping = true) = 0;
};

SCF_VERSION (iCelPersistenceContext, 0, 0, 1);

struct iCelPersistenceContext : public iBase
{
  /**
   * Load an entity from the persitent data including all property classes.
   */
  virtual csPtr<iCelEntity> LoadEntity () = 0;

  /**
   * Save an entity to persistent data.
   */
  virtual bool SaveEntity (iCelEntity* entity) = 0;

  /* returns the ID mapped to the ID on the other side of the connection */
  virtual uint GetMappedID(iCelEntity* entity) = 0;

  /* returns the entitiy after mapping the ID */
  virtual iCelEntity* GetMappedEntity(uint id) = 0;

  /**
   * Clears internal data and buffers (like list of already saved entities)
   */
  virtual void Clear () = 0;
};

// Deprecated: Backward compatibility for long-term spelling mistake.
SCF_VERSION (iCelPersistance, 0, 0, 1);
struct iCelPersistance : public iCelPersistence {};
SCF_VERSION (iCelPersistanceContext, 0, 0, 1);
struct iCelPersistanceContext : public iCelPersistenceContext {};

#endif // __CEL_PL_PERSIST__
