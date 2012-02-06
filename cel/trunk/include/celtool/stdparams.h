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
#include "csutil/refarr.h"
#include "behaviourlayer/behave.h"
#include "tools/expression.h"

#include "celtool/celtoolextern.h"

struct iObjectRegistry;
struct iDocumentNode;
struct iParameter;
struct iParameterManager;
struct iCelPlLayer;
class celVariableParameterBlock;

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

/// The description of a parameter
struct celParSpec
{
  /// The type of the parameter
  celDataType type;

  /// The ID of the parameter
  csStringID id;

  /// The value of the parameter
  csString value;

  celParSpec () { }
  celParSpec (celDataType type, csStringID id, const char* value) :
    type (type), id (id), value (value) { }
};

/**
 * Parameter tools.
 */
class CEL_CELTOOL_EXPORT celParameterTools
{
public:
  /**
   * Parse the parameters in a 'params' node. and return
   * an implementation of iCelParameterBlock.
   * After the node comes a list of parameter pairs (a string and a
   * celData pointer) with the parameter name being equal to CEL_PARAM_END.
   * These parameter pairs are also added to the given parameter block by
   * default.
   */
  static csRef<iCelParameterBlock> ParseParams (iObjectRegistry* object_reg,
      iDocumentNode* node, ...);

  /**
   * Get the value of this data type as a string.
   * Returns false if there is no sensible way to convert to string.
   */
  static bool ToString (const celData& in, csString& out);

  /**
   * Get the value of this data type as a long.
   * Returns false if there is no sensible way to convert to long.
   */
  static bool ToLong (const celData& in, long& out);

  /**
   * Get the value of this data type as a bool.
   * Returns false if there is no sensible way to convert to bool.
   */
  static bool ToBool (const celData& in, bool& out);

  /**
   * Get the value of this data type as a float.
   * Returns false if there is no sensible way to convert to float.
   */
  static bool ToFloat (const celData& in, float& out);

  /**
   * Get the value of this data type as a vector2.
   * Returns false if there is no sensible way to convert to vector2.
   */
  static bool ToVector2 (const celData& in, csVector2& out);

  /**
   * Get the value of this data type as a vector3.
   * Returns false if there is no sensible way to convert to vector3.
   */
  static bool ToVector3 (const celData& in, csVector3& out);

  /**
   * Get the value of this data type as a vector4.
   * Returns false if there is no sensible way to convert to vector4.
   */
  static bool ToVector4 (const celData& in, csVector4& out);

  /**
   * Get the value of this data type as a color.
   * Returns false if there is no sensible way to convert to color.
   */
  static bool ToColor (const celData& in, csColor& out);

  /**
   * Get the value of this data type as a color4.
   * Returns false if there is no sensible way to convert to color4.
   */
  static bool ToColor4 (const celData& in, csColor4& out);

  /**
   * Convert one celData to another type (if possible).
   * Returns false if the conversion is not possible.
   * Only the following types are supported as destination type:
   * - CEL_DATA_BOOL
   * - CEL_DATA_LONG
   * - CEL_DATA_FLOAT
   * - CEL_DATA_VECTOR2
   * - CEL_DATA_VECTOR3
   * - CEL_DATA_VECTOR4
   * - CEL_DATA_COLOR
   * - CEL_DATA_COLOR4
   * - CEL_DATA_STRING
   */
  static bool Convert (const celData& in, celDataType type, celData& out);

  /**
   * Parse an XML node and fill a parameter specification array.
   * The parameters array will be cleared first.
   * Returns false on failure (error already reported).
   */
  static bool ParseParSpecBlock (iObjectRegistry* object_reg,
    iDocumentNode* node, csArray<celParSpec>& parameters);

  /**
   * Write a parameter specification array to XML.
   */
  static bool WriteParSpecBlock (iObjectRegistry* object_reg,
    iDocumentNode* node, const csArray<celParSpec>& parameters);

  /**
   * This is a convenience function to get a parameter block which
   * knows how to recognize parameter usage (starting with '$' or '@') and will in
   * that case try to resolve the parameter by finding it in 'params'.
   * \param params is an input block of parameters.
   * \param paramspec is the parameter specifications and unparsed values.
   * \param dyn_parameters will be filled with the parameters.
   */
  static csPtr<celVariableParameterBlock> GetParameterBlock (
        iParameterManager* pm,
  	iCelParameterBlock* params,
	const csArray<celParSpec>& parameters,
	csRefArray<iParameter>& dyn_parameters);

   /**
   * Fill in the dynamic parameters in a parameter block. Return whether or
   * not the filling has been made.
   * \param params is the parameter block given to the reward.
   * \param msg_params is the resolved parameter block as returned by
   * GetParameterBlock().
   * \param parameters is the parameter specifications and unparsed values.
   * \param dyn_parameters is an array with the dynamic parameters.
   */
  static bool FillParameterBlock (
        iCelPlLayer* pl,
        iCelParameterBlock* params,
	celVariableParameterBlock* act_params,
	const csArray<celParSpec>& parameters,
	const csRefArray<iParameter>& dyn_parameters);

  /**
   * Get a string representation of 'data' useful for debugging.
   */
  static csString GetDebugData (const celData* data);

  /**
   * Debugging utility to dump a parameter block.
   * The 'pl' is optional but will be used to get the parameter names if given.
   */
  static void Dump (const char* title,
      iCelParameterBlock* params, iCelPlLayer* pl = 0);
};

struct celVariable
{
  csStringID id;
  celData data;

  celVariable () { }
  celVariable (csStringID id, const celData& data) : id (id), data (data) { }
};

/**
 * Variable parameter block implementation.
 */
