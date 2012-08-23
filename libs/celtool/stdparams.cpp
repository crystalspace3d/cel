/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein

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
#include "csutil/scanstr.h"
#include "csutil/sysfunc.h"
#include "iutil/document.h"
#include "iutil/objreg.h"
#include "imap/services.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "celtool/stdparams.h"
#include "tools/parameters.h"

//---------------------------------------------------------------------------

csStringID celEntityParameterBlock::thisID = csInvalidStringID;

celEntityParameterBlock::celEntityParameterBlock (iCelPlLayer* pl, iCelEntity* entity) :
  scfImplementationType (this), entity (entity)
{
  if (thisID == csInvalidStringID)
    thisID = pl->FetchStringID ("this");
  thisData.Set (entity);
}

//---------------------------------------------------------------------------

csString celParameterTools::GetTypeName (celDataType type)
{
  switch (type)
  {
    case CEL_DATA_NONE: return "none";
    case CEL_DATA_BOOL: return "bool";
    case CEL_DATA_BYTE: return "byte";
    case CEL_DATA_WORD: return "word";
    case CEL_DATA_LONG: return "long";
    case CEL_DATA_UBYTE: return "ubyte";
    case CEL_DATA_UWORD: return "uword";
    case CEL_DATA_ULONG: return "ulong";
    case CEL_DATA_FLOAT: return "float";
    case CEL_DATA_VECTOR2: return "vector2";
    case CEL_DATA_VECTOR3: return "vector3";
    case CEL_DATA_VECTOR4: return "vector4";
    case CEL_DATA_COLOR: return "color";
    case CEL_DATA_COLOR4: return "color4";
    case CEL_DATA_STRING: return "string";
    case CEL_DATA_PCLASS: return "pclass";
    case CEL_DATA_ENTITY: return "entity";
    case CEL_DATA_ACTION: return "action";
    case CEL_DATA_IBASE: return "ibase";
    case CEL_DATA_PARAMETER: return "parameter";
    default: return "?";
  }
}

celDataType celParameterTools::GetType (const char* name)
{
  csString n = name;
  n.Downcase ();
  if (n == "long") return CEL_DATA_LONG;
  if (n == "float") return CEL_DATA_FLOAT;
  if (n == "bool") return CEL_DATA_BOOL;
  if (n == "string") return CEL_DATA_STRING;
  if (n == "byte") return CEL_DATA_BYTE;
  if (n == "word") return CEL_DATA_WORD;
  if (n == "ubyte") return CEL_DATA_UBYTE;
  if (n == "uword") return CEL_DATA_UWORD;
  if (n == "ulong") return CEL_DATA_ULONG;
  if (n == "vector2") return CEL_DATA_VECTOR2;
  if (n == "vector3") return CEL_DATA_VECTOR3;
  if (n == "vector4") return CEL_DATA_VECTOR4;
  if (n == "color") return CEL_DATA_COLOR;
  if (n == "color4") return CEL_DATA_COLOR4;
  if (n == "pclass") return CEL_DATA_PCLASS;
  if (n == "entity") return CEL_DATA_ENTITY;
  if (n == "action") return CEL_DATA_ACTION;
  if (n == "ibase") return CEL_DATA_IBASE;
  if (n == "parameter") return CEL_DATA_PARAMETER;
  return CEL_DATA_NONE;
}

