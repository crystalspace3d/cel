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
#include "csutil/scanstr.h"

#include "physicallayer/pl.h"

#include "plugins/behaviourlayer/xml/blxml.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "plugins/behaviourlayer/xml/xmlscript.h"
#include "plugins/behaviourlayer/xml/token.h"

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
  XMLTOKEN_PRINT,
  XMLTOKEN_CREATEENTITY,
  XMLTOKEN_CREATEPROPCLASS,

  XMLTOKEN_FLOAT,
  XMLTOKEN_BOOL,
  XMLTOKEN_STRING,
  XMLTOKEN_LONG,
  XMLTOKEN_VECTOR,
  XMLTOKEN_COLOR
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
  xmltokens.Register ("createentity", XMLTOKEN_CREATEENTITY);
  xmltokens.Register ("createpropclass", XMLTOKEN_CREATEPROPCLASS);

  xmltokens.Register ("float", XMLTOKEN_FLOAT);
  xmltokens.Register ("bool", XMLTOKEN_BOOL);
  xmltokens.Register ("string", XMLTOKEN_STRING);
  xmltokens.Register ("long", XMLTOKEN_LONG);
  xmltokens.Register ("vector", XMLTOKEN_VECTOR);
  xmltokens.Register ("color", XMLTOKEN_COLOR);

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
  csRef<iDocumentAttributeIterator> it = child->GetAttributes ();
  while (it->HasNext ())
  {
    csRef<iDocumentAttribute> attr = it->Next ();
    const char* attr_name = attr->GetName ();
    csStringID id = xmltokens.Request (attr_name);
    switch (id)
    {
      case XMLTOKEN_FLOAT:
	h->AddArgument ().SetFloat (attr->GetValueAsFloat ());
	break;
      case XMLTOKEN_STRING:
        h->AddArgument ().SetString (attr->GetValue ());
	break;
      case XMLTOKEN_BOOL:
        h->AddArgument ().SetBool ((bool)attr->GetValueAsInt ());
	break;
      case XMLTOKEN_LONG:
        h->AddArgument ().SetInt32 ((long)attr->GetValueAsInt ());
	break;
      case XMLTOKEN_VECTOR:
        {
	  csVector3 v;
	  int rc = csScanStr (attr->GetValue (), "%f,%f,%f", &v.x, &v.y, &v.z);
	  if (rc == 3)
            h->AddArgument ().SetVector (v);
	  else
	  {
	    csVector2 v2;
	    csScanStr (attr->GetValue (), "%f,%f", &v2.x, &v2.y);
            h->AddArgument ().SetVector (v2);
	  }
        }
	break;
      case XMLTOKEN_COLOR:
        {
	  csColor v;
	  csScanStr (attr->GetValue (), "%f,%f,%f", &v.red, &v.green, &v.blue);
          h->AddArgument ().SetColor (v);
        }
	break;
      case XMLTOKEN_VAR:
	h->AddArgument ().SetVar (attr->GetValue ());
	break;
    }
  }
  return true;
}

bool celBlXml::ParseExpression (iDocumentNode* child,
	celXmlScriptEventHandler* h, const char* attrname, const char* name,
	bool optional)
{
  const char* input = child->GetAttributeValue (attrname);
  if (!input && !optional)
  {
    synldr->ReportError ("cel.behaviour.xml", child,
		"Can't find attribute '%s' for '%s'!", attrname, name);
    return false;
  }
  if (optional)
  {
    // @@@@@@@
  }
  char buf[100];
  sprintf (buf, "%s(%s)", name, attrname);
  bool rc = ParseExpression (input, child, h, buf, CEL_PRIORITY_NORMAL);
  if (!rc) return false;

  // Check if there are remaining tokens.
  input = celXmlSkipWhiteSpace (input);
  if (*input != 0)
  {
    synldr->ReportError ("cel.behaviour.xml", child,
		"Unexpected tokens found for '%s'!", buf);
    return false;
  }

  return true;
}