class CEL_CELTOOL_EXPORT celVariableParameterBlock : public scfImplementation1<
	celVariableParameterBlock,iCelParameterBlock>
{
private:
  csArray<celVariable> vars;

public:
  celVariableParameterBlock (int capacity = 0) :
    scfImplementationType (this), vars (capacity)
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
      celDataType type;
      for (size_t idx = 0; idx < other->GetParameterCount (); idx++)
      {
	AddParameter (other->GetParameterDef (idx, type), *other->GetParameterByIndex (idx));
      }
    }
  }
  virtual ~celVariableParameterBlock ()
  {
  }

  void Clear ()
  {
    vars.Empty ();
  }

  /**
   * Add a new parameter and returns the index.
   */
  size_t AddParameter (csStringID id, const celData& data)
  {
    celVariable var (id, data);
    return vars.Push (var);
  }

  /**
   * Add a new parameter and return a reference to the data.
   */
  celData& AddParameter (csStringID id)
  {
    celVariable var;
    var.id = id;
    size_t idx = vars.Push (var);
    return vars[idx].data;
  }

  void SetParameterDef (size_t idx, csStringID id)
  {
    vars.GetExtend (idx).id = id;
  }
  celData& GetParameter (size_t idx)
  {
    return vars.GetExtend (idx).data;
  }

  virtual size_t GetParameterCount () const { return vars.GetSize (); }
  virtual csStringID GetParameterDef (size_t idx, celDataType& t) const
  {
    if (/*idx < 0 || */idx >= vars.GetSize ())
    {
      t = CEL_DATA_NONE;
      return csInvalidStringID;
    }
    t = vars[idx].data.type;
    return vars[idx].id;
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    size_t i;
    for (i = 0 ; i < vars.GetSize () ; i++)
      if (id == vars[i].id)
        return &vars[i].data;
    return 0;
  }
  virtual const celData* GetParameterByIndex (size_t idx) const
  {
    return (idx >= vars.GetSize ()) ? 0 : &vars[idx].data;
  }

  celData* GetParameter (csStringID id)
  {
    size_t i;
    for (i = 0 ; i < vars.GetSize () ; i++)
      if (id == vars[i].id)
        return &vars[i].data;
    return 0;
  }

  /**
   * Merge the given parameter block with this one.
   * Existing properties will be overwritten.
   */
  void Merge (iCelParameterBlock* params);
};

/**
 * Specific parameter block implementation that supports an entity
 * parameter using "this".
 */
class CEL_CELTOOL_EXPORT celEntityParameterBlock : public scfImplementation1<
	celEntityParameterBlock, iCelParameterBlock>
{
private:
  static csStringID thisID;
  celData thisData;
  iCelEntity* entity;

public:
  celEntityParameterBlock (iCelPlLayer* pl, iCelEntity* entity);
  virtual ~celEntityParameterBlock () { }

  virtual size_t GetParameterCount () const { return 1; }
  virtual csStringID GetParameterDef (size_t idx, celDataType& t) const
  {
    if (idx == 0) { t = CEL_DATA_ENTITY; return thisID; }
    return csInvalidStringID;
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    if (id == thisID) return &thisData;
    return 0;
  }
  virtual const celData* GetParameterByIndex (size_t idx) const
  {
    if (idx == 0) return &thisData;
    return 0;
  }
};

/**
 * Parameter block implementation if only one parameter is desired.
 */
class CEL_CELTOOL_EXPORT celOneParameterBlock : public scfImplementation1<
	celOneParameterBlock, iCelParameterBlock>
{
private:
  csStringID id;
  celData data;

public:
  celOneParameterBlock () : scfImplementationType (this)
  {
  }
  /// Conveniance method to have a parameter block with one parameter of type string.
  celOneParameterBlock (csStringID id, const char* str)
    : scfImplementationType (this)
  {
    SetParameterDef (id);
    GetParameter (0).Set (str);
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
  virtual csStringID GetParameterDef (size_t idx, celDataType& t) const
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
class CEL_CELTOOL_EXPORT celCombineParameterBlock : public scfImplementation1<
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
  virtual csStringID GetParameterDef (size_t idx, celDataType& t) const
  {
    if (idx < b1->GetParameterCount ())
    {
      return b1->GetParameterDef (idx, t);
    }
    else if (b2)
    {
      return b2->GetParameterDef (idx-b1->GetParameterCount (), t);
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
  csStringID source;
  csStringID dest;
  csRef<iCelExpression> expression;
};

/**
 * Parameter block implementation which supports parameter mapping
 * and expressions.
 */
class CEL_CELTOOL_EXPORT celMappedParameterBlock : public celVariableParameterBlock
{
public:
  celMappedParameterBlock (iCelEntity* entity, iCelParameterBlock* params,
      const csArray<celParameterMapping>& mapping)
  {
    for (size_t mi = 0 ; mi < mapping.GetSize () ; mi++)
    {
      const celParameterMapping& m = mapping[mi];
      SetParameterDef (mi, m.dest);
      if (m.expression)
	m.expression->Execute (entity, GetParameter (mi), params);
      else
      {
        for (size_t i = 0 ; i < params->GetParameterCount () ; i++)
        {
          celDataType t;
          csStringID id = params->GetParameterDef (i, t);
          if (id == m.source)
	  {
	    GetParameter (mi) = *params->GetParameterByIndex (i);
	    break;
	  }
        }
      }
    }
    for (size_t i = 0 ; i < params->GetParameterCount () ; i++)
    {
      celDataType t;
      csStringID id = params->GetParameterDef (i, t);
      size_t idx = i+mapping.GetSize ();
      SetParameterDef (idx, id);
      const celData* data = params->GetParameterByIndex (i);
      if (data)
        GetParameter (idx) = *data;
    }
  }
  virtual ~celMappedParameterBlock ()
  {
  }
};

#endif // __CEL_CELTOOL_PARAMS__

