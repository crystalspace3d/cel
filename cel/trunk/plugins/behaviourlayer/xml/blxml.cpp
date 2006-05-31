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
#include <ctype.h>
#include "iutil/document.h"
#include "imap/services.h"
#include "ivaria/reporter.h"
#include "iengine/engine.h"
#include "csutil/scanstr.h"
#include "csutil/stringarray.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"

#include "plugins/behaviourlayer/xml/blxml.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "plugins/behaviourlayer/xml/xmlscript.h"
#include "plugins/behaviourlayer/xml/token.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celBlXml)

enum
{
  XMLTOKEN_PROPERTY,
  XMLTOKEN_ACTION,
  XMLTOKEN_BB_MOVELAYER,
  XMLTOKEN_BB_MOVE,
  XMLTOKEN_BB_TOFRONT,
  XMLTOKEN_BB_TOBACK,
  XMLTOKEN_BB_UP,
  XMLTOKEN_BB_DOWN,
  XMLTOKEN_PAR,
  XMLTOKEN_VAR,
  XMLTOKEN_LVAR,
  XMLTOKEN_EXPR,
  XMLTOKEN_IF,
  XMLTOKEN_WHILE,
  XMLTOKEN_FOR,
  XMLTOKEN_PRINT,
  XMLTOKEN_CALL,
  XMLTOKEN_CASE,
  XMLTOKEN_HITBEAM,
  XMLTOKEN_INHERIT,
  XMLTOKEN_DESTROYENTITY,
  XMLTOKEN_CREATEENTITY,
  XMLTOKEN_CREATEPROPCLASS,
  XMLTOKEN_DESTROYPROPCLASS,
  XMLTOKEN_DEFAULT,
  XMLTOKEN_INVENTORY,
  XMLTOKEN_INVENTORY_ADD,
  XMLTOKEN_INVENTORY_REM,
  XMLTOKEN_SOUND,
  XMLTOKEN_SUPER,
  XMLTOKEN_SWITCH,
  XMLTOKEN_STRSPLIT,
  XMLTOKEN_RETURN,
  XMLTOKEN_REPORTERROR,
  XMLTOKEN_CALLSTACK,
  XMLTOKEN_VARIABLES,
  XMLTOKEN_TRACEON,
  XMLTOKEN_TRACEOFF,
  XMLTOKEN_SELECTENTITY,
  XMLTOKEN_STOP,
  XMLTOKEN_RANDOMIZE,
  XMLTOKEN_QUIT,

  XMLTOKEN_LAST
};

enum
{
  XMLFUNCTION_PC,
  XMLFUNCTION_PCTAG,
  XMLFUNCTION_PARAM,
  XMLFUNCTION_PROPERTY,
  XMLFUNCTION_ID,
  XMLFUNCTION_PARID,
  XMLFUNCTION_ACTID,
  XMLFUNCTION_PROPID,
  XMLFUNCTION_RGB,
  XMLFUNCTION_ABS,
  XMLFUNCTION_MIN,
  XMLFUNCTION_MAX,
  XMLFUNCTION_MOUSEX,
  XMLFUNCTION_MOUSEY,
  XMLFUNCTION_NORMALIZE,
  XMLFUNCTION_BB_MOUSEX,
  XMLFUNCTION_BB_MOUSEY,
  XMLFUNCTION_SIGN,
  XMLFUNCTION_SQRT,
  XMLFUNCTION_SQDIST,
  XMLFUNCTION_INTPOL,
  XMLFUNCTION_INT,
  XMLFUNCTION_FLOAT,
  XMLFUNCTION_RAND,
  XMLFUNCTION_BB_TESTCOLLIDE,
  XMLFUNCTION_IF,
  XMLFUNCTION_ENTNAME,
  XMLFUNCTION_ENT,
  XMLFUNCTION_INVENTORY_GET,
  XMLFUNCTION_INVENTORY_COUNT,
  XMLFUNCTION_INVENTORY_IN,
  XMLFUNCTION_STRSUB,
  XMLFUNCTION_STRIDX,
  XMLFUNCTION_STRLEN,
  XMLFUNCTION_SCR_WIDTH,
  XMLFUNCTION_SCR_HEIGHT,
  XMLFUNCTION_TESTVAR,
  XMLFUNCTION_READFILE,
  XMLFUNCTION_WRITEFILE,
  XMLFUNCTION_GETYROT,
  XMLFUNCTION_VECX,
  XMLFUNCTION_VECY,
  XMLFUNCTION_VECZ,
  XMLFUNCTION_COLRED,
  XMLFUNCTION_COLGREEN,
  XMLFUNCTION_COLBLUE,

  XMLFUNCTION_LAST
};

celBlXml::celBlXml (iBase* parent) : scfImplementationType (this, parent)
{
  varprop_trace = false;
}

