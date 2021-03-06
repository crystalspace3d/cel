/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin

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

#include "cssysdef.h"
#include "plugins/tools/parameters/cel_parameters.h"

#include "csutil/scfstr.h"
#include "iutil/objreg.h"
#include "iutil/plugin.h"
#include "celtool/stdparams.h"
#include "csutil/scanstr.h"
#include "physicallayer/entity.h"
#include "tools/expression.h"

//---------------------------------------------------------------------------

//CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celParameterManager)

//---------------------------------------------------------------------------

static const celData celDataNone;

static const char* ToString (csString& str, const celData* data)
{
  celParameterTools::ToString (*data, str);
  return str;
}

static int32 ToLong (const celData* data)
{
  long l;
  celParameterTools::ToLong (*data, l);
  return l;
}

static float ToFloat (const celData* data)
{
  float f;
  celParameterTools::ToFloat (*data, f);
  return f;
}

static bool ToBool (const celData* data)
{
  bool b;
  celParameterTools::ToBool (*data, b);
  return b;
}

//---------------------------------------------------------------------------

bool celParameterManager::Initialize (iObjectRegistry* r)
{
  object_reg = r;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  return true;
}

iCelExpressionParser* celParameterManager::GetParser ()
{
  if (expparser) return expparser;
  csRef<iObjectRegistryIterator> it = object_reg->Get (
      scfInterfaceTraits<iCelExpressionParser>::GetID (),
      scfInterfaceTraits<iCelExpressionParser>::GetVersion ());
  iBase* b = it->Next ();
  csRef<iCelExpressionParser> parser;
  if (b)
  {
    parser = scfQueryInterface<iCelExpressionParser> (b);
  }
  if (!parser)
  {
    csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
	object_reg);
    parser = csLoadPlugin<iCelExpressionParser> (plugmgr,
      "cel.behaviourlayer.xml");
    if (!parser)

    {
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.parameters.manager",
		"Can't find the expression parser!");
      return 0;
    }
    object_reg->Register (parser, "iCelExpressionParser");
  }
  expparser = parser;
  return expparser;
}

csPtr<iParameter> celParameterManager::GetParameter (
  	iCelParameterBlock* params,
	const char* param,
        celDataType type)
{
  celData data;
  ResolveParameterData (data, params, param);
  if (data.type == CEL_DATA_NONE) return new celConstantParameter ();
  if (data.type == CEL_DATA_STRING)
  {
    const char* val = data.value.s->GetData ();
    if (!val)
      return new celConstantParameter ();
    else if (*val == '@' && *(val+1) != '@')
    {
      csStringID dynamic_id = pl->FetchStringID (val+1);
      return new celDynamicParameter (object_reg, dynamic_id, val+1, type, *val);
    }
    else if (*val == '=' && *(val+1) != '=')
    {
      csRef<iCelExpression> expression = GetParser ()->Parse (val+1);
      if (!expression)
      {
        csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		  "cel.parameters.manager",
		  "Can't parse expression '%s'!", val+1);
        return 0;
      }
      // We are looking for 'this' in the parameter block. If we can find it
      // then it indicates the name of the entity. We will find the entity for
      // the expression so that the expression can show things local to the
      // entity (or access properties from the current entity).
      csStringID thisid = pl->FetchStringID ("this");
      iCelEntity* entity = 0;
      for (size_t i = 0 ; i < params->GetParameterCount () ; i++)
      {
        celDataType t;
        csStringID id = params->GetParameterDef (i, t);
        if (thisid == id)
        {
          csString name;
	  const celData* thisData = params->GetParameterByIndex (i);
	  if (thisData->type == CEL_DATA_ENTITY) entity = thisData->value.ent;
	  else
	  {
            celParameterTools::ToString (*thisData, name);
            entity = pl->FindEntity (name);
	  }
          if (entity) break;
        }
      }
      celExpressionParameter* par = new celExpressionParameter (object_reg, entity, expression, val+1, type);
      if (rememberExpression)
        par->SetOriginalExpression (val);
      return par;
    }
    return new celConstantParameter (val, type);
  }
  else
  {
    return new celConstantParameter (data, type);
  }
}

