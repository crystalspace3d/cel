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
#include "cstool/initapp.h"
#include "cstool/enginetools.h"
#include "csgeom/math3d.h"
#include "csutil/objreg.h"
#include "csutil/csstring.h"
#include "csutil/databuf.h"
#include "csutil/scanstr.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "iutil/csinput.h"
#include "iutil/cfgmgr.h"
#include "ivaria/reporter.h"
#include "ivideo/graph3d.h"
#include "iengine/camera.h"
#include "iengine/sector.h"
#include "iengine/mesh.h"

#include "csgeom/vector3.h"
#include "isndsys/ss_source.h"
#include "isndsys/ss_manager.h"
#include "isndsys/ss_renderer.h"
#include "iengine/engine.h"
#include "iutil/plugin.h"

#include "plugins/behaviourlayer/xml/xmlscript.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "plugins/behaviourlayer/xml/blxml.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/pl.h"
#include "behaviourlayer/bl.h"
#include "propclass/prop.h"
#include "propclass/rules.h"
#include "propclass/billboard.h"
#include "propclass/inv.h"
#include "propclass/camera.h"
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
#define DUMP_EXEC(x) dump_exec x
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
    case CEL_DATA_IBASE: arg.ref = other.arg.ref; break;
    case CEL_DATA_ID: arg.id = other.arg.id; break;
    case CEL_DATA_EVENTHANDLER: arg.h = other.arg.h; break;
    case CEL_DATA_CODELOCATION:
      arg.codelocation = other.arg.codelocation;
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

celXmlScriptEventHandler::celXmlScriptEventHandler (iCelPlLayer* pl,
	celXmlScript* script)
{
  name = 0;
  celXmlScriptEventHandler::pl = pl;
  celXmlScriptEventHandler::script = script;
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

static const char* ArgToString (const char* prefix, const char* prefix2,
		const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_%d", prefix, prefix2, a.arg.i);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ULONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_%u", prefix, prefix2, a.arg.ui);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_FLOAT:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_%g", prefix, prefix2, a.arg.f);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_BOOL:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_%s", prefix, prefix2, a.arg.b ? "true" : "false");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_STRING:
       {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_%s", prefix, prefix2, a.arg.str.s);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR2:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_[%g,%g]", prefix, prefix2, a.arg.vec.x, a.arg.vec.y);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR3:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_[%g,%g,%g]", prefix, prefix2,
			a.arg.vec.x, a.arg.vec.y, a.arg.vec.z);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_PCLASS:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_pc(%p)", prefix, prefix2, a.arg.pc);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ENTITY:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_ent(%s)", prefix, prefix2,
			a.arg.entity ? a.arg.entity->GetName () : "<null>");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_IBASE:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_ibase(%p)", prefix, prefix2,
			a.arg.ref);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ID:
      {
        csString* str = GetUnusedString ();
        str->Format("%s_%s_id(%lu)", prefix, prefix2, (unsigned long)a.arg.id);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_COLOR:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s_rgb(%g,%g,%g)", prefix, prefix2,
			a.arg.col.red, a.arg.col.green,
      	  a.arg.col.blue);
	used_strings.Push (str);
        return *str;
      }
    default:
      return 0;
  }
}