csString celParameterTools::GetDebugData (const celData* data)
{
  csString buffer;
  switch (data->type)
  {
    case CEL_DATA_NONE:
      buffer = "\tData not typed!";
      break;
    case CEL_DATA_BOOL:
      buffer.Format ("\tBool: %d", data->value.bo);
      break;
    case CEL_DATA_BYTE:
      buffer.Format ("\tByte: %d", data->value.b);
      break;
    case CEL_DATA_WORD:
      buffer.Format ("\tWord: %d", data->value.w);
      break;
    case CEL_DATA_LONG:
      buffer.Format ("\tLong: %d", data->value.l);
      break;
    case CEL_DATA_UBYTE:
      buffer.Format ("\tuByte: %d", data->value.ub);
      break;
    case CEL_DATA_UWORD:
      buffer.Format ("\tuWord: %d", data->value.uw);
      break;
    case CEL_DATA_ULONG:
      buffer.Format ("\tuLong: %d", data->value.ul);
      break;
    case CEL_DATA_FLOAT:
      buffer.Format ("\tFloat: %f", data->value.f);
      break;
    case CEL_DATA_VECTOR2:
      buffer.Format ("\tVector2: %f %f", data->value.v.x, data->value.v.y);
      break;
    case CEL_DATA_VECTOR3:
      buffer.Format ("\tVector3: %f %f %f", data->value.v.x, data->value.v.y, data->value.v.z);
      break;
    case CEL_DATA_VECTOR4:
      buffer.Format ("\tVector4: %f %f %f %f", data->value.v.x, data->value.v.y, data->value.v.z, data->value.v.w);
      break;
    case CEL_DATA_COLOR:
      buffer.Format ("\tColor: %f %f %f", data->value.col.red, data->value.col.green, data->value.col.blue);
      break;
    case CEL_DATA_COLOR4:
      buffer.Format ("\tColor4: %f %f %f %f", data->value.col.red, data->value.col.green, data->value.col.blue, data->value.col.alpha);
      break;
    case CEL_DATA_STRING:
      buffer.Format ("\tString: %s", data->value.s->GetData ());
      break;
    case CEL_DATA_PCLASS:
      buffer.Format ("\tPC: %s %s", data->value.pc->GetName (), data->value.pc->GetTag ());
      break;
    case CEL_DATA_ENTITY:
      buffer.Format ("\tEnt: id %d", data->value.ent->GetID ());
      break;
    case CEL_DATA_ACTION:
      buffer.Format ("\tAction: %s", data->value.s->GetData ());
      break;
    case CEL_DATA_IBASE:
      buffer.Format ("\tiBase");
      break;
    case CEL_DATA_PARAMETER:
      buffer.Format ("\tParameter: name %s type %d", data->value.par.parname->GetData (), data->value.par.partype);
      break;
    default:
      buffer.Format ("\tError: not a type!");
      break;
  }
  return buffer;
}

void celParameterTools::Dump (const char* title, iCelParameterBlock* params,
    iCelPlLayer* pl)
{
  if (params)
  {
    printf ("%s\n", title);
  }
  else
  {
    printf ("%s (empty)\n", title);
    return;
  }
  for (size_t i = 0 ; i < params->GetParameterCount () ; i++)
  {
    csString parName;
    celDataType t;
    csStringID id = params->GetParameterDef (i, t);
    if (pl) parName = pl->FetchString (id);
    csPrintf ("    %zu: %d/%s -> ", i, (int)id, parName.GetData ());
    const celData* data = params->GetParameterByIndex (i);
    csString debugInfo = GetDebugData (data);
    printf ("%s\n", debugInfo.GetData ());
  }
  fflush (stdout);
}

bool celParameterTools::WriteParams (iObjectRegistry* object_reg,
    iDocumentNode* node, iCelParameterBlock* params)
{
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  csRef<iSyntaxService> synldr = csQueryRegistry<iSyntaxService> (object_reg);

  for (size_t i = 0 ; i < params->GetParameterCount () ; i++)
  {
    celDataType t;
    csStringID id = params->GetParameterDef (i, t);
    csString name = pl->FetchString (id);
    const celData* data = params->GetParameterByIndex (i);
    if (!data)
    {
      if (synldr)
        synldr->ReportError ("cel.params", node,
              "Parameter block is missing data!");
      else
	printf ("Parameter block is missing data!\n");
      return false;
    }
    csRef<iDocumentNode> parNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    parNode->SetValue ("par");
    parNode->SetAttribute ("name", name);
    csString value;
    ToString (*data, value);
    parNode->SetAttribute (GetTypeName (t), value);
  }
  return true;
}

