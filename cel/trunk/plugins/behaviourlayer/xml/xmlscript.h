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

#define CEL_OPERATION_END 0			// A:-	S:-	OS:-
#define CEL_OPERATION_PROPERTY 1		// A: PC, ID, ?
#define CEL_OPERATION_GETPROPERTY 2		// A: S, PC, ID
#define CEL_OPERATION_ACTION 3			// A: PC, ID, S
#define CEL_OPERATION_VAR 4			// A: S, ?
#define CEL_OPERATION_GETPROPCLASS 5		// A: S, S S
#define CEL_OPERATION_PRINT 6			// A:-		S:S	OS:-
#define CEL_OPERATION_IF 7			// A:E,E	S:?	OS:-
#define CEL_OPERATION_TESTCOLLIDE 8		// A:E,E	S:S	OS:-
#define CEL_OPERATION_CREATEENTITY 9		// A:-		S:S,S	OS:-
#define CEL_OPERATION_CREATEPROPCLASS 10	// A:-		S:S	OS:-

#define CEL_OPERATION_PUSH 11			// A:?		S:-	OS:?
#define CEL_OPERATION_DEREFVAR 12		// A:-		S:S	OS:?
#define CEL_OPERATION_COLOR 13			// A:-		S:F,F,F	OS:C
#define CEL_OPERATION_VECTOR2 14		// A:-		S:F,F	OS:V2
#define CEL_OPERATION_VECTOR3 15		// A:-		S:F,F,F	OS:V3
#define CEL_OPERATION_UNARYMINUS 16		// A:-		S:?	OS:?
#define CEL_OPERATION_MINUS 17			// A:-		S:?,?	OS:?
#define CEL_OPERATION_ADD 18			// A:-		S:?,?	OS:?
#define CEL_OPERATION_MULT 19			// A:-		S:?,?	OS:?
#define CEL_OPERATION_DIV 20			// A:-		S:?,?	OS:?
#define CEL_OPERATION_ENTITY 21			// A:-		S:S	OS:EN
#define CEL_OPERATION_PC 22			// A:-		S:S,S	OS:PC

#define CEL_TYPE_NONE 0
#define CEL_TYPE_UINT32 1
#define CEL_TYPE_INT32 2
#define CEL_TYPE_FLOAT 3
#define CEL_TYPE_STRING 4
#define CEL_TYPE_BOOL 5
#define CEL_TYPE_PC_REF 6
#define CEL_TYPE_PC 7
#define CEL_TYPE_ID 8
#define CEL_TYPE_ARGLIST 9
#define CEL_TYPE_EVENTHANDLER 10
#define CEL_TYPE_VECTOR2 11
#define CEL_TYPE_VECTOR3 12
#define CEL_TYPE_COLOR 13
#define CEL_TYPE_VAR 14

// A property class parameter resolver.
struct celXmlPCResolver
{
  char* entname;
  char* pcname;
  iCelPropertyClass* pc;
  celXmlPCResolver () : entname (0), pcname (0), pc (0) { }
  ~celXmlPCResolver () { delete[] entname; delete[] pcname; }
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
    const char* s;	// Also used for CEL_TYPE_VAR.
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
  void SetString (const char* s)
  {
    Cleanup ();
    type = CEL_TYPE_STRING;
    arg.s = csStrNew (s);
  }
  void SetVar (const char* s)
  {
    Cleanup ();
    type = CEL_TYPE_VAR;
    arg.s = csStrNew (s);
  }
  void SetPC (iCelPropertyClass* pc)
  {
    Cleanup ();
    type = CEL_TYPE_PC;
    arg.pc = pc;
  }
  void SetPC (int pc_ref)
  {
    Cleanup ();
    type = CEL_TYPE_PC_REF;
    arg.pc_ref = pc_ref;
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
  csArray<celXmlPCResolver> resolvers;
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

  void ResolveParameters (iCelEntity* entity);
  int GetResolver (const char* entname, const char* pcname);

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

