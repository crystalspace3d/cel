/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein
  
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

#ifndef __CEL_CELTOOL_PARAMS__
#define __CEL_CELTOOL_PARAMS__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strhash.h"
#include "csutil/util.h"
#include "csutil/array.h"
#include "csutil/stringarray.h"
#include "behaviourlayer/behave.h"

// The following macros will set 'var' to the required variable and
// 'p_var' will be made to 0 if there is a failure.
#define CEL_FETCH_STRING_PAR(var,params,id) \
  const celData* p_##var = params->GetParameter (id); \
  const char* var = 0; \
  if (p_##var && p_##var->type == CEL_DATA_STRING) { \
    var = p_##var->value.s->GetData (); \
  } else { p_##var = 0; }
#define CEL_FETCH_VECTOR3_PAR(var,params,id) \
  const celData* p_##var = params->GetParameter (id); \
  csVector3 var; \
  if (p_##var && p_##var->type == CEL_DATA_VECTOR3) { \
    var.Set (p_##var->value.v.x, p_##var->value.v.y, p_##var->value.v.z); \
  } else { p_##var = 0; }
#define CEL_FETCH_FLOAT_PAR(var,params,id) \
  const celData* p_##var = params->GetParameter (id); \
  float var = 0.0f; \
  if (p_##var) { \
    if (p_##var->type == CEL_DATA_FLOAT) \
      var = p_##var->value.f; \
    else if (p_##var->type == CEL_DATA_LONG) \
      var = float (p_##var->value.l); \
    else p_##var = 0; \
  }
#define CEL_FETCH_LONG_PAR(var,params,id) \
  const celData* p_##var = params->GetParameter (id); \
  long var = 0; \
  if (p_##var) { \
    if (p_##var->type == CEL_DATA_LONG) \
      var = p_##var->value.l; \
    else if (p_##var->type == CEL_DATA_FLOAT) \
      var = long (p_##var->value.f); \
    else p_##var = 0; \
  }
#define CEL_FETCH_BOOL_PAR(var,params,id) \
  const celData* p_##var = params->GetParameter (id); \
  bool var = false; \
  if (p_##var) { \
    if (p_##var->type == CEL_DATA_BOOL) \
      var = p_##var->value.bo; \
    else if (p_##var->type == CEL_DATA_LONG) \
      var = bool (p_##var->value.l); \
    else p_##var = 0; \
  }

/**
 * Generic parameter block implementation.
 */
class celGenericParameterBlock : public iCelParameterBlock
{
private:
  size_t count;
  csStringID* ids;
  celData* data;
  char** names;

public:
  celGenericParameterBlock (size_t count)
  {
    SCF_CONSTRUCT_IBASE (0);
    celGenericParameterBlock::count = count;
    ids = new csStringID[count];
    data = new celData[count];
    names = new char*[count];
    memset (names, 0, sizeof (char*)*count);
  }
  virtual ~celGenericParameterBlock ()
  {
    delete[] ids;
    delete[] data;
    size_t i;
    for (i = 0 ; i < count ; i++)
      delete[] names[i];
    delete[] names;
    SCF_DESTRUCT_IBASE ();
  }

  void SetParameterDef (size_t idx, csStringID id, const char* parname)
  {
    ids[idx] = id;
    delete[] names[idx];
    names[idx] = csStrNew (parname);
  }
  celData& GetParameter (size_t idx) { return data[idx]; }

  SCF_DECLARE_IBASE;

  virtual size_t GetParameterCount () const { return count; }
  virtual const char* GetParameter (size_t idx, csStringID& id,
  	celDataType& t) const
  {
    if (/*idx < 0 || */idx >= count)
    {
      id = csInvalidStringID;
      t = CEL_DATA_NONE;
      return 0;
    }
    id = ids[idx];
    t = data[idx].type;
    return names[idx];
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    size_t i;
    for (i = 0 ; i < count ; i++)
      if (id == ids[i])
        return &data[i];
    return 0;
  }
};

/**
 * Variable parameter block implementation.
 */
class celVariableParameterBlock : public iCelParameterBlock
{
private:
  csArray<csStringID> ids;
  csArray<celData> data;
  csStringArray names;

public:
  celVariableParameterBlock ()
  {
    SCF_CONSTRUCT_IBASE (0);
  }
  /**
   * Copy constructor.
   */
  celVariableParameterBlock (iCelParameterBlock* other)
  {
    SCF_CONSTRUCT_IBASE (0);
    if (other != 0)
    {
      const char* name = 0;
      csStringID id;
      celDataType type;
      for (size_t idx = 0; idx < other->GetParameterCount (); idx++)
      {
        name = other->GetParameter (idx, id, type);
        SetParameterDef (idx, id, name);
        data.GetExtend (idx) = *other->GetParameter (id);
      }
    }
  }
  virtual ~celVariableParameterBlock ()
  {
    SCF_DESTRUCT_IBASE ();
  }

  void SetParameterDef (size_t idx, csStringID id, const char* parname)
  {
    ids.GetExtend (idx) = id;
    if (idx >= names.Length ())
      names.SetLength (idx+1);
    names.Put (idx, parname);
  }
  celData& GetParameter (size_t idx) { return data.GetExtend (idx); }

  SCF_DECLARE_IBASE;

  virtual size_t GetParameterCount () const { return data.Length (); }
  virtual const char* GetParameter (size_t idx, csStringID& id,
  	celDataType& t) const
  {
    if (/*idx < 0 || */idx >= data.Length ())
    {
      id = csInvalidStringID;
      t = CEL_DATA_NONE;
      return 0;
    }
    id = ids[idx];
    t = data[idx].type;
    return names[idx];
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    size_t i;
    for (i = 0 ; i < data.Length () ; i++)
      if (id == ids[i])
        return &data[i];
    return 0;
  }
};

/**
 * Parameter block implementation if only one parameter is desired.
 */
class celOneParameterBlock : public iCelParameterBlock
{
private:
  csStringID id;
  celData data;
  char* name;

public:
  celOneParameterBlock ()
  {
    SCF_CONSTRUCT_IBASE (0);
    name = 0;
  }
  virtual ~celOneParameterBlock ()
  {
    delete[] name;
    SCF_DESTRUCT_IBASE ();
  }

  void SetParameterDef (csStringID id, const char* parname)
  {
    celOneParameterBlock::id = id;
    delete[] name;
    name = csStrNew (parname);
  }
  celData& GetParameter (int) { return data; }

  SCF_DECLARE_IBASE;

  virtual size_t GetParameterCount () const { return 1; }
  virtual const char* GetParameter (size_t idx, csStringID& id,
  	celDataType& t) const
  {
    if (idx != 0)
    {
      id = csInvalidStringID;
      t = CEL_DATA_NONE;
      return 0;
    }
    id = celOneParameterBlock::id;
    t = data.type;
    return name;
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    if (id != celOneParameterBlock::id) return 0;
    return &data;
  }
};

#endif // __CEL_CELTOOL_PARAMS__

