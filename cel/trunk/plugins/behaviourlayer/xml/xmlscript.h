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

#ifndef __CEL_BLXML_SCRIPT__
#define __CEL_BLXML_SCRIPT__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/util.h"
#include "csutil/parray.h"
#include "csutil/hashhandlers.h"
#include "csutil/hash.h"
#include "csutil/strhash.h"
#include "csgeom/vector2.h"
#include "csgeom/vector3.h"
#include "csutil/cscolor.h"

struct iCelEntity;
struct iCelPlLayer;
struct iCelPropertyClass;
class celBehaviourXml;
class celXmlScriptEventHandler;

enum
{
  CEL_OPERATION_END = 0,	// A:-		S:-		OS:-
  CEL_OPERATION_PROPERTY,	// A:-		S:PC,ID,?	OS:-
  CEL_OPERATION_GETPROPERTY,	// A:-		S:PC,ID		OS:?
  CEL_OPERATION_GETPROPERTY1,	// A:-		S:ID		OS:?
  CEL_OPERATION_ACTION,		// A:-		S:PC,ID,S	OS:-
  CEL_OPERATION_VAR,		// A:-		S:S,?		OS:-
  CEL_OPERATION_PRINT,		// A:-		S:S		OS:-
  CEL_OPERATION_IF,		// A:E,E	S:?		OS:-
  CEL_OPERATION_FOR,		// A:E		S:S,?,?		OS:-
  CEL_OPERATION_TESTCOLLIDE,	// A:E,E	S:S		OS:-
  CEL_OPERATION_CREATEENTITY,	// A:-		S:S,S		OS:-
  CEL_OPERATION_CREATEPROPCLASS,// A:-		S:S		OS:-
  CEL_OPERATION_DEFAULTPC,	// A:-		S:PC		OS:-

  CEL_OPERATION_PUSH,		// A:?		S:-		OS:?
  CEL_OPERATION_DEREFVAR,	// A:-		S:S		OS:?
  CEL_OPERATION_COLOR,		// A:-		S:F,F,F		OS:C
  CEL_OPERATION_VECTOR2,	// A:-		S:F,F		OS:V2
  CEL_OPERATION_VECTOR3,	// A:-		S:F,F,F		OS:V3
  CEL_OPERATION_ENTITY,		// A:-		S:S		OS:EN
  CEL_OPERATION_PC,		// A:-		S:S,S		OS:PC
  CEL_OPERATION_PCTHIS,		// A:-		S:S		OS:PC
  CEL_OPERATION_UNARYMINUS,	// A:-		S:?		OS:?
  CEL_OPERATION_MINUS,		// A:-		S:?,?		OS:?
  CEL_OPERATION_ADD,		// A:-		S:?,?		OS:?
  CEL_OPERATION_MULT,		// A:-		S:?,?		OS:?
  CEL_OPERATION_DIV,		// A:-		S:?,?		OS:?
  CEL_OPERATION_EQ,		// A:-		S:?,?		OS:B
  CEL_OPERATION_NE,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LT,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LE,		// A:-		S:?,?		OS:B
  CEL_OPERATION_GT,		// A:-		S:?,?		OS:B
  CEL_OPERATION_GE,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LOGAND,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LOGOR,		// A:-		S:?,?		OS:B
  CEL_OPERATION_CALCID,		// A:-		S:S		OS:ID
  CEL_OPERATION_CALCPROPID,	// A:-		S:S		OS:ID

  CEL_OPERATION_FINALOP
};

enum
{
  CEL_TYPE_NONE = 0,
  CEL_TYPE_UINT32,
  CEL_TYPE_INT32,
  CEL_TYPE_FLOAT,
  CEL_TYPE_STRING,
  CEL_TYPE_BOOL,
  CEL_TYPE_PC,
  CEL_TYPE_ID,
  CEL_TYPE_ARGLIST,
  CEL_TYPE_EVENTHANDLER,
  CEL_TYPE_VECTOR2,
  CEL_TYPE_VECTOR3,
  CEL_TYPE_COLOR,
  CEL_TYPE_VAR,

  CEL_TYPE_FINALTYPE
};

struct celXmlArgList;

