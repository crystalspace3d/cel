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
#ifndef __DATABHLP_H__
#define __DATABHLP_H__

#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"

class celDataBufHelper
{
public:
  celDataBufHelper (iCelDataBuffer* databuf)
    : databuf(databuf), j(0), rc(true)
  { }

  void SetDataCount (size_t cnt)
  { databuf->SetDataCount(cnt); }
  
  void SetPos(size_t idx)
  {
    if (j>=databuf->GetDataCount())
      return;
    j=idx;
    rc=true;
  }

  bool AllOk()	{ return rc; }

  size_t GetPos()
  {  return j; }

  void Set (int8 v) { databuf->GetData(j++)->Set(v); }
  void Set (uint8 v) { databuf->GetData(j++)->Set(v); }
  void Set (int16 v) { databuf->GetData(j++)->Set(v); }
  void Set (uint16 v) { databuf->GetData(j++)->Set(v); }
  void Set (int32 v) { databuf->GetData(j++)->Set(v); }
  void Set (uint32 v) { databuf->GetData(j++)->Set(v); }  
  void Set (float v) { databuf->GetData(j++)->Set(v); }
  void Set (const csVector3& v) { databuf->GetData(j++)->Set(v); }
  void Set (const csColor& v) { databuf->GetData(j++)->Set(v); }
  void Set (const char* v) { databuf->GetData(j++)->Set(v); }
  void Set (iCelEntity* v) { databuf->GetData(j++)->Set(v); }
  void Set (iCelPropertyClass* v) { databuf->GetData(j++)->Set(v); }
  void Set (iCelDataBuffer* v) { databuf->GetData(j++)->Set(v); }
  void SetAction (const char* v) { databuf->GetData(j++)->SetAction(v); }
  void SetBool (bool v) { databuf->GetData(j++)->SetBool(v); }

  // the following functions work with reference parameters, because c++
  // doesn't allow overriding based on return type
#define DATABHLP_GET(dt,varname,TYPE)					\
  bool Get(dt& v)							\
  { celData* data = databuf->GetData(j++);				\
    if (data->type != TYPE) { rc=false; return false; }			\
    v = data->value.varname;						\
    return true;							\
  }
  
  DATABHLP_GET(int8,b,CEL_DATA_BYTE)
  DATABHLP_GET(uint8,ub,CEL_DATA_UBYTE)
  DATABHLP_GET(int16,w,CEL_DATA_WORD)
  DATABHLP_GET(uint16,uw,CEL_DATA_UWORD)
  DATABHLP_GET(int32,l,CEL_DATA_LONG)
  DATABHLP_GET(uint32,ul,CEL_DATA_ULONG)
  DATABHLP_GET(float,f,CEL_DATA_FLOAT)
  DATABHLP_GET(iCelEntity*,ent,CEL_DATA_ENTITY)
  DATABHLP_GET(iCelPropertyClass*,pc,CEL_DATA_PCLASS)
  DATABHLP_GET(iCelDataBuffer*,db,CEL_DATA_BUFFER)
  DATABHLP_GET(bool,bo,CEL_DATA_BOOL)
#undef DATABHLP_GET
  
  bool Get (csColor& v)
  {
    celData* data = databuf->GetData(j++);
    if (data->type != CEL_DATA_COLOR) { rc=false; return false; }
    v.red = data->value.col.red;
    v.green = data->value.col.green;
    v.blue = data->value.col.blue;
    return true;
  }
  bool Get (csVector2& v)
  {
    celData* data = databuf->GetData(j++);
    if (data->type != CEL_DATA_VECTOR2) { rc=false; return false; }
    v.x = data->value.v.x;
    v.y = data->value.v.y;
    return true;
  }
  bool Get (csVector3& v)
  {
    celData* data = databuf->GetData(j++);
    if (data->type != CEL_DATA_VECTOR3) { rc=false; return false; }
    v.x = data->value.v.x;
    v.y = data->value.v.y;
    v.z = data->value.v.z;
    return true;
  }
  bool Get (const char*& v)
  {
    celData* data = databuf->GetData(j++);
    if (data->type != CEL_DATA_STRING) { rc=false; return false; }
    v = *data->value.s;
    return true;
  }
  bool GetAction (const char*& v)
  {
    celData* data = databuf->GetData(j++);
    if (data->type != CEL_DATA_ACTION) { rc=false; return false; }
    v=*data->value.s;
    return true;
  }
  
protected:
  iCelDataBuffer* databuf;
  size_t j;
  bool rc;
};

#endif