static const char* ArgToString (const char* prefix, const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%d", prefix, a.arg.i);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ULONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%u", prefix, a.arg.ui);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_FLOAT:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%g", prefix, a.arg.f);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_BOOL:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s", prefix, a.arg.b ? "true" : "false");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_STRING:
       {
        csString* str = GetUnusedString ();
        str->Format ("%s_%s", prefix, a.arg.str.s);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR2:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_[%g,%g]", prefix, a.arg.vec.x, a.arg.vec.y);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR3:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_[%g,%g,%g]", prefix, a.arg.vec.x, a.arg.vec.y, a.arg.vec.z);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_PCLASS:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_pc(%p)", prefix, a.arg.pc);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ENTITY:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_ent(%s)", prefix,
			a.arg.entity ? a.arg.entity->GetName () : "<null>");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_IBASE:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_ibase(%p)", prefix, a.arg.ref);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ID:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_id(%lu)", prefix, (unsigned long)a.arg.id);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_COLOR:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s_rgb(%g,%g,%g)", prefix, a.arg.col.red, a.arg.col.green,
      	  a.arg.col.blue);
	used_strings.Push (str);
        return *str;
      }
    default:
      return 0;
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
        str->Format ("pc(%p)", a.arg.pc);
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
    case CEL_DATA_IBASE:
      {
        csString* str = GetUnusedString ();
        str->Format ("ibase(%p)", a.arg.ref);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ID:
      {
        csString* str = GetUnusedString ();
        str->Format ("id(%lu)", (unsigned long)a.arg.id);
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

static const char* ArgToStringTpl (const celXmlArg& a)
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
        str->Format ("%g,%g", a.arg.vec.x, a.arg.vec.y);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR3:
      {
        csString* str = GetUnusedString ();
        str->Format ("%g,%g,%g", a.arg.vec.x, a.arg.vec.y, a.arg.vec.z);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ENTITY:
      {
        csString* str = GetUnusedString ();
        str->Format ("%s", a.arg.entity ? a.arg.entity->GetName () : "");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_COLOR:
      {
        csString* str = GetUnusedString ();
        str->Format ("%g,%g,%g", a.arg.col.red, a.arg.col.green,
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
        str->Format ("{pc:%p}", a.arg.pc);
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
    case CEL_DATA_IBASE:
      {
        csString* str = GetUnusedString ();
        str->Format ("{ibase:%p}", a.arg.ref);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ID:
      {
        csString* str = GetUnusedString ();
        str->Format ("{id:%lu}", (unsigned long)a.arg.id);
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

static iBase* ArgToIBase (const celXmlArg& a)
{
  if (a.type == CEL_DATA_IBASE) return a.arg.ref;
  else return 0;
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
  if (a.type == CEL_DATA_PCLASS) return a.arg.pc;
  else return 0;
}

static csStringID ArgToID (const celXmlArg& a)
{
  if (a.type == CEL_DATA_ID) return a.arg.id;
  else return csInvalidStringID;
}

static csColor ArgToColor (const celXmlArg& a)
{
  if (a.type == CEL_DATA_COLOR)
    return csColor (a.arg.col.red, a.arg.col.green,
    	a.arg.col.blue);
  else
    return csColor (0, 0, 0);
}

static csVector3 ArgToVector3 (const celXmlArg& a)
{
  if (a.type == CEL_DATA_VECTOR3)
    return csVector3 (a.arg.vec.x, a.arg.vec.y,
    	a.arg.vec.z);
  else
    return csVector3 (0);
}

static csVector2 ArgToVector2 (const celXmlArg& a)
{
  if (a.type == CEL_DATA_VECTOR2)
    return csVector2 (a.arg.vec.x, a.arg.vec.y);
  else
    return csVector2 (0, 0);
}

static bool ArgToBool (const celXmlArg& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG: return (a.arg.i)? true : false;
    case CEL_DATA_ULONG: return (a.arg.ui)? true : false;
    case CEL_DATA_FLOAT: return bool (ABS (a.arg.f) < SMALL_EPSILON);
    case CEL_DATA_BOOL: return a.arg.b;
    case CEL_DATA_ENTITY: return a.arg.entity != 0;
    case CEL_DATA_IBASE: return a.arg.ref != 0;
    case CEL_DATA_PCLASS: return a.arg.pc != 0;
    case CEL_DATA_STRING:
      return a.arg.str.s ? (*a.arg.str.s != 0) : false;
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
    case CEL_DATA_IBASE: return true;
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

bool celXmlScriptEventHandler::ReportError (celBlXml* cbl,
	const char* msg, ...)
{
  //DumpVariables (behave);
  DumpCallStack (cbl);
  va_list arg;
  va_start (arg, msg);
  csReportV (cbl->GetObjectRegistry (), CS_REPORTER_SEVERITY_ERROR,
  	"cel.behaviourlayer.xml", msg, arg);
  va_end (arg);
  return false;
}

bool celXmlScriptEventHandler::EvaluateTrue (
    const celXmlArg& eval, celBlXml* cbl, bool& rc)
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
    case CEL_DATA_IBASE:
      rc = eval.arg.ref != 0;
      break;
    case CEL_DATA_PCLASS:
      rc = eval.arg.pc != 0;
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
      return ReportError (cbl, "Can't test on this type!");
  }
  return true;
}

static const char* D2S (const celData& a)
{
  switch (a.type)
  {
    case CEL_DATA_LONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("{int32:%d}", a.value.l);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ULONG:
      {
        csString* str = GetUnusedString ();
        str->Format ("{uint32:%u}", a.value.ul);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_FLOAT:
      {
        csString* str = GetUnusedString ();
        str->Format ("{float:%g}", a.value.f);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_BOOL:
      {
        csString* str = GetUnusedString ();
        str->Format ("{bool:%s}", a.value.bo ? "true" : "false");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_STRING:
      {
        csString* str = GetUnusedString ();
        str->Format ("{str:%s}", (const char*)(*a.value.s));
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR2:
      {
        csString* str = GetUnusedString ();
        str->Format ("{vec:[%g,%g]}", a.value.v.x, a.value.v.y);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_VECTOR3:
      {
        csString* str = GetUnusedString ();
        str->Format ("{vec:[%g,%g,%g]}", a.value.v.x, a.value.v.y, a.value.v.z);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_COLOR:
      {
        csString* str = GetUnusedString ();
        str->Format ("{rgb:(%g,%g,%g)}", a.value.col.red, a.value.col.green,
      	  a.value.col.blue);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_PCLASS:
      {
        csString* str = GetUnusedString ();
        str->Format ("{pc:%p}", a.value.pc);
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_ENTITY:
      {
        csString* str = GetUnusedString ();
        str->Format ("{ent:%s}", a.value.ent ? a.value.ent->GetName ()
		: "<null>");
	used_strings.Push (str);
        return *str;
      }
    case CEL_DATA_IBASE:
      {
        csString* str = GetUnusedString ();
        str->Format ("{ibase:%p}", a.value.ibase);
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

static bool prop2celXmlArg (iPcProperties* props, size_t idx, celXmlArg& out)
{
  switch (props->GetPropertyType (idx))
  {
    case CEL_DATA_ULONG:
      {
	long l = props->GetPropertyLong (idx);
	out.SetUInt32 ((uint32)l);
      }
      break;
    case CEL_DATA_LONG:
      {
	long l = props->GetPropertyLong (idx);
	out.SetInt32 (l);
      }
      break;
    case CEL_DATA_FLOAT:
      {
	float l = props->GetPropertyFloat (idx);
	out.SetFloat (l);
      }
      break;
    case CEL_DATA_BOOL:
      {
	bool l = props->GetPropertyBool (idx);
	out.Set (l);
      }
      break;
    case CEL_DATA_STRING:
      {
	const char* l = props->GetPropertyString (idx);
	out.SetString (l, true);
      }
      break;
    case CEL_DATA_VECTOR2:
      {
	csVector2 v;
	props->GetPropertyVector (idx, v);
	out.SetVector (v);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
	csVector3 v;
	props->GetPropertyVector (idx, v);
	out.SetVector (v);
      }
      break;
    case CEL_DATA_COLOR:
      {
	csColor v;
	props->GetPropertyColor (idx, v);
	out.SetColor (v);
      }
      break;
    case CEL_DATA_PCLASS:
      {
	iCelPropertyClass* l = props->GetPropertyPClass (idx);
	out.SetPC (l);
      }
      break;
    case CEL_DATA_ENTITY:
      {
	iCelEntity* l = props->GetPropertyEntity (idx);
	out.SetEntity (l);
      }
      break;
    case CEL_DATA_IBASE:
      {
	iBase* l = props->GetPropertyIBase (idx);
	out.SetIBase (l);
      }
      break;
    default:
      return false;
  }
  return true;
}

static bool celData2celXmlArg (const celData& in, celXmlArg& out)
{
  switch (in.type)
  {
    case CEL_DATA_BOOL: out.Set ((in.value.b? true : false)); break;
    case CEL_DATA_FLOAT: out.SetFloat (in.value.f); break;
    case CEL_DATA_STRING: out.SetString (in.value.s->GetData (), true); break;
    case CEL_DATA_LONG: out.SetInt32 (in.value.l); break;
    case CEL_DATA_ULONG: out.SetUInt32 (in.value.ul); break;
    case CEL_DATA_PCLASS: out.SetPC (in.value.pc); break;
    case CEL_DATA_ENTITY: out.SetEntity (in.value.ent); break;
    case CEL_DATA_IBASE: out.SetIBase (in.value.ibase); break;
    case CEL_DATA_COLOR:
      {
        csColor col (in.value.col.red, in.value.col.green,
	  in.value.col.blue);
        out.SetColor (col);
      }
      break;
    case CEL_DATA_VECTOR2:
      {
	csVector2 v (in.value.v.x, in.value.v.y);
	out.SetVector (v);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
	csVector3 v (in.value.v.x, in.value.v.y, in.value.v.z);
	out.SetVector (v);
      }
      break;
    default:
      return false;
  }
  return true;
}

static bool pcProp2celXmlArg (iCelPropertyClass* pc, csStringID id,
    celXmlArg& out)
{
  celDataType t = pc->GetPropertyOrActionType (id);
  switch (t)
  {
    case CEL_DATA_BOOL:
      out.Set (pc->GetPropertyBool (id));
      break;
    case CEL_DATA_FLOAT:
      out.SetFloat (pc->GetPropertyFloat (id));
      break;
    case CEL_DATA_STRING:
      out.SetString (pc->GetPropertyString (id), true);
      break;
    case CEL_DATA_LONG:
      out.SetInt32 (pc->GetPropertyLong (id));
      break;
    case CEL_DATA_ULONG:
      out.SetUInt32 ((uint32)pc->GetPropertyLong (id));
      break;
    case CEL_DATA_PCLASS:
      out.SetPC (pc->GetPropertyPClass (id));
      break;
    case CEL_DATA_ENTITY:
      out.SetEntity (pc->GetPropertyEntity (id));
      break;
    case CEL_DATA_IBASE:
      out.SetIBase (pc->GetPropertyIBase (id));
      break;
    case CEL_DATA_COLOR:
      {
	csColor col;
	pc->GetPropertyColor (id, col);
	out.SetColor (col);
      }
      break;
    case CEL_DATA_VECTOR2:
      {
	csVector2 v;
	pc->GetPropertyVector (id, v);
	out.SetVector (v);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
	csVector3 v;
	pc->GetPropertyVector (id, v);
	out.SetVector (v);
      }
      break;
    default:
      return false;
  }
  return true;
}

static bool celXmlArg2prop (const celXmlArg& val, iPcProperties* props,
    const char* varname)
{
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
    case CEL_DATA_IBASE:
      props->SetProperty (varname, val.arg.ref);
      break;
    case CEL_DATA_PCLASS:
      props->SetProperty (varname, val.arg.pc);
      break;
    default:
      return false;
  }
  return true;
}

static bool celData2prop (const celData& val, iPcProperties* props,
    const char* varname)
{
  switch (val.type)
  {
    case CEL_DATA_BOOL:
      props->SetProperty (varname, val.value.bo);
      break;
    case CEL_DATA_FLOAT:
      props->SetProperty (varname, val.value.f);
      break;
    case CEL_DATA_STRING:
      props->SetProperty (varname, (const char*)(*val.value.s));
      break;
    case CEL_DATA_ULONG:
      props->SetProperty (varname, (long)val.value.ul);
      break;
    case CEL_DATA_LONG:
      props->SetProperty (varname, (long)val.value.l);
      break;
    case CEL_DATA_COLOR:
      {
	csColor col;
	col.red = val.value.col.red;
	col.green = val.value.col.green;
	col.blue = val.value.col.blue;
	props->SetProperty (varname, col);
      }
      break;
    case CEL_DATA_VECTOR2:
      {
	csVector2 vec;
	vec.x = val.value.v.x;
	vec.y = val.value.v.y;
	props->SetProperty (varname, vec);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
	csVector3 vec;
	vec.x = val.value.v.x;
	vec.y = val.value.v.y;
	vec.z = val.value.v.z;
	props->SetProperty (varname, vec);
      }
      break;
    case CEL_DATA_ENTITY:
      props->SetProperty (varname, val.value.ent);
      break;
    case CEL_DATA_IBASE:
      props->SetProperty (varname, val.value.ibase);
      break;
    case CEL_DATA_PCLASS:
      props->SetProperty (varname, val.value.pc);
      break;
    default:
      return false;
  }
  return true;
}

void celXmlScriptEventHandler::DumpCallStack (celBlXml* cbl)
{
  size_t i;
  printf ("### Callstack ###\n");
  for (i = 0 ; i < cbl->call_stack.Length () ; i++)
  {
    printf ("%lu %s (entity=%s)\n", (unsigned long)i, cbl->call_stack[i],
	cbl->call_stack_entity[i]->GetName ());
    if (cbl->call_stack_params[i])
    {
      size_t j;
      iCelParameterBlock* p = cbl->call_stack_params[i];
      for (j = 0 ; j < p->GetParameterCount () ; j++)
      {
        csStringID id;
        celDataType t;
	const char* parm;
	parm = p->GetParameter (j, id, t);
	const char* idstr = pl->FetchString (id);
	const celData* param = p->GetParameter (id);
        printf ("  par:%lu name=%s id=%s val=%s\n", (unsigned long)j,
	    parm, idstr, D2S (*param));
      }
    }
  }
  fflush (stdout);
}

void celXmlScriptEventHandler::DumpVariables (celBehaviourXml* behave)
{
  if (!behave) return;
  size_t i;
  printf ("### Variables ###\n");
  iPcProperties* props = behave->GetProperties ();
  CS_ASSERT (props != 0);
  for (i = 0 ; i < props->GetPropertyCount () ; i++)
  {
    printf ("%lu %s ", (unsigned long)i, props->GetPropertyName (i));
    switch (props->GetPropertyType (i))
    {
      case CEL_DATA_LONG:
        printf ("val={int32:%ld}\n", props->GetPropertyLong (i));
	break;
      case CEL_DATA_FLOAT:
        printf ("val={float:%g}\n", props->GetPropertyFloat (i));
	break;
      case CEL_DATA_BOOL:
        printf ("val={bool:%s}\n", props->GetPropertyBool (i) ? "true" : "false");
	break;
      case CEL_DATA_STRING:
        printf ("val={str:%s}\n", props->GetPropertyString (i));
	break;
      case CEL_DATA_VECTOR2:
        {
	  csVector2 v;
	  props->GetPropertyVector (i, v);
          printf ("val={vec:[%g,%g]}\n", v.x, v.y);
	}
	break;
      case CEL_DATA_VECTOR3:
        {
	  csVector3 v;
	  props->GetPropertyVector (i, v);
          printf ("val={vec:[%g,%g,%g]}\n", v.x, v.y, v.z);
	}
	break;
      case CEL_DATA_COLOR:
        {
	  csColor v;
	  props->GetPropertyColor (i, v);
          printf ("val={rgb:[%g,%g,%g]}\n", v.red, v.green, v.blue);
	}
	break;
      case CEL_DATA_PCLASS:
        printf ("val={pc:%p}\n", props->GetPropertyPClass (i));
	break;
      case CEL_DATA_ENTITY:
        printf ("val={ent:%s}\n",
		props->GetPropertyEntity (i)
			? props->GetPropertyEntity (i)->GetName ()
			: "<null>");
	break;
      case CEL_DATA_IBASE:
        printf ("val={ibase:%p}\n", props->GetPropertyIBase (i));
	break;
      default:
        printf ("val={unknown}\n");
	break;
    }
  }
  fflush (stdout);
}

iPcRules* celXmlScriptEventHandler::GetRules (iCelEntity* entity,
      celBehaviourXml* behave)
{
  if (behave) return behave->GetRules ();
  if (!entity) return 0;
  csRef<iPcRules> p;
  p = CEL_QUERY_PROPCLASS_ENT (entity, iPcRules);
  return p;
}

iPcProperties* celXmlScriptEventHandler::GetProperties (iCelEntity* entity,
      celBehaviourXml* behave)
{
  if (behave) return behave->GetProperties ();
  if (!entity) return 0;
  csRef<iPcProperties> p;
  p = CEL_QUERY_PROPCLASS_ENT (entity, iPcProperties);
  return p;
}

static float GetAngle (const csVector3& v1, const csVector3& v2)
{
  float len = sqrt (csSquaredDist::PointPoint (v1, v2));
  float angle = acos ((v2.x-v1.x) / len);
  if ((v2.z-v1.z) > 0) angle = 2*PI - angle;
  angle += PI / 2.0f;
  if (angle > 2*PI) angle -= 2*PI;
  return angle;
}

#ifdef CS_DEBUG
#define CHECK_STACK(i) \
  if (stack.Length () < i) \
    return ReportError (cbl, "Stack underflow!");
#else
#define CHECK_STACK(i)
#endif

bool celXmlScriptEventHandler::Execute (iCelEntity* entity,
    	celBlXml* cbl,
	celBehaviourXml* behave, celData& ret, iCelParameterBlock* params,
	size_t startop, bool newscope, int expected_stack_size)
{
  // We keep a reference to the entity here to prevent
  // deletion of that entity during script execution.
  csRef<iCelEntity> e = entity;
  bool varprop_trace = cbl->varprop_trace;

  size_t stack_size = stack.Length () + expected_stack_size;
  size_t i = startop;
  if (startop == 0)
  {
    DUMP_EXEC ((":::: entity=%s behave=%s\n", entity->GetName (),
  	behave ? behave->GetName () : "<null>"));
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
	  return ReportError (cbl, "Stack size mismatch!");
	}
        return true;
      case CEL_OPERATION_CALLSTACK:
	DumpCallStack (cbl);
        break;
      case CEL_OPERATION_VARIABLES:
	DumpVariables (behave);
        break;
      case CEL_OPERATION_TRACEON:
        {
	  cbl->varprop_trace = true;
	  varprop_trace = true;
	}
        break;
      case CEL_OPERATION_TRACEOFF:
        {
	  cbl->varprop_trace = false;
	  varprop_trace = false;
	}
        break;
      case CEL_OPERATION_PARAM:
        {
	  CHECK_STACK(1)
	  celXmlArg a_arg = stack.Pop ();
          DUMP_EXEC ((":%04d: param %s\n", i-1, A2S (a_arg)));
	  csStringID id = ArgToID (a_arg);
	  size_t si = stack.Push (celXmlArg ());
	  if (!params)
	    return ReportError (cbl,
		"No parameters were supplied while calling this event!");
	  const celData* data = params->GetParameter (id);
	  if (!data)
	    return ReportError (cbl, "Can't find parameter!");
	  if (!celData2celXmlArg (*data, stack[si]))
	    return ReportError (cbl, "Type not supported for 'param'!");
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
	  size_t si = stack.Push (celXmlArg ());
	  stack[si].SetString (op.arg.arg.str.s, false);
	}
        break;
      case CEL_OPERATION_PCTHIS:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: pcthis pc=%s\n", i-1, A2S (top)));

          iCelPropertyClass* other_pc = entity->GetPropertyClassList ()->
	  	FindByName (ArgToString (top));
	  top.SetPC (other_pc);	// Can be 0.
	}
	break;
      case CEL_OPERATION_PCTAGTHIS:
        {
	  CHECK_STACK(2)
	  celXmlArg a_tag = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: pctagthis pc=%s tag=%s\n",
	  	i-1, A2S (top), A2S (a_tag)));

          iCelPropertyClass* other_pc = entity->GetPropertyClassList ()->
	  	FindByNameAndTag (ArgToString (top), ArgToString (a_tag));
	  top.SetPC (other_pc);	// Can be 0.
	}
	break;
      case CEL_OPERATION_CALCPARID:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: calcparid %s\n", i-1, A2S (top)));
	  csString str = "cel.parameter.";
	  str += ArgToString (top);
	  csStringID id = pl->FetchStringID ((const char*)str);
	  top.SetID (id);
	}
	break;
      case CEL_OPERATION_CALCACTID:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: calcactid %s\n", i-1, A2S (top)));
	  csString str = "cel.action.";
	  str += ArgToString (top);
	  csStringID id = pl->FetchStringID ((const char*)str);
	  top.SetID (id);
	}
	break;
      case CEL_OPERATION_CALCPROPID:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: calcpropid %s\n", i-1, A2S (top)));
	  csString str = "cel.property.";
	  str += ArgToString (top);
	  csStringID id = pl->FetchStringID ((const char*)str);
	  top.SetID (id);
	}
	break;
      case CEL_OPERATION_CALCID:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: calcid %s\n", i-1, A2S (top)));
	  csStringID id = pl->FetchStringID (ArgToString (top));
	  top.SetID (id);
	}
	break;
      case CEL_OPERATION_CLASS_ADD:
	{
	  CHECK_STACK(2)
	  celXmlArg a_class = stack.Pop ();
	  celXmlArg a_ent = stack.Pop ();
          DUMP_EXEC ((":%04d: class_add ent=%s id=%s\n", i-1, A2S (a_ent),
	  	A2S (a_class)));

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	      	"Can't find entity '%s'!", A2S (a_ent));
	  other_ent->AddClass (ArgToID (a_class));
	}
        break;
      case CEL_OPERATION_CLASS_REM:
	{
	  CHECK_STACK(2)
	  celXmlArg a_class = stack.Pop ();
	  celXmlArg a_ent = stack.Pop ();
          DUMP_EXEC ((":%04d: class_rem ent=%s id=%s\n", i-1, A2S (a_ent),
	  	A2S (a_class)));

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	      	"Can't find entity '%s'!", A2S (a_ent));
	  other_ent->RemoveClass (ArgToID (a_class));
	}
        break;
      case CEL_OPERATION_HASCLASS:
	{
	  CHECK_STACK(2)
	  celXmlArg a_class = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: hasclass ent=%s id=%s\n", i-1, A2S (top),
	  	A2S (a_class)));

	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	      	"Can't find entity '%s'!", A2S (top));
	  top.Set (other_ent->HasClass (ArgToID (a_class)));
	}
        break;
      case CEL_OPERATION_PC:
        {
	  CHECK_STACK(2)
	  celXmlArg a_pc = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: pc ent=%s pc=%s\n", i-1, A2S (top), A2S (a_pc)));

	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	  {
	    top.SetPC ((iCelPropertyClass*)0);
	    break;
	  }
          iCelPropertyClass* other_pc = other_ent->GetPropertyClassList ()->
	  	FindByName (ArgToString (a_pc));
	  top.SetPC (other_pc);	// Can be 0.
	}
        break;
      case CEL_OPERATION_PCTAG:
        {
	  CHECK_STACK(3)
	  celXmlArg a_tag = stack.Pop ();
	  celXmlArg a_pc = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: pctag ent=%s pc=%s tag=%s\n",
	  	i-1, A2S (top), A2S (a_pc), A2S (a_tag)));

	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	  {
	    top.SetPC ((iCelPropertyClass*)0);
	    break;
	  }
          iCelPropertyClass* other_pc = other_ent->GetPropertyClassList ()->
	  	FindByNameAndTag (ArgToString (a_pc), ArgToString (a_tag));
	  top.SetPC (other_pc);	// Can be 0.
	}
        break;
      case CEL_OPERATION_VECX:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: vecx vec=%s\n", i-1, A2S (top)));
	  if (top.type == CEL_DATA_VECTOR2)
	  {
	    csVector2 v = ArgToVector2 (top);
	    top.SetFloat (v.x);
	  }
	  else
	  {
	    csVector3 v = ArgToVector3 (top);
	    top.SetFloat (v.x);
	  }
	}
	break;
      case CEL_OPERATION_VECY:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: vecy vec=%s\n", i-1, A2S (top)));
	  if (top.type == CEL_DATA_VECTOR2)
	  {
	    csVector2 v = ArgToVector2 (top);
	    top.SetFloat (v.y);
	  }
	  else
	  {
	    csVector3 v = ArgToVector3 (top);
	    top.SetFloat (v.y);
	  }
	}
	break;
      case CEL_OPERATION_VECZ:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: vecz vec=%s\n", i-1, A2S (top)));
	  csVector3 v = ArgToVector3 (top);
	  top.SetFloat (v.z);
	}
	break;
      case CEL_OPERATION_COLRED:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: colred vec=%s\n", i-1, A2S (top)));
	  csColor v = ArgToColor (top);
	  top.SetFloat (v.red);
	}
	break;
      case CEL_OPERATION_COLGREEN:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: colgreen vec=%s\n", i-1, A2S (top)));
	  csColor v = ArgToColor (top);
	  top.SetFloat (v.green);
	}
	break;
      case CEL_OPERATION_COLBLUE:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: colblue vec=%s\n", i-1, A2S (top)));
	  csColor v = ArgToColor (top);
	  top.SetFloat (v.blue);
	}
	break;
      case CEL_OPERATION_VECTOR2:
        {
	  CHECK_STACK(2)
	  celXmlArg ely = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: vector2 %s, %s\n", i-1, A2S (top), A2S (ely)));
	  top.SetVector (csVector2 (ArgToFloat (top), ArgToFloat (ely)));
	}
	break;
      case CEL_OPERATION_VECTOR3:
        {
	  CHECK_STACK(3)
	  celXmlArg elz = stack.Pop ();
	  celXmlArg ely = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: vector3 %s, %s, %s\n", i-1, A2S (top),
	  	A2S (ely), A2S (elz)));
	  top.SetVector (csVector3 (ArgToFloat (top),
	  	ArgToFloat (ely), ArgToFloat (elz)));
	}
	break;
      case CEL_OPERATION_COLOR:
        {
	  CHECK_STACK(3)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg elg = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: color %s, %s, %s\n", i-1, A2S (top),
	  	A2S (elg), A2S (elb)));
	  top.SetColor (csColor (ArgToFloat (top),
	  	ArgToFloat (elg), ArgToFloat (elb)));
	}
	break;
      case CEL_OPERATION_MOUSEX:
        {
          DUMP_EXEC ((":%04d: mousex ()\n", i-1));
	  size_t si = stack.Push (celXmlArg ());
	  int mousex = cbl->GetMouseDriver ()->GetLastX ();
	  stack[si].SetInt32 (mousex);
	}
	break;
      case CEL_OPERATION_MOUSEY:
        {
          DUMP_EXEC ((":%04d: mousey ()\n", i-1));
	  size_t si = stack.Push (celXmlArg ());
	  int mousey = cbl->GetMouseDriver ()->GetLastY ();
	  stack[si].SetInt32 (mousey);
	}
	break;
      case CEL_OPERATION_BB_MOUSEX:
        {
          DUMP_EXEC ((":%04d: bb_mousex ()\n", i-1));
	  size_t si = stack.Push (celXmlArg ());
	  int mousex = cbl->GetMouseDriver ()->GetLastX ();
	  iBillboardManager* bbmgr = cbl->GetBillboardManager ();
	  stack[si].SetInt32 (bbmgr->ScreenToBillboardX (mousex));
	}
	break;
      case CEL_OPERATION_BB_MOUSEY:
        {
          DUMP_EXEC ((":%04d: bb_mousey ()\n", i-1));
	  size_t si = stack.Push (celXmlArg ());
	  int mousey = cbl->GetMouseDriver ()->GetLastY ();
	  iBillboardManager* bbmgr = cbl->GetBillboardManager ();
	  stack[si].SetInt32 (bbmgr->ScreenToBillboardY (mousey));
	}
	break;
      case CEL_OPERATION_SCR_WIDTH:
        {
          DUMP_EXEC ((":%04d: scr_width ()\n", i-1));
	  size_t si = stack.Push (celXmlArg ());
	  stack[si].SetInt32 (cbl->GetG3D ()->GetWidth ());
	}
	break;
      case CEL_OPERATION_SCR_HEIGHT:
        {
          DUMP_EXEC ((":%04d: scr_height ()\n", i-1));
	  size_t si = stack.Push (celXmlArg ());
	  stack[si].SetInt32 (cbl->GetG3D ()->GetHeight ());
	}
	break;
      case CEL_OPERATION_ENTNAMETHIS:
        {
          DUMP_EXEC ((":%04d: entnamethis ()\n", i-1));
	  size_t si = stack.Push (celXmlArg ());
	  stack[si].SetString (entity->GetName (), true);
	}
	break;
      case CEL_OPERATION_ENTNAME:
        {
          CHECK_STACK(1)
          celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: entname (%s)\n", i-1, A2S (top)));
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	  {
	    return ReportError (cbl,
	      	"Can't find entity '%s'!", A2S (top));
	  }
	  top.SetString (other_ent->GetName (), true);
	}
	break;
      case CEL_OPERATION_RANDOMIZE:
        {
          DUMP_EXEC ((":%04d: randomize\n", i-1));
          behave->Randomize ();
        }
        break;
      case CEL_OPERATION_RAND:
        {
          CHECK_STACK(1)
          celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: rand (%s)\n", i-1, A2S (top)));
          int32 rndi;
          uint32 rndu;
          float rndf;
          switch (top.type)
          {
            case CEL_DATA_LONG:
              rndi = ArgToInt32 (top);
              top.SetInt32 (behave->GetRandInt32 (rndi));
            break;
            case CEL_DATA_ULONG:
              rndu = ArgToUInt32 (top);
              top.SetUInt32 (behave->GetRandUInt32 (rndu));
            break;
            default:
              rndf = ArgToFloat (top);
              top.SetFloat (behave->GetRandFloat (rndf));
          }
        }
        break;
      case CEL_OPERATION_FLOAT:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: float %s\n", i-1, A2S (top)));
	  switch (top.type)
	  {
	    case CEL_DATA_LONG: top.SetFloat (float (top.arg.i)); break;
	    case CEL_DATA_ULONG: top.SetFloat (float (top.arg.ui)); break;
	    case CEL_DATA_FLOAT: break;
	    case CEL_DATA_BOOL: top.SetFloat (float (top.arg.b)); break;
	    case CEL_DATA_STRING:
	      {
	        float i;
		if (top.arg.str.s)
		  sscanf (top.arg.str.s, "%f", &i);
		else
		  i = 0.0;
	        top.SetFloat (i);
	      }
	      break;
	    default:
	      return ReportError (cbl,
	      	"Can't convert element on stack to 'float'!");
	  }
	}
	break;
      case CEL_OPERATION_INT:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: int %s\n", i-1, A2S (top)));
	  switch (top.type)
	  {
	    case CEL_DATA_LONG:
	    case CEL_DATA_ULONG:
	      break;
	    case CEL_DATA_FLOAT:
	      top.SetInt32 (int32 (top.arg.f));
	      break;
	    case CEL_DATA_BOOL:
	      top.SetInt32 (int32 (top.arg.b));
	      break;
	    case CEL_DATA_STRING:
	      {
	        int i;
		if (top.arg.str.s)
		  sscanf (top.arg.str.s, "%d", &i);
		else
		  i = 0;
	        top.SetInt32 ((int32)i);
	      }
	      break;
	    default:
	      return ReportError (cbl,
	      	"Can't convert element on stack to 'int'!");
	  }
	}
	break;
      case CEL_OPERATION_NORMALIZE:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: normalize %s\n", i-1, A2S (top)));
	  csVector3 v = ArgToVector3 (top);
	  v.Normalize ();
	  top.SetVector (v);
	}
	break;
      case CEL_OPERATION_SIGN:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: sign %s\n", i-1, A2S (top)));
	  switch (top.type)
	  {
	    case CEL_DATA_LONG: top.arg.i = SIGN (top.arg.i); break;
	    case CEL_DATA_ULONG: top.SetInt32 (top.arg.ui > 0 ? 1 : 0); break;
	    case CEL_DATA_FLOAT: top.SetInt32 (SIGN (top.arg.f)); break;
	    default:
	      return ReportError (cbl,
	      	"Can't calculate 'sign' for element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_COS:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: cos %s\n", i-1, A2S (top)));
	  float f = ArgToFloat (top);
	  top.SetFloat (cosf (f));
	}
	break;
      case CEL_OPERATION_SIN:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: sin %s\n", i-1, A2S (top)));
	  float f = ArgToFloat (top);
	  top.SetFloat (sinf (f));
	}
	break;
      case CEL_OPERATION_TAN:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: tan %s\n", i-1, A2S (top)));
	  float f = ArgToFloat (top);
	  top.SetFloat (tanf (f));
	}
	break;
      case CEL_OPERATION_ACOS:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: acos %s\n", i-1, A2S (top)));
	  float f = ArgToFloat (top);
	  top.SetFloat (acosf (f));
	}
	break;
      case CEL_OPERATION_ASIN:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: asin %s\n", i-1, A2S (top)));
	  float f = ArgToFloat (top);
	  top.SetFloat (asinf (f));
	}
	break;
      case CEL_OPERATION_ATAN:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: atan %s\n", i-1, A2S (top)));
	  float f = ArgToFloat (top);
	  top.SetFloat (atanf (f));
	}
	break;
      case CEL_OPERATION_SQRT:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: sqrt %s\n", i-1, A2S (top)));
	  float f = ArgToFloat (top);
	  if (f < 0.0f)
	    return ReportError (cbl,
	      "Attempting to calculate sqrt of a negative number (%g)!", f);
	  top.SetFloat (sqrt (f));
	}
	break;
      case CEL_OPERATION_SQDIST:
        {
	  CHECK_STACK(2)
	  celXmlArg p2 = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: sqdist p1=%s p2=%s\n", i-1, A2S (top), A2S (p2)));
	  csVector3 v1 = ArgToVector3 (top);
	  csVector3 v2 = ArgToVector3 (p2);
	  float sqdist = csSquaredDist::PointPoint (v1, v2);
	  top.SetFloat (sqdist);
	}
	break;
      case CEL_OPERATION_ABS:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: abs %s\n", i-1, A2S (top)));
	  switch (top.type)
	  {
	    case CEL_DATA_LONG: top.arg.i = ABS (top.arg.i); break;
	    case CEL_DATA_ULONG: break;
	    case CEL_DATA_FLOAT: top.arg.f = ABS (top.arg.f); break;
	    default:
	      return ReportError (cbl,
	      	"Can't calculate 'abs' for element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_BOOL:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: bool %s\n", i-1, A2S (top)));
	  switch (top.type)
	  {
	    case CEL_DATA_LONG: top.Set (top.arg.i != 0); break;
	    case CEL_DATA_ULONG: top.Set (top.arg.ui != 0); break;
	    case CEL_DATA_FLOAT: top.Set (fabs (top.arg.f) >= SMALL_EPSILON); break;
	    case CEL_DATA_BOOL: break;
	    case CEL_DATA_STRING:
	      {
		bool b;
		csScanStr (top.arg.str.s, "%b", &b);
		top.Set (b);
	      }
	      break;
	    case CEL_DATA_ENTITY:
	    case CEL_DATA_IBASE:
	    case CEL_DATA_PCLASS:
	      break;
	    default:
	      return ReportError (cbl, "Can't log-not element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_LOGNOT:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: ! %s\n", i-1, A2S (top)));
	  switch (top.type)
	  {
	    case CEL_DATA_LONG: top.Set (!top.arg.i); break;
	    case CEL_DATA_ULONG: top.Set (!top.arg.ui); break;
	    case CEL_DATA_FLOAT: top.Set (fabs (top.arg.f) < SMALL_EPSILON); break;
	    case CEL_DATA_BOOL: top.arg.b = !top.arg.b; break;
	    case CEL_DATA_ENTITY:
	    case CEL_DATA_IBASE:
	    case CEL_DATA_PCLASS:
	    case CEL_DATA_STRING:
	      top.Set (!ArgToBool (top));
	      break;
	    default:
	      return ReportError (cbl, "Can't convert element on stack to bool!");
	  }
	}
	break;
      case CEL_OPERATION_BITNOT:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: ~ %s\n", i-1, A2S (top)));
	  switch (top.type)
	  {
	    case CEL_DATA_LONG: top.arg.i = ~top.arg.i; break;
	    case CEL_DATA_ULONG: top.arg.ui = ~top.arg.ui; break;
	    case CEL_DATA_BOOL: top.arg.b = !top.arg.b; break;
	    default:
	      return ReportError (cbl, "Can't bit-not element on stack!");
	  }
	}
	break;
      case CEL_OPERATION_UNARYMINUS:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: unaryminus %s\n", i-1, A2S (top)));
	  switch (top.type)
	  {
	    case CEL_DATA_LONG: top.arg.i = -top.arg.i; break;
	    case CEL_DATA_ULONG: top.arg.ui = (unsigned int)(-(int)top.arg.ui); break;
	    case CEL_DATA_FLOAT: top.arg.f = -top.arg.f; break;
	    case CEL_DATA_BOOL: top.arg.b = !top.arg.b; break;
	    case CEL_DATA_COLOR:
	      top.arg.col.red = -top.arg.col.red;
	      top.arg.col.green = -top.arg.col.green;
	      top.arg.col.blue = -top.arg.col.blue;
	      break;
	    case CEL_DATA_VECTOR3:
	      top.arg.vec.x = -top.arg.vec.x;
	      top.arg.vec.y = -top.arg.vec.y;
	      top.arg.vec.z = -top.arg.vec.z;
	      break;
	    case CEL_DATA_VECTOR2:
	      top.arg.vec.x = -top.arg.vec.x;
	      top.arg.vec.y = -top.arg.vec.y;
	      break;
	    default:
	      return ReportError (cbl, "Can't negate element on stack!");
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
	  const char* s = strstr (str, del);
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  if (s)
	  {
            csString s1, s2;
            s1.Replace (str, s - str);
            s2.Replace (s + strlen (del));
	    props->SetProperty (leftvar, s1);
	    props->SetProperty (rightvar, s2);
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
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: stridx (%s,%s)\n", i-1, A2S (top),
	  	A2S (a_sub)));
	  const char* str = ArgToString (top);
	  const char* sub = ArgToString (a_sub);
	  const char* rc = strstr (str, sub);
	  if (rc)
	    top.SetInt32 (rc-str);
	  else
	    top.SetInt32 (-1);
	}
        break;
      case CEL_OPERATION_STRSUB:
        {
	  CHECK_STACK(3)
	  celXmlArg a_len = stack.Pop ();
	  celXmlArg a_pos = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: strsub (%s,%s,%s)\n", i-1, A2S (top),
	  	A2S (a_pos), A2S (a_len)));
	  const char* str = ArgToString (top);
	  int32 pos = ArgToInt32 (a_pos);
	  int32 len = ArgToInt32 (a_len);
	  size_t real_len = strlen (str) - pos;
	  real_len = MIN (real_len, (size_t)len);
	  char* newstr = new char[real_len+1];
	  if (real_len)
	    strncpy (newstr, str+pos, real_len);
	  newstr[real_len] = 0;
	  top.SetStringPrealloc (newstr);
	}
        break;
      case CEL_OPERATION_STRLEN:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: strlen (%s)\n", i-1, A2S (top)));
	  const char* str = ArgToString (top);
	  if (str) top.SetInt32 ((int32)strlen (str));
	  else top.SetInt32 (0);
	}
	break;
      case CEL_OPERATION_CONFIG_SAVE:
        {
          DUMP_EXEC ((":%04d: config_save\n", i-1));
	  if (!script)
	    return ReportError (cbl,
	    	"'config_save' not supported in this context!");
	  script->GetConfigAccess ()->Save ();
	}
	break;
      case CEL_OPERATION_CONFIG_ADD:
        {
	  CHECK_STACK(1)
	  celXmlArg p_file = stack.Pop ();
          DUMP_EXEC ((":%04d: config_add file=%s\n", i-1, A2S (p_file)));
	  const char* file = ArgToString (p_file);
	  if (!script)
	    return ReportError (cbl,
	    	"'config_add' not supported in this context!");
	  script->GetConfigAccess ().AddConfig (cbl->GetObjectRegistry (),
	  	file);
	}
	break;
      case CEL_OPERATION_CONFIG_REM:
        {
	  CHECK_STACK(1)
	  celXmlArg p_file = stack.Pop ();
          DUMP_EXEC ((":%04d: config_rem file=%s\n", i-1, A2S (p_file)));
	  const char* file = ArgToString (p_file);
	  csRef<iConfigManager> app_cfg =
	      csQueryRegistry<iConfigManager> (cbl->GetObjectRegistry ());
	  if (!app_cfg)
	    return ReportError (cbl,
	    	"'config_rem' not supported in this context!");
	  app_cfg->RemoveDomain (file);
	}
	break;
      case CEL_OPERATION_CONFIG_SET:
        {
	  CHECK_STACK(2)
	  celXmlArg p_value = stack.Pop ();
	  celXmlArg p_key = stack.Pop ();
          DUMP_EXEC ((":%04d: config_set key=%s value=%s\n", i-1, A2S (p_key),
		A2S (p_value)));
	  const char* key = ArgToString (p_key);
	  const char* val = ArgToString (p_value);
	  iConfigFile* cfgfile;
	  if (script)
	    cfgfile = (iConfigFile*)(script->GetConfigAccess ());
	  else
	  {
	    csRef<iConfigManager> config = csQueryRegistry<iConfigManager> (
	      cbl->GetObjectRegistry ());
	    cfgfile = (iConfigManager*)config;
	  }
	  if (!cfgfile)
	    return ReportError (cbl, "Can't find configuration manager!");
	  cfgfile->SetStr (key, val);
	}
	break;
      case CEL_OPERATION_CONFIG:
        {
	  CHECK_STACK(2)
	  celXmlArg p_default = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: config key=%s default=%s\n", i-1, A2S (top),
		A2S (p_default)));
	  const char* key = ArgToString (top);
	  const char* def = ArgToString (p_default);
	  iConfigFile* cfgfile;
	  if (script)
	    cfgfile = (iConfigFile*)(script->GetConfigAccess ());
	  else
	  {
	    csRef<iConfigManager> config = csQueryRegistry<iConfigManager> (
	      cbl->GetObjectRegistry ());
	    cfgfile = (iConfigManager*)config;
	  }
	  if (!cfgfile)
	    return ReportError (cbl, "Can't find configuration manager!");
	  top.SetString (cfgfile->GetStr (key, def), true);
	}
	break;
      case CEL_OPERATION_TESTVAR:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: testvar (%s)\n", i-1, A2S (top)));
	  const char* str = ArgToString (top);
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props)
	  {
	    top.Set (false);
	    break;
	  }
	  size_t idx = props->GetPropertyIndex (str);
          if (idx == csArrayItemNotFound)
	    top.Set (false);
	  else
	    top.Set (true);
	}
	break;
      case CEL_OPERATION_TESTVARENT:
        {
	  CHECK_STACK(2)
	  celXmlArg e_var = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: testvarent (%s,%s)\n", i-1, A2S (top),
	  	A2S (e_var)));
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  const char* str = ArgToString (e_var);
	  iPcProperties* props = GetProperties (other_ent, 0);
	  if (!props)
	  {
	    top.Set (false);
	    break;
	  }
	  size_t idx = props->GetPropertyIndex (str);
          if (idx == csArrayItemNotFound)
	    top.Set (false);
	  else
	    top.Set (true);
	}
	break;
      case CEL_OPERATION_INTPOL:
        {
	  CHECK_STACK(3)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg ela = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: intpol (%s,%s,%s)\n", i-1, A2S (top),
	  	A2S (ela), A2S (elb)));
	  float delta = ArgToFloat (top);
	  int t = GetCalculationType (ela, elb);
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      {
	        float fa = ArgToFloat (ela);
	        float fb = ArgToFloat (elb);
	        top.SetFloat ((1.0-delta)*fa + delta*fb);
	      }
	      break;
	    case CEL_DATA_LONG:
	      {
	        int32 fa = ArgToInt32 (ela);
	        int32 fb = ArgToInt32 (elb);
	        top.SetInt32 (int32 (
			(1.0-delta)*float (fa) + delta*float (fb)));
	      }
	      break;
	    case CEL_DATA_ULONG:
	      {
	        uint32 fa = ArgToUInt32 (ela);
	        uint32 fb = ArgToUInt32 (elb);
	        top.SetUInt32 (uint32 (
			(1.0-delta)*float (fa) + delta*float (fb)));
	      }
	      break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 fa = ArgToVector2 (ela);
	        csVector2 fb = ArgToVector2 (elb);
	        top.SetVector ((1.0-delta)*fa + delta*fb);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 fa = ArgToVector3 (ela);
	        csVector3 fb = ArgToVector3 (elb);
	        top.SetVector ((1.0-delta)*fa + delta*fb);
	      }
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor fa = ArgToColor (ela);
	        csColor fb = ArgToColor (elb);
	        top.SetColor ((1.0-delta)*fa + delta*fb);
	      }
	      break;
	    default:
	      return ReportError (cbl,
	      	"Can't 'interpolate' these types!");
	  }
	}
	break;
      case CEL_OPERATION_MIN:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: min (%s,%s)\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_STRING:
	      {
	        const char* sa = ArgToString (top);
	        const char* sb = ArgToString (elb);
	        if (strcmp (sa, sb) < 0)
	          top.SetString (sa, true);
	        else
	          top.SetString (sb, true);
	      }
	      break;
	    case CEL_DATA_FLOAT:
	      {
	        float fa = ArgToFloat (top);
	        float fb = ArgToFloat (elb);
	        top.SetFloat (MIN (fa, fb));
	      }
	      break;
	    case CEL_DATA_LONG:
	      {
	        int32 fa = ArgToInt32 (top);
	        int32 fb = ArgToInt32 (elb);
	        top.SetInt32 (MIN (fa, fb));
	      }
	      break;
	    case CEL_DATA_ULONG:
	      {
	        uint32 fa = ArgToUInt32 (top);
	        uint32 fb = ArgToUInt32 (elb);
	        top.SetUInt32 (MIN (fa, fb));
	      }
	      break;
	    default:
	      return ReportError (cbl,
	      	"Can't calculate 'min' of these types!");
	  }
	}
	break;
      case CEL_OPERATION_MAX:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: max (%s,%s)\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_STRING:
	      {
	        const char* sa = ArgToString (top);
	        const char* sb = ArgToString (elb);
	        if (strcmp (sa, sb) > 0)
	          top.SetString (sa, true);
	        else
	          top.SetString (sb, true);
	      }
	      break;
	    case CEL_DATA_FLOAT:
	      {
	        float fa = ArgToFloat (top);
	        float fb = ArgToFloat (elb);
	        top.SetFloat (MAX (fa, fb));
	      }
	      break;
	    case CEL_DATA_LONG:
	      {
	        int32 fa = ArgToInt32 (top);
	        int32 fb = ArgToInt32 (elb);
	        top.SetInt32 (MAX (fa, fb));
	      }
	      break;
	    case CEL_DATA_ULONG:
	      {
	        uint32 fa = ArgToUInt32 (top);
	        uint32 fb = ArgToUInt32 (elb);
	        top.SetUInt32 (MAX (fa, fb));
	      }
	      break;
	    default:
	      return ReportError (cbl,
	      	"Can't calculate 'max' of these types!");
	  }
	}
	break;
      case CEL_OPERATION_MINUS_I:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s - %s\n", i-1, A2S (top), A2S (op.arg)));
	  int t = GetCalculationType (top, op.arg);
	  switch (t)
	  {
	    case CEL_DATA_NONE:
	      return ReportError (cbl, "Can't subtract these types!");
	    case CEL_DATA_BOOL:
	      return ReportError (cbl, "Can't subtract booleans!");
	    case CEL_DATA_STRING:
	      return ReportError (cbl, "Can't subtract strings!");
	    case CEL_DATA_FLOAT:
	      top.SetFloat (ArgToFloat (top) - ArgToFloat (op.arg));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) - ArgToInt32 (op.arg));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) - ArgToUInt32 (op.arg));
	      break;
	    case CEL_DATA_VECTOR2:
	      top.SetVector (ArgToVector2 (top) - ArgToVector2 (op.arg));
	      break;
	    case CEL_DATA_VECTOR3:
	      top.SetVector (ArgToVector3 (top) - ArgToVector3 (op.arg));
	      break;
	    case CEL_DATA_COLOR:
	      top.SetColor (ArgToColor (top) - ArgToColor (op.arg));
	      break;
	  }
	}
	break;
      case CEL_OPERATION_MINUS:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s - %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_NONE:
	      return ReportError (cbl, "Can't subtract these types!");
	    case CEL_DATA_BOOL:
	      return ReportError (cbl, "Can't subtract booleans!");
	    case CEL_DATA_STRING:
	      return ReportError (cbl, "Can't subtract strings!");
	    case CEL_DATA_FLOAT:
	      top.SetFloat (ArgToFloat (top) - ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) - ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) - ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      top.SetVector (ArgToVector2 (top) - ArgToVector2 (elb));
	      break;
	    case CEL_DATA_VECTOR3:
	      top.SetVector (ArgToVector3 (top) - ArgToVector3 (elb));
	      break;
	    case CEL_DATA_COLOR:
	      top.SetColor (ArgToColor (top) - ArgToColor (elb));
	      break;
	  }
	}
	break;
      case CEL_OPERATION_ADD_I:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s + %s (i)\n", i-1, A2S (top), A2S (op.arg)));
	  int t = GetCalculationType (op.arg, top);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      return ReportError (cbl, "Can't add booleans!");
	    case CEL_DATA_FLOAT:
	      top.SetFloat (ArgToFloat (op.arg) + ArgToFloat (top));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (op.arg) + ArgToInt32 (top));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (op.arg) + ArgToUInt32 (top));
	      break;
	    case CEL_DATA_VECTOR2:
	      top.SetVector (ArgToVector2 (op.arg) + ArgToVector2 (top));
	      break;
	    case CEL_DATA_VECTOR3:
	      top.SetVector (ArgToVector3 (op.arg) + ArgToVector3 (top));
	      break;
	    case CEL_DATA_COLOR:
	      top.SetColor (ArgToColor (op.arg) + ArgToColor (top));
	      break;
	    case CEL_DATA_NONE:
	    case CEL_DATA_STRING:
	      {
		const char* astr = ArgToString (top);
		const char* bstr = ArgToString (op.arg);
		size_t alen = astr ? strlen (astr) : 0;
		size_t blen = bstr ? strlen (bstr) : 0;
		if (alen || blen)
		{
		  if (!blen)
	            ; // top already contains the right string.
		  else if (!alen)
	            top.SetString (bstr, true);
		  else
		  {
		    char* str = new char[alen+blen+1];
		    strcpy (str, astr);
		    strcpy (str+alen, bstr);
	            top.SetStringPrealloc (str);
		  }
	        }
		else
		{
	          top.SetString ("", true);
		}
	      }
	      break;
	  }
	}
	break;
      case CEL_OPERATION_ADD:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s + %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      return ReportError (cbl, "Can't add booleans!");
	    case CEL_DATA_FLOAT:
	      top.SetFloat (ArgToFloat (top) + ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) + ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) + ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      top.SetVector (ArgToVector2 (top) + ArgToVector2 (elb));
	      break;
	    case CEL_DATA_VECTOR3:
	      top.SetVector (ArgToVector3 (top) + ArgToVector3 (elb));
	      break;
	    case CEL_DATA_COLOR:
	      top.SetColor (ArgToColor (top) + ArgToColor (elb));
	      break;
	    case CEL_DATA_NONE:
	    case CEL_DATA_STRING:
	      {
		const char* astr = ArgToString (top);
		const char* bstr = ArgToString (elb);
		size_t alen = astr ? strlen (astr) : 0;
		size_t blen = bstr ? strlen (bstr) : 0;
		if (alen || blen)
		{
		  if (!blen)
	            ; // top already contains the right string.
		  else if (!alen)
	            top.SetString (bstr, true);
		  else
		  {
		    char* str = new char[alen+blen+1];
		    strcpy (str, astr);
		    strcpy (str+alen, bstr);
	            top.SetStringPrealloc (str);
		  }
	        }
		else
		{
	          top.SetString ("", true);
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
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s * %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_NONE:
	      if (IsNumericType (top) && IsVectorType (elb))
	      {
	        switch (elb.type)
		{
		  case CEL_DATA_VECTOR2:
		    top.SetVector (ArgToFloat (top) * ArgToVector2 (elb));
		    break;
		  case CEL_DATA_VECTOR3:
		    top.SetVector (ArgToFloat (top) * ArgToVector3 (elb));
		    break;
		  case CEL_DATA_COLOR:
		    top.SetColor (ArgToFloat (top) * ArgToColor (elb));
		    break;
		}
	      }
	      else if (IsVectorType (top) && IsNumericType (elb))
	      {
	        switch (top.type)
		{
		  case CEL_DATA_VECTOR2:
		    top.SetVector (ArgToFloat (elb) * ArgToVector2 (top));
		    break;
		  case CEL_DATA_VECTOR3:
		    top.SetVector (ArgToFloat (elb) * ArgToVector3 (top));
		    break;
		  case CEL_DATA_COLOR:
		    top.SetColor (ArgToFloat (elb) * ArgToColor (top));
		    break;
		}
	      }
	      else
	        return ReportError (cbl, "Can't multiply these types!");
	      break;
	    case CEL_DATA_STRING:
	      return ReportError (cbl, "Can't multiply strings!");
	    case CEL_DATA_COLOR:
	      return ReportError (cbl, "Can't multiply colors!");
	    case CEL_DATA_FLOAT:
	      top.SetFloat (ArgToFloat (top) * ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) * ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) * ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      top.SetFloat (ArgToVector2 (top) * ArgToVector2 (elb));
	      break;
	    case CEL_DATA_VECTOR3:
	      top.SetFloat (ArgToVector3 (top) * ArgToVector3 (elb));
	      break;
	    case CEL_DATA_BOOL:
	      top.Set ((int (ArgToBool (top))
              * int (ArgToBool (elb)))? true:false);
	      break;
	    default:
	      return ReportError (cbl, "Can't multiply these types!");
	  }
	}
	break;
      case CEL_OPERATION_MODULO:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s %% %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_VECTOR3:
	      top.SetVector (ArgToVector3 (top) % ArgToVector3 (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) % ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) % ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (cbl,
	      	"Can't compute modulo or cross of these types!");
	  }
	}
	break;
      case CEL_OPERATION_DIV:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s / %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_NONE:
	      if (IsVectorType (top) && IsNumericType (elb))
	      {
	        switch (top.type)
		{
		  case CEL_DATA_VECTOR2:
		    top.SetVector (ArgToVector2 (top) / ArgToFloat (elb));
		    break;
		  case CEL_DATA_VECTOR3:
		    top.SetVector (ArgToVector3 (top) / ArgToFloat (elb));
		    break;
		  default:
	            return ReportError (cbl, "Can't divide these types!");
		}
	      }
	      else
	        return ReportError (cbl, "Can't divide these types!");
	      break;
	    case CEL_DATA_STRING:
	      return ReportError (cbl, "Can't divide strings!");
	    case CEL_DATA_BOOL:
	      return ReportError (cbl, "Can't divide booleans!");
	    case CEL_DATA_VECTOR2:
	      return ReportError (cbl, "Can't divide vectors!");
	    case CEL_DATA_VECTOR3:
	      return ReportError (cbl, "Can't divide vectors!");
	    case CEL_DATA_COLOR:
	      return ReportError (cbl, "Can't divide colors!");
	    case CEL_DATA_FLOAT:
	      top.SetFloat (ArgToFloat (top) / ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) / ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) / ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (cbl, "Can't divide these types!");
	  }
	}
	break;
      case CEL_OPERATION_BITAND:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s & %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      top.Set (ArgToBool (top) & ArgToBool (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) & ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) & ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (cbl, "Can't bit-and these types!");
	  }
	}
	break;
      case CEL_OPERATION_BITOR:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s | %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      top.Set (ArgToBool (top) | ArgToBool (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) | ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) | ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (cbl, "Can't bit-or these types!");
	  }
	}
	break;
      case CEL_OPERATION_BITXOR:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s ^ %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      top.Set (ArgToBool (top) ^ ArgToBool (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.SetInt32 (ArgToInt32 (top) ^ ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.SetUInt32 (ArgToUInt32 (top) ^ ArgToUInt32 (elb));
	      break;
	    default:
	      return ReportError (cbl, "Can't bit-or these types!");
	  }
	}
	break;
      case CEL_OPERATION_LOGAND:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s && %s\n", i-1, A2S (top), A2S (elb)));
	  top.Set (ArgToBool (top) && ArgToBool (elb));
	}
	break;
      case CEL_OPERATION_LOGOR:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s || %s\n", i-1, A2S (top), A2S (elb)));
	  top.Set (ArgToBool (top) || ArgToBool (elb));
	}
	break;
      case CEL_OPERATION_NE:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s != %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      top.Set (ArgToBool (top) != ArgToBool (elb));
	      break;
	    case CEL_DATA_FLOAT:
	      top.Set (ABS (ArgToFloat (top) - ArgToFloat (elb))
	      	>= SMALL_EPSILON);
	      break;
	    case CEL_DATA_LONG:
	      top.Set (ArgToInt32 (top) != ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.Set (ArgToUInt32 (top) != ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      top.Set (!((ArgToVector2 (top) - ArgToVector2 (elb))
	      	< SMALL_EPSILON));
	      break;
	    case CEL_DATA_VECTOR3:
	      top.Set (!((ArgToVector3 (top) - ArgToVector3 (elb))
	      	< SMALL_EPSILON));
	      break;
	    case CEL_DATA_ENTITY:
	      top.Set (top.arg.entity != elb.arg.entity);
	      break;
	    case CEL_DATA_IBASE:
	      top.Set (top.arg.ref != elb.arg.ref);
	      break;
	    case CEL_DATA_PCLASS:
	      top.Set (ArgToPClass (top) != ArgToPClass (elb));
	      break;
	    case CEL_DATA_ID:
	      top.Set (ArgToID (top) != ArgToID (elb));
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor c1 = ArgToColor (top);
	        csColor c2 = ArgToColor (elb);
	        top.Set (!(ABS (c1.red-c2.red) < SMALL_EPSILON &&
	        		     ABS (c1.green-c2.green) < SMALL_EPSILON &&
	        		     ABS (c1.blue-c2.blue) < SMALL_EPSILON));
	      }
	      break;
	    case CEL_DATA_STRING:
	      top.Set (strcmp (ArgToString (top),
              ArgToString (elb))? true : false);
	      break;
	    default:
	      return ReportError (cbl, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_EQ:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s == %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_BOOL:
	      top.Set (ArgToBool (top) == ArgToBool (elb));
	      break;
	    case CEL_DATA_FLOAT:
	      top.Set (ABS (ArgToFloat (top) - ArgToFloat (elb))
	      	< SMALL_EPSILON);
	      break;
	    case CEL_DATA_LONG:
	      top.Set (ArgToInt32 (top) == ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.Set (ArgToUInt32 (top) == ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_VECTOR2:
	      top.Set ((ArgToVector2 (top) - ArgToVector2 (elb))
	      	< SMALL_EPSILON);
	      break;
	    case CEL_DATA_VECTOR3:
	      top.Set ((ArgToVector3 (top) - ArgToVector3 (elb))
	      	< SMALL_EPSILON);
	      break;
	    case CEL_DATA_PCLASS:
	      top.Set (ArgToPClass (top) == ArgToPClass (elb));
	      break;
	    case CEL_DATA_ENTITY:
	      top.Set (top.arg.entity ==  elb.arg.entity);
	      break;
	    case CEL_DATA_IBASE:
	      top.Set (top.arg.ref ==  elb.arg.ref);
	      break;
	    case CEL_DATA_ID:
	      top.Set (ArgToID (top) == ArgToID (elb));
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor c1 = ArgToColor (top);
	        csColor c2 = ArgToColor (elb);
	        top.Set (ABS (c1.red-c2.red) < SMALL_EPSILON &&
	        		   ABS (c1.green-c2.green) < SMALL_EPSILON &&
	        		   ABS (c1.blue-c2.blue) < SMALL_EPSILON);
	      }
	      break;
	    case CEL_DATA_STRING:
	      top.Set (!strcmp (ArgToString (top),
	      	ArgToString (elb)));
	      break;
	    default:
	      return ReportError (cbl, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_LT:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s < %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      top.Set (ArgToFloat (top) < ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.Set (ArgToInt32 (top) < ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.Set (ArgToUInt32 (top) < ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_STRING:
	      top.Set (strcmp (ArgToString (top),
	      	ArgToString (elb)) < 0);
	      break;
	    default:
	      return ReportError (cbl, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_LE:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s <= %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      top.Set (ArgToFloat (top) <= ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.Set (ArgToInt32 (top) <= ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.Set (ArgToUInt32 (top) <= ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_STRING:
	      top.Set (strcmp (ArgToString (top),
	      	ArgToString (elb)) <= 0);
	      break;
	    default:
	      return ReportError (cbl, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_GT:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s > %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      top.Set (ArgToFloat (top) > ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.Set (ArgToInt32 (top) > ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.Set (ArgToUInt32 (top) > ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_STRING:
	      top.Set (strcmp (ArgToString (top),
	      	ArgToString (elb)) > 0);
	      break;
	    default:
	      return ReportError (cbl, "Can't compare these types!");
	  }
	}
	break;
      case CEL_OPERATION_GE:
        {
	  CHECK_STACK(2)
	  celXmlArg elb = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: %s >= %s\n", i-1, A2S (top), A2S (elb)));
	  int t = GetCalculationType (top, elb);
	  switch (t)
	  {
	    case CEL_DATA_FLOAT:
	      top.Set (ArgToFloat (top) >= ArgToFloat (elb));
	      break;
	    case CEL_DATA_LONG:
	      top.Set (ArgToInt32 (top) >= ArgToInt32 (elb));
	      break;
	    case CEL_DATA_ULONG:
	      top.Set (ArgToUInt32 (top) >= ArgToUInt32 (elb));
	      break;
	    case CEL_DATA_STRING:
	      top.Set (strcmp (ArgToString (top),
	      	ArgToString (elb)) >= 0);
	      break;
	    default:
	      return ReportError (cbl, "Can't compare these types!");
	  }
	}
	break;

      case CEL_OPERATION_DEREFLVAR:
        {
          DUMP_EXEC ((":%04d: dereflvar varidx=%s\n", i-1, A2S (op.arg)));
	  stack.Push (local_vars.GetExtend (op.arg.arg.ui));
	}
        break;
      case CEL_OPERATION_LVAR:
        {
	  CHECK_STACK(1)
	  celXmlArg val = stack.Pop ();
	  DUMP_EXEC ((":%04d: lvar varidx=%s value=%s\n", i-1, A2S (op.arg),
	  	A2S (val)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: lvar varidx=%s value=%s\n",
	    	cbl->call_stack.Top (),
	        (unsigned long)(i-1), A2S (op.arg), A2S (val));
	    fflush (stdout);
	  }
	  celXmlArg& lv = local_vars.GetExtend (op.arg.arg.ui);
	  if (val.type == CEL_DATA_STRING)
	    lv.SetString (val.arg.str.s, true);
	  else
	    lv = val;
	}
	break;

      case CEL_OPERATION_DEREFARRENT2_STR:
        {
	  CHECK_STACK(3)
	  celXmlArg a_index2 = stack.Pop ();
	  celXmlArg a_index1 = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefarrent2_str entity=%s arr=%s index1=%s index2=%s\n", i-1,
	  	A2S (top), A2S (op.arg), A2S (a_index1), A2S (a_index2)));
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'derefarrent2_str'!",
	    	EntityNameForError (top));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (cbl,
	    	"Entity '%s' doesn't have 'pcproperties'!",
	    	EntityNameForError (top));
	  const char* varname = ArgToString (op.arg.arg.str.s, ArgToString (a_index1), a_index2);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;
      case CEL_OPERATION_DEREFARRENT2:
        {
	  CHECK_STACK(4)
	  celXmlArg a_index2 = stack.Pop ();
	  celXmlArg a_index1 = stack.Pop ();
	  celXmlArg a_array = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefarrent entity=%s arr=%s index1=%s index2=%s\n", i-1,
	  	A2S (top), A2S (a_array), A2S (a_index1), A2S (a_index2)));
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'derefarrent2_str'!",
	    	EntityNameForError (top));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (cbl,
	    	"Entity '%s' doesn't have 'pcproperties'!",
	    	EntityNameForError (top));
	  const char* varname = ArgToString (ArgToString (a_array), ArgToString (a_index1), a_index2);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;

      case CEL_OPERATION_DEREFARR2_STR:
        {
	  CHECK_STACK(2)
	  celXmlArg a_index2 = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefarr2_str arr=%s index1=%s index2=%s\n", i-1,
	  	A2S (op.arg), A2S (top), A2S (a_index2)));
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (op.arg.arg.str.s,
	      ArgToString (top), a_index2);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;
      case CEL_OPERATION_DEREFARR2:
        {
	  CHECK_STACK(3)
	  celXmlArg a_index2 = stack.Pop ();
	  celXmlArg a_index1 = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefarr2 arr=%s index1=%s index2=%s\n", i-1,
	  	A2S (top), A2S (a_index1), A2S (a_index2)));
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (ArgToString (top),
	      ArgToString (a_index1), a_index2);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;

      case CEL_OPERATION_DEREFARRENT_STR:
        {
	  CHECK_STACK(2)
	  celXmlArg a_index = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefarrent_str entity=%s arr=%s index=%s\n", i-1,
	  	A2S (top), A2S (op.arg), A2S (a_index)));
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'derefarrent_str'!",
	    	EntityNameForError (top));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (cbl,
	    	"Entity '%s' doesn't have 'pcproperties'!",
	    	EntityNameForError (top));
	  const char* varname = ArgToString (op.arg.arg.str.s, a_index);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;
      case CEL_OPERATION_DEREFARRENT:
        {
	  CHECK_STACK(3)
	  celXmlArg a_index = stack.Pop ();
	  celXmlArg a_array = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefarrent entity=%s arr=%s index=%s\n", i-1,
	  	A2S (top), A2S (a_array), A2S (a_index)));
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'derefarrent_str'!",
	    	EntityNameForError (top));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (cbl,
	    	"Entity '%s' doesn't have 'pcproperties'!",
	    	EntityNameForError (top));
	  const char* varname = ArgToString (ArgToString (a_array), a_index);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;

      case CEL_OPERATION_DEREFARR_STR:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefarr_str arr=%s index=%s\n", i-1,
	  	A2S (op.arg), A2S (top)));
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (op.arg.arg.str.s, top);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;
      case CEL_OPERATION_DEREFARR:
        {
	  CHECK_STACK(2)
	  celXmlArg a_index = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefarr arr=%s index=%s\n", i-1,
	  	A2S (top), A2S (a_index)));
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (ArgToString (top), a_index);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;

      case CEL_OPERATION_DEREFRVAR_STR:
        {
	  iPcRules* rules = GetRules (entity, behave);
	  if (rules)
	  {
	    DUMP_EXEC ((":%04d: derefrvar_str %s\n", i-1, A2S (op.arg)));
	    const char* varname = op.arg.arg.str.s;
	    celData ret;
	    rules->GetProperty (varname, ret);
	    size_t si = stack.Push (celXmlArg ());
	    if (!celData2celXmlArg (ret, stack[si]))
	      return ReportError (cbl, "Property '%s' has wrong type!", varname);
	    break;
	  }
	  // FALL THRU!
	}
      case CEL_OPERATION_DEREFVAR_STR:
        {
          DUMP_EXEC ((":%04d: derefvar_str %s\n", i-1, A2S (op.arg)));
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = op.arg.arg.str.s;
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  size_t si = stack.Push (celXmlArg ());
	  if (!prop2celXmlArg (props, idx, stack[si]))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;
      case CEL_OPERATION_DEREFRVAR:
        {
	  iPcRules* rules = GetRules (entity, behave);
	  if (rules)
	  {
	    CHECK_STACK(1)
	    celXmlArg& top = stack.Top ();
            DUMP_EXEC ((":%04d: derefrvar %s\n", i-1, A2S (top)));
	    const char* varname = ArgToString (top);
	    celData ret;
	    rules->GetProperty (varname, ret);
	    if (!celData2celXmlArg (ret, top))
	      return ReportError (cbl, "Property '%s' has wrong type!", varname);
	    break;
	  }
	  // FALL THRU!
	}
      case CEL_OPERATION_DEREFVAR:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefvar %s\n", i-1, A2S (top)));
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (top);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl, "Can't find variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;
      case CEL_OPERATION_DEREFRVARENT_STR:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  iPcRules* rules = GetRules (other_ent, 0);
	  if (rules)
	  {
            DUMP_EXEC ((":%04d: derefrvarent_str ent=%s var=%s\n", i-1,
		A2S (top), A2S (op.arg)));
	    const char* varname = op.arg.arg.str.s;
	    celData ret;
	    rules->GetProperty (varname, ret);
	    if (!celData2celXmlArg (ret, top))
	      return ReportError (cbl, "Property '%s' has wrong type!", varname);
	    break;
	  }
	  // FALL THRU!
	}
      case CEL_OPERATION_DEREFVARENT_STR:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefvarent_str ent=%s var=%s\n", i-1,
		A2S (top), A2S (op.arg)));
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'derefvarent_str'!",
	    	EntityNameForError (top));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS_ENT (other_ent,
	  	iPcProperties);
	  if (!props)
	    return ReportError (cbl,
	    	"Entity '%s' doesn't have 'pcproperties'!",
	    	EntityNameForError (top));
	  const char* varname = op.arg.arg.str.s;
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl,
	    	"Entity doesn't have variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;
      case CEL_OPERATION_DEREFRVARENT:
        {
	  CHECK_STACK(2)
	  celXmlArg& s_ent = stack[stack.Length ()-2];
	  iCelEntity* other_ent = ArgToEntity (s_ent, pl);
	  iPcRules* rules = GetRules (other_ent, 0);
	  if (rules)
	  {
	    celXmlArg a_var = stack.Pop ();
	    celXmlArg& top = stack.Top ();
            DUMP_EXEC ((":%04d: derefrvarent ent=%s var=%s\n", i-1, A2S (top),
	  	A2S (a_var)));
	    const char* varname = ArgToString (a_var);
	    celData ret;
	    rules->GetProperty (varname, ret);
	    if (!celData2celXmlArg (ret, top))
	      return ReportError (cbl, "Property '%s' has wrong type!", varname);
	    break;
	  }
	  // FALL THRU!
	}
      case CEL_OPERATION_DEREFVARENT:
        {
	  CHECK_STACK(2)
	  celXmlArg a_var = stack.Pop ();
	  celXmlArg& top = stack.Top ();
          DUMP_EXEC ((":%04d: derefvarent ent=%s var=%s\n", i-1, A2S (top),
	  	A2S (a_var)));
	  iCelEntity* other_ent = ArgToEntity (top, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'derefvarent'!",
	    	EntityNameForError (top));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (cbl,
	    	"Entity '%s' doesn't have 'pcproperties'!",
	    	EntityNameForError (top));
	  const char* varname = ArgToString (a_var);
	  size_t idx = props->GetPropertyIndex (varname);
          if (idx == csArrayItemNotFound)
	    return ReportError (cbl,
	    	"Entity doesn't have variable '%s'!", varname);
	  if (!prop2celXmlArg (props, idx, top))
	    return ReportError (cbl, "Property '%s' has wrong type!", varname);
	}
        break;
      case CEL_OPERATION_ENTTHIS:
        {
	  size_t si = stack.Push (celXmlArg ());
	  stack[si].SetEntity (entity);
	}
	break;

      case CEL_OPERATION_ENT:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  iCelEntity* ent = ArgToEntity (top, pl);
	  top.SetEntity (ent);
	}
	break;

      case CEL_OPERATION_DESTROYPC:
        {
	  CHECK_STACK(1)
	  celXmlArg top = stack.Pop ();
          DUMP_EXEC ((":%04d: destroypropclass pc=%s\n", i-1, A2S (top)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: destroypropclass %s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (top));
	    fflush (stdout);
	  }
	  const char* s = ArgToString (top);
	  iCelPropertyClassList* plist = entity->GetPropertyClassList ();
	  iCelPropertyClass* pc = plist->FindByName (s);
	  if (pc) plist->Remove (pc);
	}
        break;
      case CEL_OPERATION_CREATEPC:
        {
	  CHECK_STACK(1)
	  celXmlArg top = stack.Pop ();
          DUMP_EXEC ((":%04d: createpropclass pc=%s\n", i-1, A2S (top)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: createpropclass %s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (top));
	    fflush (stdout);
	  }
	  const char* s = ArgToString (top);
	  iCelPropertyClass* pc = pl->CreatePropertyClass (entity, s);
	  if (!pc)
	    return ReportError (cbl, "Couldn't create property class '%s'!",
	    	s);
	}
        break;
      case CEL_OPERATION_CREATEPCENT:
        {
	  CHECK_STACK(2)
	  celXmlArg aent = stack.Pop ();
	  celXmlArg top = stack.Pop ();
          DUMP_EXEC ((":%04d: createpropclass entity=%s pc=%s\n", i-1,
		A2S (aent), A2S (top)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: createpropclass %s, %s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (aent), A2S (top));
	    fflush (stdout);
	  }
	  iCelEntity* ent = ArgToEntity (aent, pl);
	  if (!ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'createpropclass'!",
	    	EntityNameForError (aent));
	  const char* s = ArgToString (top);
	  iCelPropertyClass* pc = pl->CreatePropertyClass (ent, s);
	  if (!pc)
	    return ReportError (cbl, "Couldn't create property class '%s'!",
	    	s);
	}
        break;
      case CEL_OPERATION_DESTROYPCTAG:
        {
	  CHECK_STACK(2)
	  celXmlArg atag = stack.Pop ();
	  celXmlArg apc = stack.Pop ();
          DUMP_EXEC ((":%04d: destroypropclasstag pc=%s tag=%s\n",
	  	i-1, A2S (apc), A2S (atag)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: destroypropclasstag pc=%s tag=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (apc), A2S (atag));
	    fflush (stdout);
	  }
	  const char* s = ArgToString (apc);
	  const char* tag = ArgToString (atag);
	  iCelPropertyClassList* plist = entity->GetPropertyClassList ();
	  iCelPropertyClass* pc = plist->FindByNameAndTag (s, tag);
	  if (pc) plist->Remove (pc);
	}
        break;
      case CEL_OPERATION_CREATEPCTAG:
        {
	  CHECK_STACK(2)
	  celXmlArg atag = stack.Pop ();
	  celXmlArg apc = stack.Pop ();
          DUMP_EXEC ((":%04d: createpropclasstag pc=%s tag=%s\n",
	  	i-1, A2S (apc), A2S (atag)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: createpropclasstag pc=%s tag=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (apc), A2S (atag));
	    fflush (stdout);
	  }
	  const char* s = ArgToString (apc);
	  const char* tag = ArgToString (atag);
	  iCelPropertyClass* pc = pl->CreateTaggedPropertyClass (entity, s,
	  	tag);
	  if (!pc)
	    return ReportError (cbl,
	    	"Couldn't create property class '%s' with tag '%s'!",
	    	s, tag);
	}
        break;
      case CEL_OPERATION_CREATEPCENTTAG:
        {
	  CHECK_STACK(3)
	  celXmlArg atag = stack.Pop ();
	  celXmlArg aent = stack.Pop ();
	  celXmlArg apc = stack.Pop ();
          DUMP_EXEC ((":%04d: createpropclasstag entity=%s pc=%s tag=%s\n",
	  	i-1, A2S (aent), A2S (apc), A2S (atag)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: createpropclasstag entity=%s pc=%s tag=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (aent), A2S (apc), A2S (atag));
	    fflush (stdout);
	  }
	  iCelEntity* ent = ArgToEntity (aent, pl);
	  if (!ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'createpropclass'!",
	    	EntityNameForError (aent));
	  const char* s = ArgToString (apc);
	  const char* tag = ArgToString (atag);
	  iCelPropertyClass* pc = pl->CreateTaggedPropertyClass (ent, s,
	  	tag);
	  if (!pc)
	    return ReportError (cbl,
	    	"Couldn't create property class '%s' with tag '%s'!",
	    	s, tag);
	}
        break;

      case CEL_OPERATION_CREATEENTITYL:
        {
	  CHECK_STACK(3)
	  celXmlArg alayer = stack.Pop ();
	  celXmlArg abh = stack.Pop ();
	  celXmlArg aent = stack.Pop ();
	  DUMP_EXEC ((":%04d: createentitylayer %s (layer=%s behaviour=%s)\n",
	  	i-1, A2S (aent), A2S (alayer), A2S (abh)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: createentitylayer %s layer=%s behaviour=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (aent), A2S (alayer), A2S (abh));
	    fflush (stdout);
	  }
	  csRef<iCelEntity> ent = pl->CreateEntity ();
	  const char* entname = ArgToString (aent);
	  if (entname) ent->SetName (entname);
	  const char* layername = ArgToString (alayer);
	  iCelBlLayer* bl = pl->FindBehaviourLayer (layername);
	  if (!bl)
	    return ReportError (cbl,
	    	"Couldn't find behaviour layer '%s'!", layername);

	  const char* bhname = ArgToString (abh);
	  if (bhname)
	  {
	    iCelBehaviour* bh = bl->CreateBehaviour (ent, bhname);
	    if (!bh)
	      return ReportError (cbl,
	    	  "Couldn't create behaviour '%s' for '%s' and layer '%s'!",
	    	  bhname, entname, layername);
	  }
	}
        break;
      case CEL_OPERATION_CREATEENTITY:
        {
	  CHECK_STACK(2)
	  celXmlArg abh = stack.Pop ();
	  celXmlArg aent = stack.Pop ();
	  DUMP_EXEC ((":%04d: createentity %s (behaviour=%s)\n", i-1,
	  	A2S (aent), A2S (abh)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: createentity %s behaviour=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (aent), A2S (abh));
	    fflush (stdout);
	  }
	  csRef<iCelEntity> ent = pl->CreateEntity ();
	  const char* entname = ArgToString (aent);
	  if (entname) ent->SetName (entname);
	  csRef<iCelBlLayer> bl = CS_QUERY_REGISTRY (
	  	cbl->GetObjectRegistry (), iCelBlLayer);
	  if (!bl)
	    return ReportError (cbl,
	    	"Couldn't find default behaviour layer!");

	  const char* bhname = ArgToString (abh);
	  if (bhname)
	  {
	    iCelBehaviour* bh = bl->CreateBehaviour (ent, bhname);
	    if (!bh)
	      return ReportError (cbl,
	    	  "Couldn't create behaviour '%s' for '%s'!",
	    	  bhname, entname);
	  }
	}
        break;
      case CEL_OPERATION_CREATEENTITYTPL:
        {
	  CHECK_STACK(2)
	  celXmlArg atpl = stack.Pop ();
	  celXmlArg aname = stack.Pop ();
	  DUMP_EXEC ((":%04d: createentity (template=%s, name=%s)\n", i-1,
	  	A2S (atpl), A2S (aname)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: createentity template=%s name=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (atpl), A2S (aname));
	    fflush (stdout);
	  }
	  const char* tplname = ArgToString (atpl);
	  iCelEntityTemplate* entpl = pl -> FindEntityTemplate (tplname);
	  if (!entpl)
	    return ReportError (cbl,
	    	"Couldn't find entity template '%s'!", tplname);
	  const char* entname = ArgToString (aname);
	  csRef<iCelEntity> ent = pl->CreateEntity (entpl, entname
	      ? entname : tplname, template_params);
	}
        break;
      case CEL_OPERATION_CLEARTPLPARAM:
	{
	  DUMP_EXEC ((":%04d: cleartplparam\n", i-1));
	  template_params.Empty ();
	}
	break;
      case CEL_OPERATION_TPLPARAM:
        {
	  CHECK_STACK(2)
	  celXmlArg a_val = stack.Pop ();
	  celXmlArg a_name = stack.Pop ();
	  DUMP_EXEC ((":%04d: tplparam name=%s val=%s\n", i-1,
	  	A2S (a_name), A2S (a_val)));
	  template_params.Put (ArgToString (a_name), ArgToStringTpl (a_val));
        }
        break;
      case CEL_OPERATION_CALL_I:
        {
	  celXmlScriptEventHandler* handler = op.arg.arg.h.h_true;
	  CS_ASSERT (handler != 0);
	  DUMP_EXEC ((":%04d: call_i %s\n", i-1, handler->GetName ()));
	  celData ret;
	  cbl->call_stack.Push (handler->GetName ());
	  cbl->call_stack_params.Push (params);
	  cbl->call_stack_entity.Push (entity);
	  if (!handler->Execute (entity, cbl, behave, ret, params))
	    return false;
	  cbl->call_stack_entity.Pop ();
	  cbl->call_stack_params.Pop ();
	  cbl->call_stack.Pop ();
	}
        break;
      case CEL_OPERATION_CALL_E:
        {
	  CHECK_STACK(2)
	  celXmlArg aevent = stack.Pop ();
	  celXmlArg aent = stack.Pop ();
	  DUMP_EXEC ((":%04d: call_e ent=%s event=%s\n", i-1, A2S (aent),
	  	A2S (aevent)));
	  iCelEntity* ent = ArgToEntity (aent, pl);
	  if (!ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'callent'!",
	    	EntityNameForError (aent));
	  const char* eventname = ArgToString (aevent);
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  iCelBehaviour* bh = ent->GetBehaviour ();
	  if (bh)
	    bh->SendMessage (eventname, 0, ret, action_params);
	}
        break;
      case CEL_OPERATION_CALL_ER:
        {
	  CHECK_STACK(3)
	  celXmlArg aevent = stack.Pop ();
	  celXmlArg aent = stack.Pop ();
	  celXmlArg aret = stack.Pop ();
	  DUMP_EXEC ((":%04d: call_er ent=%s event=%s return=%s\n", i-1,
		A2S (aent), A2S (aevent), A2S (aret)));
	  iCelEntity* ent = ArgToEntity (aent, pl);
	  if (!ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'callent'!",
	    	EntityNameForError (aent));
	  const char* eventname = ArgToString (aevent);
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  iCelBehaviour* bh = ent->GetBehaviour ();
	  if (bh)
	    bh->SendMessage (eventname, 0, ret, action_params);
	  else
	    ret.Set ((int32)-1);
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (aret);
	  if (!varname)
	    return ReportError (cbl, "Illegal variable name!");
	  if (!celData2prop (ret, props, varname))
	    return ReportError (cbl, "Bad type of value for call/return!");
	}
        break;
      case CEL_OPERATION_CALL:
        {
	  CHECK_STACK(1)
	  celXmlArg aevent = stack.Pop ();
	  DUMP_EXEC ((":%04d: call event=%s\n", i-1, A2S (aevent)));
	  const char* eventname = ArgToString (aevent);
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  behave->SendMessage (eventname, 0, ret, action_params);
	}
        break;
      case CEL_OPERATION_CALL_ERS:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: call_ers entity=%s event=%s\n", i-1,
	  	A2S (top), A2S (op.arg)));
	  iCelEntity* ent = ArgToEntity (top, pl);
	  if (!ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'callent_restack'!",
	    	EntityNameForError (top));
	  const char* eventname = op.arg.arg.str.s;
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  iCelBehaviour* bh = ent->GetBehaviour ();
	  if (bh)
	    bh->SendMessage (eventname, 0, ret, action_params);
	  else
	    ret.Set ((int32)-1);
	  celData2celXmlArg (ret, top);
	}
        break;
      case CEL_OPERATION_CALL_ERS2:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: call_ers2 entity=%s event=%s\n", i-1,
	  	A2S (top), A2S (op.arg)));
	  iCelEntity* ent = ArgToEntity (top, pl);
	  if (!ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'callent_restack'!",
	    	EntityNameForError (top));
	  const char* eventname = op.arg.arg.str.s;
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  iCelBehaviour* bh = ent->GetBehaviour ();
	  if (bh)
	    bh->SendMessage (eventname, 0, ret, action_params);
	  else
	    ret.Set ((int32)-1);
	  celData2celXmlArg (ret, top);
	}
        break;
      case CEL_OPERATION_CALL_RS:
        {
	  DUMP_EXEC ((":%04d: call_rs event=%s\n", i-1, A2S (op.arg)));
	  const char* eventname = op.arg.arg.str.s;
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  behave->SendMessage (eventname, 0, ret, action_params);
	  size_t si = stack.Push (celXmlArg ());
	  celData2celXmlArg (ret, stack[si]);
	}
        break;
      case CEL_OPERATION_CALL_R:
        {
	  CHECK_STACK(2)
	  celXmlArg aevent = stack.Pop ();
	  celXmlArg aret = stack.Pop ();
	  DUMP_EXEC ((":%04d: call_r event=%s return=%s\n", i-1,
	  	A2S (aevent), A2S (aret)));
	  const char* eventname = ArgToString (aevent);
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  behave->SendMessage (eventname, 0, ret, action_params);
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (aret);
	  if (!varname)
	    return ReportError (cbl, "Illegal variable name!");
	  if (!celData2prop (ret, props, varname))
	    return ReportError (cbl, "Bad type of value for call/return!");
	}
        break;
      case CEL_OPERATION_RETURN:
        {
	  CHECK_STACK(1)
	  celXmlArg a_val = stack.Pop ();
	  DUMP_EXEC ((":%04d: return val=%s\n", i-1, A2S (a_val)));
	  switch (a_val.type)
	  {
	    case CEL_DATA_ENTITY: ret.Set (a_val.arg.entity); break;
	    case CEL_DATA_IBASE: ret.SetIBase (a_val.arg.ref); break;
	    case CEL_DATA_PCLASS: ret.Set (a_val.arg.pc); break;
	    case CEL_DATA_LONG: ret.Set (a_val.arg.i); break;
	    case CEL_DATA_ULONG: ret.Set (a_val.arg.ui); break;
	    case CEL_DATA_BOOL: ret.Set (a_val.arg.b); break;
	    case CEL_DATA_FLOAT: ret.Set (a_val.arg.f); break;
	    case CEL_DATA_STRING: ret.Set (a_val.arg.str.s); break;
	    case CEL_DATA_VECTOR2:
	      {
	        csVector2 v;
		v.x = a_val.arg.vec.x;
		v.y = a_val.arg.vec.y;
	        ret.Set (v);
	      }
	      break;
	    case CEL_DATA_VECTOR3:
	      {
	        csVector3 v;
		v.x = a_val.arg.vec.x;
		v.y = a_val.arg.vec.y;
		v.z = a_val.arg.vec.z;
	        ret.Set (v);
	      }
	      break;
	    case CEL_DATA_COLOR:
	      {
	        csColor v;
		v.red = a_val.arg.col.red;
		v.green = a_val.arg.col.green;
		v.blue = a_val.arg.col.blue;
	        ret.Set (v);
	      }
	      break;
	    default:
	      return ReportError (cbl, "Bad type of value for return!");
	  }
	}
	break;
      case CEL_OPERATION_QUIT:
        {
          DUMP_EXEC ((":%04d: quit\n", i-1));
          behave->Quit ();
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
	  if (!pc)
	    return ReportError (cbl,
	    	  "No valid property class for 'action'!");
	  csStringID id = ArgToID (a_id);
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  pc->PerformAction (id, action_params, ret);
	}
        break;
      case CEL_OPERATION_ACTIONFUN:
        {
	  CHECK_STACK(2)
	  celXmlArg a_id = stack.Pop ();
	  celXmlArg& a_pc = stack.Top ();
          DUMP_EXEC ((":%04d: actionfun pc=%s id=%d\n", i-1, A2S (a_pc),
	  	A2S (a_id)));
	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc) pc = default_pc;
	  if (!pc)
	    return ReportError (cbl,
	    	  "No valid property class for 'action'!");
	  csStringID id = ArgToID (a_id);
	  csRef<iCelParameterBlock> ref = action_params;
	  celData ret;
	  bool rc = pc->PerformAction (id, action_params, ret);
	  if (ret.type == CEL_DATA_NONE)
	    a_pc.Set (rc);
	  else
	    celData2celXmlArg (ret, a_pc);
	}
        break;
      case CEL_OPERATION_DESTROYENTITY:
        {
	  CHECK_STACK(1)
	  celXmlArg aent = stack.Pop ();
	  DUMP_EXEC ((":%04d: destroyentity %s\n", i-1, A2S (aent)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: destroyentity %s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (aent));
	    fflush (stdout);
	  }
	  iCelEntity* ent = ArgToEntity (aent, pl);
	  if (!ent)
	    return ReportError (cbl,
	    	  "Couldn't find entity with name '%s' for 'destroyentity'!",
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
	  DUMP_EXEC ((":%04d: bb_movelayer layer=%s x=%s y=%s\n", i-1,
	  	A2S (a_layer),
	  	A2S (a_x), A2S (a_y)));
	  iBillboardManager* bbmgr = cbl->GetBillboardManager ();
	  const char* layername = ArgToString (a_layer);
	  iBillboardLayer* layer = bbmgr->FindBillboardLayer (layername);
	  if (!layer)
	    layer = bbmgr->CreateBillboardLayer (layername);
	  layer->SetOffset (ArgToInt32 (a_x), ArgToInt32 (a_y));
	}
	break;
      case CEL_OPERATION_BB_MOVEDELTA_E:
        {
	  CHECK_STACK(4)
	  celXmlArg adelta = stack.Pop ();
	  celXmlArg ay = stack.Pop ();
	  celXmlArg ax = stack.Pop ();
	  celXmlArg top = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_movedelta_e pc=%s x=%s y=%s delta=%s\n",
	  	i-1, A2S (top), A2S (ax), A2S (ay), A2S (adelta)));
	  int32 x = ArgToInt32 (ax);
	  int32 y = ArgToInt32 (ay);
	  int32 delta = ArgToInt32 (adelta);
	  iCelPropertyClass* pc = ArgToPClass (top);
	  if (!pc)
	    return ReportError (cbl, "Bad property class!\n");
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb)
	    return ReportError (cbl,
	    	"Property class is not a billboard!\n");
	  other_bb->GetBillboard ()->MoveToPosition (delta, x, y);
        }
	break;
      case CEL_OPERATION_BB_MOVEDELTA:
        {
	  CHECK_STACK(3)
	  celXmlArg adelta = stack.Pop ();
	  celXmlArg ay = stack.Pop ();
	  celXmlArg ax = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_movedelta x=%s y=%s delta=%s\n", i-1, A2S (ax),
	  	A2S (ay), A2S (adelta)));
	  int32 x = ArgToInt32 (ax);
	  int32 y = ArgToInt32 (ay);
	  int32 delta = ArgToInt32 (adelta);
	  behave->GetBillboard ()->GetBillboard ()->MoveToPosition (
	  	delta, x, y);
	}
	break;
      case CEL_OPERATION_BB_TOFRONT:
        {
	  DUMP_EXEC ((":%04d: bb_tofront\n", i-1));
	  behave->GetBillboard ()->GetBillboard ()->StackTop ();
	}
	break;
      case CEL_OPERATION_BB_TOBACK:
        {
	  DUMP_EXEC ((":%04d: bb_toback\n", i-1));
	  behave->GetBillboard ()->GetBillboard ()->StackBottom ();
	}
	break;
      case CEL_OPERATION_BB_UP:
        {
	  DUMP_EXEC ((":%04d: bb_up\n", i-1));
	  behave->GetBillboard ()->GetBillboard ()->StackUp ();
	}
	break;
      case CEL_OPERATION_BB_DOWN:
        {
	  DUMP_EXEC ((":%04d: bb_down\n", i-1));
	  behave->GetBillboard ()->GetBillboard ()->StackDown ();
	}
	break;
      case CEL_OPERATION_BB_TOFRONT_E:
        {
	  CHECK_STACK(1)
	  celXmlArg top = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_tofront_e pc=%s\n", i-1, A2S (top)));
	  iCelPropertyClass* pc = ArgToPClass (top);
	  if (!pc)
	    return ReportError (cbl, "Bad property class!\n");
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb)
	    return ReportError (cbl,
	    	"Property class is not a billboard!\n");
	  other_bb->GetBillboard ()->StackTop ();
	}
	break;
      case CEL_OPERATION_BB_TOBACK_E:
        {
	  CHECK_STACK(1)
	  celXmlArg top = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_toback_e pc=%s\n", i-1, A2S (top)));
	  iCelPropertyClass* pc = ArgToPClass (top);
	  if (!pc)
	    return ReportError (cbl, "Bad property class!\n");
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb)
	    return ReportError (cbl,
	    	"Property class is not a billboard!\n");
	  other_bb->GetBillboard ()->StackBottom ();
	}
	break;
      case CEL_OPERATION_BB_UP_E:
        {
	  CHECK_STACK(1)
	  celXmlArg top = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_up_e pc=%s\n", i-1, A2S (top)));
	  iCelPropertyClass* pc = ArgToPClass (top);
	  if (!pc)
	    return ReportError (cbl, "Bad property class!\n");
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb)
	    return ReportError (cbl,
	    	"Property class is not a billboard!\n");
	  other_bb->GetBillboard ()->StackUp ();
	}
	break;
      case CEL_OPERATION_BB_DOWN_E:
        {
	  CHECK_STACK(1)
	  celXmlArg top = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_down_e pc=%s\n", i-1, A2S (top)));
	  iCelPropertyClass* pc = ArgToPClass (top);
	  if (!pc)
	    return ReportError (cbl, "Bad property class!\n");
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb)
	    return ReportError (cbl,
	    	"Property class is not a billboard!\n");
	  other_bb->GetBillboard ()->StackDown ();
	}
	break;
      case CEL_OPERATION_BB_MOVE:
        {
	  CHECK_STACK(2)
	  celXmlArg ay = stack.Pop ();
	  celXmlArg ax = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_move x=%s y=%s\n", i-1, A2S (ax), A2S (ay)));
	  int32 x = ArgToInt32 (ax);
	  int32 y = ArgToInt32 (ay);
	  behave->GetBillboard ()->GetBillboard ()->SetPosition (x, y);
	}
	break;
      case CEL_OPERATION_BB_MOVE_E:
        {
	  CHECK_STACK(3)
	  celXmlArg ay = stack.Pop ();
	  celXmlArg ax = stack.Pop ();
	  celXmlArg top = stack.Pop ();
	  DUMP_EXEC ((":%04d: bb_move_e pc=%s x=%s y=%s\n", i-1, A2S (top), A2S (ax), A2S (ay)));
	  int32 x = ArgToInt32 (ax);
	  int32 y = ArgToInt32 (ay);
	  iCelPropertyClass* pc = ArgToPClass (top);
	  if (!pc)
	    return ReportError (cbl, "Bad property class!\n");
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb)
	    return ReportError (cbl,
	    	"Property class is not a billboard!\n");
	  other_bb->GetBillboard ()->SetPosition (x, y);
	}
	break;
      case CEL_OPERATION_BB_TESTCOLLIDE:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: bb_testcollide %s\n", i-1, A2S (top)));
	  iCelPropertyClass* pc = ArgToPClass (top);
	  if (!pc)
	    return ReportError (cbl, "Bad property class!\n");

	  // @@@ Efficiency?
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb)
	    return ReportError (cbl,
	    	"Property class is not a billboard!\n");
	  iPcBillboard* bb = behave->GetBillboard ();
	  if (!bb)
	    return ReportError (cbl,
	    	"This entity does not have a pcbillboard!\n");
	  iBillboardManager* bbmgr = cbl->GetBillboardManager ();
	  if (!bbmgr)
	    return ReportError (cbl,
	    	"Billboard manager is missing!\n");
	  top.Set (bbmgr->TestCollision (bb->GetBillboard (),
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
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (a_var);
	  if (!varname)
	    return ReportError (cbl, "Illegal variable name!");
	  int32 start = ArgToInt32 (a_start);
	  int32 end = ArgToInt32 (a_end);
	  int32 v;
	  celXmlScriptEventHandler* truebranch = op.arg.arg.h.h_true;
	  CS_ASSERT (truebranch != 0);
	  celData ret;
	  cbl->call_stack.Push (truebranch->GetName ());
	  cbl->call_stack_params.Push (params);
	  cbl->call_stack_entity.Push (entity);
	  for (v = start ; v <= end ; v++)
	  {
	    props->SetProperty (varname, (long)v);
	    if (!truebranch->Execute (entity, cbl, behave, ret, params))
	      return false;
	  }
	  cbl->call_stack_entity.Pop ();
	  cbl->call_stack_params.Pop ();
	  cbl->call_stack.Pop ();
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
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (a_var);
	  if (!varname)
	    return ReportError (cbl, "Illegal variable name!");
	  char* copy_varname = csStrNew (varname);
	  int32 start = ArgToInt32 (a_start);
	  int32 end = ArgToInt32 (a_end);
	  int32 v;
	  size_t endlocation = op.arg.arg.codelocation;
	  celData ret;
	  cbl->call_stack.Push (GetName ());
	  cbl->call_stack_params.Push (params);
	  cbl->call_stack_entity.Push (entity);
	  for (v = start ; v <= end ; v++)
	  {
	    props->SetProperty (copy_varname, (long)v);
	    if (!Execute (entity, cbl, behave, ret, params, i, false))
	      return false;
	  }
	  cbl->call_stack_entity.Pop ();
	  cbl->call_stack_params.Pop ();
	  cbl->call_stack.Pop ();
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
	  if (!EvaluateTrue (eval, cbl, rc)) return false;
	  if (rc)
	    stack.Push (ela);
	  else
	    stack.Push (elb);
	}
	break;
      case CEL_OPERATION_DROP:
        {
	  CHECK_STACK(1)
	  DUMP_EXEC ((":%04d: drop %s\n", i-1, A2S (stack[stack.Length ()-1])));
	  stack.Pop ();
	}
	break;
      case CEL_OPERATION_CLEARSTACK:
        {
	  DUMP_EXEC ((":%04d: clearstack\n", i-1));
	  stack.DeleteAll ();
	}
	break;
      case CEL_OPERATION_DUP:
        {
	  CHECK_STACK(1)
	  size_t si = stack.Length ()-1;
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
	  if (!EvaluateTrue (eval, cbl, rc)) return false;
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
	  if (!EvaluateTrue (eval, cbl, rc)) return false;
	  celData ret;
	  if (rc)
	  {
	    celXmlScriptEventHandler* truebranch = op.arg.arg.h.h_true;
	    if (truebranch)
	    {
	      cbl->call_stack.Push (truebranch->GetName ());
	      cbl->call_stack_params.Push (params);
	      cbl->call_stack_entity.Push (entity);
	      if (!truebranch->Execute (entity, cbl, behave, ret, params))
	        return false;
	      cbl->call_stack_entity.Pop ();
	      cbl->call_stack_params.Pop ();
	      cbl->call_stack.Pop ();
	    }
	  }
	  else
	  {
	    celXmlScriptEventHandler* falsebranch = op.arg.arg.h.h_false;
	    if (falsebranch)
	    {
	      cbl->call_stack.Push (falsebranch->GetName ());
	      cbl->call_stack_params.Push (params);
	      cbl->call_stack_entity.Push (entity);
	      if (!falsebranch->Execute (entity, cbl, behave, ret, params))
	        return false;
	      cbl->call_stack_entity.Pop ();
	      cbl->call_stack_params.Pop ();
	      cbl->call_stack.Pop ();
	    }
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
      case CEL_OPERATION_INHERITPARAMS:
        {
	  DUMP_EXEC ((":%04d: inheritparams\n", i-1));
          action_params = params;
        }
        break;
      case CEL_OPERATION_ACTIONPARAMS:
        {
	  DUMP_EXEC ((":%04d: actionparams\n", i-1));
          action_params = csPtr<iCelParameterBlock> (
		(iCelParameterBlock*)new celVariableParameterBlock ());
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
	  celVariableParameterBlock* vb = (celVariableParameterBlock*)(
	      	iCelParameterBlock*) action_params;
	  vb->SetParameterDef (op.arg.arg.ui, ArgToID (a_id), "");
	  celData& data = vb->GetParameter ((int)op.arg.arg.ui);
	  switch (a_val.type)
	  {
	    case CEL_DATA_ENTITY: data.Set (a_val.arg.entity); break;
	    case CEL_DATA_IBASE: data.SetIBase (a_val.arg.ref); break;
	    case CEL_DATA_PCLASS: data.Set (a_val.arg.pc); break;
	    case CEL_DATA_LONG: data.Set (a_val.arg.i); break;
	    case CEL_DATA_ULONG: data.Set (a_val.arg.ui); break;
	    case CEL_DATA_BOOL: data.Set (a_val.arg.b); break;
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
	      return ReportError (cbl, "Bad type of value for action/param!");
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
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'varent_str'!",
	    	EntityNameForError (a_ent));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (cbl,
	    	"Entity '%s' doesn't have 'pcproperties'!",
			EntityNameForError (a_ent));

	  const char* varname = op.arg.arg.str.s;
	  if (!varname)
	    return ReportError (cbl, "Illegal variable name!");
	  if (!celXmlArg2prop (val, props, varname))
	    return ReportError (cbl, "Bad type of value for var!");
	}
	break;
      case CEL_OPERATION_VARENT:
        {
	  CHECK_STACK(3)
	  celXmlArg val = stack.Pop ();
	  celXmlArg var = stack.Pop ();
	  celXmlArg a_ent = stack.Pop ();
	  DUMP_EXEC ((":%04d: varent ent=%s var=%s value=%s\n", i-1,
	  	A2S (a_ent), A2S (var), A2S (val)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: varent ent=%s var=%s value=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (a_ent), A2S (var), A2S (val));
	    fflush (stdout);
	  }

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'varent'!",
		EntityNameForError (a_ent));
	  csRef<iPcProperties> props = CEL_QUERY_PROPCLASS (
	  	other_ent->GetPropertyClassList (), iPcProperties);
	  if (!props)
	    return ReportError (cbl,
	    	"Entity '%s' doesn't have 'pcproperties'!",
			EntityNameForError (a_ent));

	  const char* varname = ArgToString (var);
	  if (!varname)
	    return ReportError (cbl, "Illegal variable name!");
	  if (!celXmlArg2prop (val, props, varname))
	    return ReportError (cbl, "Bad type of value for var!");
	}
	break;
      case CEL_OPERATION_VAR_STR:
        {
	  CHECK_STACK(1)
	  celXmlArg val = stack.Pop ();
	  DUMP_EXEC ((":%04d: var_str var=%s value=%s\n", i-1, A2S (op.arg),
	  	A2S (val)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: var_str var=%s value=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (op.arg), A2S (val));
	    fflush (stdout);
	  }

	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = op.arg.arg.str.s;
	  if (!varname)
	    return ReportError (cbl, "Illegal variable name!");
	  if (!celXmlArg2prop (val, props, varname))
	    return ReportError (cbl, "Bad type of value for var!");
	}
	break;
      case CEL_OPERATION_VAR:
        {
	  CHECK_STACK(2)
	  celXmlArg val = stack.Pop ();
	  celXmlArg var = stack.Pop ();
	  DUMP_EXEC ((":%04d: var var=%s value=%s\n", i-1, A2S (var),
	  	A2S (val)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: var var=%s value=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (var), A2S (val));
	    fflush (stdout);
	  }

	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* varname = ArgToString (var);
	  if (!varname)
	    return ReportError (cbl, "Illegal variable name!");
	  if (!celXmlArg2prop (val, props, varname))
	    return ReportError (cbl, "Bad type of value for var!");
	}
	break;
      case CEL_OPERATION_GETPROPERTY1:
        {
	  CHECK_STACK(1)
	  celXmlArg a_id = stack.Pop ();
	  DUMP_EXEC ((":%04d: getproperty1 id=%s\n", i-1, A2S (a_id)));
	  size_t si = stack.Push (celXmlArg ());

	  iCelPropertyClass* pc = default_pc;
	  if (!pc)
	    return ReportError (cbl, "Default property class is 0!");
	  csStringID id = ArgToID (a_id);
	  if (!pcProp2celXmlArg (pc, id, stack[si]))
	    return ReportError (cbl, "Type not supported for getproperty1!");
	}
	break;
      case CEL_OPERATION_GETPROPERTY:
        {
	  CHECK_STACK(2)
	  celXmlArg a_id = stack.Pop ();
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: getproperty pc=%s id=%s\n", i-1, A2S (top),
	  	A2S (a_id)));

	  iCelPropertyClass* pc = ArgToPClass (top);
	  if (!pc)
	    return ReportError (cbl, "Property class is 0!");
	  csStringID id = ArgToID (a_id);
	  if (!pcProp2celXmlArg (pc, id, top))
	    return ReportError (cbl, "Type not supported for getproperty!");
	}
	break;
      case CEL_OPERATION_INVENTORY_ADD:
        {
	  CHECK_STACK(1)
	  celXmlArg a_ent = stack.Pop ();
	  DUMP_EXEC ((":%04d: inventory_add ent=%s\n", i-1, A2S (a_ent)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: inventory_add ent=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (a_ent));
	    fflush (stdout);
	  }

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for inventory_add'!",
		EntityNameForError (a_ent));
	  if (!default_inv)
	    return ReportError (cbl, "Default inventory isn't set!");
	  default_inv->AddEntity (other_ent);
	}
	break;
      case CEL_OPERATION_INVENTORY_REM:
        {
	  CHECK_STACK(1)
	  celXmlArg a_ent = stack.Pop ();
	  DUMP_EXEC ((":%04d: inventory_rem ent=%s\n", i-1, A2S (a_ent)));
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: inventory_rem ent=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (a_ent));
	    fflush (stdout);
	  }

	  iCelEntity* other_ent = ArgToEntity (a_ent, pl);
	  if (!other_ent)
	    return ReportError (cbl,
	    	"Couldn't find entity '%s' for 'inventory_rem'!",
		EntityNameForError (a_ent));
	  if (!default_inv)
	    return ReportError (cbl, "Default inventory isn't set!");
	  default_inv->RemoveEntity (other_ent);
	}
	break;
      case CEL_OPERATION_INVENTORY_COUNT:
        {
	  DUMP_EXEC ((":%04d: inventory_count ()\n", i-1));
	  if (!default_inv)
	    return ReportError (cbl, "Default inventory isn't set!");
	  size_t si = stack.Push (celXmlArg ());
	  stack[si].SetInt32 ((int32)default_inv->GetEntityCount ());
	}
	break;
      case CEL_OPERATION_INVENTORY_GET:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: inventory_get idx=%s\n", i-1, A2S (top)));
	  if (!default_inv)
	    return ReportError (cbl, "Default inventory isn't set!");
	  top.SetEntity (default_inv->GetEntity (ArgToInt32 (top)));
	}
	break;
      case CEL_OPERATION_INVENTORY_INNAME:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: inventory_inname name=%s\n", i-1, A2S (top)));
	  if (!default_inv)
	    return ReportError (cbl, "Default inventory isn't set!");
	  top.Set (default_inv->In (ArgToString (top)));
	}
	break;
      case CEL_OPERATION_INVENTORY_FINDNAME:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: inventory_findname name=%s\n", i-1, A2S (top)));
	  if (!default_inv)
	    return ReportError (cbl, "Default inventory isn't set!");
	  top.SetUInt32 ((uint32)default_inv->FindEntity (ArgToString (top)));
	}
	break;
      case CEL_OPERATION_INVENTORY_IN:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: inventory_in ent=%s\n", i-1, A2S (top)));
	  if (!default_inv)
	    return ReportError (cbl, "Default inventory isn't set!");
	  top.Set (default_inv->In (ArgToEntity (top, pl)));
	}
	break;
      case CEL_OPERATION_INVENTORY_FIND:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: inventory_find ent=%s\n", i-1, A2S (top)));
	  if (!default_inv)
	    return ReportError (cbl, "Default inventory isn't set!");
	  top.SetUInt32 ((uint32)default_inv->FindEntity (ArgToEntity (top, pl)));
	}
	break;
      case CEL_OPERATION_DEFAULTINV:
        {
	  CHECK_STACK(1)
	  celXmlArg a_pc = stack.Pop ();
	  DUMP_EXEC ((":%04d: defaultinv pc=%s\n", i-1, A2S (a_pc)));
	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc)
	    return ReportError (cbl, "Property class is 0!");
	  csRef<iPcInventory> inv = SCF_QUERY_INTERFACE (pc, iPcInventory);
	  if (!inv)
	    return ReportError (cbl, "Property class is not an inventory!");
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
	    return ReportError (cbl, "Property class is 0!");
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
	  if (varprop_trace)
	  {
	    printf (":%s/%04lu: property pc=%s id=%s val=%s\n",
	    	cbl->call_stack.Top (),
		(unsigned long)i-1, A2S (a_pc), A2S (a_id), A2S (a_val));
	    fflush (stdout);
	  }
	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc) pc = default_pc;
	  if (!pc)
	    return ReportError (cbl,
	    	  "No valid property class for 'property'!");
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
	    case CEL_DATA_IBASE:
	      pc->SetProperty (id, a_val.arg.ref);
	      break;
	    default:
	      return ReportError (cbl,
	      	"Bad type for setting property value!");
	  }
	}
        break;
      case CEL_OPERATION_CHDIRAUTO:
        {
	  CHECK_STACK(2)
	  celXmlArg a_file = stack.Pop ();
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: chdirauto dir=%s file=%s\n", i-1, A2S (top),
	  	A2S (a_file)));
	  csRef<iVFS> vfs = CS_QUERY_REGISTRY (cbl->GetObjectRegistry (),
	  	iVFS);
	  top.Set (vfs->ChDirAuto (ArgToString (top), 0, 0,
		ArgToString (a_file)));
	}
	break;
      case CEL_OPERATION_READFILE:
        {
	  CHECK_STACK(2)
	  celXmlArg a_file = stack.Pop ();
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: readfile vararray=%s file=%s\n", i-1, A2S (top),
	  	A2S (a_file)));
	  csRef<iVFS> vfs = CS_QUERY_REGISTRY (cbl->GetObjectRegistry (),
	  	iVFS);
	  csRef<iDataBuffer> buf = vfs->ReadFile (ArgToString (a_file));
	  if (!buf)
	  {
	    top.SetInt32 (-1);
	  }
	  else
	  {
	    iPcProperties* props = GetProperties (entity, behave);
	    if (!props) return ReportError (cbl, "Can't find properties!");

	    const char* vararray = ArgToString (top);
	    char* var = new char[strlen (vararray)+10];
	    strcpy (var, vararray);
	    char* varnum = var+strlen (var);

	    int idx = 0;
	    char* d = buf->GetData ();
	    while (d)
	    {
	      char* nl = strchr (d, '\n');
	      if (!nl) nl = d + strlen (d);
	      char old = *nl;
	      *nl = 0;
	      sprintf (varnum, "%d", idx);
	      idx++;
	      props->SetProperty (var, d);
	      *nl = old;
	      if (*nl == 0) d = 0;
	      else
	      {
	        nl++;
	        if (*nl == '\n') nl++;
		if (*nl == 0) d = 0;
	        else d = nl;
	      }
	    }
	    top.SetInt32 (idx);
	  }
	}
	break;
      case CEL_OPERATION_WRITEFILE:
        {
	  CHECK_STACK(4)
	  celXmlArg a_end = stack.Pop ();
	  celXmlArg a_start = stack.Pop ();
	  celXmlArg a_file = stack.Pop ();
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: writefile vararray=%s file=%s start=%s stop=%s\n",
	  	i-1, A2S (top), A2S (a_file), A2S (a_start), A2S (a_end)));
	  csRef<iVFS> vfs = CS_QUERY_REGISTRY (cbl->GetObjectRegistry (),
	  	iVFS);
	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* vararray = ArgToString (top);
	  char* var = new char[strlen (vararray)+10];
	  strcpy (var, vararray);
	  char* varnum = var+strlen (var);

	  int st = ArgToInt32 (a_start);
	  int en = ArgToInt32 (a_end);
	  csString data;
	  int i;
	  for (i = st ; i <= en ; i++)
	  {
	    sprintf (varnum, "%d", i);
	    size_t idx = props->GetPropertyIndex (var);
            if (idx == csArrayItemNotFound)
	      return ReportError (cbl, "Can't find variable '%s'!", var);
	    const char* l = props->GetPropertyString (idx);
	    data += l;
	    data += "\n";
	  }
	  bool success = vfs->WriteFile (ArgToString (a_file),
	  	(const char *)data, data.Length ());
	  top.Set (success);
	}
	break;
      case CEL_OPERATION_REPORTERROR:
        {
	  CHECK_STACK(1)
	  celXmlArg a_msg = stack.Pop ();
	  DUMP_EXEC ((":%04d: reporterror msg=%s\n", i-1, A2S (a_msg)));
	  return ReportError (cbl, ArgToString (a_msg));
	}
	break;
      case CEL_OPERATION_SOUNDFUN:
        {
	  CHECK_STACK(4)
	  celXmlArg a_play = stack.Pop ();
	  celXmlArg a_volume = stack.Pop ();
	  celXmlArg a_loop = stack.Pop ();
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: sound name=%s loop=%s vol=%s play=%s\n",
		i-1, A2S (top), A2S (a_loop), A2S (a_volume), A2S (a_play)));
	  csRef<iSndSysManager> sndmngr = CS_QUERY_REGISTRY (
	  	cbl->GetObjectRegistry (), iSndSysManager);
	  if (!sndmngr)
	    return ReportError (cbl, "Error! No sound manager!");
	  csRef<iSndSysWrapper> w = sndmngr->FindSoundByName(ArgToString (
		top));
	  if (w)
	  {
	    csRef<iSndSysRenderer> renderer = csQueryRegistryOrLoad<
	    	iSndSysRenderer> (cbl->GetObjectRegistry (),
		"crystalspace.sndsys.renderer.software");
	    if (!renderer)
	      return ReportError (cbl, "Error! No sound renderer!");
	    csRef<iSndSysSource> sound_source;
	    sound_source = renderer->CreateSource(w->GetStream());
	    sound_source->SetVolume (ArgToFloat (a_volume));
	    sound_source->GetStream ()->ResetPosition ();
	    sound_source->GetStream ()->SetLoopState(ArgToInt32 (a_loop));
	    if (ArgToBool (a_play))
	      sound_source->GetStream ()->Unpause ();
	    top.SetIBase (static_cast<iBase*> (sound_source));
	  }
	}
        break;
      case CEL_OPERATION_SOUNDPAUSED:
        {
	  CHECK_STACK(1)
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: sound_paused (%s)\n",
		i-1, A2S (top)));
	  iBase* src_ibase = ArgToIBase (top);
	  if (!src_ibase)
	    return ReportError (cbl, "Error! Sound source is null!");
	  csRef<iSndSysSource> sound_source = scfQueryInterface<
	    	iSndSysSource> (src_ibase);
	  if (!sound_source)
	    return ReportError (cbl, "Error! This is not a sound source!");
	  int pause = sound_source->GetStream ()->GetPauseState ();
	  if (pause == CS_SNDSYS_STREAM_PAUSED)
	    top.Set (true);
	  //else if (pause == CS_SNDSYS_STREAM_UNPAUSED)
	  //  top.Set (0);
	  else
	    top.Set (false);
	}
        break;
      case CEL_OPERATION_SOUND_STOP:
	{
	  CHECK_STACK(1)
	  celXmlArg a_source = stack.Pop ();
	  DUMP_EXEC ((":%04d: sound_stop source=%s\n",
		i-1, A2S (a_source)));
	  iBase* src_ibase = ArgToIBase (a_source);
	  if (!src_ibase)
	    return ReportError (cbl, "Error! Sound source is null!");
	  csRef<iSndSysSource> sound_source = scfQueryInterface<
	    	iSndSysSource> (src_ibase);
	  if (!sound_source)
	    return ReportError (cbl, "Error! This is not a sound source!");
	  csRef<iSndSysRenderer> renderer = csQueryRegistryOrLoad<
	    	iSndSysRenderer> (cbl->GetObjectRegistry (),
		"crystalspace.sndsys.renderer.software");
	  if (!renderer)
	    return ReportError (cbl, "Error! No sound renderer!");
	  renderer->RemoveSource (sound_source);
	}
	break;
      case CEL_OPERATION_SOUND_PAUSE:
	{
	  CHECK_STACK(1)
	  celXmlArg a_source = stack.Pop ();
	  DUMP_EXEC ((":%04d: sound_pause source=%s\n",
		i-1, A2S (a_source)));
	  iBase* src_ibase = ArgToIBase (a_source);
	  if (!src_ibase)
	    return ReportError (cbl, "Error! Sound source is null!");
	  csRef<iSndSysSource> sound_source = scfQueryInterface<
	    	iSndSysSource> (src_ibase);
	  if (!sound_source)
	    return ReportError (cbl, "Error! This is not a sound source!");
	  sound_source->GetStream ()->Pause ();
	}
	break;
      case CEL_OPERATION_SOUND_RESTART:
	{
	  CHECK_STACK(1)
	  celXmlArg a_source = stack.Pop ();
	  DUMP_EXEC ((":%04d: sound_restart source=%s\n",
		i-1, A2S (a_source)));
	  iBase* src_ibase = ArgToIBase (a_source);
	  if (!src_ibase)
	    return ReportError (cbl, "Error! Sound source is null!");
	  csRef<iSndSysSource> sound_source = scfQueryInterface<
	    	iSndSysSource> (src_ibase);
	  if (!sound_source)
	    return ReportError (cbl, "Error! This is not a sound source!");
	  sound_source->GetStream ()->ResetPosition ();
	  sound_source->GetStream ()->Unpause ();
	}
	break;
      case CEL_OPERATION_SOUND_UNPAUSE:
	{
	  CHECK_STACK(1)
	  celXmlArg a_source = stack.Pop ();
	  DUMP_EXEC ((":%04d: sound_unpause source=%s\n",
		i-1, A2S (a_source)));
	  iBase* src_ibase = ArgToIBase (a_source);
	  if (!src_ibase)
	    return ReportError (cbl, "Error! Sound source is null!");
	  csRef<iSndSysSource> sound_source = scfQueryInterface<
	    	iSndSysSource> (src_ibase);
	  if (!sound_source)
	    return ReportError (cbl, "Error! This is not a sound source!");
	  sound_source->GetStream ()->Unpause ();
	}
	break;
      case CEL_OPERATION_SOUND_VOLUME:
	{
	  CHECK_STACK(2)
	  celXmlArg a_volume = stack.Pop ();
	  celXmlArg a_source = stack.Pop ();
	  DUMP_EXEC ((":%04d: sound_volume source=%s volume=%s\n",
		i-1, A2S (a_source), A2S (a_volume)));
	  iBase* src_ibase = ArgToIBase (a_source);
	  if (!src_ibase)
	    return ReportError (cbl, "Error! Sound source is null!");
	  csRef<iSndSysSource> sound_source = scfQueryInterface<
	    	iSndSysSource> (src_ibase);
	  if (!sound_source)
	    return ReportError (cbl, "Error! This is not a sound source!");
	  sound_source->SetVolume (ArgToFloat (a_volume));
	}
	break;
      case CEL_OPERATION_SOUND:
        {
	  CHECK_STACK(3)
	  celXmlArg a_volume = stack.Pop ();
	  celXmlArg a_loop = stack.Pop ();
	  celXmlArg a_name = stack.Pop ();
	  DUMP_EXEC ((":%04d: sound name=%s loop=%s vol=%s\n",
		i-1, A2S (a_name), A2S (a_loop), A2S (a_volume)));
	  csRef<iSndSysManager> sndmngr = CS_QUERY_REGISTRY (
	  	cbl->GetObjectRegistry (), iSndSysManager);
	  if (!sndmngr)
	    return ReportError (cbl, "Error! No sound manager!");
	  csRef<iSndSysWrapper> w = sndmngr->FindSoundByName(ArgToString (
		a_name));
	  if (w)
	  {
	    csRef<iSndSysRenderer> renderer = csQueryRegistryOrLoad<
	    	iSndSysRenderer> (cbl->GetObjectRegistry (),
		"crystalspace.sndsys.renderer.software");
	    if (!renderer)
	      return ReportError (cbl, "Error! No sound renderer!");
	    csRef<iSndSysSource> sound_source;
	    sound_source = renderer->CreateSource(w->GetStream());
	    sound_source->SetVolume (ArgToFloat (a_volume));
	    sound_source->GetStream ()->ResetPosition ();
	    sound_source->GetStream ()->SetLoopState(ArgToInt32 (a_loop));
	    sound_source->GetStream ()->Unpause ();
	  }
	}
        break;
      case CEL_OPERATION_HITBEAM:
        {
	  CHECK_STACK(6)
	  celXmlArg a_meshvar = stack.Pop ();
	  celXmlArg a_entvar = stack.Pop ();
	  celXmlArg a_isectvar = stack.Pop ();
	  celXmlArg a_end = stack.Pop ();
	  celXmlArg a_start = stack.Pop ();
	  celXmlArg a_sector = stack.Pop ();
	  DUMP_EXEC ((":%04d: hitbeam sector=%s start=%s end=%s isectv=%s entv=%s meshv=%s\n",
		i-1, A2S (a_sector), A2S (a_start), A2S (a_end),
		A2S (a_isectvar), A2S (a_entvar), A2S (a_meshvar)));

	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* meshvarname = ArgToString (a_meshvar);
	  if (!meshvarname)
	    return ReportError (cbl,
		"Illegal variable name for 'hitbeam'!");
	  const char* entvarname = ArgToString (a_entvar);
	  if (!entvarname)
	    return ReportError (cbl,
		"Illegal variable name for 'hitbeam'!");
	  const char* isectvarname = ArgToString (a_isectvar);
	  if (!isectvarname)
	    return ReportError (cbl,
		"Illegal variable name for 'hitbeam'!");

	  const char* sectorname = ArgToString (a_sector);
	  if (!sectorname)
	    return ReportError (cbl,
		"'sectorname' is missing or invalid for 'hitbeam'!");
	  iEngine* engine = cbl->GetEngine ();
	  iSector* sector = engine->FindSector (sectorname);
	  if (!sector)
	    return ReportError (cbl,
		"Can't find sector '%s' for 'hitbeam'!", sectorname);
	  csVector3 start = ArgToVector3 (a_start);
	  csVector3 end = ArgToVector3 (a_end);
	  iCelEntity* selent;
	  csVector3 isect;
	  iMeshWrapper* mesh;
	  HitBeam (sector, start, end, isect, selent, mesh);

	  props->SetProperty (entvarname, selent);
	  props->SetProperty (isectvarname, isect);
	  props->SetProperty (meshvarname,
	  	mesh ? mesh->QueryObject ()->GetName () : "");
	}
	break;
      case CEL_OPERATION_SELECTENTITY:
        {
	  CHECK_STACK(6)
	  celXmlArg a_entvar = stack.Pop ();
	  celXmlArg a_isectvar = stack.Pop ();
	  celXmlArg a_maxdist = stack.Pop ();
	  celXmlArg a_screeny = stack.Pop ();
	  celXmlArg a_screenx = stack.Pop ();
	  celXmlArg a_pc = stack.Pop ();
	  DUMP_EXEC ((":%04d: selectentity pc=%s sx=%s sy=%s md=%s isectv=%s entv=%s\n",
		i-1, A2S (a_pc), A2S (a_screenx), A2S (a_screeny),
		A2S (a_maxdist), A2S (a_isectvar), A2S (a_entvar)));

	  iPcProperties* props = GetProperties (entity, behave);
	  if (!props) return ReportError (cbl, "Can't find properties!");
	  const char* entvarname = ArgToString (a_entvar);
	  if (!entvarname)
	    return ReportError (cbl,
		"Illegal variable name for 'selectentity'!");
	  const char* isectvarname = ArgToString (a_isectvar);
	  if (!isectvarname)
	    return ReportError (cbl,
		"Illegal variable name for 'selectentity'!");

	  iCelPropertyClass* pc = ArgToPClass (a_pc);
	  if (!pc)
	    return ReportError (cbl,
		"Illegal property class for 'selectentity'!");
	  csRef<iPcCamera> pccam = scfQueryInterface<iPcCamera> (pc);
	  if (!pccam)
	    return ReportError (cbl,
		"Property class is not a pccamera for 'selectentity'!");
	  int screenx = ArgToInt32 (a_screenx);
	  int screeny = ArgToInt32 (a_screeny);
	  float maxdist = ArgToFloat (a_maxdist);
	  if (maxdist < 0.000001f) maxdist = 1000000000.0f;
	  iCelEntity* selent;
	  csVector3 isect;
	  FindMouseTarget (pccam, screenx, screeny, maxdist, isect, selent);

	  props->SetProperty (entvarname, selent);
	  props->SetProperty (isectvarname, isect);
	}
	break;
      case CEL_OPERATION_GETYROT:
        {
	  CHECK_STACK(2)
	  celXmlArg a_dy = stack.Pop ();
	  celXmlArg& top = stack.Top ();
	  DUMP_EXEC ((":%04d: getyrot dx=%s dy=%s\n", i-1, A2S (top),
	  	A2S (a_dy)));
          csVector3 v1 = ArgToVector3 (top);
          csVector3 v2 = ArgToVector3 (a_dy);
	  top.SetFloat (GetAngle (v1, v2));
	}
	break;
    }
  }
}

