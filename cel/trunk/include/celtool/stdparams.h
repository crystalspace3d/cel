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
#include "tools/expression.h"

// The following macros will set 'var' to the required variable and
// 'p_var' will be made to 0 if there is a failure.
#define CEL_FETCH_STRING_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  const char* var = 0; \
  if (p_##var && p_##var->type == CEL_DATA_STRING) { \
    var = p_##var->value.s->GetData (); \
  } else { p_##var = 0; }
#define CEL_FETCH_VECTOR2_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  csVector2 var; \
  if (p_##var && p_##var->type == CEL_DATA_VECTOR2) { \
    var.Set (p_##var->value.v.x, p_##var->value.v.y); \
  } else { p_##var = 0; }
#define CEL_FETCH_VECTOR3_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  csVector3 var; \
  if (p_##var && p_##var->type == CEL_DATA_VECTOR3) { \
    var.Set (p_##var->value.v.x, p_##var->value.v.y, p_##var->value.v.z); \
  } else { p_##var = 0; }
#define CEL_FETCH_VECTOR4_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  csVector4 var; \
  if (p_##var && p_##var->type == CEL_DATA_VECTOR4) { \
    var.Set (p_##var->value.v.x, p_##var->value.v.y, p_##var->value.v.z, p_##var->value.v.w); \
  } else { p_##var = 0; }
#define CEL_FETCH_COLOR_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  csColor var; \
  if (p_##var && p_##var->type == CEL_DATA_COLOR) { \
    var.Set (p_##var->value.col.red, p_##var->value.col.green, p_##var->value.col.blue); \
  } else { p_##var = 0; }
#define CEL_FETCH_COLOR4_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  csColor4 var; \
  if (p_##var && p_##var->type == CEL_DATA_COLOR4) { \
    var.Set (p_##var->value.col.red, p_##var->value.col.green, p_##var->value.col.blue, p_##var->value.col.alpha); \
  } else { p_##var = 0; }
#define CEL_FETCH_FLOAT_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  float var = 0.0f; \
  if (p_##var) { \
    if (p_##var->type == CEL_DATA_FLOAT) \
      var = p_##var->value.f; \
    else if (p_##var->type == CEL_DATA_LONG) \
      var = float (p_##var->value.l); \
    else p_##var = 0; \
  }
#define CEL_FETCH_LONG_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  long var = 0; \
  if (p_##var) { \
    if (p_##var->type == CEL_DATA_LONG) \
      var = p_##var->value.l; \
    else if (p_##var->type == CEL_DATA_FLOAT) \
      var = long (p_##var->value.f); \
    else p_##var = 0; \
  }
#define CEL_FETCH_BOOL_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  bool var = false; \
  if (p_##var) { \
    if (p_##var->type == CEL_DATA_BOOL) \
      var = p_##var->value.bo; \
    else if (p_##var->type == CEL_DATA_LONG) \
    var =  ((p_##var->value.l)? true : false); \
    else p_##var = 0; \
  }
#define CEL_FETCH_PCLASS_PAR(var,params,id) \
  const celData* p_##var = params ? params->GetParameter (id) : 0; \
  iCelPropertyClass* var = 0; \
  if (p_##var) { \
    if (p_##var->type == CEL_DATA_PCLASS) \
      var = p_##var->value.pc; \
  }

/**
 * Generic parameter block implementation.
 */
class celGenericParameterBlock : public scfImplementation1<
	celGenericParameterBlock, iCelParameterBlock>
{
private:
  size_t count;
  csStringID* ids;
  celData* data;

public:
  celGenericParameterBlock (size_t count) :
    scfImplementationType (this)
  {
    celGenericParameterBlock::count = count;
    ids = new csStringID[count];
    data = new celData[count];
  }
  virtual ~celGenericParameterBlock ()
  {
    delete[] ids;
    delete[] data;
  }

  void SetParameterDef (size_t idx, csStringID id)
  {
    ids[idx] = id;
  }
  celData& GetParameter (size_t idx) { return data[idx]; }

  virtual size_t GetParameterCount () const { return count; }
  virtual csStringID GetParameter (size_t idx, celDataType& t) const
  {
    if (/*idx < 0 || */idx >= count)
    {
      t = CEL_DATA_NONE;
      return csInvalidStringID;
    }
    t = data[idx].type;
    return ids[idx];
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    size_t i;
    for (i = 0 ; i < count ; i++)
      if (id == ids[i])
        return &data[i];
    return 0;
  }
  virtual const celData* GetParameterByIndex (size_t idx) const
  {
    return (idx >= count) ? 0 : &data[idx];
  }
};

/**
 * Variable parameter block implementation.
 */
class celVariableParameterBlock : public scfImplementation1<
	celVariableParameterBlock,iCelParameterBlock>
{
private:
  csArray<csStringID> ids;
  csArray<celData> data;

public:
  celVariableParameterBlock () : scfImplementationType (this)
  {
  }
  /**
   * Copy constructor.
   */
  celVariableParameterBlock (iCelParameterBlock* other) :
    scfImplementationType (this)
  {
    if (other != 0)
    {
      csStringID id;
      celDataType type;
      for (size_t idx = 0; idx < other->GetParameterCount (); idx++)
      {
        id = other->GetParameter (idx, type);
        SetParameterDef (idx, id);
        data.GetExtend (idx) = *other->GetParameter (id);
      }
    }
  }
  virtual ~celVariableParameterBlock ()
  {
  }

  void SetParameterDef (size_t idx, csStringID id)
  {
    ids.GetExtend (idx) = id;
  }
  celData& GetParameter (size_t idx) { return data.GetExtend (idx); }

  virtual size_t GetParameterCount () const { return data.GetSize (); }
  virtual csStringID GetParameter (size_t idx, celDataType& t) const
  {
    if (/*idx < 0 || */idx >= data.GetSize ())
    {
      t = CEL_DATA_NONE;
      return csInvalidStringID;
    }
    t = data[idx].type;
    return ids[idx];
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    size_t i;
    for (i = 0 ; i < data.GetSize () ; i++)
      if (id == ids[i])
        return &data[i];
    return 0;
  }
  virtual const celData* GetParameterByIndex (size_t idx) const
  {
    return (idx >= data.GetSize ()) ? 0 : &data[idx];
  }
};

/**
 * Parameter block implementation if only one parameter is desired.
 */
class celOneParameterBlock : public scfImplementation1<
	celOneParameterBlock, iCelParameterBlock>
{
private:
  csStringID id;
  celData data;

public:
  celOneParameterBlock () : scfImplementationType (this)
  {
  }
  virtual ~celOneParameterBlock ()
  {
  }

  void SetParameterDef (csStringID id)
  {
    celOneParameterBlock::id = id;
  }
  celData& GetParameter (int) { return data; }

  virtual size_t GetParameterCount () const { return 1; }
  virtual csStringID GetParameter (size_t idx, celDataType& t) const
  {
    if (idx != 0)
    {
      t = CEL_DATA_NONE;
      return csInvalidStringID;
    }
    t = data.type;
    return celOneParameterBlock::id;
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    if (id != celOneParameterBlock::id) return 0;
    return &data;
  }
  virtual const celData* GetParameterByIndex (size_t idx) const
  {
    return (idx != 0) ? 0 : &data;
  }
};

/**
 * Parameter block implementation to combine two existing parameter
 * blocks.
 */
class celCombineParameterBlock : public scfImplementation1<
	celCombineParameterBlock, iCelParameterBlock>
{
private:
  csRef<iCelParameterBlock> b1;
  csRef<iCelParameterBlock> b2;

public:
  /**
   * The second parameter block is optional (can be null).
   */
  celCombineParameterBlock (iCelParameterBlock* b1, iCelParameterBlock* b2)
    : scfImplementationType (this), b1 (b1), b2 (b2)
  {
  }
  virtual ~celCombineParameterBlock ()
  {
  }
  void SetParameterBlock1 (iCelParameterBlock* b1)
  {
    celCombineParameterBlock::b1 = b1;
  }
  void SetParameterBlock2 (iCelParameterBlock* b2)
  {
    celCombineParameterBlock::b2 = b2;
  }

  virtual size_t GetParameterCount () const
  {
    return b1->GetParameterCount () + (b2 ? b2->GetParameterCount () : 0);
  }
  virtual csStringID GetParameter (size_t idx, celDataType& t) const
  {
    if (idx < b1->GetParameterCount ())
    {
      return b1->GetParameter (idx, t);
    }
    else if (b2)
    {
      return b2->GetParameter (idx-b1->GetParameterCount (), t);
    }
    else
    {
      return csInvalidStringID;
    }
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    const celData* data = b1->GetParameter (id);
    if (data) return data;
    if (!b2) return 0;
    return b2->GetParameter (id);
  }
  virtual const celData* GetParameterByIndex (size_t idx) const
  {
    if (idx < b1->GetParameterCount ())
    {
      return b1->GetParameterByIndex (idx);
    }
    else if (b2)
    {
      return b2->GetParameterByIndex (idx-b1->GetParameterCount ());
    }
    else
    {
      return 0;
    }
  }
};

struct celParameterMapping
{
  csString source;
  csString dest;
  csRef<iCelExpression> expression;
};

#if 0

// Below is work in progress @@@

/**
 * Parameter block implementation which supports parameter mapping
 * and expressions.
 */
class celMappedParameterBlock : public scfImplementation1<
	celMappedParameterBlock, iCelParameterBlock>
{
private:
  csRef<iCelParameterBlock> params;
  const csArray<celParameterMapping>& mapping;

public:
  celMappedParameterBlock (iCelParameterBlock* params,
      const csArray<celParameterMapping>& mapping)
    : scfImplementationType (this), params (params),
      mapping (mapping)
  {
    for (size_t i = 0 ; i < params->GetParameterCount () ; i++)

  }
  virtual ~celMappedParameterBlock ()
  {
  }

  virtual size_t GetParameterCount () const
  {
    return params->GetParameterCount () + (b2 ? b2->GetParameterCount () : 0);
  }
  virtual const char* GetParameter (size_t idx, csStringID& id,
  	celDataType& t) const
  {
    if (idx < b1->GetParameterCount ())
    {
      return b1->GetParameter (idx, id, t);
    }
    else if (b2)
    {
      return b2->GetParameter (idx-b1->GetParameterCount (), id, t);
    }
    else
    {
      return 0;
    }
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    const celData* data = b1->GetParameter (id);
    if (data) return data;
    if (!b2) return 0;
    return b2->GetParameter (id);
  }
  virtual const celData* GetParameterByIndex (size_t idx) const
  {
    if (idx < b1->GetParameterCount ())
    {
      return b1->GetParameterByIndex (idx);
    }
    else if (b2)
    {
      return b2->GetParameterByIndex (idx-b1->GetParameterCount ());
    }
    else
    {
      return 0;
    }
  }
};
#endif

#endif // __CEL_CELTOOL_PARAMS__

