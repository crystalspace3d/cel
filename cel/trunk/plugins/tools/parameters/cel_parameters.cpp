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
  switch (data->type)
  {
    case CEL_DATA_STRING: return data->value.s->GetData ();
    case CEL_DATA_BOOL: if (data->value.bo) str = "true"; else str = "false"; break;
    case CEL_DATA_BYTE: str.Format ("%d", data->value.b); break;
    case CEL_DATA_UBYTE: str.Format ("%d", data->value.ub); break;
    case CEL_DATA_WORD: str.Format ("%d", data->value.w); break;
    case CEL_DATA_UWORD: str.Format ("%d", data->value.uw); break;
    case CEL_DATA_LONG: str.Format ("%d", data->value.l); break;
    case CEL_DATA_ULONG: str.Format ("%d", data->value.ul); break;
    case CEL_DATA_FLOAT: str.Format ("%g", data->value.f); break;
    case CEL_DATA_VECTOR2: str.Format ("%g,%g", data->value.v.x, data->value.v.y); break;
    case CEL_DATA_VECTOR3: str.Format ("%g,%g,%g",
				 data->value.v.x, data->value.v.y, data->value.v.z);
			     break;
    case CEL_DATA_VECTOR4: str.Format ("%g,%g,%g,%g", data->value.v.x, data->value.v.y,
				 data->value.v.z, data->value.v.w);
			     break;
    case CEL_DATA_COLOR4: str.Format ("%g,%g,%g,%g", data->value.col.red,
				data->value.col.green, data->value.col.blue, data->value.col.alpha);
			     break;
    case CEL_DATA_PCLASS: str.Format ("pc(%p)", (iCelPropertyClass*)data->value.pc); break;
    case CEL_DATA_IBASE: str.Format ("ibase(%p)", (iBase*)data->value.ibase); break;
    case CEL_DATA_ENTITY: str.Format ("ent('%s')", data->value.ent->GetName ()); break;
    case CEL_DATA_ACTION: str.Format ("action('%s')", data->value.s->GetData ()); break;
    case CEL_DATA_PARAMETER: str.Format ("par('%s')", data->value.par.parname->GetData ()); break;
    case CEL_DATA_NONE: str.Free (); break;
    default: str = "unknown()"; break;
  }
  return str;
}

static int32 ToLong (const celData* data)
{
  switch (data->type)
  {
    case CEL_DATA_STRING: return atol (data->value.s->GetData ());
    case CEL_DATA_BOOL: return data->value.bo;
    case CEL_DATA_BYTE: return data->value.b;
    case CEL_DATA_UBYTE: return data->value.ub;
    case CEL_DATA_WORD: return data->value.w;
    case CEL_DATA_UWORD: return data->value.uw;
    case CEL_DATA_LONG: return data->value.l;
    case CEL_DATA_ULONG: return data->value.ul;
    case CEL_DATA_FLOAT: return (int32) data->value.f;
    default: return 0;
  }
}

static float ToFloat (const celData* data)
{
  switch (data->type)
  {
    case CEL_DATA_STRING: return atof (data->value.s->GetData ());
    case CEL_DATA_BOOL: return (float)data->value.bo;
    case CEL_DATA_BYTE: return (float)data->value.b;
    case CEL_DATA_UBYTE: return (float)data->value.ub;
    case CEL_DATA_WORD: return (float)data->value.w;
    case CEL_DATA_UWORD: return (float)data->value.uw;
    case CEL_DATA_LONG: return (float)data->value.l;
    case CEL_DATA_ULONG: return (float)data->value.ul;
    case CEL_DATA_FLOAT: return data->value.f;
    default: return 0.0f;
  }
}

static bool ToBool (const celData* data)
{
  bool rc;
  switch (data->type)
  {
    case CEL_DATA_STRING: csScanStr (data->value.s->GetData (), "%b", &rc);
			  return rc;
    case CEL_DATA_BOOL: return data->value.bo;
    case CEL_DATA_BYTE: return data->value.b != 0;
    case CEL_DATA_UBYTE: return data->value.ub != 0;
    case CEL_DATA_WORD: return data->value.w != 0;
    case CEL_DATA_UWORD: return data->value.uw != 0;
    case CEL_DATA_LONG: return data->value.l != 0;
    case CEL_DATA_ULONG: return data->value.ul != 0;
    case CEL_DATA_FLOAT: return fabs (data->value.f) > 0.000001;
    case CEL_DATA_PCLASS: return data->value.pc != 0;
    case CEL_DATA_IBASE: return data->value.ibase != 0;
    case CEL_DATA_ENTITY: return data->value.ent != 0;
    default: return false;
  }
}

static csVector2 ToVector2 (const celData* data)
{
  csVector2 v;
  switch (data->type)
  {
    case CEL_DATA_STRING: csScanStr (data->value.s->GetData (), "%f,%f", &v.x, &v.y);
			  return v;
    case CEL_DATA_VECTOR2: v.x = data->value.v.x; v.y = data->value.v.y;
			   return v;
    default: v.x = v.y = 0.0f;
	     return v;
  }
}