csRef<iCelParameterBlock> celParameterTools::ParseParams (
    iObjectRegistry* object_reg, iDocumentNode* node, ...)
{
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  csRef<iSyntaxService> synldr = csQueryRegistry<iSyntaxService> (object_reg);

  csRef<celVariableParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());

  va_list args;
  va_start (args, node);
  char const* par = va_arg (args, char*);
  while (par != CEL_PARAM_END)
  {
    celData const* val = va_arg (args, celData*);
    params->AddParameter (pl->FetchStringID (par), *val);
    par = va_arg (args, char*);
  }
  va_end (args);

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* par_value = child->GetValue ();
    if (!strcmp (par_value, "par"))
    {
      const char* name = child->GetAttributeValue ("name");
      if (!name)
      {
        if (synldr)
          synldr->ReportError ("cel.params", child,
              "'name' attribute missing for parameter!");
        else
          printf ("'name' attribute missing for parameter!\n");
        return 0;
      }

      celData& data = params->AddParameter (pl->FetchStringID (name));

      const char* value = child->GetAttributeValue ("value");
      if (value) { data.Set (value); continue; }
      const char* str_value = child->GetAttributeValue ("string");
      if (str_value) { data.Set (str_value); continue; }
      const char* vec_value = child->GetAttributeValue ("vector");
      if (vec_value)
      {
        csVector3 v;
        int rc = csScanStr (vec_value, "%f,%f,%f", &v.x, &v.y, &v.z);
        if (rc == 3)
          data.Set (v);
        else
        {
          csVector2 v2;
          csScanStr (vec_value, "%f,%f", &v2.x, &v2.y);
          data.Set (v2);
        }
        continue;
      }
      const char* vec3_value = child->GetAttributeValue ("vector3");
      if (vec3_value)
      {
        csVector3 v;
        csScanStr (vec3_value, "%f,%f,%f", &v.x, &v.y, &v.z);
        data.Set (v);
        continue;
      }
      const char* vec2_value = child->GetAttributeValue ("vector2");
      if (vec2_value)
      {
        csVector2 v;
        csScanStr (vec2_value, "%f,%f", &v.x, &v.y);
        data.Set (v);
        continue;
      }
      const char* col_value = child->GetAttributeValue ("color");
      if (col_value)
      {
        csColor v;
        csScanStr (col_value, "%f,%f,%f", &v.red, &v.green, &v.blue);
        data.Set (v);
        continue;
      }
      const char* float_value = child->GetAttributeValue ("float");
      if (float_value)
      {
        float f;
        csScanStr (float_value, "%f", &f);
        data.Set (f);
        continue;
      }
      const char* bool_value = child->GetAttributeValue ("bool");
      if (bool_value)
      {
        bool b;
        csScanStr (bool_value, "%b", &b);
        data.Set (b);
        continue;
      }
      const char* long_value = child->GetAttributeValue ("long");
      if (long_value)
      {
        int32 l;
        csScanStr (long_value, "%d", &l);
        data.Set (l);
        continue;
      }
      if (synldr)
        synldr->ReportError (
          "cel.params",
          child, "Type for parameter '%s' not yet supported!", name);
      else
        printf ("Type for parameter '%s' not yet supported!", name);
      return 0;
    }
    else
    {
      if (synldr)
        synldr->ReportBadToken (child);
      else
        printf ("Unrecognized token '%s'!\n", par_value);
      return 0;
    }
  }

  return params;
}

bool celParameterTools::ToLong (const celData& in, long& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out = 0; return true;
    case CEL_DATA_BOOL: if (in.value.bo) out = 1; else out = 0; return true;
    case CEL_DATA_BYTE: out = in.value.b; return true;
    case CEL_DATA_WORD: out = in.value.w; return true;
    case CEL_DATA_LONG: out = in.value.l; return true;
    case CEL_DATA_UBYTE: out = in.value.ub; return true;
    case CEL_DATA_UWORD: out = in.value.uw; return true;
    case CEL_DATA_ULONG: out = in.value.ul; return true;
    case CEL_DATA_FLOAT: out = long (in.value.f); return true;
    case CEL_DATA_STRING:
	if (in.value.s && in.value.s->GetData ())
	  csScanStr (in.value.s->GetData (), "%d", &out);
	else
	  out = 0;
	return true;
    default: out = 0; return false;
  }
}

