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
  XMLTOKEN_GETPROPERTY,
  XMLTOKEN_ACTION,

  XMLTOKEN_VAR,
  XMLTOKEN_GETPROPCLASS,
  XMLTOKEN_TESTCOLLIDE,
  XMLTOKEN_IF,
  XMLTOKEN_PRINT
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
  xmltokens.Register ("getproperty", XMLTOKEN_GETPROPERTY);
  xmltokens.Register ("action", XMLTOKEN_ACTION);
  xmltokens.Register ("var", XMLTOKEN_VAR);
  xmltokens.Register ("getpropclass", XMLTOKEN_GETPROPCLASS);
  xmltokens.Register ("testcollide", XMLTOKEN_TESTCOLLIDE);
  xmltokens.Register ("if", XMLTOKEN_IF);
  xmltokens.Register ("print", XMLTOKEN_PRINT);

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

const char* celBlXml::GetAttributeString (iDocumentNode* child,
	const char* attrname, const char* parentname)
{
  const char* value = child->GetAttributeValue (attrname);
  if (!value && parentname)
  {
    synldr->ReportError (
	"cel.behaviour.xml", child,
	"'%s' attribute is missing for <%s>!",
	attrname, parentname);
    return 0;
  }
  return value;
}

bool celBlXml::ParseValueArg (iDocumentNode* child, celXmlScriptEventHandler* h)
{
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
      else
      {
        attr = child->GetAttribute ("long");
        if (attr)
        {
          h->AddArgument ().SetInt32 ((long)attr->GetValueAsInt ());
        }
	else
	{
	  return false;
	}
      }
    }
  }
  return true;
}

bool celBlXml::ParseEventHandler (celXmlScriptEventHandler* h,
	iDocumentNode* node, celXmlScript* script)
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
      case XMLTOKEN_VAR:
        {
	  const char* varname = GetAttributeString (child, "name", "var");
	  if (!varname) return false;

	  h->AddOperation (CEL_OPERATION_VAR);
	  h->AddArgument ().SetString (varname);
	  if (!ParseValueArg (child, h))
	  {
            synldr->ReportError (
	        "cel.behaviour.xml", child,
		"Value is missing for <var>!");
	    return false;
	  }
	}
	break;
      case XMLTOKEN_GETPROPCLASS:
        {
	  const char* varname = GetAttributeString (child, "var",
	  	"getpropclass");
	  if (!varname) return false;
	  const char* entityname = GetAttributeString (child, "entity",
	  	"getpropclass");
	  if (!entityname) return false;
	  const char* pclassname = GetAttributeString (child, "propclass",
	  	"getpropclass");
	  if (!pclassname) return false;
	  h->AddOperation (CEL_OPERATION_GETPROPCLASS);
	  h->AddArgument ().SetString (varname);
	  h->AddArgument ().SetString (entityname);
	  h->AddArgument ().SetString (pclassname);
	}
        break;
      case XMLTOKEN_TESTCOLLIDE:
        {
	  const char* varname = GetAttributeString (child, "var",
	  	"testcollide");
	  if (!varname) return false;
	  const char* truename = GetAttributeString (child, "true", 0);
          celXmlScriptEventHandler* truehandler = truename
	  	? script->FindOrCreateEventHandler (truename)
		: 0;
	  const char* falsename = GetAttributeString (child, "false", 0);
          celXmlScriptEventHandler* falsehandler = falsename
	  	? script->FindOrCreateEventHandler (falsename)
		: 0;
	  h->AddOperation (CEL_OPERATION_TESTCOLLIDE);
	  h->AddArgument ().SetString (varname);
	  h->AddArgument ().SetEventHandler (truehandler);
	  h->AddArgument ().SetEventHandler (falsehandler);
	}
        break;
      case XMLTOKEN_IF:
        {
	  const char* varname = GetAttributeString (child, "var", "if");
	  if (!varname) return false;
	  const char* truename = GetAttributeString (child, "true", 0);
          celXmlScriptEventHandler* truehandler = truename
	  	? script->FindOrCreateEventHandler (truename)
		: 0;
	  const char* falsename = GetAttributeString (child, "false", 0);
          celXmlScriptEventHandler* falsehandler = falsename
	  	? script->FindOrCreateEventHandler (falsename)
		: 0;
	  h->AddOperation (CEL_OPERATION_IF);
	  h->AddArgument ().SetString (varname);
	  h->AddArgument ().SetEventHandler (truehandler);
	  h->AddArgument ().SetEventHandler (falsehandler);
	}
        break;
      case XMLTOKEN_PRINT:
        h->AddOperation (CEL_OPERATION_PRINT);
	h->AddArgument ().SetString (child->GetContentsValue ());
        break;
      case XMLTOKEN_GETPROPERTY:
	{
	  const char* pcname = GetAttributeValue (child, "propclass");
	  if (!pcname) return false;
	  csStringID propid = GetAttributeID (child, "cel.property.", "name");
	  if (propid == csInvalidStringID) return false;
	  const char* varname = GetAttributeString (child, "var",
	  	"getproperty");
	  if (!varname) return false;
	  h->AddOperation (CEL_OPERATION_GETPROPERTY);
	  h->AddArgument ().SetString (varname);
	  h->AddArgument ().SetPC (h->GetResolver (pcname));
	  h->AddArgument ().SetID (propid);
	}
	break;
      case XMLTOKEN_PROPERTY:
        {
	  const char* pcname = GetAttributeValue (child, "propclass");
	  if (!pcname) return false;
	  csStringID propid = GetAttributeID (child, "cel.property.", "name");
	  if (propid == csInvalidStringID) return false;

	  h->AddOperation (CEL_OPERATION_PROPERTY);
	  h->AddArgument ().SetPC (h->GetResolver (pcname));
	  h->AddArgument ().SetID (propid);
	  if (!ParseValueArg (child, h))
	  {
            synldr->ReportError (
	        "cel.behaviour.xml", child,
		"Value is missing for <property>!");
	    return false;
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
  h->AddOperation (CEL_OPERATION_END);
  return true;
}

bool celBlXml::CreateBehaviourScriptFromDoc (const char* name,
  	iDocumentNode* node)
{
  celXmlScript* script = new celXmlScript (pl);
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
      celXmlScriptEventHandler* h = script->FindOrCreateEventHandler (
      	eventname);
      if (!ParseEventHandler (h, child, script))
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


