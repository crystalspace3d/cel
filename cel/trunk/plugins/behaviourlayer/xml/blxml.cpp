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
  XMLTOKEN_IF,
  XMLTOKEN_FOR,
  XMLTOKEN_PRINT,
  XMLTOKEN_CALL,
  XMLTOKEN_DESTROYENTITY,
  XMLTOKEN_CREATEENTITY,
  XMLTOKEN_CREATEPROPCLASS,
  XMLTOKEN_DEFAULT,
  XMLTOKEN_STOP,

  XMLTOKEN_LAST
};

enum
{
  XMLFUNCTION_PC,
  XMLFUNCTION_PARAM,
  XMLFUNCTION_PROPERTY,
  XMLFUNCTION_ID,
  XMLFUNCTION_PARID,
  XMLFUNCTION_PROPID,
  XMLFUNCTION_RGB,
  XMLFUNCTION_ABS,
  XMLFUNCTION_MIN,
  XMLFUNCTION_MAX,
  XMLFUNCTION_SIGN,
  XMLFUNCTION_INTPOL,
  XMLFUNCTION_INT,
  XMLFUNCTION_FLOAT,
  XMLFUNCTION_RAND,
  XMLFUNCTION_TESTCOLLIDE,
  XMLFUNCTION_IF,

  XMLFUNCTION_LAST
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
  xmltokens.Register ("if", XMLTOKEN_IF);
  xmltokens.Register ("for", XMLTOKEN_FOR);
  xmltokens.Register ("print", XMLTOKEN_PRINT);
  xmltokens.Register ("call", XMLTOKEN_CALL);
  xmltokens.Register ("destroyentity", XMLTOKEN_DESTROYENTITY);
  xmltokens.Register ("createentity", XMLTOKEN_CREATEENTITY);
  xmltokens.Register ("createpropclass", XMLTOKEN_CREATEPROPCLASS);
  xmltokens.Register ("default", XMLTOKEN_DEFAULT);
  xmltokens.Register ("stop", XMLTOKEN_STOP);