bool celParameterTools::ToBool (const celData& in, bool& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out = false; return true;
    case CEL_DATA_BOOL: out = in.value.bo; return true;
    case CEL_DATA_BYTE: out = bool (in.value.b); return true;
    case CEL_DATA_WORD: out = bool (in.value.w); return true;
    case CEL_DATA_LONG: out = bool (in.value.l); return true;
    case CEL_DATA_UBYTE: out = bool (in.value.ub); return true;
    case CEL_DATA_UWORD: out = bool (in.value.uw); return true;
    case CEL_DATA_ULONG: out = bool (in.value.ul); return true;
    case CEL_DATA_FLOAT: out = fabs (in.value.f) > 0.000001; return true;
    case CEL_DATA_STRING:
	if (in.value.s && in.value.s->GetData ())
	  csScanStr (in.value.s->GetData (), "%b", &out);
	else
	  out = false;
	return true;
    case CEL_DATA_PCLASS: out = in.value.pc != 0; return true;
    case CEL_DATA_ENTITY: out = in.value.ent != 0; return true;
    default: out = false; return false;
  }
}

bool celParameterTools::ToFloat (const celData& in, float& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out = 0; return true;
    case CEL_DATA_BOOL: if (in.value.bo) out = 1; else out = 0; return true;
    case CEL_DATA_BYTE: out = float (in.value.b); return true;
    case CEL_DATA_WORD: out = float (in.value.w); return true;
    case CEL_DATA_LONG: out = float (in.value.l); return true;
    case CEL_DATA_UBYTE: out = float (in.value.ub); return true;
    case CEL_DATA_UWORD: out = float (in.value.uw); return true;
    case CEL_DATA_ULONG: out = float (in.value.ul); return true;
    case CEL_DATA_FLOAT: out = in.value.f; return true;
    case CEL_DATA_STRING:
	if (in.value.s && in.value.s->GetData ())
	  csScanStr (in.value.s->GetData (), "%f", &out);
	else
	  out = 0;
	return true;
    default: out = 0; return false;
  }
}

bool celParameterTools::ToVector2 (const celData& in, csVector2& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out.Set (0, 0); return true;
    case CEL_DATA_VECTOR2: out.Set (in.value.v.x, in.value.v.y); return true;
    case CEL_DATA_STRING:
	if (in.value.s && in.value.s->GetData ())
	  csScanStr (in.value.s->GetData (), "%f,%f", &out.x, &out.y);
	else
	  out.Set (0, 0);
	return true;
    default: out.Set (0, 0); return false;
  }
}

bool celParameterTools::ToVector3 (const celData& in, csVector3& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out.Set (0, 0, 0); return true;
    case CEL_DATA_VECTOR3: out.Set (in.value.v.x, in.value.v.y, in.value.v.z); return true;
    case CEL_DATA_COLOR: out.Set (in.value.col.red, in.value.col.green, in.value.col.blue); return true;
    case CEL_DATA_STRING:
	if (in.value.s && in.value.s->GetData ())
	  csScanStr (in.value.s->GetData (), "%f,%f,%f", &out.x, &out.y, &out.z);
	else
	  out.Set (0, 0, 0);
	return true;
    default: out.Set (0, 0, 0); return false;
  }
}

bool celParameterTools::ToVector4 (const celData& in, csVector4& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out.Set (0, 0, 0, 0); return true;
    case CEL_DATA_VECTOR4: out.Set (in.value.v.x, in.value.v.y, in.value.v.z, in.value.v.w); return true;
    case CEL_DATA_COLOR4: out.Set (in.value.col.red, in.value.col.green, in.value.col.blue, in.value.col.alpha); return true;
    case CEL_DATA_STRING:
	if (in.value.s && in.value.s->GetData ())
	  csScanStr (in.value.s->GetData (), "%f,%f,%f,%f", &out.x, &out.y, &out.z, &out.w);
	else
	  out.Set (0, 0, 0, 0);
	return true;
    default: out.Set (0, 0, 0, 0); return false;
  }
}

