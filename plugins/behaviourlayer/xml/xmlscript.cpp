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
#include "iutil/object.h"
#include "ivaria/reporter.h"
#include "cstool/sndwrap.h"

#include "isound/source.h"
#include "isound/handle.h"
#include "isound/renderer.h"
#include "iengine/engine.h"

#include "plugins/behaviourlayer/xml/xmlscript.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/pl.h"
#include "behaviourlayer/bl.h"
#include "propclass/prop.h"
#include "propclass/billboard.h"
#include "propclass/inv.h"
#include "tools/billboard.h"
#include "celtool/stdparams.h"

#define DO_DUMP 0

inline void dump_exec (const char* msg, ...)
{
#if DO_DUMP
  va_list arg;
  va_start (arg, msg);
  vprintf (msg, arg);
  va_end (arg);
  fflush (stdout);
#endif
}

#if DO_DUMP
#define DUMP_EXEC(x) dump_exec ##x
#else
#define DUMP_EXEC(x)
#endif


//---------------------------------------------------------------------------

celXmlArg::celXmlArg (const celXmlArg& other)
{
  type = other.type;
  switch (type)
  {
    case CEL_DATA_STRING:
      arg.str.cleanup = other.arg.str.cleanup;
      if (arg.str.cleanup)
        arg.str.s = csStrNew (other.arg.str.s);
      else
        arg.str.s = other.arg.str.s;
      break;
    case CEL_DATA_ULONG: arg.ui = other.arg.ui; break;
    case CEL_DATA_LONG: arg.i = other.arg.i; break;
    case CEL_DATA_FLOAT: arg.f = other.arg.f; break;
    case CEL_DATA_BOOL: arg.b = other.arg.b; break;
    case CEL_DATA_PCLASS: arg.pc = other.arg.pc; break;
    case CEL_DATA_ENTITY: arg.entity = other.arg.entity; break;
    case CEL_DATA_ID: arg.id = other.arg.id; break;
    case CEL_DATA_EVENTHANDLER: arg.h = other.arg.h; break;
    case CEL_DATA_CODELOCATION:
      arg.codelocation = other.arg.codelocation;
      break;
    case CEL_DATA_LOCALVAR:
      arg.localvar = other.arg.localvar;
      break;
    case CEL_DATA_VECTOR2: arg.vec = other.arg.vec; break;
    case CEL_DATA_VECTOR3: arg.vec = other.arg.vec; break;
    case CEL_DATA_COLOR: arg.col = other.arg.col; break;
    case CEL_DATA_NONE: break;
    default:
      CS_ASSERT (false);
  }
}

