/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "csutil/xmltiny.h"
#include "iutil/objreg.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "ivaria/reporter.h"
#include "imap/services.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entitytpl.h"
#include "behaviourlayer/bl.h"
#include "propclass/mesh.h"
#include "propclass/chars.h"
#include "celtool/stdparams.h"

#include "celentitytpl.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celAddOnCelEntityTemplate)

enum
{
  XMLTOKEN_BEHAVIOUR,
  XMLTOKEN_PROPCLASS,
  XMLTOKEN_TEMPLATE,
  XMLTOKEN_PROPERTY,
  XMLTOKEN_ACTION,
  XMLTOKEN_CALL,
  XMLTOKEN_CLASS,
  XMLTOKEN_PAR,
  XMLTOKEN_CHARACTERISTIC,

  XMLTOKEN_FLOAT,
  XMLTOKEN_BOOL,
  XMLTOKEN_STRING,
  XMLTOKEN_LONG,
  XMLTOKEN_VECTOR,
  XMLTOKEN_VECTOR2,
  XMLTOKEN_VECTOR3,
  XMLTOKEN_COLOR
};


celAddOnCelEntityTemplate::celAddOnCelEntityTemplate (iBase* parent) :
  scfImplementationType (this, parent)
{
  object_reg = 0;
}

celAddOnCelEntityTemplate::~celAddOnCelEntityTemplate ()
{
}

bool celAddOnCelEntityTemplate::Initialize (iObjectRegistry* object_reg)
{
  celAddOnCelEntityTemplate::object_reg = object_reg;
  synldr = csQueryRegistry<iSyntaxService> (object_reg);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.celentitytpl",
	"Can't find syntax services!");
    return false;
  }
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.celentitytpl",
	"Can't find physical layer!");
    return false;
  }

  xmltokens.Register ("behaviour", XMLTOKEN_BEHAVIOUR);
  xmltokens.Register ("propclass", XMLTOKEN_PROPCLASS);
  xmltokens.Register ("template", XMLTOKEN_TEMPLATE);
  xmltokens.Register ("property", XMLTOKEN_PROPERTY);
  xmltokens.Register ("action", XMLTOKEN_ACTION);
  xmltokens.Register ("call", XMLTOKEN_CALL);
  xmltokens.Register ("class", XMLTOKEN_CLASS);
  xmltokens.Register ("par", XMLTOKEN_PAR);
  xmltokens.Register ("characteristic", XMLTOKEN_CHARACTERISTIC);

  xmltokens.Register ("float", XMLTOKEN_FLOAT);
  xmltokens.Register ("bool", XMLTOKEN_BOOL);
  xmltokens.Register ("string", XMLTOKEN_STRING);
  xmltokens.Register ("long", XMLTOKEN_LONG);
  xmltokens.Register ("vector", XMLTOKEN_VECTOR);
  xmltokens.Register ("vector2", XMLTOKEN_VECTOR2);
  xmltokens.Register ("vector3", XMLTOKEN_VECTOR3);
  xmltokens.Register ("color", XMLTOKEN_COLOR);

  return true;
}

const char* celAddOnCelEntityTemplate::GetAttributeValue (iDocumentNode* child,
	const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
	"cel.addons.celentitytpl", child,
	"Can't find attribute '%s'!", propname);
    return 0;
  }
  return rc;
}

csStringID celAddOnCelEntityTemplate::GetAttributeID (iDocumentNode* child,
	const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
	"cel.addons.celentitytpl", child,
	"Can't find attribute '%s'!", propname);
    return csInvalidStringID;
  }
  return pl->FetchStringID (rc);
}

csStringID celAddOnCelEntityTemplate::GetAttributeID (iDocumentNode* child,
	const char* prefix, const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
	"cel.addons.celentitytpl", child,
	"Can't find attribute '%s'!", propname);
    return csInvalidStringID;
  }
  csString p (prefix);
  p += rc;
  return pl->FetchStringID ((const char*)p);
}

