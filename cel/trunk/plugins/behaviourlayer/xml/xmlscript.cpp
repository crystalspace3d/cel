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
#include "csutil/objreg.h"
#include "csutil/csstring.h"
#include "ivaria/reporter.h"

#include "plugins/behaviourlayer/xml/xmlscript.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/pl.h"
#include "behaviourlayer/bl.h"
#include "propclass/prop.h"
#include "propclass/billboard.h"
#include "tools/billboard.h"

inline void DUMP_EXEC (const char* msg, ...)
{
#if 1
  va_list arg;
  va_start (arg, msg);
  vprintf (msg, arg);
  va_end (arg);
  fflush (stdout);
#endif
}


//---------------------------------------------------------------------------

celXmlArg::celXmlArg (const celXmlArg& other)
{
  type = other.type;
  switch (type)
  {
    case CEL_TYPE_STRING: arg.s = csStrNew (other.arg.s); break;
    case CEL_TYPE_ARGLIST: arg.a = new celXmlArgList (*other.arg.a); break;
    case CEL_TYPE_UINT32: arg.ui = other.arg.ui; break;
    case CEL_TYPE_INT32: arg.i = other.arg.i; break;
    case CEL_TYPE_FLOAT: arg.f = other.arg.f; break;
    case CEL_TYPE_BOOL: arg.b = other.arg.b; break;
    case CEL_TYPE_PC_REF: arg.pc_ref = other.arg.pc_ref; break;
    case CEL_TYPE_PC: arg.pc = other.arg.pc; break;
    case CEL_TYPE_ID: arg.id = other.arg.id; break;
    case CEL_TYPE_EVENTHANDLER: arg.h = other.arg.h; break;
    case CEL_TYPE_VECTOR2: arg.vec = other.arg.vec; break;
    case CEL_TYPE_VECTOR3: arg.vec = other.arg.vec; break;
    case CEL_TYPE_COLOR: arg.col = other.arg.col; break;
    case CEL_TYPE_VAR: arg.s = csStrNew (other.arg.s); break;
    case CEL_TYPE_NONE: break;
    default:
      CS_ASSERT (false);
  }
}

void celXmlArg::Cleanup ()
{
  switch (type)
  {
    case CEL_TYPE_VAR: delete[] arg.s; break;
    case CEL_TYPE_STRING: delete[] arg.s; break;
    case CEL_TYPE_ARGLIST: delete arg.a; break;
  }
}

void celXmlArg::SetArgList ()
{
  Cleanup ();
  type = CEL_TYPE_ARGLIST;
  arg.a = new celXmlArgList ();
}

//---------------------------------------------------------------------------

celXmlScriptEventHandler::celXmlScriptEventHandler (iCelPlLayer* pl)
{
  name = 0;
  celXmlScriptEventHandler::pl = pl;
}

celXmlScriptEventHandler::~celXmlScriptEventHandler ()
{
  delete[] name;
}

void celXmlScriptEventHandler::ResolveParameters (iCelEntity* entity)
{
  int i;
  iCelPropertyClassList* pclist = entity->GetPropertyClassList ();
  for (i = 0 ; i < resolvers.Length () ; i++)
  {
    if (resolvers[i].entname)
    {
      iCelEntity* ent = pl->FindEntity (resolvers[i].entname);
      iCelPropertyClassList* pcl= ent->GetPropertyClassList ();
      resolvers[i].pc = pcl->FindByName (resolvers[i].pcname);
    }
    else
    {
      resolvers[i].pc = pclist->FindByName (resolvers[i].pcname);
    }
  }
}

static csArray<csString*> used_strings;
static csPDelArray<csString> unused_strings;

static csString* GetUnusedString ()
{
  return new csString ();
  csString* str;
  if (unused_strings.Length () > 0)
    str = unused_strings.Pop ();
  else
    str = 0;
  if (!str) str = new csString ();
  return str;
}

static void CleanupTemporaryStrings ()
{
return;
  while (used_strings.Length () > 0)
  {
    csString* str = used_strings.Pop ();
    unused_strings.Push (str);
  }
}

