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

#ifndef __CEL_PL_DATATYPE__
#define __CEL_PL_DATATYPE__

#include "cstypes.h"
#include "csutil/util.h"
#include "csgeom/vector3.h"
#include "pl/persist.h"

/**
 * Type for celData.
 */
enum celDataType
{
  CEL_DATA_NONE = 0,
  CEL_DATA_BOOL,
  CEL_DATA_INT,
  CEL_DATA_UINT,
  CEL_DATA_BYTE,
  CEL_DATA_WORD,
  CEL_DATA_LONG,
  CEL_DATA_UBYTE,
  CEL_DATA_UWORD,
  CEL_DATA_ULONG,
  CEL_DATA_FLOAT,
  CEL_DATA_VECTOR3,
  CEL_DATA_STRING,
  CEL_DATA_PCLASS,
  CEL_DATA_ENTITY,
  CEL_DATA_BUFFER,
  CEL_DATA_ACTION
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
    int i;
    unsigned int ui;
    int8 b;
    uint8 ub;
    int16 w;
    uint16 uw;
    int32 l;
    uint32 ul;
    float f;
    char* s;
    struct
    {
      float x, y, z;
    } v;
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
    if (type == CEL_DATA_STRING || type == CEL_DATA_ACTION) delete[] value.s;
    else if (type == CEL_DATA_BUFFER && value.db) value.db->DecRef ();
    type = CEL_DATA_NONE;
  }
  /**
   * Initialize and set the value.
   */
  void SetBool (bool v) { Clear (); type = CEL_DATA_BOOL; value.bo = v; }
  void Set (int v) { Clear (); type = CEL_DATA_INT; value.i = v; }
  void Set (unsigned int v) { Clear (); type = CEL_DATA_UINT; value.ui = v; }
  void Set (int8 v) { Clear (); type = CEL_DATA_BYTE; value.b = v; }
  void Set (uint8 v) { Clear (); type = CEL_DATA_UBYTE; value.ub = v; }
  void Set (int16 v) { Clear (); type = CEL_DATA_WORD; value.w = v; }
  void Set (uint16 v) { Clear (); type = CEL_DATA_UWORD; value.uw = v; }
  void Set (int32 v) { Clear (); type = CEL_DATA_LONG; value.l = v; }
  void Set (uint32 v) { Clear (); type = CEL_DATA_ULONG; value.ul = v; }
  void Set (float v) { Clear (); type = CEL_DATA_FLOAT; value.f = v; }
  void Set (const csVector3& v)
  {
    Clear ();
    type = CEL_DATA_VECTOR3;
    value.v.x = v.x;
    value.v.y = v.y;
    value.v.z = v.z;
  }
  void Set (const char* s)
  {
    Clear ();
    type = CEL_DATA_STRING;
    value.s = csStrNew (s);
  }
  void Set (iCelPropertyClass* pc)
  {
    Clear ();
    type = CEL_DATA_PCLASS;
    value.pc = pc;
  }
  void Set (iCelEntity* ent)
  {
    Clear ();
    type = CEL_DATA_ENTITY;
    value.ent = ent;
  }
  void Set (iCelDataBuffer* db)
  {
    Clear ();
    type = CEL_DATA_BUFFER;
    value.db = db;
    db->IncRef ();
  }
  void SetAction (const char* s)
  {
    Clear ();
    type = CEL_DATA_ACTION;
    value.s = csStrNew (s);
  }
};

#endif // __CEL_PL_DATATYPE__