csRef<celVariableParameterBlock> celAddOnCelEntityTemplate::ParseParameterBlock
	(iDocumentNode* child)
{
  csRef<celVariableParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  csRef<iDocumentNodeIterator> par_it = child->GetNodes ();
  size_t par_idx = 0;
  while (par_it->HasNext ())
  {
    csRef<iDocumentNode> par_child = par_it->Next ();
    if (par_child->GetType () != CS_NODE_ELEMENT) continue;
    const char* par_value = par_child->GetValue ();
    csStringID par_id = xmltokens.Request (par_value);
    if (par_id == XMLTOKEN_PAR)
    {
      csStringID parid = GetAttributeID (par_child, "name");
      if (parid == csInvalidStringID) return 0;
      params->SetParameterDef (par_idx, parid);
      par_idx++;

      const char* str_value = par_child->GetAttributeValue ("string");
      if (str_value)
      {
	if (*str_value == '$')
          params->GetParameter (par_idx-1).SetParameter (str_value+1,
	  	CEL_DATA_STRING);
	else
          params->GetParameter (par_idx-1).Set (str_value);
	continue;
      }
      const char* vec_value = par_child->GetAttributeValue ("vector");
      if (vec_value)
      {
	if (*vec_value == '$')
          params->GetParameter (par_idx-1).SetParameter (vec_value+1,
	  	CEL_DATA_VECTOR3);
	else
	{
	  csVector3 v;
	  int rc = csScanStr (vec_value, "%f,%f,%f", &v.x, &v.y, &v.z);
	  if (rc == 3)
	    params->GetParameter (par_idx-1).Set (v);
	  else
	  {
	    csVector2 v2;
	    csScanStr (vec_value, "%f,%f", &v2.x, &v2.y);
	    params->GetParameter (par_idx-1).Set (v2);
	  }
	}
        continue;
      }
      const char* vec2_value = par_child->GetAttributeValue ("vector2");
      if (vec2_value)
      {
	if (*vec2_value == '$')
          params->GetParameter (par_idx-1).SetParameter (vec2_value+1,
	  	CEL_DATA_VECTOR2);
	else
	{
	  csVector2 v;
	  csScanStr (vec2_value, "%f,%f", &v.x, &v.y);
	  params->GetParameter (par_idx-1).Set (v);
	}
        continue;
      }
      const char* vec3_value = par_child->GetAttributeValue ("vector3");
      if (vec3_value)
      {
	if (*vec3_value == '$')
          params->GetParameter (par_idx-1).SetParameter (vec3_value+1,
	  	CEL_DATA_VECTOR3);
	else
	{
	  csVector3 v;
	  csScanStr (vec3_value, "%f,%f,%f", &v.x, &v.y, &v.z);
	  params->GetParameter (par_idx-1).Set (v);
	}
        continue;
      }
      const char* col_value = par_child->GetAttributeValue ("color");
      if (col_value)
      {
	if (*col_value == '$')
          params->GetParameter (par_idx-1).SetParameter (col_value+1,
	  	CEL_DATA_COLOR);
	else
	{
	  csColor v;
	  csScanStr (col_value, "%f,%f,%f", &v.red, &v.green, &v.blue);
	  params->GetParameter (par_idx-1).Set (v);
	}
        continue;
      }
      const char* float_value = par_child->GetAttributeValue ("float");
      if (float_value)
      {
	if (*float_value == '$')
          params->GetParameter (par_idx-1).SetParameter (float_value+1,
	  	CEL_DATA_FLOAT);
	else
	{
	  float f;
	  csScanStr (float_value, "%f", &f);
	  params->GetParameter (par_idx-1).Set (f);
	}
        continue;
      }
      const char* bool_value = par_child->GetAttributeValue ("bool");
      if (bool_value)
      {
	if (*bool_value == '$')
          params->GetParameter (par_idx-1).SetParameter (bool_value+1,
	  	CEL_DATA_BOOL);
	else
	{
	  bool b;
	  csScanStr (bool_value, "%b", &b);
	  params->GetParameter (par_idx-1).Set (b);
        }
        continue;
      }
      const char* long_value = par_child->GetAttributeValue ("long");
      if (long_value)
      {
	if (*long_value == '$')
          params->GetParameter (par_idx-1).SetParameter (long_value+1,
	  	CEL_DATA_LONG);
	else
	{
	  int l;
	  csScanStr (long_value, "%d", &l);
	  params->GetParameter (par_idx-1).Set ((int32)l);
	}
        continue;
      }
      synldr->ReportError (
        "cel.addons.celentitytpl",
        par_child, "Type for parameter not yet supported!");
      return 0;
    }
    else
    {
      synldr->ReportBadToken (par_child);
      return 0;
    }
  }
  return params;
}