csPtr<iParameter> celParameterManager::GetParameter (const char* val,
    celDataType type)
{
  if (val == 0) return new celConstantParameter ();
  if ((*val == '@' && *(val+1) != '@') || (*val == '$' && *(val+1) != '$'))
  {
    csStringID dynamic_id = pl->FetchStringID (val+1);
    return new celDynamicParameter (object_reg, dynamic_id, val+1, type, *val);
  }
  else if (*val == '=' && *(val+1) != '=')
  {
    csRef<iCelExpression> expression = GetParser ()->Parse (val+1);
    if (!expression)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.parameters.manager",
		"Can't parse expression '%s'!", val+1);
      return 0;
    }
    iCelEntity* entity = 0;
    // @@@ Can we do this somehow?
#if 0
    // We are looking for 'this' in the parameter block. If we can find it
    // then it indicates the name of the entity. We will find the entity for
    // the expression so that the expression can show things local to the
    // entity (or access properties from the current entity).
    csStringID thisid = pl->FetchStringID ("this");
    for (size_t i = 0 ; i < params->GetParameterCount () ; i++)
    {
      celDataType t;
      csStringID id = params->GetParameterDef (i, t);
      if (thisid == id)
      {
        csString name;
        celParameterTools::ToString (*params->GetParameter (i), name);
        entity = pl->FindEntity (name);
	break;
      }
    }
#endif
    celExpressionParameter* par = new celExpressionParameter (object_reg, entity, expression, val+1, type);
    if (rememberExpression)
      par->SetOriginalExpression (val);
    return par;
  }
  return new celConstantParameter (val, type);
}

void celParameterManager::ResolveParameterData (
    celData& out,
    iCelParameterBlock* params,
    const char* param)
{
  if (param == 0) { out.Clear (); return; }
  if (*param != '$') { out.Set (param); return; }
  if (*(param+1) == '$') { out.Set (param+1); }
  csStringID id = pl->FetchStringID (param+1);
  const celData* data = params->GetParameter (id);
  if (!data)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.parameters.manager",
		"Can't resolve parameter %s!", param);
    return;
  }

  out = *data;
}

const char* celParameterManager::ResolveParameter (
  	iCelParameterBlock* params,
	const char* param)
{
  if (param == 0) return param;
  if (*param != '$') return param;
  if (*(param+1) == '$') return param+1;	// Double $ means to quote the '$'.
  csStringID id = pl->FetchStringID (param+1);
  const celData* data = params->GetParameter (id);
  if (!data)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.parameters.manager",
		"Can't resolve parameter %s!", param);
    return 0;
  }
  celParameterTools::ToString (*data, str);
  return str;
}

const char* celParameterManager::ResolveEntityParameter (
  	iCelParameterBlock* params,
	const char* param,
        uint& entid)
{
  entid = ~0;
  if (param == 0) return 0;
  if (*param != '$') return param;
  if (*(param+1) == '$') return param+1;

  csStringID id = pl->FetchStringID (param+1);
  const celData* data = params->GetParameter (id);
  if (!data)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.parameters.manager",
		"Can't resolve parameter %s!", param);
    return 0;
  }
  if (data->type == CEL_DATA_ENTITY)
  {
    entid = data->value.ent->GetID ();
    return 0;
  }
  if (data->type == CEL_DATA_STRING)
  {
    celParameterTools::ToString (*data, str);
    return str;
  }
  long lentid;
  celParameterTools::ToLong (*data, lentid);
  entid = (uint)lentid;
  return 0;
}

iCelEntity* celParameterManager::ResolveEntityParameter (
      iCelPlLayer* pl,
      iCelParameterBlock* params, iParameter* param,
      iCelEntity* ent)
{
  const celData* data = param->GetData (params);
  if (data->type == CEL_DATA_ENTITY) return data->value.ent;
  if (data->type == CEL_DATA_LONG)
  {
    long entid;
    celParameterTools::ToLong (*data, entid);
    return pl->GetEntity ((uint)entid);
  }

  bool changed;
  const char* e = param->Get (params, changed);
  if (changed) ent = 0;
  if (!ent) ent = pl->FindEntity (e);
  return ent;
}

