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

#include <ctype.h>
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
  XMLTOKEN_ACTION,
  XMLTOKEN_VAR,
  XMLTOKEN_TESTCOLLIDE,
  XMLTOKEN_IF,
  XMLTOKEN_FOR,
  XMLTOKEN_PRINT,
  XMLTOKEN_CREATEENTITY,
  XMLTOKEN_CREATEPROPCLASS,
  XMLTOKEN_DEFAULT,

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
  xmltokens.Register ("action", XMLTOKEN_ACTION);
  xmltokens.Register ("var", XMLTOKEN_VAR);
  xmltokens.Register ("testcollide", XMLTOKEN_TESTCOLLIDE);
  xmltokens.Register ("if", XMLTOKEN_IF);
  xmltokens.Register ("for", XMLTOKEN_FOR);
  xmltokens.Register ("print", XMLTOKEN_PRINT);
  xmltokens.Register ("createentity", XMLTOKEN_CREATEENTITY);
  xmltokens.Register ("createpropclass", XMLTOKEN_CREATEPROPCLASS);
  xmltokens.Register ("default", XMLTOKEN_DEFAULT);

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

bool celBlXml::ParseExpression (iDocumentNode* child,
	celXmlScriptEventHandler* h, const char* attrname, const char* name,
	int optional_type)
{
  const char* input = child->GetAttributeValue (attrname);
  if (!input && optional_type == CEL_TYPE_NONE)
  {
    synldr->ReportError ("cel.behaviour.xml", child,
		"Can't find attribute '%s' for '%s'!", attrname, name);
    return false;
  }
  if (!input && optional_type != CEL_TYPE_NONE)
  {
    h->AddOperation (CEL_OPERATION_PUSH);
    switch (optional_type)
    {
      case CEL_TYPE_STRING: h->AddArgument ().SetString (0, false); break;
      case CEL_TYPE_PC: h->AddArgument ().SetPC (0); break;
      default: CS_ASSERT (false);
    }
    return true;
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
  input = celXmlSkipWhiteSpace (input);
  const char* pinput = input;
  input = celXmlParseToken (input, token);

  switch (token)
  {
    case CEL_TOKEN_ERROR:
      synldr->ReportError ("cel.behaviour.xml", child,
		"Error parsing expression for '%s'!", name);
      return false;
    case CEL_TOKEN_DEREFVAR:
      if (!ParseExpression (input, child, h, name, CEL_PRIORITY_ONETERM))
        return false;
      h->AddOperation (CEL_OPERATION_DEREFVAR);
      break;
    case CEL_TOKEN_PROPERTY:
      {
        if (!ParseExpression (input, child, h, name, 0))
          return false;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_CLOSE)
	{
	  // Use default property class.
          h->AddOperation (CEL_OPERATION_GETPROPERTY1);
	}
	else
	{
	  if (token != CEL_TOKEN_COMMA)
	  {
            synldr->ReportError ("cel.behaviour.xml", child,
		        "Expected ',' while parsing property for '%s'!", name);
            return false;
	  }
          if (!ParseExpression (input, child, h, name, 0))
	    return false;
	  input = celXmlParseToken (input, token);
	  if (token != CEL_TOKEN_CLOSE)
	  {
            synldr->ReportError ("cel.behaviour.xml", child,
	        "Expected ')' while parsing property for '%s'!", name);
            return false;
	  }
          h->AddOperation (CEL_OPERATION_GETPROPERTY);
        }
      }
      break;
    case CEL_TOKEN_PROPID:
      {
	input = celXmlSkipWhiteSpace (input);
        const char* i = input;
	bool idconstant = true;
	while (*i && *i != ')')
	{
	  if (!isalnum (*i) && *i != '_' && *i != '.')
	  {
	    idconstant = false;
	    break;
	  }
	  i++;
	}
	if (!*i)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Missing ')' at end of 'propid' for '%s'!", name);
          return false;
	}
	if (idconstant)
	{
	  const char* prefix = "cel.property.";
	  char* str = new char [strlen (prefix) + i-input+1];
	  strcpy (str, prefix);
	  strncpy (str+strlen (prefix), input, i-input);
	  str[strlen (prefix) + i-input] = 0;
	  csStringID id = pl->FetchStringID (str);
	  delete[] str;
          h->AddOperation (CEL_OPERATION_PUSH);
          h->AddArgument ().SetID (id);
	  input = i+1;
	}
	else
	{
          if (!ParseExpression (input, child, h, name, 0))
            return false;
	  input = celXmlParseToken (input, token);
	  if (token != CEL_TOKEN_CLOSE)
	  {
            synldr->ReportError ("cel.behaviour.xml", child,
		      "Missing ')' at end of 'propid' for '%s'!", name);
            return false;
	  }
          h->AddOperation (CEL_OPERATION_CALCPROPID);
	}
      }
      break;
    case CEL_TOKEN_ID:
      {
	input = celXmlSkipWhiteSpace (input);
        const char* i = input;
	bool idconstant = true;
	while (*i && *i != ')')
	{
	  if (!isalnum (*i) && *i != '_' && *i != '.')
	  {
	    idconstant = false;
	    break;
	  }
	  i++;
	}
	if (!*i)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Missing ')' at end of 'id' for '%s'!", name);
          return false;
	}
	if (idconstant)
	{
	  char* str = new char [i-input+1];
	  strncpy (str, input, i-input);
	  str[i-input] = 0;
	  csStringID id = pl->FetchStringID (str);
	  delete[] str;
          h->AddOperation (CEL_OPERATION_PUSH);
          h->AddArgument ().SetID (id);
	  input = i+1;
	}
	else
	{
          if (!ParseExpression (input, child, h, name, 0))
            return false;
	  input = celXmlParseToken (input, token);
	  if (token != CEL_TOKEN_CLOSE)
	  {
            synldr->ReportError ("cel.behaviour.xml", child,
		      "Missing ')' at end of 'id' for '%s'!", name);
            return false;
	  }
          h->AddOperation (CEL_OPERATION_CALCID);
	}
      }
      break;
    case CEL_TOKEN_IDENTIFIER:
      {
        char* str;
	str = new char[input-pinput+1];
	strncpy (str, pinput, input-pinput);
	str[input-pinput] = 0;
        h->AddOperation (CEL_OPERATION_PUSH);
        h->AddArgument ().SetString (str, true);
	delete[] str;
      }
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
        h->AddArgument ().SetString (str, true);
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
	if (token == CEL_TOKEN_CLOSE)
	{
	  // We have only one argument to pc(). This means we search
	  // for a property class for the current entity.
          h->AddOperation (CEL_OPERATION_PCTHIS);
	}
	else
	{
	  if (token != CEL_TOKEN_COMMA)
	  {
            synldr->ReportError ("cel.behaviour.xml", child,
		      "Expected ',' while parsing pc for '%s'!", name);
            return false;
	  }
          if (!ParseExpression (input, child, h, name, 0))
	    return false;
	  input = celXmlParseToken (input, token);
	  if (token != CEL_TOKEN_CLOSE)
	  {
            synldr->ReportError ("cel.behaviour.xml", child,
		      "Expected ')' while parsing pc for '%s'!", name);
            return false;
	  }
          h->AddOperation (CEL_OPERATION_PC);
        }
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
      case CEL_TOKEN_EQ:
        if (stoppri >= CEL_PRIORITY_EQUAL) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_EQUAL))
	  return false;
        h->AddOperation (CEL_OPERATION_EQ);
        break;
      case CEL_TOKEN_NE:
        if (stoppri >= CEL_PRIORITY_EQUAL) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_EQUAL))
	  return false;
        h->AddOperation (CEL_OPERATION_NE);
        break;
      case CEL_TOKEN_LT:
        if (stoppri >= CEL_PRIORITY_LTGT) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_LTGT))
	  return false;
        h->AddOperation (CEL_OPERATION_LT);
        break;
      case CEL_TOKEN_LE:
        if (stoppri >= CEL_PRIORITY_LTGT) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_LTGT))
	  return false;
        h->AddOperation (CEL_OPERATION_LE);
        break;
      case CEL_TOKEN_GT:
        if (stoppri >= CEL_PRIORITY_LTGT) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_LTGT))
	  return false;
        h->AddOperation (CEL_OPERATION_GT);
        break;
      case CEL_TOKEN_GE:
        if (stoppri >= CEL_PRIORITY_LTGT) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_LTGT))
	  return false;
        h->AddOperation (CEL_OPERATION_GE);
        break;
      case CEL_TOKEN_MINUS:
        if (stoppri >= CEL_PRIORITY_ADDSUB) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_ADDSUB))
	  return false;
        h->AddOperation (CEL_OPERATION_MINUS);
        break;
      case CEL_TOKEN_ADD:
        if (stoppri >= CEL_PRIORITY_ADDSUB) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_ADDSUB))
	  return false;
        h->AddOperation (CEL_OPERATION_ADD);
        break;
      case CEL_TOKEN_MULT:
        if (stoppri >= CEL_PRIORITY_MULTDIV) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_MULTDIV))
	  return false;
        h->AddOperation (CEL_OPERATION_MULT);
        break;
      case CEL_TOKEN_DIV:
        if (stoppri >= CEL_PRIORITY_MULTDIV) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_MULTDIV))
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
          if (!ParseExpression (child, h, "name", "createpropclass"))
	    return false;
	  h->AddOperation (CEL_OPERATION_CREATEPROPCLASS);
	}
	break;
      case XMLTOKEN_CREATEENTITY:
        {
          if (!ParseExpression (child, h, "name", "createentity"))
	    return false;
          if (!ParseExpression (child, h, "behaviour", "createentity"))
	    return false;
	  h->AddOperation (CEL_OPERATION_CREATEENTITY);
	}
	break;
      case XMLTOKEN_VAR:
        {
          if (!ParseExpression (child, h, "name", "var"))
	    return false;
          if (!ParseExpression (child, h, "value", "var"))
	    return false;
	  h->AddOperation (CEL_OPERATION_VAR);
	}
	break;
      case XMLTOKEN_TESTCOLLIDE:
        {
          if (!ParseExpression (child, h, "pcbillboard", "testcollide"))
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
      case XMLTOKEN_FOR:
        {
          if (!ParseExpression (child, h, "var", "for"))
	    return false;
          if (!ParseExpression (child, h, "start", "for"))
	    return false;
          if (!ParseExpression (child, h, "end", "for"))
	    return false;
	  const char* execname = GetAttributeString (child, "exec", 0);
          celXmlScriptEventHandler* handler = execname
	  	? script->FindOrCreateEventHandler (execname)
		: 0;
	  h->AddOperation (CEL_OPERATION_FOR);
	  h->AddArgument ().SetEventHandler (handler);
	}
        break;
      case XMLTOKEN_IF:
        {
          if (!ParseExpression (child, h, "eval", "if"))
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
        if (!ParseExpression (child, h, "value", "print"))
	  return false;
        h->AddOperation (CEL_OPERATION_PRINT);
        break;
      case XMLTOKEN_DEFAULT:
        if (!ParseExpression (child, h, "propclass", "default"))
	  return false;
        h->AddOperation (CEL_OPERATION_DEFAULTPC);
        break;
      case XMLTOKEN_PROPERTY:
        {
          if (!ParseExpression (child, h, "propclass", "property", CEL_TYPE_PC))
	    return false;
          if (!ParseExpression (child, h, "id", "property"))
	    return false;
          if (!ParseExpression (child, h, "value", "property"))
	    return false;
	  h->AddOperation (CEL_OPERATION_PROPERTY);
	}
	break;
      case XMLTOKEN_ACTION:
        {
          if (!ParseExpression (child, h, "propclass", "action", CEL_TYPE_PC))
	    return false;
          if (!ParseExpression (child, h, "id", "action"))
	    return false;
          if (!ParseExpression (child, h, "params", "action", CEL_TYPE_STRING))
	    return false;
	  h->AddOperation (CEL_OPERATION_ACTION);
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