static const char* ArgToString (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_INT32:
      {
        csString* str = GetUnusedString ();
        str->Format ("%d", a.arg.i);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_UINT32:
      {
        csString* str = GetUnusedString ();
        str->Format ("%u", a.arg.ui);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_FLOAT:
      {
        csString* str = GetUnusedString ();
        str->Format ("%g", a.arg.f);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_BOOL: return a.arg.b ? "true" : "false";
    case CEL_TYPE_STRING: return a.arg.s;
    case CEL_TYPE_VECTOR2:
      {
        csString* str = GetUnusedString ();
        str->Format ("[%g,%g]", a.arg.vec.x, a.arg.vec.y);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_VECTOR3:
      {
        csString* str = GetUnusedString ();
        str->Format ("[%g,%g,%g]", a.arg.vec.x, a.arg.vec.y, a.arg.vec.z);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_COLOR:
      {
        csString* str = GetUnusedString ();
        str->Format ("rgb(%g,%g,%g)", a.arg.col.red, a.arg.col.green,
      	  a.arg.col.blue);
	used_strings.Push (str);
        return *str;
      }
    default:
      return 0;
  }
}

// For debug reasons.
static const char* A2S (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_INT32:
      {
        csString* str = GetUnusedString ();
        str->Format ("{int32:%d}", a.arg.i);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_UINT32:
      {
        csString* str = GetUnusedString ();
        str->Format ("{uint32:%u}", a.arg.ui);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_FLOAT:
      {
        csString* str = GetUnusedString ();
        str->Format ("{float:%g}", a.arg.f);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_BOOL:
      {
        csString* str = GetUnusedString ();
        str->Format ("{bool:%s}", a.arg.b ? "true" : "false");
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_STRING:
      {
        csString* str = GetUnusedString ();
        str->Format ("{str:%s}", a.arg.s);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_VECTOR2:
      {
        csString* str = GetUnusedString ();
        str->Format ("{vec:[%g,%g]}", a.arg.vec.x, a.arg.vec.y);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_VECTOR3:
      {
        csString* str = GetUnusedString ();
        str->Format ("{vec:[%g,%g,%g]}", a.arg.vec.x, a.arg.vec.y, a.arg.vec.z);
	used_strings.Push (str);
        return *str;
      }
    case CEL_TYPE_COLOR:
      {
        csString* str = GetUnusedString ();
        str->Format ("{rgb:(%g,%g,%g)}", a.arg.col.red, a.arg.col.green,
      	  a.arg.col.blue);
	used_strings.Push (str);
        return *str;
      }
    default:
      {
        csString* str = GetUnusedString ();
        str->Format ("{unknown}");
	used_strings.Push (str);
        return *str;
      }
  }
}

static iCelPropertyClass* ArgToPClass (const celXmlArg& a,
	const csArray<celXmlPCResolver>& resolvers)
{
  switch (a.type)
  {
    case CEL_TYPE_PC_REF: return resolvers[a.arg.pc_ref].pc;
    case CEL_TYPE_PC: return a.arg.pc;
    default: return 0;
  }
}

static csColor ArgToColor (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_COLOR: return csColor (a.arg.col.red, a.arg.col.green,
    	a.arg.col.blue);
    default: return csColor (0, 0, 0);
  }
}

static csVector3 ArgToVector3 (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_VECTOR3: return csVector3 (a.arg.vec.x, a.arg.vec.y,
    	a.arg.vec.z);
    default: return csVector3 (0, 0, 0);
  }
}

static csVector2 ArgToVector2 (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_VECTOR2: return csVector2 (a.arg.vec.x, a.arg.vec.y);
    default: return csVector2 (0, 0);
  }
}

static bool ArgToBool (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_INT32: return bool (a.arg.i);
    case CEL_TYPE_UINT32: return bool (a.arg.ui);
    case CEL_TYPE_FLOAT: return bool (ABS (a.arg.f) < SMALL_EPSILON);
    case CEL_TYPE_BOOL: return a.arg.b;
    case CEL_TYPE_STRING: return !strcmp (a.arg.s, "true");
    default:
      return false;
  }
}

static uint32 ArgToUInt32 (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_INT32: return uint32 (a.arg.i);
    case CEL_TYPE_UINT32: return a.arg.ui;
    case CEL_TYPE_FLOAT: return uint32 (a.arg.f);
    case CEL_TYPE_BOOL: return a.arg.b ? 1 : 0;
    case CEL_TYPE_STRING:
      {
	uint i;
        sscanf (a.arg.s, "%u", &i);
	return i;
      }
    default:
      return 0;
  }
}

static int32 ArgToInt32 (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_INT32: return a.arg.i;
    case CEL_TYPE_UINT32: return int32 (a.arg.ui);
    case CEL_TYPE_FLOAT: return int32 (a.arg.f);
    case CEL_TYPE_BOOL: return a.arg.b ? 1 : 0;
    case CEL_TYPE_STRING:
      {
        int i;
	sscanf (a.arg.s, "%d", &i);
	return i;
      }
    default:
      return 0;
  }
}

static float ArgToFloat (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_INT32: return float (a.arg.i);
    case CEL_TYPE_UINT32: return float (a.arg.ui);
    case CEL_TYPE_FLOAT: return a.arg.f;
    case CEL_TYPE_BOOL: return a.arg.b ? 1.0 : 0.0;
    case CEL_TYPE_STRING:
      {
        float f;
	sscanf (a.arg.s, "%f", &f);
	return f;
      }
    default:
      return 0.0;
  }
}

static bool IsVectorType (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_VECTOR2: return true;
    case CEL_TYPE_VECTOR3: return true;
    case CEL_TYPE_COLOR: return true;
    default: return false;
  }
}

static bool IsNumericType (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_INT32: return true;
    case CEL_TYPE_UINT32: return true;
    case CEL_TYPE_FLOAT: return true;
    case CEL_TYPE_BOOL: return true;
    default: return false;
  }
}

static bool IsStringConvertible (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_TYPE_INT32: return true;
    case CEL_TYPE_UINT32: return true;
    case CEL_TYPE_FLOAT: return true;
    case CEL_TYPE_BOOL: return true;
    case CEL_TYPE_VECTOR2: return true;
    case CEL_TYPE_VECTOR3: return true;
    case CEL_TYPE_COLOR: return true;
    default: return false;
  }
}