csRef<iCelEntityList> celParameterManager::ResolveEntityListParameter (
      iCelPlLayer* pl,
      iCelParameterBlock* params, iParameter* param)
{
  csRef<iCelEntityList> list = pl->CreateEmptyEntityList ();
  const celData* data = param->GetData (params);
  if (data->type == CEL_DATA_ENTITY)
  {
    list->Add (data->value.ent);
  }
  else
  {
    bool changed;
    const char* e = param->Get (params, changed);
    csStringArray array (e, ",");
    for (size_t i = 0 ; i < array.GetSize () ; i++)
    {
      iCelEntity* ent = pl->FindEntity (array.Get (i));
      if (ent)
        list->Add (ent);
    }
  }
  return list;
}

//---------------------------------------------------------------------------

celConstantParameter::celConstantParameter (const celData& in, celDataType type) :
    scfImplementationType (this)
{
  if (type == CEL_DATA_NONE) type = in.type;
  celParameterTools::Convert (in, type, data);
}

celConstantParameter::celConstantParameter (const char* c, celDataType type) :
    scfImplementationType (this)
{
  if (type != CEL_DATA_NONE)
  {
    celData in; in.Set (c);
    celParameterTools::Convert (in, type, data);
  }
  else
  {
    data.Set (c);
  }
}

const char* celConstantParameter::Get (iCelParameterBlock*)
{
  return ToString (str, &data);
}

int32 celConstantParameter::GetLong (iCelParameterBlock*)
{
  return ToLong (&data);
}

float celConstantParameter::GetFloat (iCelParameterBlock*)
{
  return ToFloat (&data);
}

bool celConstantParameter::GetBool (iCelParameterBlock*)
{
  return ToBool (&data);
}

//---------------------------------------------------------------------------

const celData* celDynamicParameter::GetData (iCelParameterBlock* params)
{
  if (!params)
  {
    csReport (object_reg,
	CS_REPORTER_SEVERITY_ERROR, "cel.parameters.dynamic",
	"Cannot resolve dynamic parameter '%s' (no parameters given)!",
	(const char*)parname);
    return &celDataNone;
  }

  const celData* data = params->GetParameter (dynamic_id);
  if (!data)
  {
    csReport (object_reg,
	CS_REPORTER_SEVERITY_ERROR, "cel.parameters.dynamic",
	"Cannot resolve dynamic parameter '%s'!", (const char*)parname);
    return &celDataNone;
  }
  if (desiredType == data->type || desiredType == CEL_DATA_NONE)
    return data;
  else
  {
    celParameterTools::Convert (*data, desiredType, converted);
    return &converted;
  }
}

const char* celDynamicParameter::Get (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToString (str, data);
}

int32 celDynamicParameter::GetLong (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToLong (data);
}

float celDynamicParameter::GetFloat (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToFloat (data);
}

bool celDynamicParameter::GetBool (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToBool (data);
}

const char* celDynamicParameter::Get (iCelParameterBlock* params,
    bool& changed)
{
  const char* s = Get (params);
  if (s == 0)
  {
    changed = !oldvalue.IsEmpty ();
    return s;
  }
  changed = oldvalue != s;
  oldvalue = s;
  return s;
}

//---------------------------------------------------------------------------

const celData* celExpressionParameter::GetData (iCelParameterBlock* params)
{
  if (!expression->Execute (entity, data, params))
  {
    csReport (object_reg,
	CS_REPORTER_SEVERITY_ERROR, "cel.parameters.expression",
	"Cannot execute expression '%s'!", (const char*)parname);
    return &celDataNone;
  }
  if (desiredType != data.type && desiredType != CEL_DATA_NONE)
  {
    celData d = data;
    celParameterTools::Convert (d, desiredType, data);
  }
  return &data;
}

const char* celExpressionParameter::Get (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToString (str, data);
}

int32 celExpressionParameter::GetLong (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToLong (data);
}

float celExpressionParameter::GetFloat (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToFloat (data);
}

bool celExpressionParameter::GetBool (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToBool (data);
}

const char* celExpressionParameter::Get (iCelParameterBlock* params,
    bool& changed)
{
  const char* s = Get (params);
  if (s == 0)
  {
    changed = !oldvalue.IsEmpty ();
    return s;
  }
  changed = oldvalue != s;
  oldvalue = s;
  return s;
}

//---------------------------------------------------------------------------