  functions.Register ("pc", XMLFUNCTION_PC);
  functions.Register ("param", XMLFUNCTION_PARAM);
  functions.Register ("property", XMLFUNCTION_PROPERTY);
  functions.Register ("id", XMLFUNCTION_ID);
  functions.Register ("parid", XMLFUNCTION_PARID);
  functions.Register ("propid", XMLFUNCTION_PROPID);
  functions.Register ("rgb", XMLFUNCTION_RGB);
  functions.Register ("abs", XMLFUNCTION_ABS);
  functions.Register ("min", XMLFUNCTION_MIN);
  functions.Register ("max", XMLFUNCTION_MAX);
  functions.Register ("sign", XMLFUNCTION_SIGN);
  functions.Register ("intpol", XMLFUNCTION_INTPOL);
  functions.Register ("int", XMLFUNCTION_INT);
  functions.Register ("float", XMLFUNCTION_FLOAT);
  functions.Register ("rand", XMLFUNCTION_RAND);
  functions.Register ("testcollide", XMLFUNCTION_TESTCOLLIDE);
  functions.Register ("if", XMLFUNCTION_IF);

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
  if (!input && optional_type == CEL_DATA_NONE)
  {
    synldr->ReportError ("cel.behaviour.xml", child,
		"Can't find attribute '%s' for '%s'!", attrname, name);
    return false;
  }
  if (!input && optional_type != CEL_DATA_NONE)
  {
    h->AddOperation (CEL_OPERATION_PUSH);
    switch (optional_type)
    {
      case CEL_DATA_STRING: h->GetArgument ().SetString (0, false); break;
      case CEL_DATA_PCLASS: h->GetArgument ().SetPC (0); break;
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

bool celBlXml::SkipComma (const char*& input, iDocumentNode* child,
	const char* name)
{
  int token;
  input = celXmlParseToken (input, token);
  if (token != CEL_TOKEN_COMMA)
  {
    synldr->ReportError ("cel.behaviour.xml", child,
		    "Expected ',' for '%s'!", name);
    return false;
  }
  return true;
}

bool celBlXml::ParseFunction (const char*& input, const char* pinput,
	iDocumentNode* child,
	celXmlScriptEventHandler* h, const char* name)
{
  int token;
  char str[256];	// @@@ Hardcoded!
  strncpy (str, pinput, input-pinput-1);	// Copy all execpt '('.
  str[input-pinput-1] = 0;
  csStringID fun_id = functions.Request (str);
  switch (fun_id)
  {
    case XMLFUNCTION_TESTCOLLIDE:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
	h->AddOperation (CEL_OPERATION_TESTCOLLIDE);
      }
      break;
    case XMLFUNCTION_ABS:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_ABS);
      }
      break;
    case XMLFUNCTION_IF:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_IFFUN);
      }
      break;
    case XMLFUNCTION_MIN:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_MIN);
      }
      break;
    case XMLFUNCTION_MAX:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_MAX);
      }
      break;
    case XMLFUNCTION_SIGN:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_SIGN);
      }
      break;
    case XMLFUNCTION_INTPOL:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_INTPOL);
      }
      break;
    case XMLFUNCTION_RAND:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_RAND);
      }
      break;
    case XMLFUNCTION_INT:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_INT);
      }
      break;
    case XMLFUNCTION_FLOAT:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_FLOAT);
      }
      break;
    case XMLFUNCTION_RGB:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
        h->AddOperation (CEL_OPERATION_COLOR);
      }
      break;
    case XMLFUNCTION_PC:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
	pinput = input;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_COMMA)
	{
          if (!ParseExpression (input, child, h, name, 0))
	    return false;
          h->AddOperation (CEL_OPERATION_PC);
	}
	else
	{
	  input = pinput;	// Set back to ')'
	  // We have only one argument to pc(). This means we search
	  // for a property class for the current entity.
          h->AddOperation (CEL_OPERATION_PCTHIS);
	}
      }
      break;
    case XMLFUNCTION_PROPERTY:
      {
        if (!ParseExpression (input, child, h, name, 0)) return false;
	pinput = input;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_COMMA)
	{
          if (!ParseExpression (input, child, h, name, 0)) return false;
          h->AddOperation (CEL_OPERATION_GETPROPERTY);
	}
	else
	{
	  input = pinput;	// Set back to ')'
	  // Use default property class.
          h->AddOperation (CEL_OPERATION_GETPROPERTY1);
	}
      }
      break;
    case XMLFUNCTION_ID:
    case XMLFUNCTION_PROPID:
    case XMLFUNCTION_PARID:
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
	     "Missing ')' at end of '%s' for '%s'!", str, name);
          return false;
	}
	if (idconstant)
	{
	  const char* prefix;
	  if (fun_id == XMLFUNCTION_PARID)
	    prefix = "cel.behaviour.parameter.";
	  else if (fun_id == XMLFUNCTION_PROPID)
	    prefix = "cel.property.";
	  else
	    prefix = "";
	  char* str = new char [strlen (prefix) + i-input+1];
	  strcpy (str, prefix);
	  strncpy (str+strlen (prefix), input, i-input);
	  str[strlen (prefix) + i-input] = 0;
	  csStringID id = pl->FetchStringID (str);
	  delete[] str;
          h->AddOperation (CEL_OPERATION_PUSH);
          h->GetArgument ().SetID (id);
	  input = i;	// Right before close bracket.
	}
	else
	{
          if (!ParseExpression (input, child, h, name, 0)) return false;
          h->AddOperation (
		fun_id == XMLFUNCTION_PARID
		? CEL_OPERATION_CALCPARID
		: fun_id == XMLFUNCTION_PROPID
		? CEL_OPERATION_CALCPROPID
		: CEL_OPERATION_CALCID);
        }
      }
      break;
    case XMLFUNCTION_PARAM:
      if (!ParseExpression (input, child, h, name, CEL_PRIORITY_NORMAL))
        return false;
      h->AddOperation (CEL_OPERATION_PARAM);
      break;
    default:
      synldr->ReportError ("cel.behaviour.xml", child,
	  "Unknown function '%s' for '%s'!", str, name);
      return false;
  }
  input = celXmlParseToken (input, token);
  if (token != CEL_TOKEN_CLOSE)
  {
    synldr->ReportError ("cel.behaviour.xml", child,
	"Expected ')' after '%s' for '%s'!", str, name);
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
      pinput = input;
      input = celXmlParseToken (input, token);
      if (token == CEL_TOKEN_DOT)
      {
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_ONETERM))
          return false;
        h->AddOperation (CEL_OPERATION_DEREFVARENT);
      }
      else
      {
        input = pinput;	// Restore!
        h->AddOperation (CEL_OPERATION_DEREFVAR);
      }
      break;
    case CEL_TOKEN_FUNCTION:
      if (!ParseFunction (input, pinput, child, h, name))
        return false;
      break;
    case CEL_TOKEN_IDENTIFIER:
      {
        char* str;
	str = new char[input-pinput+1];
	strncpy (str, pinput, input-pinput);
	str[input-pinput] = 0;
        h->AddOperation (CEL_OPERATION_PUSH);
        h->GetArgument ().SetString (str, true);
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
        h->GetArgument ().SetString (str, true);
        delete[] str;
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
        h->GetArgument ().SetInt32 (i);
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
        h->GetArgument ().SetUInt32 (i);
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
        h->GetArgument ().SetFloat (i);
      }
      break;
    case CEL_TOKEN_VECTOR:
      {
        int op = CEL_OPERATION_VECTOR2;
        if (!ParseExpression (input, child, h, name, 0)) return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, child, h, name, 0)) return false;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_COMMA)
	{
          op = CEL_OPERATION_VECTOR3;
          if (!ParseExpression (input, child, h, name, 0)) return false;
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
      h->GetArgument ().SetBool (true);
      break;
    case CEL_TOKEN_BOOLFALSE:
      h->AddOperation (CEL_OPERATION_PUSH);
      h->GetArgument ().SetBool (false);
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
      if (!ParseExpression (input, child, h, name, CEL_PRIORITY_ONETERM))
	return false;
      h->AddOperation (CEL_OPERATION_UNARYMINUS);
      break;
    case CEL_TOKEN_BITNOT:
      if (!ParseExpression (input, child, h, name, CEL_PRIORITY_ONETERM))
	return false;
      h->AddOperation (CEL_OPERATION_BITNOT);
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
      case CEL_TOKEN_BITAND:
        if (stoppri >= CEL_PRIORITY_BITAND) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_BITAND))
	  return false;
        h->AddOperation (CEL_OPERATION_BITAND);
        break;
      case CEL_TOKEN_BITOR:
        if (stoppri >= CEL_PRIORITY_BITOR) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_BITOR))
	  return false;
        h->AddOperation (CEL_OPERATION_BITOR);
        break;
      case CEL_TOKEN_BITXOR:
        if (stoppri >= CEL_PRIORITY_BITXOR) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_BITXOR))
	  return false;
        h->AddOperation (CEL_OPERATION_BITXOR);
        break;
      case CEL_TOKEN_LOGAND:
        if (stoppri >= CEL_PRIORITY_LOGAND) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_LOGAND))
	  return false;
        h->AddOperation (CEL_OPERATION_LOGAND);
        break;
      case CEL_TOKEN_LOGOR:
        if (stoppri >= CEL_PRIORITY_LOGOR) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_LOGOR))
	  return false;
        h->AddOperation (CEL_OPERATION_LOGOR);
        break;
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
      case CEL_TOKEN_MODULO:
        if (stoppri >= CEL_PRIORITY_MULTDIV) { input = pinput; return true; }
        if (!ParseExpression (input, child, h, name, CEL_PRIORITY_MULTDIV))
	  return false;
        h->AddOperation (CEL_OPERATION_MODULO);
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
      case XMLTOKEN_STOP:
        h->AddOperation (CEL_OPERATION_END);
        break;
      case XMLTOKEN_CREATEPROPCLASS:
        {
          if (!ParseExpression (child, h, "name", "createpropclass"))
	    return false;
	  h->AddOperation (CEL_OPERATION_CREATEPROPCLASS);
	}
	break;
      case XMLTOKEN_CALL:
        {
          if (!ParseExpression (child, h, "entity", "call"))
	    return false;
          if (!ParseExpression (child, h, "event", "call"))
	    return false;
	  h->AddOperation (CEL_OPERATION_CALL);
	}
	break;
      case XMLTOKEN_DESTROYENTITY:
        {
          if (!ParseExpression (child, h, "name", "destroyentity"))
	    return false;
	  h->AddOperation (CEL_OPERATION_DESTROYENTITY);
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
	  const char* entname = child->GetAttributeValue ("entity");
	  if (entname)
            if (!ParseExpression (child, h, "entity", "var"))
	      return false;
          if (!ParseExpression (child, h, "name", "var"))
	    return false;
          if (!ParseExpression (child, h, "value", "var"))
	    return false;
	  h->AddOperation (entname ? CEL_OPERATION_VARENT : CEL_OPERATION_VAR);
	}
	break;
      case XMLTOKEN_FOR:
        {
	  const char* execname = GetAttributeString (child, "exec", 0);
	  if (execname)
	  {
            if (!ParseExpression (child, h, "var", "for"))
	      return false;
            if (!ParseExpression (child, h, "start", "for"))
	      return false;
            if (!ParseExpression (child, h, "end", "for"))
	      return false;
            celXmlScriptEventHandler* handler = execname
	  	? script->FindOrCreateEventHandler (execname)
		: 0;
	    h->AddOperation (CEL_OPERATION_FOR);
	    h->GetArgument ().SetEventHandlers (handler, 0);
	  }
	  else
	  {
	    // @@@ Not yet supported!
	  }
	}
        break;
      case XMLTOKEN_IF:
        {
          if (!ParseExpression (child, h, "eval", "if"))
	    return false;
	  const char* truename = GetAttributeString (child, "true", 0);
	  const char* falsename = GetAttributeString (child, "false", 0);
	  if (truename || falsename)
	  {
            celXmlScriptEventHandler* truehandler = truename
	  	? script->FindOrCreateEventHandler (truename)
		: 0;
            celXmlScriptEventHandler* falsehandler = falsename
	  	? script->FindOrCreateEventHandler (falsename)
		: 0;
	    h->AddOperation (CEL_OPERATION_IF);
	    h->GetArgument ().SetEventHandlers (truehandler, falsehandler);
	  }
	  else
	  {
	    h->AddOperation (CEL_OPERATION_IFGOTO);
	    int ifgoto_idx = h->GetLastArgumentIndex ();
	    csRef<iDocumentNode> truechild = child->GetNode ("true");
	    csRef<iDocumentNode> falsechild = child->GetNode ("false");
	    if (truechild && falsechild)
	    {
	      if (!ParseEventHandler (h, truechild, script))
	        return false;
	      h->AddOperation (CEL_OPERATION_GOTO);
	      int goto_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (ifgoto_idx).SetCodeLocation (goto_idx+1);
	      if (!ParseEventHandler (h, falsechild, script))
	        return false;
	      int last_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (goto_idx).SetCodeLocation (last_idx+1);
	    }
	    else if (truechild)
	    {
	      if (!ParseEventHandler (h, truechild, script))
	        return false;
	      int last_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (ifgoto_idx).SetCodeLocation (last_idx+1);
	    }
	    else if (falsechild)
	    {
	      h->AddOperation (CEL_OPERATION_GOTO);
	      int goto_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (ifgoto_idx).SetCodeLocation (goto_idx+1);
	      if (!ParseEventHandler (h, falsechild, script))
	        return false;
	      int last_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (goto_idx).SetCodeLocation (last_idx+1);
	    }
	    else
	    {
	      // No true or false child. Interprete single child as
	      // true child.
	      if (!ParseEventHandler (h, child, script))
	        return false;
	      int last_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (ifgoto_idx).SetCodeLocation (last_idx+1);
	    }
	  }
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
          if (!ParseExpression (child, h, "propclass", "property",
	  	CEL_DATA_PCLASS))
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
          if (!ParseExpression (child, h, "propclass", "action",
	  	CEL_DATA_PCLASS))
	    return false;
          if (!ParseExpression (child, h, "id", "action"))
	    return false;
          if (!ParseExpression (child, h, "params", "action", CEL_DATA_STRING))
	    return false;
	  h->AddOperation (CEL_OPERATION_ACTION);
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
      h->AddOperation (CEL_OPERATION_END);
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