void celXmlScriptEventHandler::FindMouseTarget (iPcCamera* pccam,
    int screenx, int screeny,
    float maxdist, csVector3& isect, iCelEntity*& selent)
{
  iCamera* camera = pccam->GetCamera ();
  csScreenTargetResult result = csEngineTools::FindScreenTarget (
      csVector2 (screenx, screeny), maxdist, camera);
  isect = result.isect;
  if (result.mesh)
    selent = pl->FindAttachedEntity (result.mesh->QueryObject ());
  else
    selent = 0;
}

void celXmlScriptEventHandler::HitBeam (iSector* sector,
	const csVector3& start,
	const csVector3& end, csVector3& isect, iCelEntity*& selent,
	iMeshWrapper*& mesh)
{
  csSectorHitBeamResult rc;
  rc = sector->HitBeamPortals (start, end);
  mesh = rc.mesh;
  if (rc.mesh == 0)
  {
    isect = end;
    selent = 0;
  }
  else
  {
    isect = rc.isect;
    selent = pl->FindAttachedEntity (rc.mesh->QueryObject ());
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

celXmlArg& celXmlScriptEventHandler::GetArgument (size_t idx)
{
  celXmlOperation& op = operations[idx];
  return op.arg;
}

size_t celXmlScriptEventHandler::AddLocalVariable ()
{
  return local_vars.Push (celXmlArg ());
}

//---------------------------------------------------------------------------

celXmlScript::celXmlScript (iCelPlLayer* pl, iObjectRegistry* object_reg)
	: config (object_reg)
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
  celXmlScriptEventHandler* h = new celXmlScriptEventHandler (pl, this);
  h->SetName (name);
  event_handlers.Push (h);
  event_handlers_hash.Put (name, h);
  return h;
}

celXmlScriptEventHandler* celXmlScript::GetEventHandler (const char* name)
{
  return event_handlers_hash.Get (name, 0);
}

//---------------------------------------------------------------------------

