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

#ifndef __CEL_PL_DATATYPE__
#define __CEL_PL_DATATYPE__

#include "cstypes.h"
#include "csutil/scfstr.h"
#include "csgeom/vector2.h"
#include "csgeom/vector3.h"
#include "csutil/cscolor.h"
#include "physicallayer/persist.h"

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
  CEL_DATA_VECTOR2,
  CEL_DATA_VECTOR3,
  CEL_DATA_STRING,
  CEL_DATA_PCLASS,
  CEL_DATA_ENTITY,
  CEL_DATA_BUFFER,
  CEL_DATA_ACTION,
  CEL_DATA_COLOR,
  CEL_DATA_IBASE,

  CEL_DATA_LAST
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
    iString* s;
    struct
    {
      float x, y, z;
    } v;
    struct
    {
      float red, green, blue;
    } col;
    iCelPropertyClass* pc;
    iCelEntity* ent;
    iCelDataBuffer* db;
    iBase* ibase;
  } value;

  celData () : type (CEL_DATA_NONE) { }
  ~celData()
  {
    Clear ();
  }
  void Clear ()
  {
    if (type == CEL_DATA_STRING || type == CEL_DATA_ACTION) value.s->DecRef ();
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
  void Set (const csVector2& v)
  {
    Clear ();
    type = CEL_DATA_VECTOR2;
    value.v.x = v.x;
    value.v.y = v.y;
  }
  void Set (const csVector3& v)
  {
    Clear ();
    type = CEL_DATA_VECTOR3;
    value.v.x = v.x;
    value.v.y = v.y;
    value.v.z = v.z;
  }
  void Set (const csColor& v)
  {
    Clear ();
    type = CEL_DATA_COLOR;
    value.col.red = v.red;
    value.col.green = v.green;
    value.col.blue = v.blue;
  }
  void Set (const char* s)
  {
    Clear ();
    type = CEL_DATA_STRING;
    value.s = new scfString (s);
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
    value.s = new scfString (s);
  }
  void SetIBase (iBase* b)
  {
    Clear ();
    type = CEL_DATA_IBASE;
    value.ibase = b;
  }
};

#endif // __CEL_PL_DATATYPE__