static csVector3 ToVector3 (const celData* data)
{
  csVector3 v;
  switch (data->type)
  {
    case CEL_DATA_STRING: csScanStr (data->value.s->GetData (), "%f,%f,%f", &v.x, &v.y, &v.z);
			  return v;
    case CEL_DATA_VECTOR3: v.x = data->value.v.x; v.y = data->value.v.y; v.z = data->value.v.z;
			   return v;
    default: v.x = v.y = v.z = 0.0f;
	     return v;
  }
}

static csColor ToColor (const celData* data)
{
  csColor v;
  switch (data->type)
  {
    case CEL_DATA_STRING: csScanStr (data->value.s->GetData (), "%f,%f,%f",
			      &v.red, &v.green, &v.blue);
			  return v;
    case CEL_DATA_VECTOR2: v.red = data->value.col.red;
			   v.green = data->value.col.green;
			   v.blue = data->value.col.blue;
			   return v;
    default: v.red = v.green = v.blue = 0.0f;
	     return v;
  }
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
  csRef<iObjectRegistryIterator> it = object_reg->Get (
      scfInterfaceTraits<iCelExpressionParser>::GetID (),
      scfInterfaceTraits<iCelExpressionParser>::GetVersion ());
  iBase* b = it->Next ();
  if (b)
  {
    expparser = scfQueryInterface<iCelExpressionParser> (b);
  }
  if (!expparser)
  {
    csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
	object_reg);
    expparser = csLoadPlugin<iCelExpressionParser> (plugmgr,
      "cel.behaviourlayer.xml");
    if (!expparser)

    {
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.parameters.manager",
		"Can't find the expression parser!");
      return 0;
    }
    object_reg->Register (expparser, "iCelExpressionParser");
  }
  return expparser;
}

csPtr<iParameter> celParameterManager::GetParameter (
  	const celParams& params,
	const char* param)
{
  const char* val = ResolveParameter (params, param);
  if (val == 0) return new celConstantParameter ();
  if (*val == '@' && *(val+1) != '@')
  {
    csStringID dynamic_id = pl->FetchStringID (val+1);
    return new celDynamicParameter (object_reg, dynamic_id, val+1);
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
    celParams::ConstGlobalIterator def_it = params.GetIterator ();
    csStringBase it_key;
    iCelEntity* entity = 0;
    while (def_it.HasNext ())
    {
      const char* name = def_it.Next (it_key);
      if (it_key == "this")
      {
	    entity = pl->FindEntity (name);
	    break;
      }
    }
    return new celExpressionParameter (object_reg, entity, expression, val+1);
  }
  return new celConstantParameter (val);
}

const char* celParameterManager::ResolveParameter (
  	const celParams& params,
	const char* param)
{
  if (param == 0) return param;
  if (*param != '$') return param;
  if (*(param+1) == '$') return param+1;	// Double $ means to quote the '$'.
  const char* val = params.Get (param+1, (const char*)0);
  if (!val)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.parameters.manager",
		"Can't resolve parameter %s!", param);
  }
  return val;
}

csPtr<celVariableParameterBlock> celParameterManager::GetParameterBlock (
  	const celParams& params,
	const csArray<celParSpec>& parameters,
	csRefArray<iParameter>& quest_parameters)
{
  celVariableParameterBlock *act_params = new celVariableParameterBlock ();
  size_t i;
  for (i = 0 ; i < parameters.GetSize () ; i++)
  {
    csRef<iParameter> par = GetParameter (params, parameters[i].value);
    quest_parameters.Put (i, par);
    act_params->SetParameterDef (i, parameters[i].id);
  }
  return act_params;
}

bool celParameterManager::FillParameterBlock (
    iCelParameterBlock* params,
	celVariableParameterBlock* act_params,
	const csArray<celParSpec>& parameters,
	const csRefArray<iParameter>& quest_parameters)
{
  if (parameters.GetSize () != quest_parameters.GetSize ())
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.parameters.manager",
	      "Can't fill parameter blocks of different size (%zu VS %zu)!",
	      parameters.GetSize (), quest_parameters.GetSize ());
    return false;
  }

  size_t i;
  for (i = 0 ; i < quest_parameters.GetSize () ; i++)
  {
    iParameter* p = quest_parameters[i];
    switch (parameters[i].type)
    {
      case CEL_DATA_STRING:
	act_params->GetParameter (i).Set (p->Get (params));
	break;
      case CEL_DATA_LONG:
	act_params->GetParameter (i).Set (p->GetLong (params));
	break;
      case CEL_DATA_FLOAT:
	act_params->GetParameter (i).Set (p->GetFloat (params));
	break;
      case CEL_DATA_BOOL:
	act_params->GetParameter (i).Set (ToBool (p->GetData (params)));
	break;
      case CEL_DATA_VECTOR2:
	act_params->GetParameter (i).Set (ToVector2 (p->GetData (params)));
	break;
      case CEL_DATA_VECTOR3:
	act_params->GetParameter (i).Set (ToVector3 (p->GetData (params)));
	break;
      case CEL_DATA_COLOR:
	act_params->GetParameter (i).Set (ToColor (p->GetData (params)));
	break;
      default:
	//@@@?
	break;
    }
  }

  return true;
}

//---------------------------------------------------------------------------

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
  return data;
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