static int GetCalculationType (const celXmlArg& a, const celXmlArg& b)
{
  if (a.type == b.type) return a.type;

  if (a.type == CEL_TYPE_STRING && IsStringConvertible (b))
    return CEL_TYPE_STRING;
  if (b.type == CEL_TYPE_STRING && IsStringConvertible (a))
    return CEL_TYPE_STRING;

  if (a.type == CEL_TYPE_FLOAT && IsNumericType (b))
    return CEL_TYPE_FLOAT;
  if (b.type == CEL_TYPE_FLOAT && IsNumericType (a))
    return CEL_TYPE_FLOAT;

  if (a.type == CEL_TYPE_INT32 && IsNumericType (b))
    return CEL_TYPE_INT32;
  if (b.type == CEL_TYPE_INT32 && IsNumericType (a))
    return CEL_TYPE_INT32;

  if (a.type == CEL_TYPE_UINT32 && IsNumericType (b))
    return CEL_TYPE_UINT32;
  if (b.type == CEL_TYPE_UINT32 && IsNumericType (a))
    return CEL_TYPE_UINT32;

  return CEL_TYPE_NONE;
}

bool celXmlScriptEventHandler::ReportError (celBehaviourXml* behave,
	const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);
  csReportV (behave->GetObjectRegistry (), CS_REPORTER_SEVERITY_ERROR,
  	"cel.behaviourlayer.xml", msg, arg);
  va_end (arg);
  return false;
}

bool celXmlScriptEventHandler::CheckStack (celBehaviourXml* behave)
{
  if (stack.Length () == 0)
    return ReportError (behave, "Stack underflow!");
  return true;
}

#ifdef CS_DEBUG
#define CHECK_STACK if (!CheckStack (behave)) return false;
#else
#define CHECK_STACK
#endif