bool celAddOnCelEntityTemplate::ParseProperties (iCelPropertyClassTemplate* pc,
	iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_PROPERTY:
        {
	  csStringID propid = GetAttributeID (child, "name");
	  if (propid == csInvalidStringID) return false;

	  csRef<iDocumentAttributeIterator> attr_it = child->GetAttributes ();
	  while (attr_it->HasNext ())
	  {
	    csRef<iDocumentAttribute> attr = attr_it->Next ();
	    const char* attr_name = attr->GetName ();
	    csStringID attr_id = xmltokens.Request (attr_name);
	    const char* attr_value = attr->GetValue ();
	    bool do_par = (attr_value && *attr_value == '$');
	    switch (attr_id)
	    {
	      case XMLTOKEN_FLOAT:
	        if (do_par)
		  pc->SetPropertyVariable (propid, CEL_DATA_FLOAT, attr_value+1);
		else
		  pc->SetProperty (propid, attr->GetValueAsFloat ());
		break;
	      case XMLTOKEN_STRING:
	        if (do_par)
		  pc->SetPropertyVariable (propid, CEL_DATA_STRING, attr_value+1);
		else
		  pc->SetProperty (propid, attr->GetValue ());
		break;
	      case XMLTOKEN_BOOL:
	        if (do_par)
		  pc->SetPropertyVariable (propid, CEL_DATA_BOOL, attr_value+1);
		else
		{
		  const char* v = attr->GetValue ();
		  bool b;
		  csScanStr (v, "%b", &b);
	          pc->SetProperty (propid, b);
		}
		break;
	      case XMLTOKEN_LONG:
	        if (do_par)
		  pc->SetPropertyVariable (propid, CEL_DATA_LONG, attr_value+1);
		else
	          pc->SetProperty (propid, (long)attr->GetValueAsInt ());
		break;
	      case XMLTOKEN_VECTOR:
	        if (do_par)
		  pc->SetPropertyVariable (propid, CEL_DATA_VECTOR3, attr_value+1);
		else
	        {
		  csVector3 v;
		  int rc = csScanStr (attr->GetValue (), "%f,%f,%f", &v.x,
		    	&v.y, &v.z);
		  if (rc == 3)
		    pc->SetProperty (propid, v);
		  else
		  {
		    csVector2 v2;
		    csScanStr (attr->GetValue (), "%f,%f", &v2.x, &v2.y);
		    pc->SetProperty (propid, v2);
		  }
		}
		break;
	      case XMLTOKEN_VECTOR2:
	        if (do_par)
		  pc->SetPropertyVariable (propid, CEL_DATA_VECTOR2,
		      attr_value+1);
		else
	        {
		  csVector2 v;
		  csScanStr (attr->GetValue (), "%f,%f", &v.x, &v.y);
		  pc->SetProperty (propid, v);
		}
		break;
	      case XMLTOKEN_VECTOR3:
	        if (do_par)
		  pc->SetPropertyVariable (propid, CEL_DATA_VECTOR3,
		      attr_value+1);
		else
	        {
		  csVector3 v;
		  csScanStr (attr->GetValue (), "%f,%f,%f", &v.x, &v.y,
		      &v.z);
		  pc->SetProperty (propid, v);
		}
		break;
	      case XMLTOKEN_COLOR:
	        if (do_par)
		  pc->SetPropertyVariable (propid, CEL_DATA_COLOR, attr_value+1);
		else
		{
		  csColor v;
		  csScanStr (attr->GetValue (), "%f,%f,%f",
		      	&v.red, &v.green, &v.blue);
		  pc->SetProperty (propid, v);
		}
		break;
	    }
	  }
	}
	break;
      case XMLTOKEN_ACTION:
        {
	  csStringID propid = GetAttributeID (child, "name");
	  if (propid == csInvalidStringID) return false;
	  csRef<celVariableParameterBlock> params = ParseParameterBlock (child);
	  if (!params) return false;
	  pc->PerformAction (propid, params);
	}
	break;
      default:
        synldr->ReportBadToken (child);
	break;
    }
  }
  return true;
}

csPtr<iBase> celAddOnCelEntityTemplate::Parse (iDocumentNode* node,
	iStreamSource*, iLoaderContext* ldr_context, iBase*)
{
  iCelEntityTemplate* ent = Load (node);
  csRef<iBase> ent_return = (iBase*)ent;
  return csPtr<iBase> (ent_return);
}

