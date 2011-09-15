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
#include "iutil/document.h"
#include "iutil/objreg.h"
#include "imap/services.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "celtool/stdparams.h"

//---------------------------------------------------------------------------

csRef<iCelParameterBlock> celParameterTools::ParseParams (
    iObjectRegistry* object_reg, iDocumentNode* node, ...)
{
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  csRef<iSyntaxService> synldr = csQueryRegistry<iSyntaxService> (object_reg);

  csRef<celVariableParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());

  size_t idx = 0;

  va_list args;
  va_start (args, node);
  char const* par = va_arg (args, char*);
  while (par != CEL_PARAM_END)
  {
    celData const* val = va_arg (args, celData*);
    csStringID id = pl->FetchStringID (par);
    params->SetParameterDef (idx, id);
    celData& data = params->GetParameter (idx);
    data.Copy (*val);
    idx++;
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

      csStringID id = pl->FetchStringID (name);
      params->SetParameterDef (idx, id);
      celData& data = params->GetParameter (idx);
      idx++;

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
    case CEL_DATA_BOOL: if (in.value.bo) out = 1; else out = 0; return true;
    case CEL_DATA_BYTE: out = in.value.b; return true;
    case CEL_DATA_WORD: out = in.value.w; return true;
    case CEL_DATA_LONG: out = in.value.l; return true;
    case CEL_DATA_UBYTE: out = in.value.ub; return true;
    case CEL_DATA_UWORD: out = in.value.uw; return true;
    case CEL_DATA_ULONG: out = in.value.ul; return true;
    case CEL_DATA_FLOAT: out = long (in.value.f); return true;
    case CEL_DATA_STRING: if (in.value.s) csScanStr (in.value.s->GetData (), "%d", &out); else out = 0; return true;
    default: return false;
  }
}

bool celParameterTools::ToBool (const celData& in, bool& out)
{
  switch (in.type)
  {
    case CEL_DATA_BOOL: out = in.value.bo; return true;
    case CEL_DATA_BYTE: out = bool (in.value.b); return true;
    case CEL_DATA_WORD: out = bool (in.value.w); return true;
    case CEL_DATA_LONG: out = bool (in.value.l); return true;
    case CEL_DATA_UBYTE: out = bool (in.value.ub); return true;
    case CEL_DATA_UWORD: out = bool (in.value.uw); return true;
    case CEL_DATA_ULONG: out = bool (in.value.ul); return true;
    case CEL_DATA_FLOAT: out = fabs (in.value.f) > 0.000001; return true;
    case CEL_DATA_STRING: if (in.value.s) csScanStr (in.value.s->GetData (), "%b", &out); else out = false; return true;
    case CEL_DATA_PCLASS: out = in.value.pc != 0; return true;
    case CEL_DATA_ENTITY: out = in.value.ent != 0; return true;
    default: return false;
  }
}

bool celParameterTools::ToFloat (const celData& in, float& out)
{
  switch (in.type)
  {
    case CEL_DATA_BOOL: if (in.value.bo) out = 1; else out = 0; return true;
    case CEL_DATA_BYTE: out = float (in.value.b); return true;
    case CEL_DATA_WORD: out = float (in.value.w); return true;
    case CEL_DATA_LONG: out = float (in.value.l); return true;
    case CEL_DATA_UBYTE: out = float (in.value.ub); return true;
    case CEL_DATA_UWORD: out = float (in.value.uw); return true;
    case CEL_DATA_ULONG: out = float (in.value.ul); return true;
    case CEL_DATA_FLOAT: out = in.value.f; return true;
    case CEL_DATA_STRING: if (in.value.s) csScanStr (in.value.s->GetData (), "%f", &out); else out = 0; return true;
    default: return false;
  }
}

bool celParameterTools::ToVector2 (const celData& in, csVector2& out)
{
  switch (in.type)
  {
    case CEL_DATA_VECTOR2: out.Set (in.value.v.x, in.value.v.y); return true;
    case CEL_DATA_STRING: if (in.value.s) csScanStr (in.value.s->GetData (), "%f,%f", &out.x, &out.y); else out.Set (0, 0); return true;
    default: return false;
  }
}

bool celParameterTools::ToVector3 (const celData& in, csVector3& out)
{
  switch (in.type)
  {
    case CEL_DATA_VECTOR3: out.Set (in.value.v.x, in.value.v.y, in.value.v.z); return true;
    case CEL_DATA_COLOR: out.Set (in.value.col.red, in.value.col.green, in.value.col.blue); return true;
    case CEL_DATA_STRING: if (in.value.s) csScanStr (in.value.s->GetData (), "%f,%f,%f", &out.x, &out.y, &out.z); else out.Set (0, 0, 0); return true;
    default: return false;
  }
}

bool celParameterTools::ToVector4 (const celData& in, csVector4& out)
{
  switch (in.type)
  {
    case CEL_DATA_VECTOR4: out.Set (in.value.v.x, in.value.v.y, in.value.v.z, in.value.v.w); return true;
    case CEL_DATA_COLOR4: out.Set (in.value.col.red, in.value.col.green, in.value.col.blue, in.value.col.alpha); return true;
    case CEL_DATA_STRING: if (in.value.s) csScanStr (in.value.s->GetData (), "%f,%f,%f,%f", &out.x, &out.y, &out.z, &out.w); else out.Set (0, 0, 0, 0); return true;
    default: return false;
  }
}

bool celParameterTools::ToColor (const celData& in, csColor& out)
{
  switch (in.type)
  {
    case CEL_DATA_COLOR: out.Set (in.value.col.red, in.value.col.green, in.value.col.blue); return true;
    case CEL_DATA_VECTOR3: out.Set (in.value.v.x, in.value.v.y, in.value.v.z); return true;
    case CEL_DATA_STRING: if (in.value.s) csScanStr (in.value.s->GetData (), "%f,%f,%f", &out.red, &out.green, &out.blue); else out.Set (0, 0, 0); return true;
    default: return false;
  }
}

bool celParameterTools::ToColor4 (const celData& in, csColor4& out)
{
  switch (in.type)
  {
    case CEL_DATA_VECTOR4: out.Set (in.value.v.x, in.value.v.y, in.value.v.z, in.value.v.w); return true;
    case CEL_DATA_COLOR4: out.Set (in.value.col.red, in.value.col.green, in.value.col.blue, in.value.col.alpha); return true;
    case CEL_DATA_STRING: if (in.value.s) csScanStr (in.value.s->GetData (), "%f,%f,%f,%f", &out.red, &out.green, &out.blue, &out.alpha); else out.Set (0, 0, 0, 0); return true;
    default: return false;
  }
}

bool celParameterTools::ToString (const celData& in, csString& out)
{
  switch (in.type)
  {
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
    case CEL_DATA_ACTION: return false;
    case CEL_DATA_COLOR: out.Format ("col(%g,%g,%g)", in.value.col.red, in.value.col.green, in.value.col.blue); return true;
    case CEL_DATA_COLOR4: out.Format ("col(%g,%g,%g,%g)", in.value.col.red, in.value.col.green, in.value.col.blue, in.value.col.alpha); return true;
    case CEL_DATA_IBASE: return false;
    case CEL_DATA_PARAMETER: return false;
    default: return false;
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