void celXmlArg::Cleanup ()
{
  if (type == CEL_DATA_STRING)
  {
    if (arg.str.cleanup) delete[] arg.str.s;
  }
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

static csArray<csString*> used_strings;
static csPDelArray<csString> unused_strings;

static csString* GetUnusedString ()
{
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
    case CEL_DATA_LONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("%d", a.arg.i);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ULONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("%u", a.arg.ui);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_FLOAT:
      {
        csString* str = GetUnusedString ();
        str->Format ("%g", a.arg.f);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_BOOL: return a.arg.b ? "true" : "false";
    case CEL_DATA_STRING: return a.arg.str.s;
    case CEL_DATA_VECTOR2:
      {
        csString* str = GetUnusedString ();
        str->Format ("[%g,%g]", a.arg.vec.x, a.arg.vec.y);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR3:
      {
        csString* str = GetUnusedString ();
        str->Format ("[%g,%g,%g]", a.arg.vec.x, a.arg.vec.y, a.arg.vec.z);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_PCLASS:
      {
        csString* str = GetUnusedString ();
        str->Format ("pc(%08lx)", (long)a.arg.pc);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ENTITY:
      {
        csString* str = GetUnusedString ();
        str->Format ("ent(%s)", a.arg.entity ? a.arg.entity->GetName () : "<null>");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ID:
      {
        csString* str = GetUnusedString ();
        str->Format ("id(%d)", a.arg.id);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_COLOR:
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
#if DO_DUMP
static const char* A2S (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("{int32:%d}", a.arg.i);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ULONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("{uint32:%u}", a.arg.ui);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_FLOAT:
      {
        csString* str = GetUnusedString ();
        str->Format ("{float:%g}", a.arg.f);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_BOOL:
      {
        csString* str = GetUnusedString ();
        str->Format ("{bool:%s}", a.arg.b ? "true" : "false");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_STRING:
      {
        csString* str = GetUnusedString ();
        str->Format ("{str:%s}", a.arg.str.s);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR2:
      {
        csString* str = GetUnusedString ();
        str->Format ("{vec:[%g,%g]}", a.arg.vec.x, a.arg.vec.y);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR3:
      {
        csString* str = GetUnusedString ();
        str->Format ("{vec:[%g,%g,%g]}", a.arg.vec.x, a.arg.vec.y, a.arg.vec.z);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_COLOR:
      {
        csString* str = GetUnusedString ();
        str->Format ("{rgb:(%g,%g,%g)}", a.arg.col.red, a.arg.col.green,
      	  a.arg.col.blue);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_PCLASS:
      {
        csString* str = GetUnusedString ();
        str->Format ("{pc:%08lx}", (long)a.arg.pc);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ENTITY:
      {
        csString* str = GetUnusedString ();
        str->Format ("{ent:%s}", a.arg.entity ? a.arg.entity->GetName ()
		: "<null>");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ID:
      {
        csString* str = GetUnusedString ();
        str->Format ("{id:%d}", a.arg.id);
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
#endif

static iCelEntity* ArgToEntity (const celXmlArg& a, iCelPlLayer* pl)
{
  if (a.type == CEL_DATA_ENTITY)
    return a.arg.entity;
  else
  {
    const char* entname = ArgToString (a);
    return pl->FindEntity (entname);
  }
}

static const char* EntityNameForError (const celXmlArg& a)
{
  if (a.type == CEL_DATA_ENTITY)
    return a.arg.entity ? a.arg.entity->GetName () : "<null>";
  else
  {
    const char* entname = ArgToString (a);
    return entname;
  }
}

static iCelPropertyClass* ArgToPClass (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_PCLASS: return a.arg.pc;
    default: return 0;
  }
}

static csStringID ArgToID (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_ID: return a.arg.id;
    default: return csInvalidStringID;
  }
}

static csColor ArgToColor (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_COLOR: return csColor (a.arg.col.red, a.arg.col.green,
    	a.arg.col.blue);
    default: return csColor (0, 0, 0);
  }
}

static csVector3 ArgToVector3 (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_VECTOR3: return csVector3 (a.arg.vec.x, a.arg.vec.y,
    	a.arg.vec.z);
    default: return csVector3 (0, 0, 0);
  }
}

static csVector2 ArgToVector2 (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_VECTOR2: return csVector2 (a.arg.vec.x, a.arg.vec.y);
    default: return csVector2 (0, 0);
  }
}

static bool ArgToBool (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG: return bool (a.arg.i);
    case CEL_DATA_ULONG: return bool (a.arg.ui);
    case CEL_DATA_FLOAT: return bool (ABS (a.arg.f) < SMALL_EPSILON);
    case CEL_DATA_BOOL: return a.arg.b;
    case CEL_DATA_STRING:
      return a.arg.str.s ? !strcmp (a.arg.str.s, "true") : false;
    default:
      return false;
  }
}

static uint32 ArgToUInt32 (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG: return uint32 (a.arg.i);
    case CEL_DATA_ULONG: return a.arg.ui;
    case CEL_DATA_FLOAT: return uint32 (a.arg.f);
    case CEL_DATA_BOOL: return a.arg.b ? 1 : 0;
    case CEL_DATA_STRING:
      {
        if (!a.arg.str.s) return 0;
	uint i;
        sscanf (a.arg.str.s, "%u", &i);
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
    case CEL_DATA_LONG: return a.arg.i;
    case CEL_DATA_ULONG: return int32 (a.arg.ui);
    case CEL_DATA_FLOAT: return int32 (a.arg.f);
    case CEL_DATA_BOOL: return a.arg.b ? 1 : 0;
    case CEL_DATA_STRING:
      {
        if (!a.arg.str.s) return 0;
        int i;
	sscanf (a.arg.str.s, "%d", &i);
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
    case CEL_DATA_LONG: return float (a.arg.i);
    case CEL_DATA_ULONG: return float (a.arg.ui);
    case CEL_DATA_FLOAT: return a.arg.f;
    case CEL_DATA_BOOL: return a.arg.b ? 1.0 : 0.0;
    case CEL_DATA_STRING:
      {
        if (!a.arg.str.s) return 0.0;
        float f;
	sscanf (a.arg.str.s, "%f", &f);
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
    case CEL_DATA_VECTOR2: return true;
    case CEL_DATA_VECTOR3: return true;
    case CEL_DATA_COLOR: return true;
    default: return false;
  }
}

static bool IsNumericType (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG: return true;
    case CEL_DATA_ULONG: return true;
    case CEL_DATA_FLOAT: return true;
    case CEL_DATA_BOOL: return true;
    default: return false;
  }
}

static bool IsStringConvertible (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG: return true;
    case CEL_DATA_ULONG: return true;
    case CEL_DATA_FLOAT: return true;
    case CEL_DATA_BOOL: return true;
    case CEL_DATA_VECTOR2: return true;
    case CEL_DATA_VECTOR3: return true;
    case CEL_DATA_COLOR: return true;
    case CEL_DATA_ID: return true;
    case CEL_DATA_PCLASS: return true;
    case CEL_DATA_ENTITY: return true;
    default: return false;
  }
}

static int GetCalculationType (const celXmlArg& a, const celXmlArg& b)
{
  if (a.type == b.type) return a.type;

  if (a.type == CEL_DATA_STRING && IsStringConvertible (b))
    return CEL_DATA_STRING;
  if (b.type == CEL_DATA_STRING && IsStringConvertible (a))
    return CEL_DATA_STRING;

  if (a.type == CEL_DATA_FLOAT && IsNumericType (b))
    return CEL_DATA_FLOAT;
  if (b.type == CEL_DATA_FLOAT && IsNumericType (a))
    return CEL_DATA_FLOAT;

  if (a.type == CEL_DATA_LONG && IsNumericType (b))
    return CEL_DATA_LONG;
  if (b.type == CEL_DATA_LONG && IsNumericType (a))
    return CEL_DATA_LONG;

  if (a.type == CEL_DATA_ULONG && IsNumericType (b))
    return CEL_DATA_ULONG;
  if (b.type == CEL_DATA_ULONG && IsNumericType (a))
    return CEL_DATA_ULONG;

  return CEL_DATA_NONE;
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

csRef<iSoundSource> sound_source;

bool celXmlScriptEventHandler::EvaluateTrue (const celXmlArg& eval,
	celBehaviourXml* behave, bool& rc)
{
  switch (eval.type)
  {
    case CEL_DATA_STRING:
      {
	const char* s = ArgToString (eval);
	rc = s ? *s != 0 : false;
      }
      break;
    case CEL_DATA_ENTITY:
      rc = eval.arg.entity != 0;
      break;
    case CEL_DATA_PCLASS:
      rc = ArgToPClass (eval) != 0;
      break;
    case CEL_DATA_BOOL:
      rc = ArgToBool (eval);
      break;
    case CEL_DATA_VECTOR2:
      {
	csVector2 v = ArgToVector2 (eval);
	rc = ABS (v.x) >= SMALL_EPSILON || ABS (v.y) >= SMALL_EPSILON;
      }
      break;
    case CEL_DATA_VECTOR3:
      {
        csVector3 v = ArgToVector3 (eval);
        rc = ABS (v.x) >= SMALL_EPSILON || ABS (v.y) >= SMALL_EPSILON ||
          ABS (v.z) >= SMALL_EPSILON;
      }
      break;
    case CEL_DATA_COLOR:
      {
        csColor v = ArgToColor (eval);
        rc = ABS (v.red) >= SMALL_EPSILON ||
            ABS (v.green) >= SMALL_EPSILON ||
            ABS (v.blue) >= SMALL_EPSILON;
      }
      break;
    case CEL_DATA_FLOAT:
      {
        float f = ArgToFloat (eval);
        rc = ABS (f) >= SMALL_EPSILON;
      }
      break;
    case CEL_DATA_LONG:
      rc = ArgToInt32 (eval) != 0;
      break;
    case CEL_DATA_ULONG:
      rc = ArgToUInt32 (eval) != 0;
      break;
    default:
      return ReportError (behave, "Can't test on this type!");
  }
  return true;
}

#ifdef CS_DEBUG
#define CHECK_STACK(i) \
  if (stack.Length () < i) \
    return ReportError (behave, "Stack underflow!");
#else
#define CHECK_STACK(i)
#endif

bool celXmlScriptEventHandler::Execute (iCelEntity* entity,
	celBehaviourXml* behave, iCelParameterBlock* params,
	int startop)
{
  // We keep a reference to the entity here to prevent
  // deletion of that entity during script execution.
  csRef<iCelEntity> e = entity;

  int stack_size = stack.Length ();
  int i = startop;
  if (startop == 0)
  {
    DUMP_EXEC ((":::: entity=%s behave=%s\n", entity->GetName (),
  	behave->GetName ()));
  }

  for (;;)
  {
    CleanupTemporaryStrings ();
    celXmlOperation& op = operations[i];
    i++;
    switch (op.op)
    {
      case CEL_OPERATION_END:
        if (stack_size != stack.Length ())
	{
	  return ReportError (behave, "Stack size mismatch!");
	}
        return true;
      case CEL_OPERATION_PARAM:
        {
	  CHECK_STACK(1)
	  celXmlArg a_arg = stack.Pop ();
          DUMP_EXEC ((":%04d: param %s\n", i-1, A2S (a_arg)));
	  csStringID id = ArgToID (a_arg);
	  int si = stack.Push (celXmlArg ());
	  const celData* data = params->GetParameter (id);
	  if (!data)
	    return ReportError (behave, "Can't find parameter!");
	  switch (data->type)
	  {
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (data->value.b);
	      break;
	    case CEL_DATA_FLOAT:
	      stack[si].SetFloat (data->value.f);
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetString (data->value.s->GetData (), true);
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (data->value.l);
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (data->value.ul);
	      break;
	    case CEL_DATA_PCLASS:
	      stack[si].SetPC (data->value.pc);
	      break;
	    case CEL_DATA_ENTITY:
	      stack[si].SetEntity (data->value.ent);
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor col (data->value.col.red, data->value.col.green,
			data->value.col.blue);
	        stack[si].SetColor (col);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 v (data->value.v.x, data->value.v.y);
	        stack[si].SetVector (v);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 v (data->value.v.x, data->value.v.y, data->value.v.z);
	        stack[si].SetVector (v);
	      }
	      break;
	    default:
	      return ReportError (behave, "Type not supported for 'arg'!");
	  }
	}
	break;
      case CEL_OPERATION_PUSH:
        {
          DUMP_EXEC ((":%04d: push %s\n", i-1, A2S (op.arg)));
	  stack.Push (op.arg);
	}
        break;
      case CEL_OPERATION_PUSHSTR:
        {
          DUMP_EXEC ((":%04d: pushstr %s\n", i-1, A2S (op.arg)));
	  // Optimization for strings. Don't copy the string.
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetString (op.arg.arg.str.s, false);
	}
        break;
      case CEL_OPERATION_PCTHIS:
        {
	  CHECK_STACK(1)
	  celXmlArg a_pc = stack.Pop ();
          DUMP_EXEC ((":%04d: pcthis %s\n", i-1, A2S (a_pc)));

	  int si = stack.Push (celXmlArg ());
          iCelPropertyClass* other_pc = entity->GetPropertyClassList ()->
	  	FindByName (ArgToString (a_pc));
	  stack[si].SetPC (other_pc);	// Can be 0.
	}
	break;
      case CEL_OPERATION_CALCPARID:
        {
	  CHECK_STACK(1)
	  celXmlArg a_id = stack.Pop ();
          DUMP_EXEC ((":%04d: calcparid %s\n", i-1, A2S (a_id)));
	  int si = stack.Push (celXmlArg ());
	  csString str = "cel.parameter.";
	  str += ArgToString (a_id);
	  csStringID id = pl->FetchStringID ((const char*)str);
	  stack[si].SetID (id);
	}
	break;
      case CEL_OPERATION_CALCPROPID:
        {
	  CHECK_STACK(1)
	  celXmlArg a_id = stack.Pop ();
          DUMP_EXEC ((":%04d: calcpropid %s\n", i-1, A2S (a_id)));
	  int si = stack.Push (celXmlArg ());
	  csString str = "cel.property.";
	  str += ArgToString (a_id);
	  csStringID id = pl->FetchStringID ((const char*)str);
	  stack[si].SetID (id);
	}
	break;
      case CEL_OPERATION_CALCID:
        {
	  CHECK_STACK(1)
	  celXmlArg a_id = stack.Pop ();
          DUMP_EXEC ((":%04d: calcid %s\n", i-1, A2S (a_id)));
	  int si = stack.Push (celXmlArg ());
	  csStringID id = pl->FetchStringID (ArgToString (a_id));
	  stack[si].SetID (id);
	}
	break;
      case CEL_OPERATION_PC:
        {
	  CHECK_STACK(2)
	  celXmlArg a_pc = stack.Pop ();
	  celXmlArg a_ent = stack.Pop ();
          DUMP_EXEC ((":%04d: pc %s %s\n", i-1, A2S (a_ent), A2S (a_pc)));

	  int si = stack.Push (celXmlArg ());
	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	  {
	    stack[si].SetPC ((iCelPropertyClass*)0);
	    break;
	  }
          iCelPropertyClass* other_pc = other_ent->GetPropertyClassList ()->
	  	FindByName (ArgToString (a_pc));
	  stack[si].SetPC (other_pc);	// Can be 0.
	}
        break;
      case CEL_OPERATION_VECTOR2:
        {
	  CHECK_STACK(2)
	  celXmlArg ely = stack.Pop ();
	  celXmlArg elx = stack.Pop ();
          DUMP_EXEC ((":%04d: vector2 %s, %s\n", i-1, A2S (elx), A2S (ely)));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetVector (csVector2 (ArgToFloat (elx), ArgToFloat (ely)));
	}
	break;
      case CEL_OPERATION_VECTOR3:
        {
	  CHECK_STACK(3)
	  celXmlArg elz = stack.Pop ();
	  celXmlArg ely = stack.Pop ();
	  celXmlArg elx = stack.Pop ();
          DUMP_EXEC ((":%04d: vector3 %s, %s, %s\n", i-1, A2S (elx),
	  	A2S (ely), A2S (elz)));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetVector (csVector3 (ArgToFloat (elx),
	  	ArgToFloat (ely), ArgToFloat (elz)));
	}
	break;
      case CEL_OPERATION_COLOR:
        {
	  CHECK_STACK(3)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg elg = stack.Pop ();
	  celXmlArg elr = stack.Pop ();
          DUMP_EXEC ((":%04d: color %s, %s, %s\n", i-1, A2S (elr),
	  	A2S (elg), A2S (elb)));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetColor (csColor (ArgToFloat (elr),
	  	ArgToFloat (elg), ArgToFloat (elb)));
	}
	break;
      case CEL_OPERATION_ENTNAME:
        {
          DUMP_EXEC ((":%04d: entname ()\n", i-1));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetString (entity->GetName (), true);
	}
	break;
      case CEL_OPERATION_RAND:
        {
	  CHECK_STACK(1)
	  int si = stack.Length ()-1;
          DUMP_EXEC ((":%04d: rand (%s)\n", i-1, A2S (stack[si])));
	  float f = ArgToFloat (stack[si]);
	  stack[si].SetFloat (float (rand () % 10000) * f / 10000.0);
	}
	break;
      case CEL_OPERATION_FLOAT:
        {
	  CHECK_STACK(1)
	  int si = stack.Length ()-1;
          DUMP_EXEC ((":%04d: float %s\n", i-1, A2S (stack[si])));
	  switch (stack[si].type)
	  {
	    case CEL_DATA_LONG:
	      stack[si].SetFloat (float (stack[si].arg.i));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetFloat (float (stack[si].arg.ui));
	      break;
	    case CEL_DATA_FLOAT:
	      break;
	    case CEL_DATA_BOOL:
	      stack[si].SetFloat (float (stack[si].arg.b));
	      break;
	    case CEL_DATA_STRING:
	      {
	        float i;
		if (stack[si].arg.str.s)
		  sscanf (stack[si].arg.str.s, "%f", &i);
		else
		  i = 0.0;
	        stack[si].SetFloat (i);
	      }
	      break;
	    default:
	      return ReportError (behave,
	      	"Can't convert element on stack to 'float'!");
	  }
	}
	break;
      case CEL_OPERATION_INT:
        {
	  CHECK_STACK(1)
	  int si = stack.Length ()-1;
          DUMP_EXEC ((":%04d: int %s\n", i-1, A2S (stack[si])));
	  switch (stack[si].type)
	  {
	    case CEL_DATA_LONG:
	    case CEL_DATA_ULONG:
	      break;
	    case CEL_DATA_FLOAT:
	      stack[si].SetInt32 (int32 (stack[si].arg.f));
	      break;
	    case CEL_DATA_BOOL:
	      stack[si].SetInt32 (int32 (stack[si].arg.b));
	      break;
	    case CEL_DATA_STRING:
	      {
	        int i;
		if (stack[si].arg.str.s)
		  sscanf (stack[si].arg.str.s, "%d", &i);
		else
		  i = 0;
	        stack[si].SetInt32 ((int32)i);
	      }
	      break;
	    default:
	      return ReportError (behave,
	      	"Can't convert element on stack to 'int'!");
	  }
	}
	break;
      case CEL_OPERATION_SIGN:
        {
	  CHECK_STACK(1)
	  int si = stack.Length ()-1;
          DUMP_EXEC ((":%04d: sign %s\n", i-1, A2S (stack[si])));
	  switch (stack[si].type)
	  {
	    case CEL_DATA_LONG:
	      stack[si].arg.i = SIGN (stack[si].arg.i);
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetInt32 (stack[si].arg.ui > 0 ? 1 : 0);
	      break;
	    case CEL_DATA_FLOAT:
	      stack[si].SetInt32 (SIGN (stack[si].arg.f));
	      break;
	    default:
	      return ReportError (behave,
	      	"Can't calculate 'sign' for element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_ABS:
        {
	  CHECK_STACK(1)
	  int si = stack.Length ()-1;
          DUMP_EXEC ((":%04d: abs %s\n", i-1, A2S (stack[si])));
	  switch (stack[si].type)
	  {
	    case CEL_DATA_LONG: stack[si].arg.i = ABS (stack[si].arg.i); break;
	    case CEL_DATA_ULONG: break;
	    case CEL_DATA_FLOAT: stack[si].arg.f = ABS (stack[si].arg.f); break;
	    default:
	      return ReportError (behave,
	      	"Can't calculate 'abs' for element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_BITNOT:
        {
	  CHECK_STACK(1)
	  int si = stack.Length ()-1;
          DUMP_EXEC ((":%04d: ~ %s\n", i-1, A2S (stack[si])));
	  switch (stack[si].type)
	  {
	    case CEL_DATA_LONG: stack[si].arg.i = ~stack[si].arg.i; break;
	    case CEL_DATA_ULONG: stack[si].arg.ui = ~stack[si].arg.ui; break;
	    case CEL_DATA_BOOL: stack[si].arg.b = !stack[si].arg.b; break;
	    default:
	      return ReportError (behave, "Can't bit-not element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_UNARYMINUS:
        {
	  CHECK_STACK(1)
	  int si = stack.Length ()-1;
          DUMP_EXEC ((":%04d: unaryminus %s\n", i-1, A2S (stack[si])));
	  switch (stack[si].type)
	  {
	    case CEL_DATA_LONG: stack[si].arg.i = -stack[si].arg.i; break;
	    case CEL_DATA_ULONG: stack[si].arg.ui = -stack[si].arg.ui; break;
	    case CEL_DATA_FLOAT: stack[si].arg.f = -stack[si].arg.f; break;
	    case CEL_DATA_BOOL: stack[si].arg.b = !stack[si].arg.b; break;
	    case CEL_DATA_COLOR:
	      stack[si].arg.col.red = -stack[si].arg.col.red;
	      stack[si].arg.col.green = -stack[si].arg.col.green;
	      stack[si].arg.col.blue = -stack[si].arg.col.blue;
	      break;
	    case CEL_DATA_VECTOR3:
	      stack[si].arg.vec.x = -stack[si].arg.vec.x;
	      stack[si].arg.vec.y = -stack[si].arg.vec.y;
	      stack[si].arg.vec.z = -stack[si].arg.vec.z;
	      break;
	    case CEL_DATA_VECTOR2:
	      stack[si].arg.vec.x = -stack[si].arg.vec.x;
	      stack[si].arg.vec.y = -stack[si].arg.vec.y;
	      break;
	    default:
	      return ReportError (behave, "Can't negate element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_STRSPLIT:
        {
	  CHECK_STACK(4)
	  celXmlArg a_right = stack.Pop ();
	  celXmlArg a_del = stack.Pop ();
	  celXmlArg a_left = stack.Pop ();
	  celXmlArg a_str = stack.Pop ();
          DUMP_EXEC ((":%04d: strsplit string=%s left=%d del=%s right=%s\n", i-1,
	  	A2S (a_str), A2S (a_left), A2S (a_del), A2S (a_right)));
	  const char* str = ArgToString (a_str);
	  const char* leftvar = ArgToString (a_left);
	  const char* del = ArgToString (a_del);
	  const char* rightvar = ArgToString (a_right);
	  char* s = strstr (str, del);
	  iPcProperties* props = behave->GetProperties ();
	  if (s)
	  {
	    char old = *s;
	    *s = 0;
	    props->SetProperty (leftvar, str);
	    *s = old;
	    props->SetProperty (rightvar, s+strlen (del));
	  }
	  else
	  {
	    props->SetProperty (leftvar, "");
	    props->SetProperty (rightvar, "");
	  }
	}
	break;
      case CEL_OPERATION_STRIDX:
        {
	  CHECK_STACK(2)
	  celXmlArg a_sub = stack.Pop ();
	  celXmlArg a_str = stack.Pop ();
          DUMP_EXEC ((":%04d: stridx (%s,%s)\n", i-1, A2S (a_str),
	  	A2S (a_sub)));
	  int si = stack.Push (celXmlArg ());
	  const char* str = ArgToString (a_str);
	  const char* sub = ArgToString (a_sub);
	  char* rc = strstr (str, sub);
	  if (rc)
	    stack[si].SetInt32 (rc-str);
	  else
	    stack[si].SetInt32 (-1);
	}
        break;
      case CEL_OPERATION_STRSUB:
        {
	  CHECK_STACK(3)
	  celXmlArg a_len = stack.Pop ();
	  celXmlArg a_pos = stack.Pop ();
	  celXmlArg a_str = stack.Pop ();
          DUMP_EXEC ((":%04d: strsub (%s,%s,%s)\n", i-1, A2S (a_str),
	  	A2S (a_pos), A2S (a_len)));
	  int si = stack.Push (celXmlArg ());
	  const char* str = ArgToString (a_str);
	  int32 pos = ArgToInt32 (a_pos);
	  int32 len = ArgToInt32 (a_len);
	  int real_len = strlen (str) - pos;
	  real_len = MIN (real_len, len);
	  char* newstr = new char[real_len+1];
	  if (real_len)
	    strncpy (newstr, str+pos, real_len);
	  newstr[real_len] = 0;
	  stack[si].SetStringPrealloc (newstr);
	}
        break;
      case CEL_OPERATION_STRLEN:
        {
	  CHECK_STACK(1)
	  celXmlArg a_str = stack.Pop ();
          DUMP_EXEC ((":%04d: strlen (%s)\n", i-1, A2S (a_str)));
	  int si = stack.Push (celXmlArg ());
	  const char* str = ArgToString (a_str);
	  if (str) stack[si].SetInt32 (strlen (str));
	  else stack[si].SetInt32 (0);
	}
	break;
      case CEL_OPERATION_INTPOL:
        {
	  CHECK_STACK(3)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
	  celXmlArg a_int = stack.Pop ();
          DUMP_EXEC ((":%04d: intpol (%s,%s,%s)\n", i-1, A2S (a_int),
	  	A2S (ela), A2S (elb)));
	  float delta = ArgToFloat (a_int);
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      {
	        float fa = ArgToFloat (ela);
	        float fb = ArgToFloat (elb);
	        stack[si].SetFloat ((1.0-delta)*fa + delta*fb);
	      }
	      break;
	    case CEL_DATA_LONG:
	      {
	        int32 fa = ArgToInt32 (ela);
	        int32 fb = ArgToInt32 (elb);
	        stack[si].SetInt32 (int32 (
			(1.0-delta)*float (fa) + delta*float (fb)));
	      }
	      break;
	    case CEL_DATA_ULONG:
	      {
	        uint32 fa = ArgToUInt32 (ela);
	        uint32 fb = ArgToUInt32 (elb);
	        stack[si].SetUInt32 (uint32 (
			(1.0-delta)*float (fa) + delta*float (fb)));
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 fa = ArgToVector2 (ela);
	        csVector2 fb = ArgToVector2 (elb);
	        stack[si].SetVector ((1.0-delta)*fa + delta*fb);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 fa = ArgToVector3 (ela);
	        csVector3 fb = ArgToVector3 (elb);
	        stack[si].SetVector ((1.0-delta)*fa + delta*fb);
	      }
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor fa = ArgToColor (ela);
	        csColor fb = ArgToColor (elb);
	        stack[si].SetColor ((1.0-delta)*fa + delta*fb);
	      }
	      break;
	    default:
	      return ReportError (behave,
	      	"Can't 'interpolate' these types!");
	  }
	}
	break;
      case CEL_OPERATION_MIN:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: min (%s,%s)\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_STRING:
	      {
	        const char* sa = ArgToString (ela);
	        const char* sb = ArgToString (elb);
	        if (strcmp (sa, sb) < 0)
	          stack[si].SetString (sa, true);
	        else
	          stack[si].SetString (sb, true);
	      }
	      break;
	    case CEL_DATA_FLOAT:
	      {
	        float fa = ArgToFloat (ela);
	        float fb = ArgToFloat (elb);
	        stack[si].SetFloat (MIN (fa, fb));
	      }
	      break;
	    case CEL_DATA_LONG:
	      {
	        int32 fa = ArgToInt32 (ela);
	        int32 fb = ArgToInt32 (elb);
	        stack[si].SetInt32 (MIN (fa, fb));
	      }
	      break;
	    case CEL_DATA_ULONG:
	      {
	        uint32 fa = ArgToUInt32 (ela);
	        uint32 fb = ArgToUInt32 (elb);
	        stack[si].SetUInt32 (MIN (fa, fb));
	      }
	      break;
	    default:
	      return ReportError (behave,
	      	"Can't calculate 'min' of these types!");
	  }
	}
	break;
      case CEL_OPERATION_MAX:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: max (%s,%s)\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_STRING:
	      {
	        const char* sa = ArgToString (ela);
	        const char* sb = ArgToString (elb);
	        if (strcmp (sa, sb) > 0)
	          stack[si].SetString (sa, true);
	        else
	          stack[si].SetString (sb, true);
	      }
	      break;
	    case CEL_DATA_FLOAT:
	      {
	        float fa = ArgToFloat (ela);
	        float fb = ArgToFloat (elb);
	        stack[si].SetFloat (MAX (fa, fb));
	      }
	      break;
	    case CEL_DATA_LONG:
	      {
	        int32 fa = ArgToInt32 (ela);
	        int32 fb = ArgToInt32 (elb);
	        stack[si].SetInt32 (MAX (fa, fb));
	      }
	      break;
	    case CEL_DATA_ULONG:
	      {
	        uint32 fa = ArgToUInt32 (ela);
	        uint32 fb = ArgToUInt32 (elb);
	        stack[si].SetUInt32 (MAX (fa, fb));
	      }
	      break;
	    default:
	      return ReportError (behave,
	      	"Can't calculate 'max' of these types!");
	  }
	}
	break;
      case CEL_OPERATION_MINUS:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s - %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_NONE:
	      return ReportError (behave, "Can't subtract these types!");
	    case CEL_DATA_BOOL:
	      return ReportError (behave, "Can't subtract booleans!");
	    case CEL_DATA_STRING:
	      return ReportError (behave, "Can't subtract strings!");
	    case CEL_DATA_FLOAT:
	      stack[si].SetFloat (ArgToFloat (ela) - ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (ArgToInt32 (ela) - ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) - ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      stack[si].SetVector (ArgToVector2 (ela) - ArgToVector2 (elb));
	      break;
	    case CEL_DATA_VECTOR3:
	      stack[si].SetVector (ArgToVector3 (ela) - ArgToVector3 (elb));
	      break;
	    case CEL_DATA_COLOR:
	      stack[si].SetColor (ArgToColor (ela) - ArgToColor (elb));
	      break;
	  }
	}
	break;
      case CEL_OPERATION_ADD:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s + %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      return ReportError (behave, "Can't add booleans!");
	    case CEL_DATA_FLOAT:
	      stack[si].SetFloat (ArgToFloat (ela) + ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (ArgToInt32 (ela) + ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) + ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      stack[si].SetVector (ArgToVector2 (ela) + ArgToVector2 (elb));
	      break;
	    case CEL_DATA_VECTOR3:
	      stack[si].SetVector (ArgToVector3 (ela) + ArgToVector3 (elb));
	      break;
	    case CEL_DATA_COLOR:
	      stack[si].SetColor (ArgToColor (ela) + ArgToColor (elb));
	      break;
	    case CEL_DATA_NONE:
	    case CEL_DATA_STRING:
	      {
		const char* astr = ArgToString (ela);
		const char* bstr = ArgToString (elb);
		int alen = astr ? strlen (astr) : 0;
		int blen = bstr ? strlen (bstr) : 0;
		if (alen || blen)
		{
		  if (!blen)
	            stack[si].SetString (astr, true);
		  else if (!alen)
	            stack[si].SetString (bstr, true);
		  else
		  {
		    char* str = new char[alen+blen+1];
		    strcpy (str, astr);
		    strcpy (str+alen, bstr);
	            stack[si].SetStringPrealloc (str);
		  }
	        }
		else
		{
	          stack[si].SetString ("", true);
		}
	      }
	      break;
	  }
	}
	break;
      case CEL_OPERATION_MULT:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s * %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_NONE:
	      if (IsNumericType (ela) && IsVectorType (elb))
	      {
	        switch (elb.type)
		{
		  case CEL_DATA_VECTOR2:
		    stack[si].SetVector (ArgToFloat (ela) * ArgToVector2 (elb));
		    break;
		  case CEL_DATA_VECTOR3:
		    stack[si].SetVector (ArgToFloat (ela) * ArgToVector3 (elb));
		    break;
		  case CEL_DATA_COLOR:
		    stack[si].SetColor (ArgToFloat (ela) * ArgToColor (elb));
		    break;
		}
	      }
	      else if (IsVectorType (ela) && IsNumericType (elb))
	      {
	        switch (ela.type)
		{
		  case CEL_DATA_VECTOR2:
		    stack[si].SetVector (ArgToFloat (elb) * ArgToVector2 (ela));
		    break;
		  case CEL_DATA_VECTOR3:
		    stack[si].SetVector (ArgToFloat (elb) * ArgToVector3 (ela));
		    break;
		  case CEL_DATA_COLOR:
		    stack[si].SetColor (ArgToFloat (elb) * ArgToColor (ela));
		    break;
		}
	      }
	      else
	        return ReportError (behave, "Can't multiply these types!");
	      break;
	    case CEL_DATA_STRING:
	      return ReportError (behave, "Can't multiply strings!");
	    case CEL_DATA_COLOR:
	      return ReportError (behave, "Can't multiply colors!");
	    case CEL_DATA_FLOAT:
	      stack[si].SetFloat (ArgToFloat (ela) * ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (ArgToInt32 (ela) * ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) * ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      stack[si].SetFloat (ArgToVector2 (ela) * ArgToVector2 (elb));
	      break;
	    case CEL_DATA_VECTOR3:
	      stack[si].SetFloat (ArgToVector3 (ela) * ArgToVector3 (elb));
	      break;
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (bool (int (ArgToBool (ela))
	      	* int (ArgToBool (elb))));
	      break;
	    default:
	      return ReportError (behave, "Can't multiply these types!");
	  }
	}
	break;
      case CEL_OPERATION_MODULO:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s %% %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_VECTOR3:
	      stack[si].SetVector (ArgToVector3 (ela) % ArgToVector3 (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (ArgToInt32 (ela) % ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) % ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (behave,
	      	"Can't compute modulo or cross of these types!");
	  }
	}
	break;
      case CEL_OPERATION_DIV:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s / %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_NONE:
	      if (IsVectorType (ela) && IsNumericType (elb))
	      {
	        switch (ela.type)
		{
		  case CEL_DATA_VECTOR2:
		    stack[si].SetVector (ArgToVector2 (ela) / ArgToFloat (elb));
		    break;
		  case CEL_DATA_VECTOR3:
		    stack[si].SetVector (ArgToVector3 (ela) / ArgToFloat (elb));
		    break;
		  default:
	            return ReportError (behave, "Can't divide these types!");
		}
	      }
	      else
	        return ReportError (behave, "Can't divide these types!");
	      break;
	    case CEL_DATA_STRING:
	      return ReportError (behave, "Can't divide strings!");
	    case CEL_DATA_BOOL:
	      return ReportError (behave, "Can't divide booleans!");
	    case CEL_DATA_VECTOR2:
	      return ReportError (behave, "Can't divide vectors!");
	    case CEL_DATA_VECTOR3:
	      return ReportError (behave, "Can't divide vectors!");
	    case CEL_DATA_COLOR:
	      return ReportError (behave, "Can't divide colors!");
	    case CEL_DATA_FLOAT:
	      stack[si].SetFloat (ArgToFloat (ela) / ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (ArgToInt32 (ela) / ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) / ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (behave, "Can't divide these types!");
	  }
	}
	break;
      case CEL_OPERATION_BITAND:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s & %s\n", i-1, A2S (ela), A2S (elb)));
	  int si = stack.Push (celXmlArg ());
	  int t = GetCalculationType (ela, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (ArgToBool (ela) & ArgToBool (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (ArgToInt32 (ela) & ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) & ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (behave, "Can't bit-and these types!");
	  }
	}
	break;
      case CEL_OPERATION_BITOR:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s | %s\n", i-1, A2S (ela), A2S (elb)));
	  int si = stack.Push (celXmlArg ());
	  int t = GetCalculationType (ela, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (ArgToBool (ela) | ArgToBool (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (ArgToInt32 (ela) | ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) | ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (behave, "Can't bit-or these types!");
	  }
	}
	break;
      case CEL_OPERATION_BITXOR:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s ^ %s\n", i-1, A2S (ela), A2S (elb)));
	  int si = stack.Push (celXmlArg ());
	  int t = GetCalculationType (ela, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (ArgToBool (ela) ^ ArgToBool (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (ArgToInt32 (ela) ^ ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 (ArgToUInt32 (ela) ^ ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (behave, "Can't bit-or these types!");
	  }
	}
	break;
      case CEL_OPERATION_LOGAND:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s && %s\n", i-1, A2S (ela), A2S (elb)));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetBool (ArgToBool (ela) && ArgToBool (elb));
	}
	break;
      case CEL_OPERATION_LOGOR:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s || %s\n", i-1, A2S (ela), A2S (elb)));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetBool (ArgToBool (ela) || ArgToBool (elb));
	}
	break;
      case CEL_OPERATION_NE:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s != %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (ArgToBool (ela) != ArgToBool (elb));
	      break;
	    case CEL_DATA_FLOAT:
	      stack[si].SetBool (ABS (ArgToFloat (ela) - ArgToFloat (elb))
	      	>= SMALL_EPSILON);
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetBool (ArgToInt32 (ela) != ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetBool (ArgToUInt32 (ela) != ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      stack[si].SetBool (!((ArgToVector2 (ela) - ArgToVector2 (elb))
	      	< SMALL_EPSILON));
	      break;
	    case CEL_DATA_VECTOR3:
	      stack[si].SetBool (!((ArgToVector3 (ela) - ArgToVector3 (elb))
	      	< SMALL_EPSILON));
	      break;
	    case CEL_DATA_ENTITY:
	      stack[si].SetBool (ela.arg.entity != elb.arg.entity);
	      break;
	    case CEL_DATA_PCLASS:
	      stack[si].SetBool (ArgToPClass (ela) != ArgToPClass (elb));
	      break;
	    case CEL_DATA_ID:
	      stack[si].SetBool (ArgToID (ela) != ArgToID (elb));
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor c1 = ArgToColor (ela);
	        csColor c2 = ArgToColor (elb);
	        stack[si].SetBool (!(ABS (c1.red-c2.red) < SMALL_EPSILON &&
	        		     ABS (c1.green-c2.green) < SMALL_EPSILON &&
	        		     ABS (c1.blue-c2.blue) < SMALL_EPSILON));
	      }
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetBool (strcmp (ArgToString (ela),
	      	ArgToString (elb)));
	      break;
	    default:
	      return ReportError (behave, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_EQ:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s == %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (ArgToBool (ela) == ArgToBool (elb));
	      break;
	    case CEL_DATA_FLOAT:
	      stack[si].SetBool (ABS (ArgToFloat (ela) - ArgToFloat (elb))
	      	< SMALL_EPSILON);
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetBool (ArgToInt32 (ela) == ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetBool (ArgToUInt32 (ela) == ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      stack[si].SetBool ((ArgToVector2 (ela) - ArgToVector2 (elb))
	      	< SMALL_EPSILON);
	      break;
	    case CEL_DATA_VECTOR3:
	      stack[si].SetBool ((ArgToVector3 (ela) - ArgToVector3 (elb))
	      	< SMALL_EPSILON);
	      break;
	    case CEL_DATA_PCLASS:
	      stack[si].SetBool (ArgToPClass (ela) == ArgToPClass (elb));
	      break;
	    case CEL_DATA_ENTITY:
	      stack[si].SetBool (ela.arg.entity ==  elb.arg.entity);
	      break;
	    case CEL_DATA_ID:
	      stack[si].SetBool (ArgToID (ela) == ArgToID (elb));
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor c1 = ArgToColor (ela);
	        csColor c2 = ArgToColor (elb);
	        stack[si].SetBool (ABS (c1.red-c2.red) < SMALL_EPSILON &&
	        		   ABS (c1.green-c2.green) < SMALL_EPSILON &&
	        		   ABS (c1.blue-c2.blue) < SMALL_EPSILON);
	      }
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetBool (!strcmp (ArgToString (ela),
	      	ArgToString (elb)));
	      break;
	    default:
	      return ReportError (behave, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_LT:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s < %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      stack[si].SetBool (ArgToFloat (ela) < ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetBool (ArgToInt32 (ela) < ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetBool (ArgToUInt32 (ela) < ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetBool (strcmp (ArgToString (ela),
	      	ArgToString (elb)) < 0);
	      break;
	    default:
	      return ReportError (behave, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_LE:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s <= %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      stack[si].SetBool (ArgToFloat (ela) <= ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetBool (ArgToInt32 (ela) <= ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetBool (ArgToUInt32 (ela) <= ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetBool (strcmp (ArgToString (ela),
	      	ArgToString (elb)) <= 0);
	      break;
	    default:
	      return ReportError (behave, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_GT:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s > %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      stack[si].SetBool (ArgToFloat (ela) > ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetBool (ArgToInt32 (ela) > ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetBool (ArgToUInt32 (ela) > ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetBool (strcmp (ArgToString (ela),
	      	ArgToString (elb)) > 0);
	      break;
	    default:
	      return ReportError (behave, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_GE:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
          DUMP_EXEC ((":%04d: %s >= %s\n", i-1, A2S (ela), A2S (elb)));
	  int t = GetCalculationType (ela, elb);
	  int si = stack.Push (celXmlArg ());
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      stack[si].SetBool (ArgToFloat (ela) >= ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetBool (ArgToInt32 (ela) >= ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetBool (ArgToUInt32 (ela) >= ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetBool (strcmp (ArgToString (ela),
	      	ArgToString (elb)) >= 0);
	      break;
	    default:
	      return ReportError (behave, "Can't compare these types!");
	  }
	}
	break;

      case CEL_OPERATION_DEREFVAR_STR:
        {
          DUMP_EXEC ((":%04d: derefvar_str %s\n", i-1, A2S (op.arg)));
	  iPcProperties* props = behave->GetProperties ();
	  CS_ASSERT (props != 0);
	  const char* varname = op.arg.arg.str.s;
	  int idx = props->GetPropertyIndex (varname);
	  if (idx == -1)
	    return ReportError (behave, "Can't find variable '%s'!", varname);
	  int si = stack.Push (celXmlArg ());
	  switch (props->GetPropertyType (idx))
	  {
	    case CEL_DATA_ULONG:
	      {
		long l = props->GetPropertyLong (idx);
		stack[si].SetUInt32 ((uint32)l);
	      }
	      break;
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
		stack[si].SetString (l, true);
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
		stack[si].SetEntity (l);
	      }
	      break;
	    default:
	      return ReportError (behave, "Property '%s' has wrong type!",
	      	varname);
	  }
	}
        break;
      case CEL_OPERATION_DEREFVAR:
        {
	  CHECK_STACK(1)
	  celXmlArg top = stack.Pop ();
          DUMP_EXEC ((":%04d: derefvar %s\n", i-1, A2S (top)));
	  iPcProperties* props = behave->GetProperties ();
	  CS_ASSERT (props != 0);
	  const char* varname = ArgToString (top);
	  int idx = props->GetPropertyIndex (varname);
	  if (idx == -1)
	    return ReportError (behave, "Can't find variable '%s'!", varname);
	  int si = stack.Push (celXmlArg ());
	  switch (props->GetPropertyType (idx))
	  {
	    case CEL_DATA_ULONG:
	      {
		long l = props->GetPropertyLong (idx);
		stack[si].SetUInt32 ((uint32)l);
	      }
	      break;
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
		stack[si].SetString (l, true);
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
		stack[si].SetEntity (l);
	      }
	      break;
	    default:
	      return ReportError (behave, "Property '%s' has wrong type!",
	      	varname);
	  }
	}
        break;
      case CEL_OPERATION_DEREFVARENT_STR:
        {
	  CHECK_STACK(1)
	  celXmlArg a_ent = stack.Pop ();
          DUMP_EXEC ((":%04d: derefvarent_str ent=%s var=%s\n", i-1,
		A2S (a_ent), A2S (op.arg)));
	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (behave, "Couldn't find entity '%s'!",
	    	EntityNameForError (a_ent));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (behave,
	    	"Entity '%s' doesn't have 'pcproperties'!",
	    	EntityNameForError (a_ent));
	  const char* varname = op.arg.arg.str.s;
	  int idx = props->GetPropertyIndex (varname);
	  if (idx == -1)
	    return ReportError (behave,
	    	"Entity doesn't have variable '%s'!", varname);
	  int si = stack.Push (celXmlArg ());
	  switch (props->GetPropertyType (idx))
	  {
	    case CEL_DATA_LONG:
	      {
		long l = props->GetPropertyLong (idx);
		stack[si].SetInt32 (l);
	      }
	      break;
	    case CEL_DATA_ULONG:
	      {
		long l = props->GetPropertyLong (idx);
		stack[si].SetUInt32 ((uint32)l);
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
		stack[si].SetString (l, true);
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
	    case CEL_DATA_ENTITY:
	      {
		iCelEntity* l = props->GetPropertyEntity (idx);
		stack[si].SetEntity (l);
	      }
	      break;
	    case CEL_DATA_PCLASS:
	      {
		iCelPropertyClass* l = props->GetPropertyPClass (idx);
		stack[si].SetPC (l);
	      }
	      break;
	    default:
	      return ReportError (behave, "Property '%s' has wrong type!",
	      	varname);
	  }
	}
        break;
      case CEL_OPERATION_DEREFVARENT:
        {
	  CHECK_STACK(2)
	  celXmlArg a_var = stack.Pop ();
	  celXmlArg a_ent = stack.Pop ();
          DUMP_EXEC ((":%04d: derefvarent ent=%s var=%s\n", i-1, A2S (a_ent),
	  	A2S (a_var)));
	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (behave, "Couldn't find entity '%s'!",
	    	EntityNameForError (a_ent));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (behave,
	    	"Entity '%s' doesn't have 'pcproperties'!",
	    	EntityNameForError (a_ent));
	  const char* varname = ArgToString (a_var);
	  int idx = props->GetPropertyIndex (varname);
	  if (idx == -1)
	    return ReportError (behave,
	    	"Entity doesn't have variable '%s'!", varname);
	  int si = stack.Push (celXmlArg ());
	  switch (props->GetPropertyType (idx))
	  {
	    case CEL_DATA_LONG:
	      {
		long l = props->GetPropertyLong (idx);
		stack[si].SetInt32 (l);
	      }
	      break;
	    case CEL_DATA_ULONG:
	      {
		long l = props->GetPropertyLong (idx);
		stack[si].SetUInt32 ((uint32)l);
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
		stack[si].SetString (l, true);
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
	    case CEL_DATA_ENTITY:
	      {
		iCelEntity* l = props->GetPropertyEntity (idx);
		stack[si].SetEntity (l);
	      }
	      break;
	    case CEL_DATA_PCLASS:
	      {
		iCelPropertyClass* l = props->GetPropertyPClass (idx);
		stack[si].SetPC (l);
	      }
	      break;
	    default:
	      return ReportError (behave, "Property '%s' has wrong type!",
	      	varname);
	  }
	}
        break;
      case CEL_OPERATION_ENTTHIS:
        {
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetEntity (entity);
	}
	break;

      case CEL_OPERATION_ENT:
        {
	  CHECK_STACK(1)
	  celXmlArg a_entname = stack.Pop ();
	  iCelEntity* ent = ArgToEntity (a_entname, pl);
	  if (!ent)
	    return ReportError (behave, "Can't find entity '%s'!",
	    	EntityNameForError (a_entname));
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetEntity (ent);
	}
	break;

      case CEL_OPERATION_CREATEPROPCLASS:
        {
	  CHECK_STACK(1)
	  celXmlArg top = stack.Pop ();
          DUMP_EXEC ((":%04d: createpropclass %s\n", i-1, A2S (top)));
	  const char* s = ArgToString (top);
	  iCelPropertyClass* pc = pl->CreatePropertyClass (entity, s);
	  if (!pc)
	    return ReportError (behave, "Couldn't create property class '%s'!",
	    	s);
	}
        break;

      case CEL_OPERATION_CREATEENTITY:
        {
	  CHECK_STACK(2)
	  celXmlArg abh = stack.Pop ();
	  celXmlArg aent = stack.Pop ();
	  DUMP_EXEC ((":%04d: createentity %s (behaviour=%s)\n", i-1,
	  	A2S (aent), A2S (abh)));
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
      case CEL_OPERATION_CALLI:
        {
	  celXmlScriptEventHandler* handler = op.arg.arg.h.h_true;
	  CS_ASSERT (handler != 0);
	  DUMP_EXEC ((":%04d: calli %s\n", i-1, handler->GetName ()));
	  handler->Execute (entity, behave, params);
	}
        break;
      case CEL_OPERATION_CALLENT:
        {
	  CHECK_STACK(2)
	  celXmlArg aevent = stack.Pop ();
	  celXmlArg aent = stack.Pop ();
	  DUMP_EXEC ((":%04d: callent ent=%s event=%s\n", i-1, A2S (aent),
	  	A2S (aevent)));
	  iCelEntity* ent = ArgToEntity (aent, pl);
	  if (!ent)
	    return ReportError (behave, "Couldn't find entity '%s'!",
	    	EntityNameForError (aent));
	  const char* eventname = ArgToString (aevent);
	  action_params->IncRef ();
	  ent->GetBehaviour ()->SendMessage (eventname, action_params);
	  action_params->DecRef ();
	}
        break;
      case CEL_OPERATION_CALL:
        {
	  CHECK_STACK(1)
	  celXmlArg aevent = stack.Pop ();
	  DUMP_EXEC ((":%04d: call event=%s\n", i-1, A2S (aevent)));
	  const char* eventname = ArgToString (aevent);
	  action_params->IncRef ();
	  behave->SendMessage (eventname, action_params);
	  action_params->DecRef ();
	}
        break;
      case CEL_OPERATION_ACTION:
        {
	  CHECK_STACK(2)
	  celXmlArg a_id = stack.Pop ();
	  celXmlArg a_pc = stack.Pop ();
          DUMP_EXEC ((":%04d: action pc=%s id=%d\n", i-1, A2S (a_pc),
	  	A2S (a_id)));
	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc) pc = default_pc;
	  csStringID id = ArgToID (a_id);
	  action_params->IncRef ();
	  pc->PerformAction (id, action_params);
	  action_params->DecRef ();
	}
        break;
      case CEL_OPERATION_DESTROYENTITY:
        {
	  CHECK_STACK(1)
	  celXmlArg aent = stack.Pop ();
	  DUMP_EXEC ((":%04d: destroyentity %s\n", i-1, A2S (aent)));
	  iCelEntity* ent = ArgToEntity (aent, pl);
	  if (!ent)
	    return ReportError (behave,
	    	  "Couldn't find entity with name '%s'!",
		  EntityNameForError (aent));
	  pl->RemoveEntity (ent);
	}
        break;
      case CEL_OPERATION_BB_MOVELAYER:
        {
	  CHECK_STACK(3)
	  celXmlArg a_y = stack.Pop ();
	  celXmlArg a_x = stack.Pop ();
	  celXmlArg a_layer = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_movelayer layer=%s x=%s y=%s\n", i-1, A2S (a_layer),
	  	A2S (a_x), A2S (a_y)));
	  // @@@ Efficiency?
	  csRef<iBillboardManager> bbmgr = CS_QUERY_REGISTRY (
	  	behave->GetObjectRegistry (), iBillboardManager);
	  const char* layername = ArgToString (a_layer);
	  iBillboardLayer* layer = bbmgr->FindBillboardLayer (layername);
	  if (!layer)
	    layer = bbmgr->CreateBillboardLayer (layername);
	  layer->SetOffset (ArgToInt32 (a_x), ArgToInt32 (a_y));
	}
	break;
      case CEL_OPERATION_BB_TESTCOLLIDE:
        {
	  CHECK_STACK(1)
	  celXmlArg apc = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_testcollide %s\n", i-1, A2S (apc)));
	  iCelPropertyClass* pc = ArgToPClass (apc);
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
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetBool (bbmgr->TestCollision (bb->GetBillboard (),
	  	other_bb->GetBillboard ()));
	}
	break;
      case CEL_OPERATION_FOR:
        {
	  CHECK_STACK(3)
	  celXmlArg a_end = stack.Pop ();
	  celXmlArg a_start = stack.Pop ();
	  celXmlArg a_var = stack.Pop ();
	  DUMP_EXEC ((":%04d: for var=%s start=%s end=%s\n", i-1, A2S (a_var),
	  	A2S (a_start), A2S (a_end)));
	  iPcProperties* props = behave->GetProperties ();
	  CS_ASSERT (props != 0);
	  const char* varname = ArgToString (a_var);
	  if (!varname)
	    return ReportError (behave, "Illegal variable name!");
	  int32 start = ArgToInt32 (a_start);
	  int32 end = ArgToInt32 (a_end);
	  int32 v;
	  celXmlScriptEventHandler* truebranch = op.arg.arg.h.h_true;
	  CS_ASSERT (truebranch != 0);
	  for (v = start ; v <= end ; v++)
	  {
	    props->SetProperty (varname, (long)v);
	    truebranch->Execute (entity, behave, params);
	  }
	}
	break;
      case CEL_OPERATION_FORI:
        {
	  CHECK_STACK(3)
	  celXmlArg a_end = stack.Pop ();
	  celXmlArg a_start = stack.Pop ();
	  celXmlArg a_var = stack.Pop ();
	  DUMP_EXEC ((":%04d: fori var=%s start=%s end=%s\n", i-1, A2S (a_var),
	  	A2S (a_start), A2S (a_end)));
	  iPcProperties* props = behave->GetProperties ();
	  CS_ASSERT (props != 0);
	  const char* varname = ArgToString (a_var);
	  if (!varname)
	    return ReportError (behave, "Illegal variable name!");
	  char* copy_varname = csStrNew (varname);
	  int32 start = ArgToInt32 (a_start);
	  int32 end = ArgToInt32 (a_end);
	  int32 v;
	  int endlocation = op.arg.arg.codelocation;
	  for (v = start ; v <= end ; v++)
	  {
	    props->SetProperty (copy_varname, (long)v);
	    Execute (entity, behave, params, i);
	  }
	  delete[] copy_varname;
	  i = endlocation;
	}
	break;
      case CEL_OPERATION_IFFUN:
        {
	  CHECK_STACK(3)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
	  celXmlArg eval = stack.Pop ();
	  DUMP_EXEC ((":%04d: iffun eval=%s true=%s false=%s\n", i-1,
	  	A2S (eval), A2S (ela), A2S (elb)));
	  bool rc = false;
	  if (!EvaluateTrue (eval, behave, rc)) return false;
	  if (rc)
	  {
	    stack.Push (ela);
	  }
	  else
	  {
	    stack.Push (elb);
	  }
	}
	break;
      case CEL_OPERATION_DROP:
        {
	  CHECK_STACK(1)
	  DUMP_EXEC ((":%04d: drop %s\n", i-1, A2S (stack[stack.Length ()-1])));
	  stack.Pop ();
	}
	break;
      case CEL_OPERATION_DUP:
        {
	  CHECK_STACK(1)
	  int si = stack.Length ()-1;
	  DUMP_EXEC ((":%04d: dup %s\n", i-1, A2S (stack[si])));
	  stack.Push (stack[si]);
	}
	break;
      case CEL_OPERATION_GOTO:
        {
	  DUMP_EXEC ((":%04d: goto :%d:\n", i-1, op.arg.arg.codelocation));
	  i = op.arg.arg.codelocation;
	}
	break;
      case CEL_OPERATION_IFGOTO:
        {
	  CHECK_STACK(1)
	  celXmlArg eval = stack.Pop ();
	  DUMP_EXEC ((":%04d: ifgoto %s :%d:\n", i-1, A2S (eval),
	  	op.arg.arg.codelocation));
	  bool rc = false;
	  if (!EvaluateTrue (eval, behave, rc)) return false;
	  if (!rc)
	  {
	    i = op.arg.arg.codelocation;
	  }
	}
	break;
      case CEL_OPERATION_IF:
        {
	  CHECK_STACK(1)
	  celXmlArg eval = stack.Pop ();
	  DUMP_EXEC ((":%04d: if %s\n", i-1, A2S (eval)));
	  bool rc = false;
	  if (!EvaluateTrue (eval, behave, rc)) return false;
	  if (rc)
	  {
	    celXmlScriptEventHandler* truebranch = op.arg.arg.h.h_true;
	    if (truebranch)
	      truebranch->Execute (entity, behave, params);
	  }
	  else
	  {
	    celXmlScriptEventHandler* falsebranch = op.arg.arg.h.h_false;
	    if (falsebranch)
	      falsebranch->Execute (entity, behave, params);
	  }
	}
	break;
      case CEL_OPERATION_PRINT:
        {
	  CHECK_STACK(1)
	  celXmlArg el = stack.Pop ();
	  DUMP_EXEC ((":%04d: print %s\n", i-1, A2S (el)));
	  const char* s = ArgToString (el);
	  printf ("%s\n", s);
	  fflush (stdout);
	}
	break;
      case CEL_OPERATION_ACTIONPARAMS:
        {
	  DUMP_EXEC ((":%04d: actionparams %s\n", i-1, A2S (op.arg)));
          action_params = csPtr<celGenericParameterBlock> (
		new celGenericParameterBlock (ArgToUInt32 (op.arg)));
        }
        break;
      case CEL_OPERATION_ACTIONPARAM:
        {
	  CHECK_STACK(2)
	  celXmlArg a_val = stack.Pop ();
	  celXmlArg a_id = stack.Pop ();
	  DUMP_EXEC ((":%04d: param idx=%d id=%s val=%s\n", i-1,
	  	op.arg.arg.ui, A2S (a_id), A2S (a_val)));
	  CS_ASSERT (action_params != 0);
	  action_params->SetParameterDef (op.arg.arg.ui, ArgToID (a_id), "");
	  celData& data = action_params->GetParameter ((int)op.arg.arg.ui);
	  switch (a_val.type)
	  {
	    case CEL_DATA_ENTITY: data.Set (a_val.arg.entity); break;
	    case CEL_DATA_PCLASS: data.Set (a_val.arg.pc); break;
	    case CEL_DATA_LONG: data.Set (a_val.arg.i); break;
	    case CEL_DATA_ULONG: data.Set (a_val.arg.ui); break;
	    case CEL_DATA_BOOL: data.SetBool (a_val.arg.b); break;
	    case CEL_DATA_FLOAT: data.Set (a_val.arg.f); break;
	    case CEL_DATA_STRING: data.Set (a_val.arg.str.s); break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 v;
		v.x = a_val.arg.vec.x;
		v.y = a_val.arg.vec.y;
	        data.Set (v);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 v;
		v.x = a_val.arg.vec.x;
		v.y = a_val.arg.vec.y;
		v.z = a_val.arg.vec.z;
	        data.Set (v);
	      }
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor v;
		v.red = a_val.arg.col.red;
		v.green = a_val.arg.col.green;
		v.blue = a_val.arg.col.blue;
	        data.Set (v);
	      }
	      break;
	    default:
	      return ReportError (behave, "Bad type of value!");
	  }
        }
        break;
      case CEL_OPERATION_VARENT_STR:
        {
	  CHECK_STACK(2)
	  celXmlArg val = stack.Pop ();
	  celXmlArg a_ent = stack.Pop ();
	  DUMP_EXEC ((":%04d: varent_str ent=%s var=%s value=%s\n", i-1,
	  	A2S (a_ent), A2S (op.arg), A2S (val)));

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (behave, "Couldn't find entity '%s'!",
	    	  EntityNameForError (a_ent));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (behave,
	    	"Entity '%s' doesn't have 'pcproperties'!",
			EntityNameForError (a_ent));

	  const char* varname = op.arg.arg.str.s;
	  if (!varname)
	    return ReportError (behave, "Illegal variable name!");
	  switch (val.type)
	  {
	    case CEL_DATA_BOOL:
	      props->SetProperty (varname, val.arg.b);
	      break;
	    case CEL_DATA_FLOAT:
	      props->SetProperty (varname, val.arg.f);
	      break;
	    case CEL_DATA_STRING:
	      props->SetProperty (varname, val.arg.str.s);
	      break;
	    case CEL_DATA_ULONG:
	      props->SetProperty (varname, (long)val.arg.ui);
	      break;
	    case CEL_DATA_LONG:
	      props->SetProperty (varname, (long)val.arg.i);
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor col;
		col.red = val.arg.col.red;
		col.green = val.arg.col.green;
		col.blue = val.arg.col.blue;
	        props->SetProperty (varname, col);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
		vec.z = val.arg.vec.z;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_DATA_ENTITY:
	      props->SetProperty (varname, val.arg.entity);
	      break;
	    case CEL_DATA_PCLASS:
	      props->SetProperty (varname, val.arg.pc);
	      break;
	    default:
	      return ReportError (behave, "Bad type of value!");
	  }
	}
	break;
      case CEL_OPERATION_VARENT:
        {
	  CHECK_STACK(3)
	  celXmlArg val = stack.Pop ();
	  celXmlArg var = stack.Pop ();
	  celXmlArg a_ent = stack.Pop ();
	  DUMP_EXEC ((":%04d: varent ent=%s var=%s value=%s\n", i-1, A2S (a_ent),
	  	A2S (var), A2S (val)));

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (behave, "Couldn't find entity '%s'!",
	    	  EntityNameForError (a_ent));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (behave,
	    	"Entity '%s' doesn't have 'pcproperties'!",
			EntityNameForError (a_ent));

	  const char* varname = ArgToString (var);
	  if (!varname)
	    return ReportError (behave, "Illegal variable name!");
	  switch (val.type)
	  {
	    case CEL_DATA_BOOL:
	      props->SetProperty (varname, val.arg.b);
	      break;
	    case CEL_DATA_FLOAT:
	      props->SetProperty (varname, val.arg.f);
	      break;
	    case CEL_DATA_STRING:
	      props->SetProperty (varname, val.arg.str.s);
	      break;
	    case CEL_DATA_ULONG:
	      props->SetProperty (varname, (long)val.arg.ui);
	      break;
	    case CEL_DATA_LONG:
	      props->SetProperty (varname, (long)val.arg.i);
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor col;
		col.red = val.arg.col.red;
		col.green = val.arg.col.green;
		col.blue = val.arg.col.blue;
	        props->SetProperty (varname, col);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
		vec.z = val.arg.vec.z;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_DATA_ENTITY:
	      props->SetProperty (varname, val.arg.entity);
	      break;
	    case CEL_DATA_PCLASS:
	      props->SetProperty (varname, val.arg.pc);
	      break;
	    default:
	      return ReportError (behave, "Bad type of value!");
	  }
	}
	break;
      case CEL_OPERATION_VAR_STR:
        {
	  CHECK_STACK(1)
	  celXmlArg val = stack.Pop ();
	  DUMP_EXEC ((":%04d: var_str var=%s value=%s\n", i-1, A2S (op.arg),
	  	A2S (val)));

	  iPcProperties* props = behave->GetProperties ();
	  CS_ASSERT (props != 0);
	  const char* varname = op.arg.arg.str.s;
	  if (!varname)
	    return ReportError (behave, "Illegal variable name!");
	  switch (val.type)
	  {
	    case CEL_DATA_BOOL:
	      props->SetProperty (varname, val.arg.b);
	      break;
	    case CEL_DATA_FLOAT:
	      props->SetProperty (varname, val.arg.f);
	      break;
	    case CEL_DATA_STRING:
	      props->SetProperty (varname, val.arg.str.s);
	      break;
	    case CEL_DATA_ULONG:
	      props->SetProperty (varname, (long)val.arg.ui);
	      break;
	    case CEL_DATA_LONG:
	      props->SetProperty (varname, (long)val.arg.i);
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor col;
		col.red = val.arg.col.red;
		col.green = val.arg.col.green;
		col.blue = val.arg.col.blue;
	        props->SetProperty (varname, col);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
		vec.z = val.arg.vec.z;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_DATA_ENTITY:
	      props->SetProperty (varname, val.arg.entity);
	      break;
	    case CEL_DATA_PCLASS:
	      props->SetProperty (varname, val.arg.pc);
	      break;
	    default:
	      return ReportError (behave, "Bad type of value!");
	  }
	}
	break;
      case CEL_OPERATION_VAR:
        {
	  CHECK_STACK(2)
	  celXmlArg val = stack.Pop ();
	  celXmlArg var = stack.Pop ();
	  DUMP_EXEC ((":%04d: var var=%s value=%s\n", i-1, A2S (var),
	  	A2S (val)));

	  iPcProperties* props = behave->GetProperties ();
	  CS_ASSERT (props != 0);
	  const char* varname = ArgToString (var);
	  if (!varname)
	    return ReportError (behave, "Illegal variable name!");
	  switch (val.type)
	  {
	    case CEL_DATA_BOOL:
	      props->SetProperty (varname, val.arg.b);
	      break;
	    case CEL_DATA_FLOAT:
	      props->SetProperty (varname, val.arg.f);
	      break;
	    case CEL_DATA_STRING:
	      props->SetProperty (varname, val.arg.str.s);
	      break;
	    case CEL_DATA_ULONG:
	      props->SetProperty (varname, (long)val.arg.ui);
	      break;
	    case CEL_DATA_LONG:
	      props->SetProperty (varname, (long)val.arg.i);
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor col;
		col.red = val.arg.col.red;
		col.green = val.arg.col.green;
		col.blue = val.arg.col.blue;
	        props->SetProperty (varname, col);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 vec;
		vec.x = val.arg.vec.x;
		vec.y = val.arg.vec.y;
		vec.z = val.arg.vec.z;
	        props->SetProperty (varname, vec);
	      }
	      break;
	    case CEL_DATA_ENTITY:
	      props->SetProperty (varname, val.arg.entity);
	      break;
	    case CEL_DATA_PCLASS:
	      props->SetProperty (varname, val.arg.pc);
	      break;
	    default:
	      return ReportError (behave, "Bad type of value!");
	  }
	}
	break;
      case CEL_OPERATION_GETPROPERTY1:
        {
	  CHECK_STACK(1)
	  celXmlArg a_id = stack.Pop ();
	  DUMP_EXEC ((":%04d: getproperty1 id=%s\n", i-1, A2S (a_id)));
	  int si = stack.Push (celXmlArg ());

	  iCelPropertyClass* pc = default_pc;
	  if (!pc)
	    return ReportError (behave, "Default property class is 0!");
	  csStringID id = ArgToID (a_id);
	  celDataType t = pc->GetPropertyOrActionType (id);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (pc->GetPropertyBool (id));
	      break;
	    case CEL_DATA_FLOAT:
	      stack[si].SetFloat (pc->GetPropertyFloat (id));
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetString (pc->GetPropertyString (id), true);
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (pc->GetPropertyLong (id));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 ((uint32)pc->GetPropertyLong (id));
	      break;
	    case CEL_DATA_PCLASS:
	      stack[si].SetPC (pc->GetPropertyPClass (id));
	      break;
	    case CEL_DATA_ENTITY:
	      stack[si].SetEntity (pc->GetPropertyEntity (id));
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor col;
		pc->GetPropertyColor (id, col);
	        stack[si].SetColor (col);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 v;
		pc->GetPropertyVector (id, v);
	        stack[si].SetVector (v);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 v;
		pc->GetPropertyVector (id, v);
	        stack[si].SetVector (v);
	      }
	      break;
	    default:
	      return ReportError (behave, "Type not supported!");
	  }
	}
	break;
      case CEL_OPERATION_GETPROPERTY:
        {
	  CHECK_STACK(2)
	  celXmlArg a_id = stack.Pop ();
	  celXmlArg a_pc = stack.Pop ();
	  DUMP_EXEC ((":%04d: getproperty pc=%s id=%s\n", i-1, A2S (a_pc),
	  	A2S (a_id)));
	  int si = stack.Push (celXmlArg ());

	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc)
	    return ReportError (behave, "Property class is 0!");
	  csStringID id = ArgToID (a_id);
	  celDataType t = pc->GetPropertyOrActionType (id);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      stack[si].SetBool (pc->GetPropertyBool (id));
	      break;
	    case CEL_DATA_FLOAT:
	      stack[si].SetFloat (pc->GetPropertyFloat (id));
	      break;
	    case CEL_DATA_STRING:
	      stack[si].SetString (pc->GetPropertyString (id), true);
	      break;
	    case CEL_DATA_LONG:
	      stack[si].SetInt32 (pc->GetPropertyLong (id));
	      break;
	    case CEL_DATA_ULONG:
	      stack[si].SetUInt32 ((uint32)pc->GetPropertyLong (id));
	      break;
	    case CEL_DATA_PCLASS:
	      stack[si].SetPC (pc->GetPropertyPClass (id));
	      break;
	    case CEL_DATA_ENTITY:
	      stack[si].SetEntity (pc->GetPropertyEntity (id));
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor col;
		pc->GetPropertyColor (id, col);
	        stack[si].SetColor (col);
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 v;
		pc->GetPropertyVector (id, v);
	        stack[si].SetVector (v);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 v;
		pc->GetPropertyVector (id, v);
	        stack[si].SetVector (v);
	      }
	      break;
	    default:
	      return ReportError (behave, "Type not supported!");
	  }
	}
	break;
      case CEL_OPERATION_INVENTORY_ADD:
        {
	  CHECK_STACK(1)
	  celXmlArg a_ent = stack.Pop ();
	  DUMP_EXEC ((":%04d: inventory_add ent=%s\n", i-1, A2S (a_ent)));

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (behave, "Couldn't find entity '%s'!",
	    	  EntityNameForError (a_ent));
	  if (!default_inv)
	    return ReportError (behave, "Default inventory isn't set!");
	  default_inv->AddEntity (other_ent);
	}
	break;
      case CEL_OPERATION_INVENTORY_REM:
        {
	  CHECK_STACK(1)
	  celXmlArg a_ent = stack.Pop ();
	  DUMP_EXEC ((":%04d: inventory_rem ent=%s\n", i-1, A2S (a_ent)));

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (behave, "Couldn't find entity '%s'!",
	    	  EntityNameForError (a_ent));
	  if (!default_inv)
	    return ReportError (behave, "Default inventory isn't set!");
	  default_inv->RemoveEntity (other_ent);
	}
	break;
      case CEL_OPERATION_INVENTORY_COUNT:
        {
	  DUMP_EXEC ((":%04d: inventory_count ()\n", i-1));
	  if (!default_inv)
	    return ReportError (behave, "Default inventory isn't set!");
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetInt32 (default_inv->GetEntityCount ());
	}
	break;
      case CEL_OPERATION_INVENTORY_GET:
        {
	  CHECK_STACK(1)
	  celXmlArg a_idx = stack.Pop ();
	  DUMP_EXEC ((":%04d: inventory_get idx=%s\n", i-1, A2S (a_idx)));
	  if (!default_inv)
	    return ReportError (behave, "Default inventory isn't set!");
	  int si = stack.Push (celXmlArg ());
	  stack[si].SetEntity (default_inv->GetEntity (ArgToInt32 (a_idx)));
	}
	break;
      case CEL_OPERATION_DEFAULTINV:
        {
	  CHECK_STACK(1)
	  celXmlArg a_pc = stack.Pop ();
	  DUMP_EXEC ((":%04d: defaultinv pc=%s\n", i-1, A2S (a_pc)));
	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc)
	    return ReportError (behave, "Property class is 0!");
	  csRef<iPcInventory> inv = SCF_QUERY_INTERFACE (pc, iPcInventory);
	  if (!inv)
	    return ReportError (behave, "Property class is not an inventory!");
	  default_inv = inv;
	}
	break;
      case CEL_OPERATION_DEFAULTPC:
        {
	  CHECK_STACK(1)
	  celXmlArg a_pc = stack.Pop ();
	  DUMP_EXEC ((":%04d: defaultpc pc=%s\n", i-1, A2S (a_pc)));
	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc)
	    return ReportError (behave, "Property class is 0!");
	  default_pc = pc;
	}
	break;
      case CEL_OPERATION_PROPERTY:
        {
	  CHECK_STACK(3)
	  celXmlArg a_val = stack.Pop ();
	  celXmlArg a_id = stack.Pop ();
	  celXmlArg a_pc = stack.Pop ();
          DUMP_EXEC ((":%04d: property pc=%s id=%s val=%s\n", i-1, A2S (a_pc),
	  	A2S (a_id), A2S (a_val)));
	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc) pc = default_pc;
	  csStringID id = ArgToID (a_id);
	  celDataType t = pc->GetPropertyOrActionType (id);
	  switch (t)
	  {
	    case CEL_DATA_LONG:
	      pc->SetProperty (id, (long)ArgToInt32 (a_val));
	      break;
	    case CEL_DATA_ULONG:
	      pc->SetProperty (id, (long)ArgToUInt32 (a_val));
	      break;
	    case CEL_DATA_FLOAT:
	      pc->SetProperty (id, ArgToFloat (a_val));
	      break;
	    case CEL_DATA_BOOL:
	      pc->SetProperty (id, ArgToBool (a_val));
	      break;
	    case CEL_DATA_STRING:
	      pc->SetProperty (id, ArgToString (a_val));
	      break;
	    case CEL_DATA_VECTOR2:
	      pc->SetProperty (id, ArgToVector2 (a_val));
	      break;
	    case CEL_DATA_VECTOR3:
	      pc->SetProperty (id, ArgToVector3 (a_val));
	      break;
	    case CEL_DATA_COLOR:
	      pc->SetProperty (id, ArgToColor (a_val));
	      break;
	    case CEL_DATA_PCLASS:
	      pc->SetProperty (id, ArgToPClass (a_val));
	      break;
	    case CEL_DATA_ENTITY:
	      pc->SetProperty (id, a_val.arg.entity);
	      break;
	    default:
	      return ReportError (behave,
	      	"Bad type for setting property value!");
	  }
	}
        break;
      case CEL_OPERATION_SOUND:
        {
	  CHECK_STACK(2)
	  celXmlArg a_loop = stack.Pop ();
	  celXmlArg a_name = stack.Pop ();
	  DUMP_EXEC ((":%04d: sound name=%s loop=%s\n", i-1, A2S (a_name),
	  	A2S (a_loop)));
	  csRef<iSoundRender> snd = CS_QUERY_REGISTRY (
	  	behave->GetObjectRegistry (), iSoundRender);
	  if (!snd) break;
	  csRef<iEngine> engine = CS_QUERY_REGISTRY (
	  	behave->GetObjectRegistry (), iEngine);
	  if (!engine) break;
	  csRef<iSoundWrapper> w = CS_GET_NAMED_CHILD_OBJECT (
		engine->QueryObject (), iSoundWrapper, ArgToString (a_name));
	  if (w) sound_source = w->GetSound ()->Play (ArgToBool (a_loop));
	}
        break;
    }
  }
}

void celXmlScriptEventHandler::AddOperation (int op)
{
  operations.Push (celXmlOperation ());
  celXmlOperation& top_op = operations[operations.Length ()-1];
  top_op.op = op;
}

celXmlArg& celXmlScriptEventHandler::GetArgument ()
{
  celXmlOperation& op = operations[operations.Length ()-1];
  return op.arg;
}

celXmlArg& celXmlScriptEventHandler::GetArgument (int idx)
{
  celXmlOperation& op = operations[idx];
  return op.arg;
}

int celXmlScriptEventHandler::AddLocalVariable ()
{
  return local_vars.Push (celXmlArg ());
}

//---------------------------------------------------------------------------

celXmlScript::celXmlScript (iCelPlLayer* pl)
{
  name = 0;
  celXmlScript::pl = pl;
  superscript = 0;
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