bool celParameterTools::ToColor (const celData& in, csColor& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out.Set (0, 0, 0); return true;
    case CEL_DATA_COLOR: out.Set (in.value.col.red, in.value.col.green, in.value.col.blue); return true;
    case CEL_DATA_VECTOR3: out.Set (in.value.v.x, in.value.v.y, in.value.v.z); return true;
    case CEL_DATA_STRING:
	if (in.value.s && in.value.s->GetData ())
	  csScanStr (in.value.s->GetData (), "%f,%f,%f", &out.red, &out.green, &out.blue);
	else
	  out.Set (0, 0, 0);
	return true;
    default: out.Set (0, 0, 0); return false;
  }
}

bool celParameterTools::ToColor4 (const celData& in, csColor4& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out.Set (0, 0, 0, 0); return true;
    case CEL_DATA_VECTOR4: out.Set (in.value.v.x, in.value.v.y, in.value.v.z, in.value.v.w); return true;
    case CEL_DATA_COLOR4: out.Set (in.value.col.red, in.value.col.green, in.value.col.blue, in.value.col.alpha); return true;
    case CEL_DATA_STRING:
	if (in.value.s && in.value.s->GetData ())
	  csScanStr (in.value.s->GetData (), "%f,%f,%f,%f", &out.red, &out.green, &out.blue, &out.alpha);
	else
	  out.Set (0, 0, 0, 0);
	return true;
    default: out.Set (0, 0, 0, 0); return false;
  }
}

bool celParameterTools::ToString (const celData& in, csString& out)
{
  switch (in.type)
  {
    case CEL_DATA_NONE: out.Empty (); return true;
    case CEL_DATA_BOOL: if (in.value.bo) out = "true"; else out = "false"; return true;
    case CEL_DATA_BYTE: out.Format ("%d", in.value.b); return true;
    case CEL_DATA_WORD: out.Format ("%d", in.value.w); return true;
    case CEL_DATA_LONG: out.Format ("%d", in.value.l); return true;
    case CEL_DATA_UBYTE: out.Format ("%d", in.value.ub); return true;
    case CEL_DATA_UWORD: out.Format ("%d", in.value.uw); return true;
    case CEL_DATA_ULONG: out.Format ("%d", in.value.ul); return true;
    case CEL_DATA_FLOAT: out.Format ("%g", in.value.f); return true;
    case CEL_DATA_VECTOR2: out.Format ("%g,%g", in.value.v.x, in.value.v.y); return true;
    case CEL_DATA_VECTOR3: out.Format ("%g,%g,%g", in.value.v.x, in.value.v.y, in.value.v.z); return true;
    case CEL_DATA_VECTOR4: out.Format ("%g,%g,%g,%g", in.value.v.x, in.value.v.y, in.value.v.z, in.value.v.w); return true;
    case CEL_DATA_STRING: if (in.value.s) out = in.value.s->GetData (); else out = ""; return true;
    case CEL_DATA_PCLASS: if (in.value.pc) out = in.value.pc->GetName (); else out = "null"; return true;
    case CEL_DATA_ENTITY: if (in.value.ent) out = in.value.ent->GetName (); else out = "null"; return true;
    case CEL_DATA_COLOR: out.Format ("%g,%g,%g", in.value.col.red, in.value.col.green, in.value.col.blue); return true;
    case CEL_DATA_COLOR4: out.Format ("%g,%g,%g,%g", in.value.col.red, in.value.col.green, in.value.col.blue, in.value.col.alpha); return true;
    default: out.Empty(); return false;
  }
}

