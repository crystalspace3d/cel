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

#include "cssysdef.h"
#include "iutil/document.h"
#include "imap/services.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"

#include "plugins/behaviourlayer/xml/blxml.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "plugins/behaviourlayer/xml/xmlscript.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celBlXml)

SCF_IMPLEMENT_IBASE (celBlXml)
  SCF_IMPLEMENTS_INTERFACE (iCelBlLayer)
  SCF_IMPLEMENTS_INTERFACE (iCelBlLayerGenerate)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBlXml::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

enum
{
  XMLTOKEN_PROPERTY,
  XMLTOKEN_ACTION
};

celBlXml::celBlXml (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celBlXml::~celBlXml ()
{
}

bool celBlXml::Initialize (iObjectRegistry* object_reg)
{
  celBlXml::object_reg = object_reg;

  synldr = CS_QUERY_REGISTRY (object_reg, iSyntaxService);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.behaviourlayer.xml",
	"Can't find syntax services!");
    return false;
  }
  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);

  xmltokens.Register ("property", XMLTOKEN_PROPERTY);
  xmltokens.Register ("action", XMLTOKEN_ACTION);

  return true;
}

const char* celBlXml::GetAttributeValue (iDocumentNode* child,
	const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
	"cel.behaviour.xml", child,
	"Can't find attribute '%s'!", propname);
    return 0;
  }
  return rc;
}

csStringID celBlXml::GetAttributeID (iDocumentNode* child,
	const char* prefix, const char* propname)
{
  const char* rc = child->GetAttributeValue (propname);
  if (!rc)
  {
    synldr->ReportError (
	"cel.behaviour.xml", child,
	"Can't find attribute '%s'!", propname);
    return csInvalidStringID;
  }
  csString p (prefix);
  p += rc;
  return pl->FetchStringID ((const char*)p);
}

bool celBlXml::ParseEventHandler (celXmlScriptEventHandler* h,
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
	  const char* pcname = GetAttributeValue (child, "propclass");
	  if (!pcname) return false;
	  csStringID propid = GetAttributeID (child, "cel.property.", "name");
	  if (propid == csInvalidStringID) return false;

	  h->AddOperation (CEL_OPERATION_PROPERTY);
	  h->AddArgument ().SetPC (h->GetResolver (pcname));
	  h->AddArgument ().SetID (propid);

	  csRef<iDocumentAttribute> attr;
	  attr = child->GetAttribute ("float");
	  if (attr)
	  {
	    h->AddArgument ().SetFloat (attr->GetValueAsFloat ());
	  }
	  else
	  {
	    attr = child->GetAttribute ("string");
	    if (attr)
	    {
	      h->AddArgument ().SetString (attr->GetValue ());
	    }
	    else
	    {
	      attr = child->GetAttribute ("bool");
	      if (attr)
	      {
	        h->AddArgument ().SetBool ((bool)attr->GetValueAsInt ());
	      }
	    }
	  }
	}
	break;
      case XMLTOKEN_ACTION:
        {
	  const char* pcname = GetAttributeValue (child, "propclass");
	  if (!pcname) return false;
	  csStringID propid = GetAttributeID (child, "cel.property.", "name");
	  if (propid == csInvalidStringID) return false;
	  const char* params = GetAttributeValue (child, "params");
	  if (!params) return false;
	  h->AddOperation (CEL_OPERATION_ACTION);
	  h->AddArgument ().SetPC (h->GetResolver (pcname));
	  h->AddArgument ().SetID (propid);
	  h->AddArgument ().SetString (params);
	}
	break;
      default:
        synldr->ReportBadToken (child);
	return false;
    }
  }
  return true;
}

bool celBlXml::CreateBehaviourScriptFromDoc (const char* name,
  	iDocumentNode* node)
{
  celXmlScript* script = new celXmlScript ();
  script->SetName (name);

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (!strcmp (value, "event"))
    {
      const char* eventname = child->GetAttributeValue ("name");
      if (!eventname)
      {
        delete script;
        synldr->ReportError (
	        "cel.behaviour.xml", child,
		"'name' attribute is missing for the event in script '%s'!",
		name);
        return false;
      }
      celXmlScriptEventHandler* h = script->CreateEventHandler (eventname);
      if (!ParseEventHandler (h, child))
      {
        delete script;
	return false;
      }
    }
    else
    {
      delete script;
      synldr->ReportBadToken (child);
      return false;
    }
  }

  scripts.Push (script);
  scripts_hash.Put (name, script);
  return true;
}

bool celBlXml::CreateBehaviourScriptFromString (const char* name,
  	const char* string)
{
  return false;
}

bool celBlXml::CreateBehaviourScriptFromFile (const char* name,
  	const char* filename)
{
  return false;
}

iCelBehaviour* celBlXml::CreateBehaviour (iCelEntity* entity, const char* name)
{
  celXmlScript* script = scripts_hash.Get (name);
  if (!script)
  {
    // First check if we have the predefined 'bootstrap' script.
    if (!strcmp (name, "bootstrap"))
    {
      celBehaviourBootstrap* bootstrap = new celBehaviourBootstrap (entity,
    	  object_reg);
      bootstrap->SetName (name);
      bootstrap->SetBehaviourLayer (this);
      return bootstrap;
    }
    return 0;	// Couldn't find script.
  }

  celBehaviourXml* behave = new celBehaviourXml (entity, object_reg);
  behave->SetName (name);
  behave->SetBehaviourLayer (this);
  behave->SetScript (script);
  behave->SendMessage ("init", 0);
  return behave;
}


