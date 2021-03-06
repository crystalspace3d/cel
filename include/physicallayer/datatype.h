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
#include "csutil/scfarray.h"
#include "csgeom/vector2.h"
#include "csgeom/vector3.h"
#include "csgeom/vector4.h"
#include "csutil/cscolor.h"

struct iCelPropertyClass;
struct iCelEntity;

/**
 * Type for celData. CEL_DATA_PARAMETER is a special type that indicates
 * a parameter. This is only useful in certain situations.
 */
enum celDataType
{
  /// Unknown type
  CEL_DATA_UNKNOWN = -1,
  /// Undefined type
  CEL_DATA_NONE = 0,
  /// Boolean type
  CEL_DATA_BOOL,
  /// Byte type (ie 8 bits)
  CEL_DATA_BYTE,
  /// Word type (ie 16 bits)
  CEL_DATA_WORD,
  /// Long type (ie 32 bits)
  CEL_DATA_LONG,
  /// Unsigned byte type
  CEL_DATA_UBYTE,
  /// Unsigned word type
  CEL_DATA_UWORD,
  /// Unsigned long type
  CEL_DATA_ULONG,
  /// Float type
  CEL_DATA_FLOAT,
  /// csVector2 type
  CEL_DATA_VECTOR2,
  /// csVector3 type
  CEL_DATA_VECTOR3,
  /// csVector4 type
  CEL_DATA_VECTOR4,
  /// String type
  CEL_DATA_STRING,
  /// Property class type
  CEL_DATA_PCLASS,
  /// Entity type
  CEL_DATA_ENTITY,
  /// Action type
  CEL_DATA_ACTION,
  /// csColor type
  CEL_DATA_COLOR,
  /// csColor4 type
  CEL_DATA_COLOR4,
  /// iBase type
  CEL_DATA_IBASE,
  /// A templated parameter 
  CEL_DATA_PARAMETER,
  /// Count of different data types
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
      float x, y, z, w;
    } v;
    struct
    {
      float red, green, blue, alpha;
    } col;
    iCelPropertyClass* pc;
    iCelEntity* ent;
    iBase* ibase;
    struct
    {
      iString* parname;
      celDataType partype;
    } par;
  } value;

  celData () : type (CEL_DATA_NONE) { }
  celData (const celData& copy)
  {
    type = copy.type;
    value = copy.value;
    if (type == CEL_DATA_STRING || type == CEL_DATA_ACTION) value.s->IncRef ();
    else if (type == CEL_DATA_PARAMETER) value.par.parname->IncRef ();
  }
  const celData& operator= (const celData& copy)
  {
    Copy (copy);
    return *this;
  }
  ~celData()
  {
    Clear ();
  }

  void Copy (const celData& copy)
  {
    Clear ();
    type = copy.type;
    value = copy.value;
    if (type == CEL_DATA_STRING || type == CEL_DATA_ACTION) value.s->IncRef ();
    else if (type == CEL_DATA_PARAMETER) value.par.parname->IncRef ();
  }

  bool operator== (const celData& other) const
  {
    if (type != other.type) return false;
    switch (type)
    {
      case CEL_DATA_NONE: return true;
      case CEL_DATA_BOOL: return value.bo == other.value.bo;
      case CEL_DATA_BYTE: return value.b == other.value.b;
      case CEL_DATA_WORD: return value.w == other.value.w;
      case CEL_DATA_LONG: return value.l == other.value.l;
      case CEL_DATA_UBYTE: return value.ub == other.value.ub;
      case CEL_DATA_UWORD: return value.uw == other.value.uw;
      case CEL_DATA_ULONG: return value.ul == other.value.ul;
      case CEL_DATA_FLOAT: return value.f == other.value.f;
      case CEL_DATA_VECTOR2: return value.v.x == other.value.v.x &&
                               value.v.y == other.value.v.y;
      case CEL_DATA_VECTOR3: return value.v.x == other.value.v.x &&
                               value.v.y == other.value.v.y &&
                               value.v.z == other.value.v.z;
      case CEL_DATA_VECTOR4: return value.v.x == other.value.v.x &&
                               value.v.y == other.value.v.y &&
                               value.v.z == other.value.v.z &&
                               value.v.w == other.value.v.w;
      case CEL_DATA_STRING: return value.s == other.value.s;
      case CEL_DATA_PCLASS: return value.pc == other.value.pc;
      case CEL_DATA_ENTITY: return value.ent == other.value.ent;
      case CEL_DATA_ACTION: return value.s == other.value.s;
      case CEL_DATA_COLOR: return value.col.red == other.value.col.red &&
                             value.col.green == other.value.col.green &&
                             value.col.blue == other.value.col.blue;
      case CEL_DATA_COLOR4: return value.col.red == other.value.col.red &&
                              value.col.green == other.value.col.green &&
                              value.col.blue == other.value.col.blue &&
                              value.col.alpha == other.value.col.alpha;
      case CEL_DATA_IBASE: return value.ibase == other.value.ibase;
      case CEL_DATA_PARAMETER: return value.par.partype == other.value.par.partype &&
                                 value.par.parname == other.value.par.parname;
      default:
        printf ("INTERNAL ERROR! Bad type %d\n", type);
        fflush (stdout);
        return false;
    }
  }

  void Clear ()
  {
    if (type == CEL_DATA_STRING || type == CEL_DATA_ACTION) value.s->DecRef ();
    else if (type == CEL_DATA_PARAMETER) value.par.parname->DecRef ();
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
  void Set (const csVector4& v)
  {
    Clear ();
    type = CEL_DATA_VECTOR4;
    value.v.x = v.x;
    value.v.y = v.y;
    value.v.z = v.z;
    value.v.w = v.w;
  }
  void Set (const csColor& v)
  {
    Clear ();
    type = CEL_DATA_COLOR;
    value.col.red = v.red;
    value.col.green = v.green;
    value.col.blue = v.blue;
  }
  void Set (const csColor4& v)
  {
    Clear ();
    type = CEL_DATA_COLOR4;
    value.col.red = v.red;
    value.col.green = v.green;
    value.col.blue = v.blue;
    value.col.alpha = v.alpha;
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
  void SetParameter (const char* s, celDataType t)
  {
    Clear ();
    type = CEL_DATA_PARAMETER;
    value.par.parname = new scfString (s);
    value.par.partype = t;
  }
};

struct iCelDataArrayReadOnly : public iArrayReadOnly<celData>
{
  SCF_IARRAYREADONLY_INTERFACE(iCelDataArrayReadOnly);
};

struct iCelDataArray : public iArrayChangeAll<celData>
{
  SCF_IARRAYCHANGEALL_INTERFACE(iCelDataArray);
};

#endif // __CEL_PL_DATATYPE__

