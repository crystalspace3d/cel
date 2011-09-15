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

