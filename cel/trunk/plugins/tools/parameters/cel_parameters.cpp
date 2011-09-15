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
  bool rc;
  celParameterTools::ToBool (*data, rc);
  return rc;
}

static csVector2 ToVector2 (const celData* data)
{
  csVector2 v;
  celParameterTools::ToVector2 (*data, v);
  return v;
}

static csVector3 ToVector3 (const celData* data)
{
  csVector3 v;
  celParameterTools::ToVector3 (*data, v);
  return v;
}

static csColor ToColor (const celData* data)
{
  csColor v;
  celParameterTools::ToColor (*data, v);
  return v;
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
