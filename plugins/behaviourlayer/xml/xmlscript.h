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
#include "csgeom/vector3.h"
#include "csutil/cscolor.h"

struct iCelEntity;
struct iCelPlLayer;
struct iCelPropertyClass;
class celBehaviourXml;
class celXmlScriptEventHandler;

#define CEL_OPERATION_END 0
#define CEL_OPERATION_PROPERTY 1	// Args: PC, ID, ?
#define CEL_OPERATION_GETPROPERTY 2	// Args: S, PC, ID
#define CEL_OPERATION_ACTION 3		// Args: PC, ID, S
#define CEL_OPERATION_VAR 4		// Args: S, ?
#define CEL_OPERATION_GETPROPCLASS 5	// Args: S, S S
#define CEL_OPERATION_PRINT 6		// Args: S
#define CEL_OPERATION_IF 7		// Args: S, E, E
#define CEL_OPERATION_TESTCOLLIDE 8	// Args: S, E, E

#define CEL_TYPE_NONE 0
#define CEL_TYPE_UINT32 1
#define CEL_TYPE_INT32 2
#define CEL_TYPE_FLOAT 3
#define CEL_TYPE_STRING 4
#define CEL_TYPE_BOOL 5
#define CEL_TYPE_PC 6
#define CEL_TYPE_ID 7
#define CEL_TYPE_ARGLIST 8
#define CEL_TYPE_EVENTHANDLER 9
#define CEL_TYPE_VECTOR3 10
#define CEL_TYPE_COLOR 11

// A property class parameter resolver.
struct celXmlPCResolver
{
  char* pcname;
  iCelPropertyClass* pc;
  celXmlPCResolver () : pcname (0), pc (0) { }
  ~celXmlPCResolver () { delete[] pcname; }
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
    const char* s;
    bool b;
    int pc;
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
  void SetPC (int pc)
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

public:
  celXmlScriptEventHandler (iCelPlLayer* pl);
  ~celXmlScriptEventHandler ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }

  void ResolveParameters (iCelEntity* entity);
  int GetResolver (const char* pcname);

  void AddOperation (int op);

  // Add argument to last operation.
  celXmlArg& AddArgument ();

  void Execute (iCelEntity* entity, celBehaviourXml* behave);
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