celBlXml::~celBlXml ()
{
#if DO_PROFILE
  int i;
  for (i = 0 ; i < profile_info.Length () ; i++)
  {
    celProfileInfo& pi = profile_info[i];
    printf ("%d %d %s\n", pi.time, pi.count, pi.msg);
  }
  fflush (stdout);
#endif
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
  csRef<iCelPlLayer> player = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  pl = (iCelPlLayer*)player;
  mouse = CS_QUERY_REGISTRY (object_reg, iMouseDriver);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  billboard_mgr = CS_QUERY_REGISTRY (object_reg, iBillboardManager);

  xmltokens.Register ("property", XMLTOKEN_PROPERTY);
  xmltokens.Register ("action", XMLTOKEN_ACTION);
  xmltokens.Register ("bb_movelayer", XMLTOKEN_BB_MOVELAYER);
  xmltokens.Register ("bb_move", XMLTOKEN_BB_MOVE);
  xmltokens.Register ("bb_tofront", XMLTOKEN_BB_TOFRONT);
  xmltokens.Register ("bb_toback", XMLTOKEN_BB_TOBACK);
  xmltokens.Register ("bb_up", XMLTOKEN_BB_UP);
  xmltokens.Register ("bb_down", XMLTOKEN_BB_DOWN);
  xmltokens.Register ("par", XMLTOKEN_PAR);
  xmltokens.Register ("var", XMLTOKEN_VAR);
  xmltokens.Register ("lvar", XMLTOKEN_LVAR);
  xmltokens.Register ("expr", XMLTOKEN_EXPR);
  xmltokens.Register ("while", XMLTOKEN_WHILE);
  xmltokens.Register ("if", XMLTOKEN_IF);
  xmltokens.Register ("for", XMLTOKEN_FOR);
  xmltokens.Register ("print", XMLTOKEN_PRINT);
  xmltokens.Register ("call", XMLTOKEN_CALL);
  xmltokens.Register ("case", XMLTOKEN_CASE);
  xmltokens.Register ("hitbeam", XMLTOKEN_HITBEAM);
  xmltokens.Register ("inherit", XMLTOKEN_INHERIT);
  xmltokens.Register ("destroyentity", XMLTOKEN_DESTROYENTITY);
  xmltokens.Register ("createentity", XMLTOKEN_CREATEENTITY);
  xmltokens.Register ("createpropclass", XMLTOKEN_CREATEPROPCLASS);
  xmltokens.Register ("destroypropclass", XMLTOKEN_DESTROYPROPCLASS);
  xmltokens.Register ("default", XMLTOKEN_DEFAULT);
  xmltokens.Register ("inventory", XMLTOKEN_INVENTORY);
  xmltokens.Register ("inventory_add", XMLTOKEN_INVENTORY_ADD);
  xmltokens.Register ("inventory_rem", XMLTOKEN_INVENTORY_REM);
  xmltokens.Register ("sound", XMLTOKEN_SOUND);
  xmltokens.Register ("super", XMLTOKEN_SUPER);
  xmltokens.Register ("switch", XMLTOKEN_SWITCH);
  xmltokens.Register ("strsplit", XMLTOKEN_STRSPLIT);
  xmltokens.Register ("return", XMLTOKEN_RETURN);
  xmltokens.Register ("reporterror", XMLTOKEN_REPORTERROR);
  xmltokens.Register ("variables", XMLTOKEN_VARIABLES);
  xmltokens.Register ("traceon", XMLTOKEN_TRACEON);
  xmltokens.Register ("traceoff", XMLTOKEN_TRACEOFF);
  xmltokens.Register ("callstack", XMLTOKEN_CALLSTACK);
  xmltokens.Register ("selectentity", XMLTOKEN_SELECTENTITY);
  xmltokens.Register ("stop", XMLTOKEN_STOP);
  xmltokens.Register ("randomize", XMLTOKEN_RANDOMIZE);
  xmltokens.Register ("quit", XMLTOKEN_QUIT);

  functions.Register ("pc", XMLFUNCTION_PC);
  functions.Register ("pctag", XMLFUNCTION_PCTAG);
  functions.Register ("param", XMLFUNCTION_PARAM);
  functions.Register ("property", XMLFUNCTION_PROPERTY);
  functions.Register ("id", XMLFUNCTION_ID);
  functions.Register ("parid", XMLFUNCTION_PARID);
  functions.Register ("actid", XMLFUNCTION_ACTID);
  functions.Register ("propid", XMLFUNCTION_PROPID);
  functions.Register ("rgb", XMLFUNCTION_RGB);
  functions.Register ("abs", XMLFUNCTION_ABS);
  functions.Register ("min", XMLFUNCTION_MIN);
  functions.Register ("max", XMLFUNCTION_MAX);
  functions.Register ("mousex", XMLFUNCTION_MOUSEX);
  functions.Register ("mousey", XMLFUNCTION_MOUSEY);
  functions.Register ("normalize", XMLFUNCTION_NORMALIZE);
  functions.Register ("bb_mousex", XMLFUNCTION_BB_MOUSEX);
  functions.Register ("bb_mousey", XMLFUNCTION_BB_MOUSEY);
  functions.Register ("sign", XMLFUNCTION_SIGN);
  functions.Register ("sqrt", XMLFUNCTION_SQRT);
  functions.Register ("sqdist", XMLFUNCTION_SQDIST);
  functions.Register ("intpol", XMLFUNCTION_INTPOL);
  functions.Register ("int", XMLFUNCTION_INT);
  functions.Register ("float", XMLFUNCTION_FLOAT);
  functions.Register ("rand", XMLFUNCTION_RAND);
  functions.Register ("bb_testcollide", XMLFUNCTION_BB_TESTCOLLIDE);
  functions.Register ("if", XMLFUNCTION_IF);
  functions.Register ("entname", XMLFUNCTION_ENTNAME);
  functions.Register ("ent", XMLFUNCTION_ENT);
  functions.Register ("inventory_get", XMLFUNCTION_INVENTORY_GET);
  functions.Register ("inventory_count", XMLFUNCTION_INVENTORY_COUNT);
  functions.Register ("inventory_in", XMLFUNCTION_INVENTORY_IN);
  functions.Register ("strlen", XMLFUNCTION_STRLEN);
  functions.Register ("strsub", XMLFUNCTION_STRSUB);
  functions.Register ("stridx", XMLFUNCTION_STRIDX);
  functions.Register ("scr_width", XMLFUNCTION_SCR_WIDTH);
  functions.Register ("scr_height", XMLFUNCTION_SCR_HEIGHT);
  functions.Register ("testvar", XMLFUNCTION_TESTVAR);
  functions.Register ("readfile", XMLFUNCTION_READFILE);
  functions.Register ("writefile", XMLFUNCTION_WRITEFILE);
  functions.Register ("getyrot", XMLFUNCTION_GETYROT);
  functions.Register ("vecx", XMLFUNCTION_VECX);
  functions.Register ("vecy", XMLFUNCTION_VECY);
  functions.Register ("vecz", XMLFUNCTION_VECZ);
  functions.Register ("colred", XMLFUNCTION_COLRED);
  functions.Register ("colgreen", XMLFUNCTION_COLGREEN);
  functions.Register ("colblue", XMLFUNCTION_COLBLUE);

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

bool celBlXml::ParseExpressionOrConstantString (
	csStringArray& local_vars, iDocumentNode* child,
	celXmlScriptEventHandler* h, const char* attrname, const char* name,
	char*& str)
{
  const char* input = child->GetAttributeValue (attrname);
  if (!input)
  {
    synldr->ReportError ("cel.behaviour.xml", child,
		"Can't find attribute '%s' for '%s'!", attrname, name);
    return false;
  }
  char buf[100];
  sprintf (buf, "%s(%s)", name, attrname);
  bool rc = ParseExpressionOrConstantString (
  	input, local_vars, child, h, buf, CEL_PRIORITY_NORMAL, str);
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

bool celBlXml::ParseExpression (csStringArray& local_vars,
	iDocumentNode* child,
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
    switch (optional_type)
    {
      case CEL_DATA_STRING:
	h->AddOperation (CEL_OPERATION_PUSHSTR);
	h->GetArgument ().SetString (0, false);
	break;
      case CEL_DATA_LONG:
	h->AddOperation (CEL_OPERATION_PUSH);
	h->GetArgument ().SetInt32 (0);
	break;
      case CEL_DATA_FLOAT:
	h->AddOperation (CEL_OPERATION_PUSH);
	h->GetArgument ().SetFloat (0.0f);
	break;
      case CEL_DATA_PCLASS:
	h->AddOperation (CEL_OPERATION_PUSH);
      	h->GetArgument ().SetPC (0);
	break;
      default: CS_ASSERT (false);
    }
    return true;
  }
  char buf[100];
  sprintf (buf, "%s(%s)", name, attrname);
  bool rc = ParseExpression (input, local_vars, child, h,
  	buf, CEL_PRIORITY_NORMAL);
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

bool celBlXml::ParseID (const char*& input, csStringArray& local_vars,
	iDocumentNode* child, celXmlScriptEventHandler* h,
	const char* name, char* str, csStringID fun_id)
{
  input = celXmlSkipWhiteSpace (input);
  const char* i = input;
  bool idconstant = true;
  while (*i && *i != ')' && *i != '=')
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
      prefix = "cel.parameter.";
    else if (fun_id == XMLFUNCTION_PROPID)
      prefix = "cel.property.";
    else if (fun_id == XMLFUNCTION_ACTID)
      prefix = "cel.action.";
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
    if (!ParseExpression (input, local_vars, child, h, name, 0))
      return false;
    h->AddOperation (
	fun_id == XMLFUNCTION_PARID
	  ? CEL_OPERATION_CALCPARID
	  : fun_id == XMLFUNCTION_PROPID
	  ? CEL_OPERATION_CALCPROPID
	  : fun_id == XMLFUNCTION_ACTID
	  ? CEL_OPERATION_CALCACTID
	  : CEL_OPERATION_CALCID);
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
	csStringArray& local_vars, iDocumentNode* child,
	celXmlScriptEventHandler* h, const char* name)
{
  int token;
  char str[256];	// @@@ Hardcoded!
  strncpy (str, pinput, input-pinput-1);	// Copy all execpt '('.
  str[input-pinput-1] = 0;
  csStringID fun_id = functions.Request (str);
  switch (fun_id)
  {
    case XMLFUNCTION_READFILE:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_READFILE);
      }
      break;
    case XMLFUNCTION_WRITEFILE:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_WRITEFILE);
      }
      break;
    case XMLFUNCTION_VECX:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_VECX);
      }
      break;
    case XMLFUNCTION_VECY:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_VECY);
      }
      break;
    case XMLFUNCTION_VECZ:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_VECZ);
      }
      break;
    case XMLFUNCTION_COLRED:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_COLRED);
      }
      break;
    case XMLFUNCTION_COLGREEN:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_COLGREEN);
      }
      break;
    case XMLFUNCTION_COLBLUE:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_COLBLUE);
      }
      break;
    case XMLFUNCTION_GETYROT:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_GETYROT);
      }
      break;
    case XMLFUNCTION_TESTVAR:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_TESTVAR);
      }
      break;
    case XMLFUNCTION_STRLEN:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_STRLEN);
      }
      break;
    case XMLFUNCTION_STRSUB:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_STRSUB);
      }
      break;
    case XMLFUNCTION_STRIDX:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_STRIDX);
      }
      break;
    case XMLFUNCTION_INVENTORY_GET:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_INVENTORY_GET);
      }
      break;
    case XMLFUNCTION_INVENTORY_IN:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_INVENTORY_IN);
      }
      break;
    case XMLFUNCTION_INVENTORY_COUNT:
      h->AddOperation (CEL_OPERATION_INVENTORY_COUNT);
      break;
    case XMLFUNCTION_ENTNAME:
      {
	celXmlParseToken (input, token);
	if (token == CEL_TOKEN_CLOSE)
	{
	  // No arguments: that means we want current entity.
          h->AddOperation (CEL_OPERATION_ENTNAMETHIS);
	}
	else
	{
	  // One argument.
          if (!ParseExpression (input, local_vars, child, h, name, 0))
	    return false;
          h->AddOperation (CEL_OPERATION_ENTNAME);
	}
      }
      break;
    case XMLFUNCTION_MOUSEX:
      h->AddOperation (CEL_OPERATION_MOUSEX);
      break;
    case XMLFUNCTION_MOUSEY:
      h->AddOperation (CEL_OPERATION_MOUSEY);
      break;
    case XMLFUNCTION_BB_MOUSEX:
      h->AddOperation (CEL_OPERATION_BB_MOUSEX);
      break;
    case XMLFUNCTION_BB_MOUSEY:
      h->AddOperation (CEL_OPERATION_BB_MOUSEY);
      break;
    case XMLFUNCTION_SCR_WIDTH:
      h->AddOperation (CEL_OPERATION_SCR_WIDTH);
      break;
    case XMLFUNCTION_SCR_HEIGHT:
      h->AddOperation (CEL_OPERATION_SCR_HEIGHT);
      break;
    case XMLFUNCTION_ENT:
      {
	celXmlParseToken (input, token);
	if (token == CEL_TOKEN_CLOSE)
	{
	  // No arguments: that means we want current entity.
          h->AddOperation (CEL_OPERATION_ENTTHIS);
	}
	else
	{
	  // One argument.
          if (!ParseExpression (input, local_vars, child, h, name, 0))
	    return false;
          h->AddOperation (CEL_OPERATION_ENT);
	}
      }
      break;
    case XMLFUNCTION_BB_TESTCOLLIDE:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	h->AddOperation (CEL_OPERATION_BB_TESTCOLLIDE);
      }
      break;
    case XMLFUNCTION_ABS:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_ABS);
      }
      break;
    case XMLFUNCTION_IF:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_IFFUN);
      }
      break;
    case XMLFUNCTION_MIN:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_MIN);
      }
      break;
    case XMLFUNCTION_MAX:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_MAX);
      }
      break;
    case XMLFUNCTION_NORMALIZE:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_NORMALIZE);
      }
      break;
    case XMLFUNCTION_SIGN:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_SIGN);
      }
      break;
    case XMLFUNCTION_SQRT:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_SQRT);
      }
      break;
    case XMLFUNCTION_SQDIST:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_SQDIST);
      }
      break;
    case XMLFUNCTION_INTPOL:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_INTPOL);
      }
      break;
    case XMLFUNCTION_RAND:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_RAND);
      }
      break;
    case XMLFUNCTION_INT:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_INT);
      }
      break;
    case XMLFUNCTION_FLOAT:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_FLOAT);
      }
      break;
    case XMLFUNCTION_RGB:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
        h->AddOperation (CEL_OPERATION_COLOR);
      }
      break;
    case XMLFUNCTION_PC:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	pinput = input;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_COMMA)
	{
          if (!ParseExpression (input, local_vars, child, h, name, 0))
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
    case XMLFUNCTION_PCTAG:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	pinput = input;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_COMMA)
	{
          if (!ParseExpression (input, local_vars, child, h, name, 0))
	    return false;
          h->AddOperation (CEL_OPERATION_PCTAG);
	}
	else
	{
	  input = pinput;	// Set back to ')'
	  // We have only two arguments to pctag(). This means we search
	  // for a property class for the current entity.
          h->AddOperation (CEL_OPERATION_PCTAGTHIS);
	}
      }
      break;
    case XMLFUNCTION_PROPERTY:
      {
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	pinput = input;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_COMMA)
	{
          if (!ParseExpression (input, local_vars, child, h, name, 0))
	    return false;
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
    case XMLFUNCTION_ACTID:
    case XMLFUNCTION_PROPID:
    case XMLFUNCTION_PARID:
      {
        if (!ParseID (input, local_vars, child, h, name, str, fun_id))
	  return false;
      }
      break;
    case XMLFUNCTION_PARAM:
      if (!ParseExpression (input, local_vars, child, h,
      	  name, CEL_PRIORITY_NORMAL))
        return false;
      h->AddOperation (CEL_OPERATION_PARAM);
      break;
    default:
      {
        // We have an unknown function. Try to parse it as an event handler
	// that we can call.
	pinput = input;
	input = celXmlParseToken (input, token);
	uint32 cnt = 0;

	if (token == CEL_TOKEN_DOTDOTDOT)
	{
	  h->AddOperation (CEL_OPERATION_INHERITPARAMS);
	  pinput = input;	// Point after ...
	}
	else
	{
	  input = pinput;	// Restore.
	  h->AddOperation (CEL_OPERATION_ACTIONPARAMS);
	  while (token != CEL_TOKEN_CLOSE)
	  {
	    // Get parameter name.
            if (!ParseID (input, local_vars, child, h, name, str,
				    XMLFUNCTION_PARID))
	      return false;
	    input = celXmlParseToken (input, token);
	    if (token != CEL_TOKEN_ASSIGN)
	    {
	      synldr->ReportError ("cel.behaviour.xml", child,
		  "Expected '=' after parameter for '%s'!", name);
	      return false;
	    }
	    // Get parameter value.
	    if (!ParseExpression (input, local_vars, child, h,
		  name, CEL_PRIORITY_NORMAL))
	      return false;

	    h->AddOperation (CEL_OPERATION_ACTIONPARAM);
	    h->GetArgument ().SetUInt32 (cnt);
	    cnt++;

	    pinput = input;
	    input = celXmlParseToken (input, token);
	    if (token != CEL_TOKEN_CLOSE && token != CEL_TOKEN_COMMA)
	    {
	      synldr->ReportError ("cel.behaviour.xml", child,
		  "Expected ')' or '=' after parameter value for '%s'!", name);
	      return false;
	    }
	  }
	}
	// Restore.
	input = pinput;

	//  First check if we have a scoped function (::).
	char* scope = strstr (str, "::");
	if (scope)
	{
	  *scope = 0;
	  h->AddOperation (CEL_OPERATION_PUSHSTR);
	  h->GetArgument ().SetString (str, true);
	  h->AddOperation (CEL_OPERATION_CALL_ERS);
	  h->GetArgument ().SetString (scope+2, true);
	  *scope = ':';
	}
	else
	{
	  h->AddOperation (CEL_OPERATION_CALL_RS);
	  h->GetArgument ().SetString (str, true);
        }
      }
      break;
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

bool celBlXml::ParseExpressionInt (
	const char*& input, const char* pinput, int token,
	csStringArray& local_vars,
	iDocumentNode* child, celXmlScriptEventHandler* h,
	const char* name,
	int stoppri)
{
  switch (token)
  {
    case CEL_TOKEN_ERROR:
      synldr->ReportError ("cel.behaviour.xml", child,
		"Error parsing expression for '%s'!", name);
      return false;
    case CEL_TOKEN_DEREFPAR:
      {
        input = celXmlSkipWhiteSpace (input);
	const char* i = input;
	while (*i && (isalnum (*i) || *i == '_')) i++;
	if (i == input)
	{
	  synldr->ReportError ("cel.behaviour.xml", child,
		"Missing parameter name after '@' for '%s'!", name);
	  return false;
	}
	const char* prefix = "cel.parameter.";
	char* str = new char [strlen (prefix) + i-input+1];
	strcpy (str, prefix);
	strncpy (str+strlen (prefix), input, i-input);
	str[strlen (prefix) + i-input] = 0;
	input = i;
	csStringID id = pl->FetchStringID (str);
	delete[] str;
	h->AddOperation (CEL_OPERATION_PUSH);
	h->GetArgument ().SetID (id);
	h->AddOperation (CEL_OPERATION_PARAM);
      }
      break;
    case CEL_TOKEN_DEREFLVAR:
      {
        char* str;
        if (!ParseExpressionOrConstantString (input, local_vars, child,
		h, name, CEL_PRIORITY_ONETERM, str))
          return false;
	if (!str)
	{
          synldr->ReportError ("cel.behaviour.xml", child,
		    "Local variables cannot use expressions for the name!");
          return false;
	}
	size_t i;
	size_t varidx = (size_t)-1;
	for (i = 0 ; i < local_vars.Length () ; i++)
	  if (!strcmp (str, local_vars[i]))
	  {
	    varidx = i;
	    break;
	  }
	if (varidx == (size_t)-1)
	{
	  varidx = local_vars.Push (str);
          //synldr->ReportError ("cel.behaviour.xml", child,
		    //"Local variable '%s' is not defined!", str);
          //return false;
	}
        h->AddOperation (CEL_OPERATION_DEREFLVAR);
	h->GetArgument ().SetUInt32 ((uint32)varidx);
      }
      break;
    case CEL_TOKEN_DEREFRVAR:
      {
        char* str;
        if (!ParseExpressionOrConstantString (input, local_vars,
		child, h, name, CEL_PRIORITY_ONETERM, str))
          return false;
        pinput = input;
        input = celXmlParseToken (input, token);
        if (token == CEL_TOKEN_DOT)
        {
	  // If we had a constant entity name we convert that to push anyway
	  // and use check if 'name' is constant instead.
	  if (str)
	  {
	    h->AddOperation (CEL_OPERATION_PUSHSTR);
	    h->GetArgument ().SetStringPrealloc (str);
            if (!ParseExpressionOrConstantString (input, local_vars,
	    	child, h, name, CEL_PRIORITY_ONETERM, str))
              return false;
	  }
	  else
	  {
            if (!ParseExpression (input, local_vars,
	    	child, h, name, CEL_PRIORITY_ONETERM))
              return false;
	  }

	  if (str)
	  {
            h->AddOperation (CEL_OPERATION_DEREFRVARENT_STR);
	    h->GetArgument ().SetStringPrealloc (str);
	  }
	  else
	  {
            h->AddOperation (CEL_OPERATION_DEREFRVARENT);
	  }
        }
        else
        {
          input = pinput;	// Restore!
	  if (str)
	  {
            h->AddOperation (CEL_OPERATION_DEREFRVAR_STR);
	    h->GetArgument ().SetStringPrealloc (str);
	  }
	  else
	  {
            h->AddOperation (CEL_OPERATION_DEREFRVAR);
	  }
        }
      }
      break;
    case CEL_TOKEN_DEREFVAR:
      {
        char* str;
        if (!ParseExpressionOrConstantString (input, local_vars,
		child, h, name, CEL_PRIORITY_ONETERM, str))
          return false;
        pinput = input;
        input = celXmlParseToken (input, token);
        if (token == CEL_TOKEN_DOT)
        {
	  // If we had a constant entity name we convert that to push anyway
	  // and use check if 'name' is constant instead.
	  if (str)
	  {
	    h->AddOperation (CEL_OPERATION_PUSHSTR);
	    h->GetArgument ().SetStringPrealloc (str);
            if (!ParseExpressionOrConstantString (input, local_vars,
	    	child, h, name, CEL_PRIORITY_ONETERM, str))
              return false;
	  }
	  else
	  {
            if (!ParseExpression (input, local_vars,
	    	child, h, name, CEL_PRIORITY_ONETERM))
              return false;
	  }

          pinput = input;
          input = celXmlParseToken (input, token);
	  if (token == CEL_TOKEN_VECTOR)
	  {
	    // Special case of dereferencing an array
	    // (?entity.array[index] notation).
            if (!ParseExpression (input, local_vars, child, h, name, 0))
	      return false;
            input = celXmlParseToken (input, token);
	    bool twoargs = false;
	    if (token == CEL_TOKEN_COMMA)
	    {
              if (!ParseExpression (input, local_vars, child, h, name, 0))
	        return false;
              input = celXmlParseToken (input, token);
	      twoargs = true;
	    }
            if (token != CEL_TOKEN_VECTORCLOSE)
	    {
              synldr->ReportError ("cel.behaviour.xml", child,
		      "Missing ']' or ',' for array in '%s'!", name);
              return false;
	    }
	    if (str)
	    {
              h->AddOperation (twoargs
	      	? CEL_OPERATION_DEREFARRENT2_STR
	      	: CEL_OPERATION_DEREFARRENT_STR);
	      h->GetArgument ().SetStringPrealloc (str);
	    }
	    else
	    {
              h->AddOperation (twoargs
	      	? CEL_OPERATION_DEREFARRENT2
	      	: CEL_OPERATION_DEREFARRENT);
	    }
	  }
	  else
	  {
	    input = pinput; // Restore

	    if (str)
	    {
              h->AddOperation (CEL_OPERATION_DEREFVARENT_STR);
	      h->GetArgument ().SetStringPrealloc (str);
	    }
	    else
	    {
              h->AddOperation (CEL_OPERATION_DEREFVARENT);
	    }
	  }
        }
	else if (token == CEL_TOKEN_VECTOR)
	{
	  // Special case of dereferencing an array (?array[index] notation).
          if (!ParseExpression (input, local_vars, child, h, name, 0))
	    return false;
          input = celXmlParseToken (input, token);
	  bool twoargs = false;
	  if (token == CEL_TOKEN_COMMA)
	  {
            if (!ParseExpression (input, local_vars, child, h, name, 0))
	      return false;
            input = celXmlParseToken (input, token);
	    twoargs = true;
	  }

          if (token != CEL_TOKEN_VECTORCLOSE)
	  {
            synldr->ReportError ("cel.behaviour.xml", child,
		    "Missing ']' or ',' for array in '%s'!", name);
            return false;
	  }
	  if (str)
	  {
            h->AddOperation (twoargs
	    	? CEL_OPERATION_DEREFARR2_STR
		: CEL_OPERATION_DEREFARR_STR);
	    h->GetArgument ().SetStringPrealloc (str);
	  }
	  else
	  {
            h->AddOperation (twoargs
	    	? CEL_OPERATION_DEREFARR2
	    	: CEL_OPERATION_DEREFARR);
	  }
	}
        else
        {
          input = pinput;	// Restore!
	  if (str)
	  {
            h->AddOperation (CEL_OPERATION_DEREFVAR_STR);
	    h->GetArgument ().SetStringPrealloc (str);
	  }
	  else
	  {
            h->AddOperation (CEL_OPERATION_DEREFVAR);
	  }
        }
      }
      break;
    case CEL_TOKEN_FUNCTION:
      if (!ParseFunction (input, pinput, local_vars, child, h, name))
        return false;
      break;
    case CEL_TOKEN_FUNCTIONSCO:
      if (!ParseFunction (input, pinput, local_vars, child, h, name))
        return false;
      break;
    case CEL_TOKEN_IDENTIFIER:
      {
        char* str;
	str = new char[input-pinput+1];
	strncpy (str, pinput, input-pinput);
	str[input-pinput] = 0;
        h->AddOperation (CEL_OPERATION_PUSHSTR);
        h->GetArgument ().SetStringPrealloc (str);
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
        h->AddOperation (CEL_OPERATION_PUSHSTR);
        h->GetArgument ().SetStringPrealloc (str);
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
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	if (!SkipComma (input, child, name)) return false;
        if (!ParseExpression (input, local_vars, child, h, name, 0))
	  return false;
	input = celXmlParseToken (input, token);
	if (token == CEL_TOKEN_COMMA)
	{
          op = CEL_OPERATION_VECTOR3;
          if (!ParseExpression (input, local_vars, child, h, name, 0))
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
      h->GetArgument ().Set (true);
      break;
    case CEL_TOKEN_BOOLFALSE:
      h->AddOperation (CEL_OPERATION_PUSH);
      h->GetArgument ().Set (false);
      break;
    case CEL_TOKEN_OPEN:
      if (!ParseExpression (input, local_vars, child, h, name, 0))
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
      if (!ParseExpression (input, local_vars, child, h, name,
			      CEL_PRIORITY_ONETERM))
        return false;
      h->AddOperation (CEL_OPERATION_UNARYMINUS);
      break;
    case CEL_TOKEN_BITNOT:
      if (!ParseExpression (input, local_vars, child, h, name,
			      CEL_PRIORITY_ONETERM))
        return false;
      h->AddOperation (CEL_OPERATION_BITNOT);
      break;
    case CEL_TOKEN_LOGNOT:
      if (!ParseExpression (input, local_vars, child, h, name,
			      CEL_PRIORITY_ONETERM))
        return false;
      h->AddOperation (CEL_OPERATION_LOGNOT);
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
      case CEL_TOKEN_SCOPE:
        {
          if (stoppri >= CEL_PRIORITY_SCOPE) { input = pinput; return true; }
          if (!ParseExpression (input, local_vars,
	  	child, h, name, CEL_PRIORITY_SCOPE))
	    return false;
	  size_t last_idx = h->GetLastArgumentIndex ();
	  if (h->GetOperation (last_idx) != CEL_OPERATION_CALL_RS)
	  {
            synldr->ReportError ("cel.behaviour.xml", child,
		    "Expected a function after '::' for '%s'!", name);
	    return false;
	  }
          h->ChangeOperation (last_idx, CEL_OPERATION_CALL_ERS2);
	}
	break;
      case CEL_TOKEN_BITAND:
        if (stoppri >= CEL_PRIORITY_BITAND) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_BITAND))
	  return false;
        h->AddOperation (CEL_OPERATION_BITAND);
        break;
      case CEL_TOKEN_BITOR:
        if (stoppri >= CEL_PRIORITY_BITOR) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_BITOR))
	  return false;
        h->AddOperation (CEL_OPERATION_BITOR);
        break;
      case CEL_TOKEN_BITXOR:
        if (stoppri >= CEL_PRIORITY_BITXOR) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_BITXOR))
	  return false;
        h->AddOperation (CEL_OPERATION_BITXOR);
        break;
      case CEL_TOKEN_LOGAND:
        if (stoppri >= CEL_PRIORITY_LOGAND) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_LOGAND))
	  return false;
        h->AddOperation (CEL_OPERATION_LOGAND);
        break;
      case CEL_TOKEN_LOGOR:
        if (stoppri >= CEL_PRIORITY_LOGOR) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_LOGOR))
	  return false;
        h->AddOperation (CEL_OPERATION_LOGOR);
        break;
      case CEL_TOKEN_EQ:
        if (stoppri >= CEL_PRIORITY_EQUAL) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_EQUAL))
	  return false;
        h->AddOperation (CEL_OPERATION_EQ);
        break;
      case CEL_TOKEN_NE:
        if (stoppri >= CEL_PRIORITY_EQUAL) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_EQUAL))
	  return false;
        h->AddOperation (CEL_OPERATION_NE);
        break;
      case CEL_TOKEN_LT:
        if (stoppri >= CEL_PRIORITY_LTGT) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_LTGT))
	  return false;
        h->AddOperation (CEL_OPERATION_LT);
        break;
      case CEL_TOKEN_LE:
        if (stoppri >= CEL_PRIORITY_LTGT) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_LTGT))
	  return false;
        h->AddOperation (CEL_OPERATION_LE);
        break;
      case CEL_TOKEN_GT:
        if (stoppri >= CEL_PRIORITY_LTGT) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_LTGT))
	  return false;
        h->AddOperation (CEL_OPERATION_GT);
        break;
      case CEL_TOKEN_GE:
        if (stoppri >= CEL_PRIORITY_LTGT) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_LTGT))
	  return false;
        h->AddOperation (CEL_OPERATION_GE);
        break;
      case CEL_TOKEN_MINUS:
        {
          if (stoppri >= CEL_PRIORITY_ADDSUB) { input = pinput; return true; }
          if (!ParseExpression (input, local_vars, child, h, name,
				  CEL_PRIORITY_ADDSUB))
	    return false;
	  if (h->GetLastOperation () == CEL_OPERATION_PUSH)
	  {
	    h->ReplaceLastOperation (CEL_OPERATION_MINUS_I);
	  }
	  else
	  {
            h->AddOperation (CEL_OPERATION_MINUS);
	  }
	}
        break;
      case CEL_TOKEN_ADD:
        {
          if (stoppri >= CEL_PRIORITY_ADDSUB) { input = pinput; return true; }
          if (!ParseExpression (input, local_vars, child, h, name,
				  CEL_PRIORITY_ADDSUB))
	    return false;
	  if (h->GetLastOperation () == CEL_OPERATION_PUSH)
	  {
	    h->ReplaceLastOperation (CEL_OPERATION_ADD_I);
	  }
	  else if (h->GetLastOperation () == CEL_OPERATION_PUSHSTR)
	  {
	    h->ReplaceLastOperation (CEL_OPERATION_ADD_I);
	  }
	  else
	  {
            h->AddOperation (CEL_OPERATION_ADD);
	  }
	}
        break;
      case CEL_TOKEN_MULT:
        if (stoppri >= CEL_PRIORITY_MULTDIV) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_MULTDIV))
	  return false;
        h->AddOperation (CEL_OPERATION_MULT);
        break;
      case CEL_TOKEN_DIV:
        if (stoppri >= CEL_PRIORITY_MULTDIV) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_MULTDIV))
	  return false;
        h->AddOperation (CEL_OPERATION_DIV);
        break;
      case CEL_TOKEN_MODULO:
        if (stoppri >= CEL_PRIORITY_MULTDIV) { input = pinput; return true; }
        if (!ParseExpression (input, local_vars, child, h, name,
				CEL_PRIORITY_MULTDIV))
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

