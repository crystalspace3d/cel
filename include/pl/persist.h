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

#ifndef __CEL_PL_PERSIST__
#define __CEL_PL_PERSIST__

#include "cstypes.h"
#include "csutil/util.h"

struct iCelPropertyClass;
struct iCelEntity;
struct celData;

SCF_VERSION (iCelDataBuffer, 0, 0, 1);

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
   * Set the number of data entries.
   */
  virtual void SetDataCount (int cnt) = 0;

  /**
   * Get the number of data entries.
   */
  virtual int GetDataCount () const = 0;

  /**
   * Get a specific data entry.
   */
  virtual celData* GetData (int idx) const = 0;
};

/**
 * Type for celData.
 */
enum celDataType
{
  CEL_DATA_NONE = 0,
  CEL_DATA_BOOL,
  CEL_DATA_BYTE,
  CEL_DATA_WORD,
  CEL_DATA_LONG,
  CEL_DATA_UBYTE,
  CEL_DATA_UWORD,
  CEL_DATA_ULONG,
  CEL_DATA_FLOAT,
  CEL_DATA_STRING,
  CEL_DATA_PCLASS,
  CEL_DATA_ENTITY,
  CEL_DATA_BUFFER
};

/**
 * Typed data for CEL.
 */
struct celData
{
  celDataType type;
  union
  {
    bool bo;
    int8 b;
    uint8 ub;
    int16 w;
    uint16 uw;
    int32 l;
    uint32 ul;
    float f;
    char* s;
    iCelPropertyClass* pc;
    iCelEntity* ent;
    iCelDataBuffer* db;
  } value;

  celData () : type (CEL_DATA_NONE) { }
  ~celData()
  {
    Clear ();
  }
  void Clear ()
  {
    if (type == CEL_DATA_STRING) delete[] value.s;
    else if (type == CEL_DATA_BUFFER && value.db) value.db->DecRef ();
    type = CEL_DATA_NONE;
  }
  /**
   * Initialize and set the value.
   */
  void Set (bool v) { Clear (); type = CEL_DATA_BOOL; value.bo = v; }
  void Set (int8 v) { Clear (); type = CEL_DATA_BYTE; value.b = v; }
  void Set (uint8 v) { Clear (); type = CEL_DATA_UBYTE; value.ub = v; }
  void Set (int16 v) { Clear (); type = CEL_DATA_WORD; value.w = v; }
  void Set (uint16 v) { Clear (); type = CEL_DATA_UWORD; value.uw = v; }
  void Set (int32 v) { Clear (); type = CEL_DATA_LONG; value.l = v; }
  void Set (uint32 v) { Clear (); type = CEL_DATA_ULONG; value.ul = v; }
  void Set (float v) { Clear (); type = CEL_DATA_FLOAT; value.f = v; }
  void Set (const char* s) { Clear (); type = CEL_DATA_STRING; value.s = csStrNew (s); }
  void Set (iCelPropertyClass* pc) { Clear (); type = CEL_DATA_PCLASS; value.pc = pc; }
  void Set (iCelEntity* ent) { Clear (); type = CEL_DATA_ENTITY; value.ent = ent; }
  void Set (iCelDataBuffer* db) { Clear (); type = CEL_DATA_BUFFER; value.db = db; db->IncRef (); }
};

SCF_VERSION (iCelPersistance, 0, 0, 1);

/**
 * This interface describes a module that can
 * persist CEL data.
 */
struct iCelPersistance : public iBase
{
  /**
   * Load an entity from the persistent data including
   * all property classes. 'name' is a name relevant for
   * the given type of persistance. It can be a filename
   * for example (VFS).
   */
  virtual iCelEntity* LoadEntity (const char* name) = 0;

  /**
   * Save an entity to persistent data.
   */
  virtual bool SaveEntity (iCelEntity* entity, const char* name) = 0;
};



#endif // __CEL_PL_PERSIST__