iCelEntityTemplate* celAddOnCelEntityTemplate::Load (const char* path,
    const char* file)
{
  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  if (path)
  {
    vfs->PushDir ();
    vfs->ChDir (path);
  }

  csRef<iFile> buf = vfs->Open (file, VFS_FILE_READ);
  if (!buf)
  {
    if (path)
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"cel.addons.celentitytpl",
	"Can't load file '%s' from '%s'!", file, path);
    else
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"cel.addons.celentitytpl",
	"Can't load file '%s'!", file);
    return 0;
  }

  csRef<iDocumentSystem> docsys = csQueryRegistry<iDocumentSystem> (object_reg);
  if (!docsys) docsys = csPtr<iDocumentSystem> (new csTinyDocumentSystem ());
  csRef<iDocument> doc = docsys->CreateDocument ();
  const char* error = doc->Parse (buf, true);
  if (error != 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	    "cell.addons.celentitytpl",
	    "Document system error for file '%s': %s!", file, error);
    return 0;
  }
  iCelEntityTemplate* tpl = Load (doc->GetRoot ()->GetNode ("addon"));

  if (path)
  {
    vfs->PopDir ();
  }

  return tpl;
}

iCelEntityTemplate* celAddOnCelEntityTemplate::Load (iDocumentNode* node)
{
  const char* entityname = node->GetAttributeValue ("entityname");
  if (!entityname)
  {
    synldr->ReportError (
	"cel.addons.celentitytpl",
	node, "'entityname' is missing for this entity template!");
  }
  csRef<iCelEntityTemplate> ent = pl->CreateEntityTemplate (entityname);

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_TEMPLATE:
        {
	  const char* tplname = child->GetAttributeValue ("name");
          iCelEntityTemplate* tpl = pl->FindEntityTemplate (tplname);
          if (!tplname)
          {
	    synldr->ReportError (
	        "cel.addons.celentitytpl",
	        child, "Can't find entity template '%s'!", tplname);
	    return 0;
          }
          ent->Merge (tpl);
        }
        break;
      case XMLTOKEN_CHARACTERISTIC:
        {
	  const char* name = child->GetAttributeValue ("name");
	  float value = child->GetAttributeValueAsFloat ("value");
          ent->GetCharacteristics ()->SetCharacteristic (name, value);
        }
        break;
      case XMLTOKEN_BEHAVIOUR:
        {
	  const char* blname = child->GetAttributeValue ("layer");
	  const char* behavename = child->GetAttributeValue ("name");
	  if (!behavename)
	  {
	    synldr->ReportError (
	        "cel.addons.celentitytpl",
	        child, "Name of the behaviour is missing!");
	    return 0;
	  }
	  ent->SetBehaviour (blname, behavename);
	}
        break;
      case XMLTOKEN_PROPCLASS:
        {
	  iCelPropertyClassTemplate* pc;
	  const char* name = child->GetAttributeValue ("name");
	  if (!name)
	  {
	    synldr->ReportError (
	        "cel.addons.celentitytpl",
	        child, "Name of the property class is missing!");
	    return 0;
	  }
	  const char* tag = child->GetAttributeValue ("tag");

          pc = ent->FindPropertyClassTemplate (name, tag);
          if (!pc)
          {
	    pc = ent->CreatePropertyClassTemplate ();
	    pc->SetName (name);
	    pc->SetTag (tag);
          }
	  if (!ParseProperties (pc, child))
	    return 0;
	}
	break;
      case XMLTOKEN_CALL:
        {
	  csRef<celVariableParameterBlock> params = ParseParameterBlock (child);
	  if (!params) return 0;
	  const char* msgid = child->GetAttributeValue ("event");
	  if (!msgid)
	  {
	    synldr->ReportError (
	        "cel.addons.celentitytpl",
	        child, "'event' name is missing for call!");
	    return 0;
	  }
	  ent->AddMessage (msgid, params);
	}
        break;
      case XMLTOKEN_CLASS:
        {
	  csStringID cls = pl->FetchStringID (child->GetContentsValue ());
	  ent->AddClass (cls);
	}
	break;
      default:
        synldr->ReportBadToken (child);
	break;
    }
  }
  return ent;
}