bool celXmlScriptEventHandler::Execute (iCelEntity* entity,
	celBehaviourXml* behave)
{
  int stack_size = stack.Length ();
  int i = 0;
  for (;;)
  {
    CleanupTemporaryStrings ();
    celXmlOperation& op = operations[i];
    i++;
    csArray<celXmlArg>& args = op.arg.arg.a->args;
    switch (op.op)
    {
      case CEL_OPERATION_END:
        if (stack_size != stack.Length ())
	{
	  return ReportError (behave, "Stack size mismatch!");
	}
        return true;
      case CEL_OPERATION_PUSH:
        {
          DUMP_EXEC (": push %s\n", A2S (op.arg));
	  stack.Push (op.arg);
	}
        break;
#if 0
      case CEL_OPERATION_ENTITY:
        {
	  CHECK_STACK
	  celXmlArg ent = stack.Pop ();
	  DUMP_EXEC (" entity %s\n", A2S (ent));
	  iCelEntity* other_ent = pl->FindEntity (ArgToString (ent));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetEntity (other_ent);
	}
	break;
#endif
      case CEL_OPERATION_VECTOR2:
        {
	  CHECK_STACK
	  celXmlArg ely = stack.Pop ();
	  CHECK_STACK
	  celXmlArg elx = stack.Pop ();
          DUMP_EXEC (": vector2 %s, %s\n", A2S (elx), A2S (ely));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetVector (csVector2 (ArgToFloat (elx), ArgToFloat (ely)));
	}
	break;
      case CEL_OPERATION_VECTOR3:
        {
	  CHECK_STACK
	  celXmlArg elz = stack.Pop ();
	  CHECK_STACK
	  celXmlArg ely = stack.Pop ();
	  CHECK_STACK
	  celXmlArg elx = stack.Pop ();
          DUMP_EXEC (": vector3 %s, %s, %s\n", A2S (elx), A2S (ely), A2S (elz));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetVector (csVector3 (ArgToFloat (elx),
	  	ArgToFloat (ely), ArgToFloat (elz)));
	}
	break;
      case CEL_OPERATION_COLOR:
        {
	  CHECK_STACK
	  celXmlArg elb = stack.Pop ();
	  CHECK_STACK
	  celXmlArg elg = stack.Pop ();
	  CHECK_STACK
	  celXmlArg elr = stack.Pop ();
          DUMP_EXEC (": color %s, %s, %s\n", A2S (elr), A2S (elg), A2S (elb));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetColor (csColor (ArgToFloat (elr),
	  	ArgToFloat (elg), ArgToFloat (elb)));
	}
	break;
      case CEL_OPERATION_UNARYMINUS:
        {
	  CHECK_STACK
	  int si = stack.Length ()-1;
          DUMP_EXEC (": unaryminus %s\n", A2S (stack[si]));
	  switch (stack[si].type)
	  {
	    case CEL_TYPE_INT32: stack[si].arg.i = -stack[si].arg.i; break;
	    case CEL_TYPE_UINT32: stack[si].arg.ui = -stack[si].arg.ui; break;
	    case CEL_TYPE_FLOAT: stack[si].arg.f = -stack[si].arg.f; break;
	    case CEL_TYPE_BOOL: stack[si].arg.b = !stack[si].arg.b; break;
	    case CEL_TYPE_COLOR:
	      stack[si].arg.col.red = -stack[si].arg.col.red;
	      stack[si].arg.col.green = -stack[si].arg.col.green;
	      stack[si].arg.col.blue = -stack[si].arg.col.blue;
	      break;
	    case CEL_TYPE_VECTOR3:
	      stack[si].arg.vec.x = -stack[si].arg.vec.x;
	      stack[si].arg.vec.y = -stack[si].arg.vec.y;
	      stack[si].arg.vec.z = -stack[si].arg.vec.z;
	      break;
	    case CEL_TYPE_VECTOR2:
	      stack[si].arg.vec.x = -stack[si].arg.vec.x;
	      stack[si].arg.vec.y = -stack[si].arg.vec.y;
	      break;
	    default:
	      return ReportError (behave, "Can't negate element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_MINUS:
        {
	  CHECK_STACK
	  celXmlArg elb = stack.Pop ();
	  CHECK_STACK
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC (": minus %s - %s\n", A2S (ela), A2S (elb));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_TYPE_NONE:
	      return ReportError (behave, "Can't subtract these types!");
	    case CEL_TYPE_BOOL:
	      return ReportError (behave, "Can't subtract booleans!");
	    case CEL_TYPE_STRING:
	      return ReportError (behave, "Can't subtract strings!");
	    case CEL_TYPE_FLOAT:
	      stack[si].SetFloat (ArgToFloat (ela) - ArgToFloat (elb));
	      break;
	    case CEL_TYPE_INT32:
	      stack[si].SetInt32 (ArgToInt32 (ela) - ArgToInt32 (elb));
	      break;
	    case CEL_TYPE_UINT32:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) - ArgToUInt32 (elb));
	      break;
	    case CEL_TYPE_VECTOR2:
	      stack[si].SetVector (ArgToVector2 (ela) - ArgToVector2 (elb));
	      break;
	    case CEL_TYPE_VECTOR3:
	      stack[si].SetVector (ArgToVector3 (ela) - ArgToVector3 (elb));
	      break;
	    case CEL_TYPE_COLOR:
	      stack[si].SetColor (ArgToColor (ela) - ArgToColor (elb));
	      break;
	  }
	}
	break;
      case CEL_OPERATION_ADD:
        {
	  CHECK_STACK
	  celXmlArg elb = stack.Pop ();
	  CHECK_STACK
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC (": add %s + %s\n", A2S (ela), A2S (elb));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_TYPE_BOOL:
	      return ReportError (behave, "Can't add booleans!");
	    case CEL_TYPE_FLOAT:
	      stack[si].SetFloat (ArgToFloat (ela) + ArgToFloat (elb));
	      break;
	    case CEL_TYPE_INT32:
	      stack[si].SetInt32 (ArgToInt32 (ela) + ArgToInt32 (elb));
	      break;
	    case CEL_TYPE_UINT32:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) + ArgToUInt32 (elb));
	      break;
	    case CEL_TYPE_VECTOR2:
	      stack[si].SetVector (ArgToVector2 (ela) + ArgToVector2 (elb));
	      break;
	    case CEL_TYPE_VECTOR3:
	      stack[si].SetVector (ArgToVector3 (ela) + ArgToVector3 (elb));
	      break;
	    case CEL_TYPE_COLOR:
	      stack[si].SetColor (ArgToColor (ela) + ArgToColor (elb));
	      break;
	    case CEL_TYPE_NONE:
	    case CEL_TYPE_STRING:
	      {
		const char* astr = ArgToString (ela);
		const char* bstr = ArgToString (elb);
		int alen = astr ? strlen (astr) : 0;
		int blen = bstr ? strlen (bstr) : 0;
		if (alen && blen)
		{
		  if (!blen)
	            stack[si].SetString (astr);
		  else if (!alen)
	            stack[si].SetString (bstr);
		  else
		  {
		    char* str = new char[alen+blen];
		    strcpy (str, astr);
		    strcpy (str+alen, bstr);
	            stack[si].SetString (str);
		    delete[] str;
		  }
	        }
		else
		{
	          stack[si].SetString ("");
		}
	      }
	      break;
	  }
	}
	break;
      case CEL_OPERATION_MULT:
        {
	  CHECK_STACK
	  celXmlArg elb = stack.Pop ();
	  CHECK_STACK
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC (": mult %s * %s\n", A2S (ela), A2S (elb));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_TYPE_NONE:
	      if (IsNumericType (ela) && IsVectorType (elb))
	      {
	        switch (elb.type)
		{
		  case CEL_TYPE_VECTOR2:
		    stack[si].SetVector (ArgToFloat (ela) * ArgToVector2 (elb));
		    break;
		  case CEL_TYPE_VECTOR3:
		    stack[si].SetVector (ArgToFloat (ela) * ArgToVector3 (elb));
		    break;
		  case CEL_TYPE_COLOR:
		    stack[si].SetColor (ArgToFloat (ela) * ArgToColor (elb));
		    break;
		}
	      }
	      else if (IsVectorType (ela) && IsNumericType (elb))
	      {
	        switch (ela.type)
		{
		  case CEL_TYPE_VECTOR2:
		    stack[si].SetVector (ArgToFloat (elb) * ArgToVector2 (ela));
		    break;
		  case CEL_TYPE_VECTOR3:
		    stack[si].SetVector (ArgToFloat (elb) * ArgToVector3 (ela));
		    break;
		  case CEL_TYPE_COLOR:
		    stack[si].SetColor (ArgToFloat (elb) * ArgToColor (ela));
		    break;
		}
	      }
	      else
	        return ReportError (behave, "Can't multiply these types!");
	      break;
	    case CEL_TYPE_STRING:
	      return ReportError (behave, "Can't multiply strings!");
	    case CEL_TYPE_COLOR:
	      return ReportError (behave, "Can't multiply colors!");
	    case CEL_TYPE_FLOAT:
	      stack[si].SetFloat (ArgToFloat (ela) * ArgToFloat (elb));
	      break;
	    case CEL_TYPE_INT32:
	      stack[si].SetInt32 (ArgToInt32 (ela) * ArgToInt32 (elb));
	      break;
	    case CEL_TYPE_UINT32:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) * ArgToUInt32 (elb));
	      break;
	    case CEL_TYPE_VECTOR2:
	      stack[si].SetFloat (ArgToVector2 (ela) * ArgToVector2 (elb));
	      break;
	    case CEL_TYPE_VECTOR3:
	      stack[si].SetFloat (ArgToVector3 (ela) * ArgToVector3 (elb));
	      break;
	    case CEL_TYPE_BOOL:
	      stack[si].SetBool (bool (int (ArgToBool (ela))
	      	* int (ArgToBool (elb))));
	      break;
	  }
	}
	break;
      case CEL_OPERATION_DIV:
        {
	  CHECK_STACK
	  celXmlArg elb = stack.Pop ();
	  CHECK_STACK
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC (": mult %s / %s\n", A2S (ela), A2S (elb));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_TYPE_NONE:
	      if (IsVectorType (ela) && IsNumericType (elb))
	      {
	        switch (ela.type)
		{
		  case CEL_TYPE_VECTOR2:
		    stack[si].SetVector (ArgToVector2 (ela) / ArgToFloat (elb));
		    break;
		  case CEL_TYPE_VECTOR3:
		    stack[si].SetVector (ArgToVector3 (ela) / ArgToFloat (elb));
		    break;
		  default:
	            return ReportError (behave, "Can't divide these types!");
		}
	      }
	      else
	        return ReportError (behave, "Can't divide these types!");
	      break;
	    case CEL_TYPE_STRING:
	      return ReportError (behave, "Can't divide strings!");
	    case CEL_TYPE_BOOL:
	      return ReportError (behave, "Can't divide booleans!");
	    case CEL_TYPE_VECTOR2:
	      return ReportError (behave, "Can't divide vectors!");
	    case CEL_TYPE_VECTOR3:
	      return ReportError (behave, "Can't divide vectors!");
	    case CEL_TYPE_COLOR:
	      return ReportError (behave, "Can't divide colors!");
	    case CEL_TYPE_FLOAT:
	      stack[si].SetFloat (ArgToFloat (ela) / ArgToFloat (elb));
	      break;
	    case CEL_TYPE_INT32:
	      stack[si].SetInt32 (ArgToInt32 (ela) / ArgToInt32 (elb));
	      break;
	    case CEL_TYPE_UINT32:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) / ArgToUInt32 (elb));
	      break;
	  }
	}
	break;
      case CEL_OPERATION_DEREFVAR:
        {
	  CHECK_STACK
	  celXmlArg top = stack.Pop ();
          DUMP_EXEC (": derefvar %s\n", A2S (top));
	  iPcProperties* props = behave->GetProperties ();
	  if (!props) break;	// @@@ Report error!
	  int idx = props->GetPropertyIndex (top.arg.s);
	  if (idx == -1) break;	// @@@ Report error!
	  int si = stack.Push (celXmlArg ());
	  switch (props->GetPropertyType (idx))
	  {
	    case CEL_DATA_LONG:
	      {
		long l = props->GetPropertyLong (idx);
		stack[si].SetInt32 (l);
	      }
	      break;
	    case CEL_DATA_FLOAT:
	      {
		float l = props->GetPropertyFloat (idx);
		stack[si].SetFloat (l);
	      }
	      break;
	    case CEL_DATA_BOOL:
	      {
		bool l = props->GetPropertyBool (idx);
		stack[si].SetBool (l);
	      }
	      break;
	    case CEL_DATA_STRING:
	      {
		const char* l = props->GetPropertyString (idx);
		stack[si].SetString (l);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 v;
	        props->GetPropertyVector (idx, v);
		stack[si].SetVector (v);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 v;
	        props->GetPropertyVector (idx, v);
		stack[si].SetVector (v);
	      }
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor v;
	        props->GetPropertyColor (idx, v);
		stack[si].SetColor (v);
	      }
	      break;
	    case CEL_DATA_PCLASS:
	      {
		iCelPropertyClass* l = props->GetPropertyPClass (idx);
		stack[si].SetPC (l);
	      }
	      break;
	    case CEL_DATA_ENTITY:
	      {
		iCelEntity* l = props->GetPropertyEntity (idx);
		//@@@ stack[si].SetEn (l);
	      }
	      break;
	    default:
	      return ReportError (behave, "Property '%s' has wrong type!",
	      	top.arg.s);
	  }
	}
        break;

      case CEL_OPERATION_CREATEPROPCLASS:
        {
	  CHECK_STACK
	  celXmlArg top = stack.Pop ();
          DUMP_EXEC (": createpropclass %s\n", A2S (top));
	  const char* s = ArgToString (top);
	  iCelPropertyClass* pc = pl->CreatePropertyClass (entity, s);
	  if (!pc)
	    return ReportError (behave, "Couldn't create property class '%s'!",
	    	s);
	  ResolveParameters (entity);
	}
        break;
      case CEL_OPERATION_CREATEENTITY:
        {
	  CHECK_STACK
	  celXmlArg abh = stack.Pop ();
	  CHECK_STACK
	  celXmlArg aent = stack.Pop ();
	  DUMP_EXEC (": createentity %s (behaviour=%s)\n",
	  	A2S (aent), A2S (abh));
	  csRef<iCelEntity> ent = pl->CreateEntity ();
	  const char* entname = ArgToString (aent);
	  if (entname) ent->SetName (entname);
	  csRef<iCelBlLayer> bl = CS_QUERY_REGISTRY (
	  	behave->GetObjectRegistry (), iCelBlLayer);
	  if (!bl)
	    return ReportError (behave,
	    	"Couldn't find default behaviour layer!");

	  const char* bhname = ArgToString (abh);
	  iCelBehaviour* bh = bl->CreateBehaviour (ent, bhname);
	  if (!bh)
	    return ReportError (behave,
	    	"Couldn't create behaviour '%s' for '%s'!",
	    	bhname, entname);
	  ent->SetBehaviour (bh);
	}
        break;
      case CEL_OPERATION_TESTCOLLIDE:
        {
	  CHECK_STACK
	  celXmlArg apc = stack.Pop ();
	  DUMP_EXEC (": testcollide %s\n", A2S (apc));
	  iCelPropertyClass* pc = ArgToPClass (apc, resolvers);
	  if (!pc)
	    return ReportError (behave, "Bad property class!\n");

	  // @@@ Efficiency?
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb)
	    return ReportError (behave,
	    	"Property class is not a billboard!\n");
	  csRef<iPcBillboard> bb = CEL_QUERY_PROPCLASS (
	  	entity->GetPropertyClassList (), iPcBillboard);
	  if (!bb)
	    return ReportError (behave,
	    	"This entity does not have a pcbillboard!\n");
	  csRef<iBillboardManager> bbmgr = CS_QUERY_REGISTRY (
	  	behave->GetObjectRegistry (), iBillboardManager);
	  if (!bbmgr)
	    return ReportError (behave,
	    	"Billboard manager is missing!\n");
	  if (bbmgr->TestCollision (bb->GetBillboard (),
	  	other_bb->GetBillboard ()))
	  {
	    celXmlScriptEventHandler* truebranch = args[0].arg.h;
	    if (truebranch)
	    {
	      truebranch->ResolveParameters (entity);
	      truebranch->Execute (entity, behave);
	    }
	  }
	  else
	  {
	    celXmlScriptEventHandler* falsebranch = args[1].arg.h;
	    if (falsebranch)
	    {
	      falsebranch->ResolveParameters (entity);
	      falsebranch->Execute (entity, behave);
	    }
	  }
	}
	break;
      case CEL_OPERATION_IF:
        {
	  CHECK_STACK
	  celXmlArg eval = stack.Pop ();
	  DUMP_EXEC (": if %s\n", A2S (eval));
	  bool rc = false;
	  switch (eval.type)
	  {
	    case CEL_TYPE_STRING:
	      {
	        const char* s = ArgToString (eval);
	        rc = s ? *s != 0 : false;
	      }
	      break;
	    case CEL_TYPE_PC:
	    case CEL_TYPE_PC_REF:
	      rc = ArgToPClass (eval, resolvers) != 0;
	      break;
	    case CEL_TYPE_BOOL:
	      rc = ArgToBool (eval);
	      break;
	    case CEL_TYPE_VECTOR2:
	      {
	        csVector2 v = ArgToVector2 (eval);
		rc = ABS (v.x) >= SMALL_EPSILON || ABS (v.y) >= SMALL_EPSILON;
	      }
	      break;
	    case CEL_TYPE_VECTOR3:
	      {
	        csVector3 v = ArgToVector3 (eval);
		rc = ABS (v.x) >= SMALL_EPSILON || ABS (v.y) >= SMALL_EPSILON ||
			ABS (v.z) >= SMALL_EPSILON;
	      }
	      break;
	    case CEL_TYPE_COLOR:
	      {
	        csColor v = ArgToColor (eval);
		rc = ABS (v.red) >= SMALL_EPSILON ||
			ABS (v.green) >= SMALL_EPSILON ||
			ABS (v.blue) >= SMALL_EPSILON;
	      }
	      break;
	    case CEL_TYPE_FLOAT:
	      {
	        float f = ArgToFloat (eval);
	        rc = ABS (f) >= SMALL_EPSILON;
	      }
	      break;
	    case CEL_TYPE_INT32:
	      rc = ArgToInt32 (eval) != 0;
	      break;
	    case CEL_TYPE_UINT32:
	      rc = ArgToUInt32 (eval) != 0;
	      break;
	    default:
	      return ReportError (behave, "Can't test on this type!");
	  }
	  if (rc)
	  {
	    celXmlScriptEventHandler* truebranch = args[0].arg.h;
	    if (truebranch)
	    {
	      truebranch->ResolveParameters (entity);
	      truebranch->Execute (entity, behave);
	    }
	  }
	  else
	  {
	    celXmlScriptEventHandler* falsebranch = args[1].arg.h;
	    if (falsebranch)
	    {
	      falsebranch->ResolveParameters (entity);
	      falsebranch->Execute (entity, behave);
	    }
	  }
	}
	break;
      case CEL_OPERATION_PRINT:
        {
	  CHECK_STACK
	  celXmlArg el = stack.Pop ();
	  DUMP_EXEC (": print %s\n", A2S (el));
	  const char* s = ArgToString (el);
	  printf ("%s\n", s);
	  fflush (stdout);
	}
	break;
      case CEL_OPERATION_ACTION:
        {
          DUMP_EXEC (": action pc=%d id=%d s=%s\n", args[0].arg.pc_ref,
	  	args[1].arg.id, args[2].arg.s);
	  resolvers[args[0].arg.pc_ref].pc->PerformAction (
	  	args[1].arg.id, args[2].arg.s);
	}
        break;
      case CEL_OPERATION_GETPROPCLASS:
        {
	  DUMP_EXEC (": getpropclass var=%s %s/%s\n", args[0].arg.s,
	  	args[1].arg.s, args[2].arg.s);
	  iPcProperties* props = behave->GetProperties ();
	  if (!props) break;	// @@@ Report error!
	  iCelEntity* other_ent = pl->FindEntity (args[1].arg.s);
	  if (!other_ent)
	  {
	    props->SetProperty (args[0].arg.s, (iCelPropertyClass*)0);
	    break;
	  }
          iCelPropertyClass* other_pc = other_ent->GetPropertyClassList ()->
	  	FindByName (args[2].arg.s);
	  if (!other_pc)
	  {
	    props->SetProperty (args[0].arg.s, (iCelPropertyClass*)0);
	    break;
	  }
	  props->SetProperty (args[0].arg.s, other_pc);
	}
	break;
      case CEL_OPERATION_VAR:
        {
	  CHECK_STACK
	  celXmlArg val = stack.Pop ();
	  CHECK_STACK
	  celXmlArg var = stack.Pop ();
	  DUMP_EXEC (": var var=%s value=%s\n", A2S (var), A2S (val));

	  iPcProperties* props = behave->GetProperties ();
	  CS_ASSERT (props != 0);
	  const char* varname = ArgToString (var);
	  if (!varname)
	    return ReportError (behave, "Illegal variable name!");
	  switch (val.type)
	  {
	    case CEL_TYPE_BOOL:
	      props->SetProperty (varname, val.arg.b);
	      break;
	    case CEL_TYPE_FLOAT:
	      props->SetProperty (varname, val.arg.f);
	      break;
	    case CEL_TYPE_STRING:
	      props->SetProperty (varname, val.arg.s);
	      break;
	    case CEL_TYPE_UINT32:
	      props->SetProperty (varname, (long)val.arg.ui);
	      break;
	    case CEL_TYPE_INT32:
	      props->SetProperty (varname, (long)val.arg.i);
	      break;
	    case CEL_TYPE_COLOR:
	      {
	        csColor col;
		col.red = val.arg.col.red;
		col.green = val.arg.col.green;
		col.blue = val.arg.col.blue;
	        props->SetProperty (varname, col);
	      }
	      break;
	    case CEL_TYPE_VECTOR2:
	      {
	        csVector2 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_TYPE_VECTOR3:
	      {
	        csVector3 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
		vec.z = val.arg.vec.z;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_TYPE_PC:
	      props->SetProperty (varname, val.arg.pc);
	      break;
	    default:
	      return ReportError (behave, "Bad type of value!");
	  }
	}
	break;
      case CEL_OPERATION_GETPROPERTY:
        {
	  iPcProperties* props = behave->GetProperties ();
	  if (!props) break;	// @@@ Report error!
	  iCelPropertyClass* pc = resolvers[args[1].arg.pc_ref].pc;
	  csStringID id = args[2].arg.id;
	  celDataType t = pc->GetPropertyOrActionType (id);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      DUMP_EXEC (": getproperty %s %d\n", args[0].arg.s,
	      	pc->GetPropertyBool (id));
	      props->SetProperty (args[0].arg.s, pc->GetPropertyBool (id));
	      break;
	    case CEL_DATA_FLOAT:
	      DUMP_EXEC (": getproperty %s %g\n", args[0].arg.s,
	      	pc->GetPropertyFloat (id));
	      props->SetProperty (args[0].arg.s, pc->GetPropertyFloat (id));
	      break;
	    case CEL_DATA_STRING:
	      DUMP_EXEC (": getproperty %s %s\n", args[0].arg.s,
	      	pc->GetPropertyString (id));
	      props->SetProperty (args[0].arg.s, pc->GetPropertyString (id));
	      break;
	    case CEL_DATA_LONG:
	      DUMP_EXEC (": getproperty %s %ld\n", args[0].arg.s,
	      	pc->GetPropertyLong (id));
	      props->SetProperty (args[0].arg.s, pc->GetPropertyLong (id));
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor col;
		pc->GetPropertyColor (id, col);
	        DUMP_EXEC (": getproperty %s %g,%g,%g\n", args[0].arg.s,
	      	  col.red, col.green, col.blue);
	        props->SetProperty (args[0].arg.s, col);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 v;
		pc->GetPropertyVector (id, v);
	        DUMP_EXEC (": getproperty %s %g,%g\n", args[0].arg.s,
	      	  v.x, v.y);
	        props->SetProperty (args[0].arg.s, v);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 v;
		pc->GetPropertyVector (id, v);
	        DUMP_EXEC (": getproperty %s %g,%g,%g\n", args[0].arg.s,
	      	  v.x, v.y, v.z);
	        props->SetProperty (args[0].arg.s, v);
	      }
	      break;
	    default:
	      printf ("Type not supported!\n");
	      fflush (stdout);
	      break;
	  }
	}
	break;
      case CEL_OPERATION_PROPERTY:
        {
          DUMP_EXEC (": property pc=%d id=%d\n", args[0].arg.pc_ref,
	  	args[1].arg.id);
	  iCelPropertyClass* pc = resolvers[args[0].arg.pc_ref].pc;
	  csStringID id = args[1].arg.id;
	  switch (args[2].type)
	  {
	    case CEL_TYPE_VAR:
	      {
		iPcProperties* props = behave->GetProperties ();
		if (!props) break;	// @@@ Report error!
		int idx = props->GetPropertyIndex (args[2].arg.s);
		if (idx == -1) break;	// @@@ Report error!
		switch (props->GetPropertyType (idx))
		{
		  case CEL_DATA_LONG:
		    pc->SetProperty (id, props->GetPropertyLong (idx));
		    break;
		  case CEL_DATA_FLOAT:
		    pc->SetProperty (id, props->GetPropertyFloat (idx));
		    break;
		  case CEL_DATA_BOOL:
		    pc->SetProperty (id, props->GetPropertyBool (idx));
		    break;
		  case CEL_DATA_STRING:
		    pc->SetProperty (id, props->GetPropertyString (idx));
		    break;
		  case CEL_DATA_VECTOR2:
		    {
		      csVector2 v;
		      props->GetPropertyVector (idx, v);
		      pc->SetProperty (id, v);
		    }
		    break;
		  case CEL_DATA_VECTOR3:
		    {
		      csVector3 v;
		      props->GetPropertyVector (idx, v);
		      pc->SetProperty (id, v);
		    }
		    break;
		  case CEL_DATA_COLOR:
		    {
		      csColor v;
		      props->GetPropertyColor (idx, v);
		      pc->SetProperty (id, v);
		    }
		    break;
		  default:
		    CS_ASSERT (false);
		    break;
		}
	      }
	      break;
	    case CEL_TYPE_BOOL:
	      pc->SetProperty (id, args[2].arg.b);
	      break;
	    case CEL_TYPE_FLOAT:
	      pc->SetProperty (id, args[2].arg.f);
	      break;
	    case CEL_TYPE_STRING:
	      pc->SetProperty (id, args[2].arg.s);
	      break;
	    case CEL_TYPE_INT32:
	      pc->SetProperty (id, (long)args[2].arg.i);
	      break;
	    case CEL_TYPE_COLOR:
	      {
	        csColor col;
		col.red = args[2].arg.col.red;
		col.green = args[2].arg.col.green;
		col.blue = args[2].arg.col.blue;
	        pc->SetProperty (id, col);
	      }
	      break;
	    case CEL_TYPE_VECTOR2:
	      {
	        csVector2 v;
		v.x = args[2].arg.vec.x;
		v.y = args[2].arg.vec.y;
	        pc->SetProperty (id, v);
	      }
	      break;
	    case CEL_TYPE_VECTOR3:
	      {
	        csVector3 v;
		v.x = args[2].arg.vec.x;
		v.y = args[2].arg.vec.y;
		v.z = args[2].arg.vec.z;
	        pc->SetProperty (id, v);
	      }
	      break;
	    default:
	      CS_ASSERT (false);
	  }
	}
        break;
    }
  }
}