// One argument.
struct celXmlArg
{
  int type;
  union
  {
    uint32 ui;
    int32 i;
    float f;
    struct
    {
      const char* s;	// Also used for CEL_TYPE_VAR.
      bool cleanup;	// If true string must be cleaned up here.
    } str;
    bool b;
    int pc_ref;
    iCelPropertyClass* pc;
    csStringID id;
    celXmlArgList* a;
    celXmlScriptEventHandler* h;
    struct { float x, y, z; } vec;
    struct { float red, green, blue; } col;
  } arg;
  celXmlArg () : type (CEL_TYPE_NONE) { }
  celXmlArg (const celXmlArg& other);

  ~celXmlArg ()
  {
    Cleanup ();
  }
  void Cleanup ();
  void SetUInt32 (uint32 i)
  {
    Cleanup ();
    type = CEL_TYPE_UINT32;
    arg.ui = i;
  }
  void SetInt32 (int32 i)
  {
    Cleanup ();
    type = CEL_TYPE_INT32;
    arg.i = i;
  }
  void SetFloat (float f)
  {
    Cleanup ();
    type = CEL_TYPE_FLOAT;
    arg.f = f;
  }
  void SetBool (bool b)
  {
    Cleanup ();
    type = CEL_TYPE_BOOL;
    arg.b = b;
  }
  // Set a preallocated string.
  void SetStringPrealloc (const char* s)
  {
    Cleanup ();
    type = CEL_TYPE_STRING;
    arg.str.s = s;
    arg.str.cleanup = true;
  }
  void SetString (const char* s, bool copy)
  {
    Cleanup ();
    type = CEL_TYPE_STRING;
    if (copy)
    {
      arg.str.s = csStrNew (s);
      arg.str.cleanup = true;
    }
    else
    {
      arg.str.s = s;
      arg.str.cleanup = false;
    }
  }
  void SetVar (const char* s)
  {
    Cleanup ();
    type = CEL_TYPE_VAR;
    arg.str.s = csStrNew (s);
    arg.str.cleanup = true;
  }
  void SetPC (iCelPropertyClass* pc)
  {
    Cleanup ();
    type = CEL_TYPE_PC;
    arg.pc = pc;
  }
  void SetID (csStringID id)
  {
    Cleanup ();
    type = CEL_TYPE_ID;
    arg.id = id;
  }
  void SetEventHandler (celXmlScriptEventHandler* h)
  {
    Cleanup ();
    type = CEL_TYPE_EVENTHANDLER;
    arg.h = h;
  }
  void SetVector (const csVector2& v)
  {
    Cleanup ();
    type = CEL_TYPE_VECTOR2;
    arg.vec.x = v.x;
    arg.vec.y = v.y;
  }
  void SetVector (const csVector3& v)
  {
    Cleanup ();
    type = CEL_TYPE_VECTOR3;
    arg.vec.x = v.x;
    arg.vec.y = v.y;
    arg.vec.z = v.z;
  }
  void SetColor (const csColor& v)
  {
    Cleanup ();
    type = CEL_TYPE_COLOR;
    arg.col.red = v.red;
    arg.col.green = v.green;
    arg.col.blue = v.blue;
  }
  void SetArgList ();
};

// An argument list.
struct celXmlArgList
{
  csArray<celXmlArg> args;
};

// An operation in an XML script.
struct celXmlOperation
{
  int op;
  celXmlArg arg;
};

/**
 * An event handler in a script.
 */
class celXmlScriptEventHandler
{
private:
  const char* name;
  csArray<celXmlOperation> operations;
  iCelPlLayer* pl;
  csArray<celXmlArg> stack;

  bool ReportError (celBehaviourXml* behave, const char* msg, ...);
  bool CheckStack (celBehaviourXml* behave);

public:
  celXmlScriptEventHandler (iCelPlLayer* pl);
  ~celXmlScriptEventHandler ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }

  void AddOperation (int op);

  // Add argument to last operation.
  celXmlArg& AddArgument ();

  bool Execute (iCelEntity* entity, celBehaviourXml* behave);
};

/**
 * A script.
 */
class celXmlScript
{
private:
  char* name;
  csPDelArray<celXmlScriptEventHandler> event_handlers;
  csHash<celXmlScriptEventHandler*,csStrKey,
  	csConstCharHashKeyHandler> event_handlers_hash;
  iCelPlLayer* pl;

public:
  celXmlScript (iCelPlLayer* pl);
  ~celXmlScript ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }

  celXmlScriptEventHandler* FindOrCreateEventHandler (const char* name);
  celXmlScriptEventHandler* CreateEventHandler (const char* name);
  celXmlScriptEventHandler* GetEventHandler (const char* name);
};

#endif // __CEL_BLXML_SCRIPT__