bool celBlXml::ParseExpression (const char*& input, iDocumentNode* child,
	celXmlScriptEventHandler* h, const char* name, int stoppri)
{
  int token;
  const char* pinput = input;
  input = celXmlParseToken (input, token);

  switch (token)
  {
    case CEL_TOKEN_ERROR:
      synldr->ReportError ("cel.behaviour.xml", child,
		"Error parsing expression for '%s'!", name);
      return false;
    case CEL_TOKEN_DEREFVAR:
      input = celXmlSkipWhiteSpace (input);
      if (isalpha (*input) || *input == '_')
      {
        pinput = input;
	input++;
        while (isalnum (*input) || *input == '_') input++;
	char* str = new char [input-pinput+1];
	strncpy (str, pinput, input-pinput);
	str[input-pinput] = 0;
        h->AddOperation (CEL_OPERATION_PUSH);
        h->AddArgument ().SetString (str);
        delete[] str;
      }
      else if (!ParseExpression (input, child, h, name, 0))
        return false;
      h->AddOperation (CEL_OPERATION_DEREFVAR);
      break;
    case CEL_TOKEN_STRINGLIT:
      {
        char* str;
        input = celXmlParseString (input, str);
        if (!str)
        {
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Error parsing string for '%s'!", name);
          return false;
        }
        h->AddOperation (CEL_OPERATION_PUSH);
        h->AddArgument ().SetString (str);
        delete[] str;
      }
      break;
    case CEL_TOKEN_ENTITY:
      {
        if (!ParseExpression (input, child, h, name, 0))
          return false;
        h->AddOperation (CEL_OPERATION_ENTITY);
      }
      break;
    case CEL_TOKEN_PC:
      {
        if (!ParseExpression (input, child, h, name, 0))
          return false;
	input = celXmlParseToken (input, token);
	if (token != CEL_TOKEN_COMMA)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Error parsing 'pc' for '%s'!", name);
          return false;
	}
        if (!ParseExpression (input, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_PC);
      }
      break;
    case CEL_TOKEN_INT32:
      {
        int l = input-pinput;
        char* str = new char [l+1];
        strncpy (str, pinput, l);
        str[l] = 0;
        int32 i;
        sscanf (str, "%u", &i);
        delete[] str;
        h->AddOperation (CEL_OPERATION_PUSH);
        h->AddArgument ().SetInt32 (i);
      }
      break;
    case CEL_TOKEN_UINT32:
      {
        int l = input-pinput;
        char* str = new char [l+1];
        strncpy (str, pinput, l);
        str[l] = 0;
        uint32 i;
        sscanf (str, "%ud", &i);
        delete[] str;
        h->AddOperation (CEL_OPERATION_PUSH);
        h->AddArgument ().SetUInt32 (i);
      }
      break;
    case CEL_TOKEN_FLOAT:
      {
        int l = input-pinput;
        char* str = new char [l+1];
        strncpy (str, pinput, l);
        str[l] = 0;
        float i;
        sscanf (str, "%f", &i);
        delete[] str;
        h->AddOperation (CEL_OPERATION_PUSH);
        h->AddArgument ().SetFloat (i);
      }
      break;
    case CEL_TOKEN_COLOR:
      {
        if (!ParseExpression (input, child, h, name, 0))
	  return false;
	input = celXmlParseToken (input, token);
	if (token != CEL_TOKEN_COMMA)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Error parsing color for '%s'!", name);
          return false;
	}
        if (!ParseExpression (input, child, h, name, 0))
	  return false;
	input = celXmlParseToken (input, token);
	if (token != CEL_TOKEN_COMMA)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Error parsing color for '%s'!", name);
          return false;
	}
        if (!ParseExpression (input, child, h, name, 0))
	  return false;
	input = celXmlParseToken (input, token);
	if (token != CEL_TOKEN_CLOSE)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Error parsing color for '%s'!", name);
          return false;
	}
        h->AddOperation (CEL_OPERATION_COLOR);
      }
      break;
    case CEL_TOKEN_VECTOR:
      {
        int op = CEL_OPERATION_VECTOR2;
        if (!ParseExpression (input, child, h, name, 0))
	  return false;
	input = celXmlParseToken (input, token);
	if (token != CEL_TOKEN_COMMA)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Error parsing vector for '%s'!", name);
          return false;
	}
        if (!ParseExpression (input, child, h, name, 0))
	  return false;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_COMMA)
	{
          op = CEL_OPERATION_VECTOR3;
          if (!ParseExpression (input, child, h, name, 0))
	    return false;
	  input = celXmlParseToken (input, token);
	}
	if (token != CEL_TOKEN_VECTORCLOSE)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Error parsing vector for '%s'!", name);
          return false;
	}
        h->AddOperation (op);
      }
      break;
    case CEL_TOKEN_BOOLTRUE:
      h->AddOperation (CEL_OPERATION_PUSH);
      h->AddArgument ().SetBool (true);
      break;
    case CEL_TOKEN_BOOLFALSE:
      h->AddOperation (CEL_OPERATION_PUSH);
      h->AddArgument ().SetBool (false);
      break;
    case CEL_TOKEN_OPEN:
      if (!ParseExpression (input, child, h, name, 0))
	return false;
      input = celXmlParseToken (input, token);
      if (token != CEL_TOKEN_CLOSE)
      {
        synldr->ReportError ("cel.behaviour.xml", child,
		    "Missing ')' in expression for '%s'!", name);
        return false;
      }
      break;
    case CEL_TOKEN_MINUS:
      if (!ParseExpression (input, child, h, name, 0))
	return false;
      h->AddOperation (CEL_OPERATION_UNARYMINUS);
      break;
    case CEL_TOKEN_COMMA:
      synldr->ReportError ("cel.behaviour.xml", child,
		    "Unexpected ',' for '%s'!", name);
      return false;
    case CEL_TOKEN_CLOSE:
      synldr->ReportError ("cel.behaviour.xml", child,
		    "Unexpected ')' for '%s'!", name);
      return false;
    case CEL_TOKEN_END:
      synldr->ReportError ("cel.behaviour.xml", child,
		    "Unexpected end of expression for '%s'!", name);
      return false;
    default:
      synldr->ReportError ("cel.behaviour.xml", child,
		    "Unexpected token for '%s'!", name);
      return false;
  }

  // If we only want one term then we stop here.
  if (stoppri == CEL_PRIORITY_ONETERM) return true;

  for (;;)
  {
    pinput = input;
    input = celXmlParseToken (input, token);

    switch (token)
    {
      case CEL_TOKEN_ERROR:
        synldr->ReportError ("cel.behaviour.xml", child,
		  "Error parsing expression for '%s'!", name);
        return false;
      case CEL_TOKEN_END:
        return true;
      case CEL_TOKEN_COMMA:
      case CEL_TOKEN_CLOSE:
      case CEL_TOKEN_VECTORCLOSE:
        input = pinput;	// Restore.
        return true;
      case CEL_TOKEN_MINUS:
        if (stoppri > CEL_PRIORITY_ADD)
        {
          input = pinput; // Restore.
	  return true;
        }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_ADD))
	  return false;
        h->AddOperation (CEL_OPERATION_MINUS);
        break;
      case CEL_TOKEN_ADD:
        if (stoppri > CEL_PRIORITY_ADD)
        {
          input = pinput; // Restore.
	  return true;
        }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_ADD))
	  return false;
        h->AddOperation (CEL_OPERATION_ADD);
        break;
      case CEL_TOKEN_MULT:
        if (stoppri > CEL_PRIORITY_MULT)
        {
          input = pinput; // Restore.
	  return true;
        }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_MULT))
	  return false;
        h->AddOperation (CEL_OPERATION_MULT);
        break;
      case CEL_TOKEN_DIV:
        if (stoppri > CEL_PRIORITY_MULT)
        {
          input = pinput; // Restore.
	  return true;
        }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_MULT))
	  return false;
        h->AddOperation (CEL_OPERATION_DIV);
        break;
      default:
        synldr->ReportError ("cel.behaviour.xml", child,
		    "Unexpected token %d for '%s' (2)!", token, name);
        return false;
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
      case XMLTOKEN_CREATEPROPCLASS:
        {
          if (!ParseExpression (child, h, "name", "createpropclass", false))
	    return false;
	  h->AddOperation (CEL_OPERATION_CREATEPROPCLASS);
	}
	break;
      case XMLTOKEN_CREATEENTITY:
        {
          if (!ParseExpression (child, h, "name", "createentity", false))
	    return false;
          if (!ParseExpression (child, h, "behaviour", "createentity", false))
	    return false;
	  h->AddOperation (CEL_OPERATION_CREATEENTITY);
	}
	break;
      case XMLTOKEN_VAR:
        {
          if (!ParseExpression (child, h, "name", "var", false))
	    return false;
          if (!ParseExpression (child, h, "value", "var", false))
	    return false;
	  h->AddOperation (CEL_OPERATION_VAR);
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
          if (!ParseExpression (child, h, "pcbillboard", "testcollide", false))
	    return false;
	  const char* truename = GetAttributeString (child, "true", 0);
          celXmlScriptEventHandler* truehandler = truename
	  	? script->FindOrCreateEventHandler (truename)
		: 0;
	  const char* falsename = GetAttributeString (child, "false", 0);
          celXmlScriptEventHandler* falsehandler = falsename
	  	? script->FindOrCreateEventHandler (falsename)
		: 0;
	  h->AddOperation (CEL_OPERATION_TESTCOLLIDE);
	  h->AddArgument ().SetEventHandler (truehandler);
	  h->AddArgument ().SetEventHandler (falsehandler);
	}
        break;
      case XMLTOKEN_IF:
        {
          if (!ParseExpression (child, h, "eval", "if", false))
	    return false;
	  const char* truename = GetAttributeString (child, "true", 0);
          celXmlScriptEventHandler* truehandler = truename
	  	? script->FindOrCreateEventHandler (truename)
		: 0;
	  const char* falsename = GetAttributeString (child, "false", 0);
          celXmlScriptEventHandler* falsehandler = falsename
	  	? script->FindOrCreateEventHandler (falsename)
		: 0;
	  h->AddOperation (CEL_OPERATION_IF);
	  h->AddArgument ().SetEventHandler (truehandler);
	  h->AddArgument ().SetEventHandler (falsehandler);
	}
        break;
      case XMLTOKEN_PRINT:
        if (!ParseExpression (child, h, "value", "print", false))
	  return false;
        h->AddOperation (CEL_OPERATION_PRINT);
        break;
      case XMLTOKEN_GETPROPERTY:
	{
	  const char* entname = child->GetAttributeValue ("entity"); // Optional
	  const char* pcname = GetAttributeValue (child, "propclass");
	  if (!pcname) return false;
	  csStringID propid = GetAttributeID (child, "cel.property.", "name");
	  if (propid == csInvalidStringID) return false;
	  const char* varname = GetAttributeString (child, "var",
	  	"getproperty");
	  if (!varname) return false;
	  h->AddOperation (CEL_OPERATION_GETPROPERTY);
	  h->AddArgument ().SetString (varname);
	  h->AddArgument ().SetPC (h->GetResolver (entname, pcname));
	  h->AddArgument ().SetID (propid);
	}
	break;
      case XMLTOKEN_PROPERTY:
        {
	  const char* entname = child->GetAttributeValue ("entity"); // Optional
	  const char* pcname = GetAttributeValue (child, "propclass");
	  if (!pcname) return false;
	  csStringID propid = GetAttributeID (child, "cel.property.", "name");
	  if (propid == csInvalidStringID) return false;

	  h->AddOperation (CEL_OPERATION_PROPERTY);
	  h->AddArgument ().SetPC (h->GetResolver (entname, pcname));
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
	  const char* entname = child->GetAttributeValue ("entity"); // Optional
	  const char* pcname = GetAttributeValue (child, "propclass");
	  if (!pcname) return false;
	  csStringID propid = GetAttributeID (child, "cel.property.", "name");
	  if (propid == csInvalidStringID) return false;
	  const char* params = GetAttributeValue (child, "params");
	  if (!params) return false;
	  h->AddOperation (CEL_OPERATION_ACTION);
	  h->AddArgument ().SetPC (h->GetResolver (entname, pcname));
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