bool celBlXml::ParseExpression (const char*& input,
	csStringArray& local_vars,
	iDocumentNode* child,
	celXmlScriptEventHandler* h, const char* name,
	int stoppri)
{
  int token;
  input = celXmlSkipWhiteSpace (input);
  const char* pinput = input;
  input = celXmlParseToken (input, token);
  return ParseExpressionInt (input, pinput, token, local_vars,
  	child, h, name, stoppri);
}

bool celBlXml::ParseExpressionOrConstantString (const char*& input,
	csStringArray& local_vars,
	iDocumentNode* child, celXmlScriptEventHandler* h,
	const char* name, int stoppri, char*& str)
{
  int token;
  input = celXmlSkipWhiteSpace (input);
  const char* pinput = input;
  input = celXmlParseToken (input, token);
  bool one_token = stoppri == CEL_PRIORITY_ONETERM || *input==0;

  if (one_token && token == CEL_TOKEN_IDENTIFIER)
  {
    str = new char[input-pinput+1];
    strncpy (str, pinput, input-pinput);
    str[input-pinput] = 0;
    return true;
  }
  else
  {
    str = 0;
    return ParseExpressionInt (input, pinput, token, local_vars,
    	child, h, name, stoppri);
  }
}

celXmlScriptEventHandler* celBlXml::FindEventHandler (
	celXmlScript* script, const char* eventname)
{
  celXmlScriptEventHandler* handler = 0;
  if (eventname)
  {
    handler = script->GetEventHandler (eventname);
    if (handler) return handler;
    celXmlScript* superscript = script->GetSuperScript ();
    while (superscript)
    {
      handler = script->GetEventHandler (eventname);
      if (handler) return handler;
      superscript = superscript->GetSuperScript ();
    }
    // We couldn't find a handler so we create one.
    handler = script->FindOrCreateEventHandler (eventname);
  }
  return handler;
}