bool celParameterTools::Convert (const celData& in, celDataType type, celData& out)
{
  if (type == in.type)
  {
    out.Copy (in);
    return true;
  }
  switch (type)
  {
    case CEL_DATA_BOOL:
      {
        bool b;
        if (!ToBool (in, b)) return false;
        out.Set (b);
      }
      return true;
    case CEL_DATA_LONG:
      {
        long l;
        if (!ToLong (in, l)) return false;
        out.Set ((int32)l);
      }
      return true;
    case CEL_DATA_FLOAT:
      {
        float f;
        if (!ToFloat (in, f)) return false;
        out.Set (f);
      }
      return true;
    case CEL_DATA_VECTOR2:
      {
        csVector2 v;
        if (!ToVector2 (in, v)) return false;
        out.Set (v);
      }
      return true;
    case CEL_DATA_VECTOR3:
      {
        csVector3 v;
        if (!ToVector3 (in, v)) return false;
        out.Set (v);
      }
      return true;
    case CEL_DATA_VECTOR4:
      {
        csVector4 v;
        if (!ToVector4 (in, v)) return false;
        out.Set (v);
      }
      return true;
    case CEL_DATA_COLOR:
      {
        csColor v;
        if (!ToColor (in, v)) return false;
        out.Set (v);
      }
      return true;
    case CEL_DATA_COLOR4:
      {
        csColor4 v;
        if (!ToColor4 (in, v)) return false;
        out.Set (v);
      }
      return true;
    case CEL_DATA_STRING:
      {
        csString s;
        if (!ToString (in, s)) return false;
        out.Set (s.GetData ());
      }
      return true;
    default:
      return false;
  }
}


csPtr<celVariableParameterBlock> celParameterTools::GetParameterBlock (
        iParameterManager* pm,
  	iCelParameterBlock* params,
	const csArray<celParSpec>& parameters,
	csRefArray<iParameter>& dyn_parameters)
{
  celVariableParameterBlock *act_params = new celVariableParameterBlock ();
  dyn_parameters.Empty ();
  for (size_t i = 0 ; i < parameters.GetSize () ; i++)
  {
    csRef<iParameter> par = pm->GetParameter (params, parameters[i].value);
    dyn_parameters.Push (par);
    act_params->SetParameterDef (i, parameters[i].id);
  }
  return act_params;
}

bool celParameterTools::FillParameterBlock (
        iCelPlLayer* pl,
        iCelParameterBlock* params,
	celVariableParameterBlock* act_params,
	const csArray<celParSpec>& parameters,
	const csRefArray<iParameter>& dyn_parameters)
{
  if (parameters.GetSize () != dyn_parameters.GetSize ())
    return false;

  for (size_t i = 0 ; i < dyn_parameters.GetSize () ; i++)
  {
    iParameter* p = dyn_parameters[i];
    const celData* data = p->GetData (params);
    if (parameters[i].type == CEL_DATA_ENTITY)
    {
      if (data->type == CEL_DATA_ENTITY)
        act_params->GetParameter (i).Set (data->value.ent);
      else if (data->type == CEL_DATA_LONG)
      {
        long entid;
        ToLong (*data, entid);
        iCelEntity* ent = pl->GetEntity ((uint)entid);
        act_params->GetParameter (i).Set (ent);
      }
      else
      {
        csString str;
        ToString (*data, str);
        iCelEntity* ent = pl->FindEntity (str);
        act_params->GetParameter (i).Set (ent);
      }
    }
    else
    {
      celParameterTools::Convert (*data, parameters[i].type,
          act_params->GetParameter (i));
    }
  }

  return true;
}

bool celParameterTools::WriteParSpecBlock (iObjectRegistry* object_reg,
    iDocumentNode* node, const csArray<celParSpec>& parameters)
{
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  for (size_t i = 0 ; i < parameters.GetSize () ; i++)
  {
    const celParSpec& par = parameters[i];
    csRef<iDocumentNode> parNode = node->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    parNode->SetValue ("par");
    parNode->SetAttribute ("name", pl->FetchString (par.id));
    switch (par.type)
    {
      case CEL_DATA_ENTITY: parNode->SetAttribute ("entity", par.value); break;
      case CEL_DATA_STRING: parNode->SetAttribute ("string", par.value); break;
      case CEL_DATA_LONG: parNode->SetAttribute ("long", par.value); break;
      case CEL_DATA_FLOAT: parNode->SetAttribute ("float", par.value); break;
      case CEL_DATA_VECTOR3: parNode->SetAttribute ("vector3", par.value); break;
      case CEL_DATA_VECTOR2: parNode->SetAttribute ("vector2", par.value); break;
      case CEL_DATA_BOOL: parNode->SetAttribute ("bool", par.value); break;
      default: return false;
    }
  }
  return true;
}