int celXmlScriptEventHandler::GetResolver (const char* entname,
	const char* pcname)
{
  int i;
  for (i = 0 ; i < resolvers.Length () ; i++)
  {
    if (!strcmp (resolvers[i].pcname, pcname))
    {
      return i;
    }
  }
  int idx = resolvers.Push (celXmlPCResolver ());
  resolvers[idx].entname = csStrNew (entname);
  resolvers[idx].pcname = csStrNew (pcname);
  return idx;
}

void celXmlScriptEventHandler::AddOperation (int op)
{
  operations.Push (celXmlOperation ());
  celXmlOperation& top_op = operations[operations.Length ()-1];
  top_op.op = op;
}

celXmlArg& celXmlScriptEventHandler::AddArgument ()
{
  celXmlOperation& op = operations[operations.Length ()-1];
  switch (op.arg.type)
  {
    case CEL_TYPE_NONE:
      return op.arg;
      break;
    default:
      {
        celXmlArg copy (op.arg);
	op.arg.SetArgList ();
        csArray<celXmlArg>& args = op.arg.arg.a->args;
	args.Push (copy);
      }
      // Fall through...
    case CEL_TYPE_ARGLIST:
      {
        csArray<celXmlArg>& args = op.arg.arg.a->args;
        args.Push (celXmlArg ());
        return args[args.Length ()-1];
      }
      break;
  }
}

//---------------------------------------------------------------------------

celXmlScript::celXmlScript (iCelPlLayer* pl)
{
  name = 0;
  celXmlScript::pl = pl;
}

celXmlScript::~celXmlScript ()
{
  delete[] name;
}

celXmlScriptEventHandler* celXmlScript::FindOrCreateEventHandler (
	const char* name)
{
  celXmlScriptEventHandler* h = GetEventHandler (name);
  if (h) return h;
  return CreateEventHandler (name);
}

celXmlScriptEventHandler* celXmlScript::CreateEventHandler (const char* name)
{
  celXmlScriptEventHandler* h = new celXmlScriptEventHandler (pl);
  h->SetName (name);
  event_handlers.Push (h);
  event_handlers_hash.Put (name, h);
  return h;
}

celXmlScriptEventHandler* celXmlScript::GetEventHandler (const char* name)
{
  return event_handlers_hash.Get (name);
}

//---------------------------------------------------------------------------