bool celBlXml::ParseEventHandler (celXmlScriptEventHandler* h,
	csStringArray& local_vars,
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
      case XMLTOKEN_STRSPLIT:
        if (!ParseExpression (local_vars, child, h, "string", "strsplit"))
	  return false;
        if (!ParseExpression (local_vars, child, h, "left", "strsplit"))
	  return false;
        if (!ParseExpression (local_vars, child, h, "delimiter", "strsplit"))
	  return false;
        if (!ParseExpression (local_vars, child, h, "right", "strsplit"))
	  return false;
	h->AddOperation (CEL_OPERATION_STRSPLIT);
        break;
      case XMLTOKEN_BB_MOVE:
        {
	  bool pc = false;
	  if (child->GetAttributeValue ("pc"))
	  {
            if (!ParseExpression (local_vars, child, h, "pc", "bb_move"))
	      return false;
	    pc = true;
	  }
          if (!ParseExpression (local_vars, child, h, "x", "bb_move"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "y", "bb_move"))
	    return false;
	  if (child->GetAttributeValue ("delta"))
	  {
            if (!ParseExpression (local_vars, child, h, "delta", "bb_move"))
	      return false;
	    h->AddOperation (pc
			    ? CEL_OPERATION_BB_MOVEDELTA_E
			    : CEL_OPERATION_BB_MOVEDELTA);
	  }
	  else
	  {
	    h->AddOperation (pc
			    ? CEL_OPERATION_BB_MOVE_E
			    : CEL_OPERATION_BB_MOVE);
	  }
	}
        break;
      case XMLTOKEN_BB_TOFRONT:
	if (child->GetAttributeValue ("pc"))
	{
          if (!ParseExpression (local_vars, child, h, "pc", "bb_tofront"))
	    return false;
	  h->AddOperation (CEL_OPERATION_BB_TOFRONT_E);
	}
	else
	  h->AddOperation (CEL_OPERATION_BB_TOFRONT);
        break;
      case XMLTOKEN_BB_TOBACK:
	if (child->GetAttributeValue ("pc"))
	{
          if (!ParseExpression (local_vars, child, h, "pc", "bb_toback"))
	    return false;
	  h->AddOperation (CEL_OPERATION_BB_TOBACK_E);
	}
	else
	  h->AddOperation (CEL_OPERATION_BB_TOBACK);
        break;
      case XMLTOKEN_BB_UP:
	if (child->GetAttributeValue ("pc"))
	{
          if (!ParseExpression (local_vars, child, h, "pc", "bb_up"))
	    return false;
	  h->AddOperation (CEL_OPERATION_BB_UP_E);
	}
	else
	  h->AddOperation (CEL_OPERATION_BB_UP);
        break;
      case XMLTOKEN_BB_DOWN:
	if (child->GetAttributeValue ("pc"))
	{
          if (!ParseExpression (local_vars, child, h, "pc", "bb_down"))
	    return false;
	  h->AddOperation (CEL_OPERATION_BB_DOWN_E);
	}
	else
	  h->AddOperation (CEL_OPERATION_BB_DOWN);
        break;
      case XMLTOKEN_BB_MOVELAYER:
        if (!ParseExpression (local_vars, child, h, "layer", "bb_movelayer"))
	  return false;
        if (!ParseExpression (local_vars, child, h, "x", "bb_movelayer"))
	  return false;
        if (!ParseExpression (local_vars, child, h, "y", "bb_movelayer"))
	  return false;
	h->AddOperation (CEL_OPERATION_BB_MOVELAYER);
        break;
      case XMLTOKEN_REPORTERROR:
        if (!ParseExpression (local_vars, child, h, "message", "reporterror"))
	  return false;
	h->AddOperation (CEL_OPERATION_REPORTERROR);
        break;
      case XMLTOKEN_SOUND:
        if (!ParseExpression (local_vars, child, h, "name", "sound"))
	  return false;
	if (child->GetAttributeValue ("loop"))
	{
          if (!ParseExpression (local_vars, child, h, "loop", "sound"))
	    return false;
	}
	else
	{
	  h->AddOperation (CEL_OPERATION_CREATEPC);
	  h->GetArgument ().Set (false);
	}
	h->AddOperation (CEL_OPERATION_SOUND);
        break;
      case XMLTOKEN_CALLSTACK:
        h->AddOperation (CEL_OPERATION_CALLSTACK);
        break;
      case XMLTOKEN_VARIABLES:
        h->AddOperation (CEL_OPERATION_VARIABLES);
        break;
      case XMLTOKEN_TRACEOFF:
        h->AddOperation (CEL_OPERATION_TRACEOFF);
        break;
      case XMLTOKEN_TRACEON:
        h->AddOperation (CEL_OPERATION_TRACEON);
        break;
      case XMLTOKEN_STOP:
        h->AddOperation (CEL_OPERATION_END);
        break;
      case XMLTOKEN_RANDOMIZE:
        h->AddOperation (CEL_OPERATION_RANDOMIZE);
        break;
      case XMLTOKEN_QUIT:
        h->AddOperation (CEL_OPERATION_QUIT);
        break;
      case XMLTOKEN_DESTROYPROPCLASS:
        {
          if (!ParseExpression (local_vars, child, h, "name",
	  	"destroypropclass"))
	    return false;
	  const char* tagname = child->GetAttributeValue ("tag");
	  if (tagname)
	  {
            if (!ParseExpression (local_vars, child, h, "tag",
	    	"destroypropclass"))
	      return false;
	    h->AddOperation (CEL_OPERATION_DESTROYPCTAG);
	  }
	  else
	  {
	    h->AddOperation (CEL_OPERATION_DESTROYPC);
	  }
        }
	break;
      case XMLTOKEN_CREATEPROPCLASS:
        {
          if (!ParseExpression (local_vars, child, h, "name",
	  	"createpropclass"))
	    return false;
	  const char* tagname = child->GetAttributeValue ("tag");
	  if (tagname)
	  {
            if (!ParseExpression (local_vars, child, h, "tag",
	    	"createpropclass"))
	      return false;
	    h->AddOperation (CEL_OPERATION_CREATEPCTAG);
	  }
	  else
	  {
	    h->AddOperation (CEL_OPERATION_CREATEPC);
	  }
        }
	break;
      case XMLTOKEN_SUPER:
        {
	  celXmlScript* superscript = script->GetSuperScript ();
	  if (!superscript)
	  {
	    synldr->ReportError (
	        "cel.behaviour.xml", child,
		"This script has no superscript!");
	    return false;
	  }
	  celXmlScriptEventHandler* handler = superscript->GetEventHandler (
	  	h->GetName ());
	  while (!handler)
	  {
	    superscript = superscript->GetSuperScript ();
	    if (!superscript) break;
	    handler = superscript->GetEventHandler (h->GetName ());
	  }
	  if (!handler)
	  {
	    synldr->ReportError (
	        "cel.behaviour.xml", child,
		"The superscript has no method called '%s'!",
		h->GetName ());
	    return false;
	  }
	  h->AddOperation (CEL_OPERATION_CALL_I);
	  h->GetArgument ().SetEventHandlers (handler, 0);
	}
        break;
      case XMLTOKEN_RETURN:
        if (!ParseExpression (local_vars, child, h, "value", "return"))
	  return false;
	h->AddOperation (CEL_OPERATION_RETURN);
	h->AddOperation (CEL_OPERATION_END);
	break;
      case XMLTOKEN_CALL:
        {
	  csRef<iDocumentNodeIterator> child_it = child->GetNodes ();
	  bool inherit = false;
	  uint32 cnt = 0;
	  csRef<iDocumentNode> return_node;
	  while (child_it->HasNext ())
	  {
	    csRef<iDocumentNode> c = child_it->Next ();
	    if (c->GetType () != CS_NODE_ELEMENT) continue;
	    csStringID child_id = xmltokens.Request (c->GetValue ());
	    if (child_id == XMLTOKEN_PAR) cnt++;
	    else if (child_id == XMLTOKEN_RETURN) { return_node = c; }
	    else if (child_id == XMLTOKEN_INHERIT) { inherit = true; break; }
	    else synldr->ReportBadToken (c);
	  }
	  if (inherit)
	  {
	    h->AddOperation (CEL_OPERATION_INHERITPARAMS);
	  }
	  else if (cnt > 0)
	  {
	    h->AddOperation (CEL_OPERATION_ACTIONPARAMS);

	    child_it = child->GetNodes ();
	    cnt = 0;
	    while (child_it->HasNext ())
	    {
	      csRef<iDocumentNode> c = child_it->Next ();
	      if (c->GetType () != CS_NODE_ELEMENT) continue;
	      csStringID child_id = xmltokens.Request (c->GetValue ());
	      if (child_id != XMLTOKEN_RETURN)
	      {
                if (!ParseExpression (local_vars, c, h, "id", "call/par"))
		  return false;
                if (!ParseExpression (local_vars, c, h, "value", "call/par"))
		  return false;
	        h->AddOperation (CEL_OPERATION_ACTIONPARAM);
	        h->GetArgument ().SetUInt32 (cnt);
	        cnt++;
	      }
	    }
	  }

	  if (return_node)
            if (!ParseExpression (local_vars, return_node, h, "var", "return"))
	      return false;
	  const char* entname = child->GetAttributeValue ("entity");
	  if (entname)
            if (!ParseExpression (local_vars, child, h, "entity", "call"))
	      return false;
          if (!ParseExpression (local_vars, child, h, "event", "call"))
	    return false;
	  if (return_node)
	    h->AddOperation (entname ? CEL_OPERATION_CALL_ER :
	  	  CEL_OPERATION_CALL_R);
	  else
	    h->AddOperation (entname ? CEL_OPERATION_CALL_E :
	  	  CEL_OPERATION_CALL);
	}
	break;
      case XMLTOKEN_ACTION:
        {
	  csRef<iDocumentNodeIterator> child_it = child->GetNodes ();
	  uint32 cnt = 0;
	  while (child_it->HasNext ())
	  {
	    csRef<iDocumentNode> c = child_it->Next ();
	    if (c->GetType () != CS_NODE_ELEMENT) continue;
	    if (xmltokens.Request (c->GetValue ()) == XMLTOKEN_PAR) cnt++;
	    else synldr->ReportBadToken (c);
	  }
	  if (cnt > 0)
	  {
	    h->AddOperation (CEL_OPERATION_ACTIONPARAMS);

	    child_it = child->GetNodes ();
	    cnt = 0;
	    while (child_it->HasNext ())
	    {
	      csRef<iDocumentNode> c = child_it->Next ();
	      if (c->GetType () != CS_NODE_ELEMENT) continue;
              if (!ParseExpression (local_vars, c, h, "id", "action/par"))
	        return false;
              if (!ParseExpression (local_vars, c, h, "value", "action/par"))
	        return false;
	      h->AddOperation (CEL_OPERATION_ACTIONPARAM);
	      h->GetArgument ().SetUInt32 (cnt);
	      cnt++;
	    }
	  }

          if (!ParseExpression (local_vars, child, h, "propclass", "action",
	  	CEL_DATA_PCLASS))
	    return false;
          if (!ParseExpression (local_vars, child, h, "id", "action"))
	    return false;
	  h->AddOperation (CEL_OPERATION_ACTION);
	}
	break;
      case XMLTOKEN_DESTROYENTITY:
        if (!ParseExpression (local_vars, child, h, "name", "destroyentity"))
	  return false;
	h->AddOperation (CEL_OPERATION_DESTROYENTITY);
	break;
      case XMLTOKEN_CREATEENTITY:
        if (child->GetAttributeValue ("name"))
        {
	  if (!ParseExpression (local_vars, child, h, "name", "createentity"))
	    return false;
	  if (!ParseExpression (local_vars, child, h, "behaviour",
				  "createentity"))
	    return false;
	  if (child->GetAttributeValue ("layer"))
	  {
	    if (!ParseExpression (local_vars, child, h, "layer",
				    "createentity"))
	      return false;
	    h->AddOperation (CEL_OPERATION_CREATEENTITYL);
	  }
	  else
	  {
	    h->AddOperation (CEL_OPERATION_CREATEENTITY);
	  }
	}
	else if (child->GetAttributeValue ("template"))
	{
	  if (!ParseExpression (local_vars, child, h, "template",
				  "createentity"))
	    return false;
	  h->AddOperation (CEL_OPERATION_CREATEENTITYTPL);
	}
	break;
      case XMLTOKEN_EXPR:
        if (!ParseExpression (local_vars, child, h, "eval", "expr"))
	  return false;
	h->AddOperation (CEL_OPERATION_CLEARSTACK);
	break;
      case XMLTOKEN_LVAR:
        {
	  const char* varname = child->GetAttributeValue ("name");
	  if (!varname)
	  {
	    synldr->ReportError (
	        "cel.behaviour.xml", child,
		"Missing 'name' for lvar!");
	    return false;
	  }
	  size_t varidx = (size_t)-1;
	  size_t i;
	  for (i = 0 ; i < local_vars.Length () ; i++)
	    if (!strcmp (varname, local_vars[i]))
	    {
	      varidx = i;
	      break;
	    }
	  if (varidx == (size_t)-1)
	  {
	    varidx = local_vars.Push (varname);
	  }
          if (!ParseExpression (local_vars, child, h, "value", "lvar"))
	    return false;
	  h->AddOperation (CEL_OPERATION_LVAR);
	  h->GetArgument ().SetUInt32 ((uint32)varidx);
	}
	break;
      case XMLTOKEN_VAR:
        {
	  const char* entname = child->GetAttributeValue ("entity");
	  if (entname)
            if (!ParseExpression (local_vars, child, h, "entity", "var"))
	      return false;
	  char* str;
          if (!ParseExpressionOrConstantString (local_vars, child, h, "name",
				  "var", str))
	    return false;
          if (!ParseExpression (local_vars, child, h, "value", "var"))
	    return false;
	  if (str)
	  {
	    h->AddOperation (entname
	    	? CEL_OPERATION_VARENT_STR
		: CEL_OPERATION_VAR_STR);
	    h->GetArgument ().SetStringPrealloc (str);
	  }
	  else
	  {
	    h->AddOperation (entname
	    	? CEL_OPERATION_VARENT
		: CEL_OPERATION_VAR);
	  }
	}
	break;
      case XMLTOKEN_HITBEAM:
	{
          if (!ParseExpression (local_vars, child, h, "sector",
		"hitbeam"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "start",
		"hitbeam"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "end",
		"hitbeam"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "isectvar",
		"hitbeam"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "entvar",
		"hitbeam"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "meshvar",
		"hitbeam"))
	    return false;
	  h->AddOperation (CEL_OPERATION_HITBEAM);
	}
	break;
      case XMLTOKEN_SELECTENTITY:
	{
          if (!ParseExpression (local_vars, child, h, "pc",
		"selectentity"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "screenx",
		"selectentity"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "screeny",
		"selectentity"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "maxdist",
		"selectentity"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "isectvar",
		"selectentity"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "entvar",
		"selectentity"))
	    return false;
	  h->AddOperation (CEL_OPERATION_SELECTENTITY);
	}
	break;
      case XMLTOKEN_WHILE:
        {
	  size_t eval_idx = h->GetLastArgumentIndex ()+1;
          if (!ParseExpression (local_vars, child, h, "eval", "for"))
	    return false;
	  const char* execname = GetAttributeString (child, "exec", 0);
	  if (execname)
	  {
	    celXmlScriptEventHandler* handler = FindEventHandler (
	    	script, execname);
	    h->AddOperation (CEL_OPERATION_IF);
	    h->GetArgument ().SetEventHandlers (handler, 0);
	    h->AddOperation (CEL_OPERATION_GOTO);	// True branch
	    h->GetArgument ().SetCodeLocation (eval_idx);
	  }
	  else
	  {
	    h->AddOperation (CEL_OPERATION_IFGOTO);
	    size_t while_idx = h->GetLastArgumentIndex ();

	    if (!ParseEventHandler (h, local_vars, child, script))
	      return false;
	    h->AddOperation (CEL_OPERATION_GOTO);	// True branch
	    h->GetArgument ().SetCodeLocation (eval_idx);

	    size_t last_idx = h->GetLastArgumentIndex ();
	    h->GetArgument (while_idx).SetCodeLocation (last_idx+1);
	  }
	}
        break;
      case XMLTOKEN_FOR:
        {
          if (!ParseExpression (local_vars, child, h, "var", "for"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "start", "for"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "end", "for"))
	    return false;
	  const char* execname = GetAttributeString (child, "exec", 0);
	  if (execname)
	  {
	    celXmlScriptEventHandler* handler = FindEventHandler (
	    	script, execname);
	    h->AddOperation (CEL_OPERATION_FOR);
	    h->GetArgument ().SetEventHandlers (handler, 0);
	  }
	  else
	  {
	    h->AddOperation (CEL_OPERATION_FORI);
	    size_t for_idx = h->GetLastArgumentIndex ();

	    if (!ParseEventHandler (h, local_vars, child, script))
	      return false;
	    h->AddOperation (CEL_OPERATION_END);

	    size_t last_idx = h->GetLastArgumentIndex ();
	    h->GetArgument (for_idx).SetCodeLocation (last_idx+1);
	  }
	}
        break;
      case XMLTOKEN_SWITCH:
        {
          if (!ParseExpression (local_vars, child, h, "eval", "switch"))
	    return false;
	  csArray<size_t> goto_end_idx;
	  csRef<iDocumentNodeIterator> child_it = child->GetNodes ();
	  while (child_it->HasNext ())
	  {
	    csRef<iDocumentNode> c = child_it->Next ();
	    if (c->GetType () != CS_NODE_ELEMENT) continue;
	    csStringID child_id = xmltokens.Request (c->GetValue ());
	    if (child_id == XMLTOKEN_CASE)
	    {
	      h->AddOperation (CEL_OPERATION_DUP);
              if (!ParseExpression (local_vars, c, h, "value", "case"))
	        return false;
	      h->AddOperation (CEL_OPERATION_EQ);
	      h->AddOperation (CEL_OPERATION_IFGOTO);
	      size_t ifgoto_idx = h->GetLastArgumentIndex ();
	      if (!ParseEventHandler (h, local_vars, c, script))
	        return false;
	      h->AddOperation (CEL_OPERATION_GOTO);
	      size_t goto_idx = h->GetLastArgumentIndex ();
	      goto_end_idx.Push (goto_idx);
	      h->GetArgument (ifgoto_idx).SetCodeLocation (goto_idx+1);
	    }
	    else if (child_id != XMLTOKEN_DEFAULT)
	      synldr->ReportBadToken (c);
	  }
	  csRef<iDocumentNode> def = child->GetNode ("default");
	  if (def)
	  {
	    if (!ParseEventHandler (h, local_vars, def, script))
	      return false;
	  }
	  size_t end_idx = h->GetLastArgumentIndex ()+1;
	  size_t i;
	  for (i = 0 ; i < goto_end_idx.Length () ; i++)
	  {
	    h->GetArgument (goto_end_idx[i]).SetCodeLocation ((uint32)end_idx);
	  }
	  h->AddOperation (CEL_OPERATION_DROP);
	}
        break;
      case XMLTOKEN_IF:
        {
          if (!ParseExpression (local_vars, child, h, "eval", "if"))
	    return false;
	  const char* truename = GetAttributeString (child, "true", 0);
	  const char* falsename = GetAttributeString (child, "false", 0);
	  if (truename || falsename)
	  {
	    celXmlScriptEventHandler* truehandler = FindEventHandler (
	    	script, truename);
	    celXmlScriptEventHandler* falsehandler = FindEventHandler (
	    	script, falsename);
	    h->AddOperation (CEL_OPERATION_IF);
	    h->GetArgument ().SetEventHandlers (truehandler, falsehandler);
	  }
	  else
	  {
	    h->AddOperation (CEL_OPERATION_IFGOTO);
	    size_t ifgoto_idx = h->GetLastArgumentIndex ();
	    csRef<iDocumentNode> truechild = child->GetNode ("true");
	    csRef<iDocumentNode> falsechild = child->GetNode ("false");
	    if (truechild && falsechild)
	    {
	      if (!ParseEventHandler (h, local_vars, truechild, script))
	        return false;
	      h->AddOperation (CEL_OPERATION_GOTO);
	      size_t goto_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (ifgoto_idx).SetCodeLocation (goto_idx+1);
	      if (!ParseEventHandler (h, local_vars, falsechild, script))
	        return false;
	      size_t last_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (goto_idx).SetCodeLocation (last_idx+1);
	    }
	    else if (truechild)
	    {
	      if (!ParseEventHandler (h, local_vars, truechild, script))
	        return false;
	      size_t last_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (ifgoto_idx).SetCodeLocation (last_idx+1);
	    }
	    else if (falsechild)
	    {
	      h->AddOperation (CEL_OPERATION_GOTO);
	      size_t goto_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (ifgoto_idx).SetCodeLocation (goto_idx+1);
	      if (!ParseEventHandler (h, local_vars, falsechild, script))
	        return false;
	      size_t last_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (goto_idx).SetCodeLocation (last_idx+1);
	    }
	    else
	    {
	      // No true or false child. Interprete single child as
	      // true child.
	      if (!ParseEventHandler (h, local_vars, child, script))
	        return false;
	      size_t last_idx = h->GetLastArgumentIndex ();
	      h->GetArgument (ifgoto_idx).SetCodeLocation (last_idx+1);
	    }
	  }
	}
        break;
      case XMLTOKEN_PRINT:
        if (!ParseExpression (local_vars, child, h, "value", "print"))
	  return false;
        h->AddOperation (CEL_OPERATION_PRINT);
        break;
      case XMLTOKEN_DEFAULT:
        if (!ParseExpression (local_vars, child, h, "propclass", "default"))
	  return false;
        h->AddOperation (CEL_OPERATION_DEFAULTPC);
        break;
      case XMLTOKEN_INVENTORY:
        if (!ParseExpression (local_vars, child, h, "propclass", "inventory"))
	  return false;
        h->AddOperation (CEL_OPERATION_DEFAULTINV);
        break;
      case XMLTOKEN_INVENTORY_ADD:
        if (!ParseExpression (local_vars, child, h, "child", "inventory_add"))
	  return false;
        h->AddOperation (CEL_OPERATION_INVENTORY_ADD);
        break;
      case XMLTOKEN_INVENTORY_REM:
        if (!ParseExpression (local_vars, child, h, "child", "inventory_rem"))
	  return false;
        h->AddOperation (CEL_OPERATION_INVENTORY_REM);
        break;
      case XMLTOKEN_PROPERTY:
        {
          if (!ParseExpression (local_vars, child, h, "propclass", "property",
	  	CEL_DATA_PCLASS))
	    return false;
          if (!ParseExpression (local_vars, child, h, "id", "property"))
	    return false;
          if (!ParseExpression (local_vars, child, h, "value", "property"))
	    return false;
	  h->AddOperation (CEL_OPERATION_PROPERTY);
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
      csStringArray local_vars;
      if (!ParseEventHandler (h, local_vars, child, script))
      {
        delete script;
	return false;
      }
      h->AddOperation (CEL_OPERATION_END);
    }
    else if (!strcmp (value, "superscript"))
    {
      const char* supername = child->GetAttributeValue ("name");
      celXmlScript* superscript = supername
	      ? scripts_hash.Get (supername, 0)
	      : 0;
      if (!superscript)
      {
        delete script;
        synldr->ReportError (
	        "cel.behaviour.xml", child,
		"Can't find superscript with name '%s'!", supername);
        return false;
      }
      if (script->GetSuperScript ())
      {
        delete script;
        synldr->ReportError (
	        "cel.behaviour.xml", child,
		"This script already has a superscript!");
        return false;
      }
      script->SetSuperScript (superscript);
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
  celXmlScript* script = scripts_hash.Get (name, 0);
  if (!script)
  {
    // First check if we have the predefined 'bootstrap' script.
    if (!strcmp (name, "bootstrap"))
    {
      celBehaviourBootstrap* bootstrap = new celBehaviourBootstrap (entity,
    	  object_reg);
      bootstrap->SetName (name);
      bootstrap->SetBehaviourLayer (this);
      entity->SetBehaviour (bootstrap);
      return bootstrap;
    }
    return 0;	// Couldn't find script.
  }

  celBehaviourXml* behave = new celBehaviourXml (entity, object_reg);
  behave->SetName (name);
  behave->SetBehaviourLayer (this);
  behave->SetScript (script);
  celData ret;
  entity->SetBehaviour (behave);
  behave->SendMessage ("init", 0, ret, 0);
  return behave;
}

static bool celXmlArg2celData (const celXmlArg& in, celData& out)
{
  switch (in.type)
  {
    case CEL_DATA_BOOL: out.Set ((in.arg.b? true : false)); break;
    case CEL_DATA_FLOAT: out.Set (in.arg.f); break;
    case CEL_DATA_STRING: out.Set (in.arg.str.s); break;
    case CEL_DATA_LONG: out.Set (in.arg.i); break;
    case CEL_DATA_ULONG: out.Set (in.arg.ui); break;
    case CEL_DATA_PCLASS: out.Set (in.arg.pc); break;
    case CEL_DATA_ENTITY: out.Set (in.arg.entity); break;
    case CEL_DATA_COLOR:
      {
        csColor col (in.arg.col.red, in.arg.col.green,
	  in.arg.col.blue);
        out.Set (col);
      }
      break;
    case CEL_DATA_VECTOR2:
      {
	csVector2 v (in.arg.vec.x, in.arg.vec.y);
	out.Set (v);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
	csVector3 v (in.arg.vec.x, in.arg.vec.y, in.arg.vec.z);
	out.Set (v);
      }
      break;
    default:
      out.Clear ();
      return false;
  }
  return true;
}

bool celExpression::Execute (iCelEntity* entity, celData& ret)
{
  celData r;
  bool rc = handler->Execute (entity, cbl, 0, r, 0, 0, true, 1);
  if (rc)
  {
    const csArray<celXmlArg>& stack = handler->GetStack ();
    const celXmlArg& a = stack.Top ();
    celXmlArg2celData (a, ret);
  }
  return rc;
}

size_t celExpression::FindLocalVariable (const char* name) const
{
  return local_vars.Find (name);
}

void celExpression::SetLocalVariable (size_t idx, const celData& value)
{
  switch (value.type)
  {
    case CEL_DATA_LONG:
      handler->GetLocalVariable (idx).SetInt32 (value.value.l);
      break;
    case CEL_DATA_ULONG:
      handler->GetLocalVariable (idx).SetUInt32 (value.value.ul);
      break;
    case CEL_DATA_FLOAT:
      handler->GetLocalVariable (idx).SetFloat (value.value.f);
      break;
    case CEL_DATA_BOOL:
      handler->GetLocalVariable (idx).Set (value.value.bo);
      break;
    case CEL_DATA_STRING:
      handler->GetLocalVariable (idx).SetString (value.value.s->GetData (), true);
      break;
    case CEL_DATA_VECTOR2:
      {
        csVector2 v;
	v.x = value.value.v.x;
	v.y = value.value.v.y;
        handler->GetLocalVariable (idx).SetVector (v);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
        csVector3 v;
	v.x = value.value.v.x;
	v.y = value.value.v.y;
	v.z = value.value.v.z;
        handler->GetLocalVariable (idx).SetVector (v);
      }
      break;
    case CEL_DATA_COLOR:
      {
        csColor v;
	v.red = value.value.col.red;
	v.green = value.value.col.green;
	v.blue = value.value.col.blue;
        handler->GetLocalVariable (idx).SetColor (v);
      }
      break;
    default:
      break;
  }
}

void celExpression::SetLocalVariableLong (size_t idx, int32 value)
{
  handler->GetLocalVariable (idx).SetInt32 (value);
}

void celExpression::SetLocalVariableULong (size_t idx, uint32 value)
{
  handler->GetLocalVariable (idx).SetUInt32 (value);
}

void celExpression::SetLocalVariableFloat (size_t idx, float value)
{
  handler->GetLocalVariable (idx).SetFloat (value);
}

void celExpression::SetLocalVariableBool (size_t idx, bool value)
{
  handler->GetLocalVariable (idx).Set (value);
}

void celExpression::SetLocalVariableString (size_t idx, const char* value)
{
  handler->GetLocalVariable (idx).SetString (value, true);
}

void celExpression::SetLocalVariableVector2 (size_t idx, const csVector2& value)
{
  handler->GetLocalVariable (idx).SetVector (value);
}

void celExpression::SetLocalVariableVector3 (size_t idx, const csVector3& value)
{
  handler->GetLocalVariable (idx).SetVector (value);
}

void celExpression::SetLocalVariableColor (size_t idx, const csColor& value)
{
  handler->GetLocalVariable (idx).SetColor (value);
}

csPtr<iCelExpression> celBlXml::Parse (const char* string)
{
  celExpression* exp = new celExpression (this);
  celXmlScriptEventHandler* handler = new celXmlScriptEventHandler (pl);
  exp->handler = handler;
  if (!ParseExpression (string, exp->local_vars, 0, handler, "expression",
	CEL_PRIORITY_NORMAL))
  {
    delete exp;
    return 0;
  }
  handler->AddOperation (CEL_OPERATION_END);
  return exp;
}