bool celParameterTools::ParseParSpecBlock (iObjectRegistry* object_reg,
    iDocumentNode* node, csArray<celParSpec>& parameters)
{
  parameters.Empty ();
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  csRef<iSyntaxService> synldr = csQueryRegistry<iSyntaxService> (object_reg);

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (!strcmp ("par", value))
    {
      const char* name = child->GetAttributeValue ("name");
      if (!name)
      {
        if (synldr)
          synldr->ReportError ("cel.params", child,
              "'name' attribute missing for parameter!");
        else
          printf ("'name' attribute missing for parameter!\n");
        return 0;
      }
      csStringID id = pl->FetchStringID (name);
      const char* ent_value = child->GetAttributeValue ("entity");
      if (ent_value)
      {
	parameters.Push (celParSpec (CEL_DATA_ENTITY, id, ent_value));
	continue;
      }
      const char* str_value = child->GetAttributeValue ("string");
      if (!str_value) str_value = child->GetAttributeValue ("value");
      if (str_value)
      {
	parameters.Push (celParSpec (CEL_DATA_STRING, id, str_value));
	continue;
      }
      const char* col3_value = child->GetAttributeValue ("color3");
      if (!col3_value) col3_value = child->GetAttributeValue ("color");
      if (col3_value)
      {
	parameters.Push (celParSpec (CEL_DATA_COLOR, id, col3_value));
	continue;
      }
      const char* col4_value = child->GetAttributeValue ("color4");
      if (col4_value)
      {
	parameters.Push (celParSpec (CEL_DATA_COLOR4, id, col4_value));
	continue;
      }
      const char* vec3_value = child->GetAttributeValue ("vector3");
      if (!vec3_value) vec3_value = child->GetAttributeValue ("vector");
      if (vec3_value)
      {
	parameters.Push (celParSpec (CEL_DATA_VECTOR3, id, vec3_value));
	continue;
      }
      const char* vec2_value = child->GetAttributeValue ("vector2");
      if (vec2_value)
      {
	parameters.Push (celParSpec (CEL_DATA_VECTOR2, id, vec2_value));
	continue;
      }
      const char* float_value = child->GetAttributeValue ("float");
      if (float_value)
      {
	parameters.Push (celParSpec (CEL_DATA_FLOAT, id, float_value));
	continue;
      }
      const char* long_value = child->GetAttributeValue ("long");
      if (long_value)
      {
	parameters.Push (celParSpec (CEL_DATA_LONG, id, long_value));
	continue;
      }
      const char* bool_value = child->GetAttributeValue ("bool");
      if (bool_value)
      {
	parameters.Push (celParSpec (CEL_DATA_BOOL, id, bool_value));
	continue;
      }
      if (synldr)
        synldr->ReportBadToken (child);
      else
        printf ("Unknown parameter type for the parameter block!\n");
      return false;
    }
    else
    {
      if (synldr)
        synldr->ReportBadToken (child);
      else
        printf ("Unexpected child '%s' for the parameter block!\n", value);
      return false;
    }
  }
  return true;
}


void celVariableParameterBlock::Merge (iCelParameterBlock* params)
{
  for (size_t i = 0 ; i < params->GetParameterCount () ; i++)
  {
    celDataType t;
    csStringID id = params->GetParameterDef (i, t);
    celData* data = GetParameter (id);
    if (data)
      data->Copy (*params->GetParameterByIndex (i));
    else
      AddParameter (id, *params->GetParameterByIndex (i));
  }
}

