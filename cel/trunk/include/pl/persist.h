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
  CEL_DATA_ENTITY
};

struct celData
{
  celDataType type;
  char* name;
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
  } value;

  celData () : type (CEL_DATA_NONE), name (NULL) { }
  ~celData()
  {
    Clear ();
  }
  void Clear ()
  {
    delete[] name;
    if (type == CEL_DATA_STRING) delete[] value.s;
    type = CEL_DATA_NONE;
  }
  void New (const char* n)
  {
    delete[] name;
    name = csStrNew (n);
    if (type == CEL_DATA_STRING) delete[] value.s;
    type = CEL_DATA_NONE;
  }
  void Set (const char* n, bool v) { New (n); type = CEL_DATA_BOOL; value.bo = v; }
  void Set (const char* n, int8 v) { New (n); type = CEL_DATA_BYTE; value.b = v; }
  void Set (const char* n, uint8 v) { New (n); type = CEL_DATA_UBYTE; value.ub = v; }
  void Set (const char* n, int16 v) { New (n); type = CEL_DATA_WORD; value.w = v; }
  void Set (const char* n, uint16 v) { New (n); type = CEL_DATA_UWORD; value.uw = v; }
  void Set (const char* n, int32 v) { New (n); type = CEL_DATA_LONG; value.l = v; }
  void Set (const char* n, uint32 v) { New (n); type = CEL_DATA_ULONG; value.ul = v; }
  void Set (const char* n, float v) { New (n); type = CEL_DATA_FLOAT; value.f = v; }
  void Set (const char* n, const char* s) { New (n); type = CEL_DATA_STRING; value.s = csStrNew (s); }
  void Set (const char* n, iCelPropertyClass* pc) { New (n); type = CEL_DATA_PCLASS; value.pc = pc; }
  void Set (const char* n, iCelEntity* ent) { New (n); type = CEL_DATA_ENTITY; value.ent = ent; }
};

SCF_VERSION (iCelDataBuffer, 0, 0, 1);

/**
 * This interface describes persistable data.
 */
struct iCelDataBuffer : public iBase
{
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

  /**
   * Get a specific data entry with a name.
   */
  virtual celData* GetData (const char* name) const = 0;
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

