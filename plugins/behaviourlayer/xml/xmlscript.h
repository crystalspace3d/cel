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

struct iCelEntity;
struct iCelPropertyClass;

#define CEL_OPERATION_END 0
#define CEL_OPERATION_PROPERTY 1	// Args: PC, ID, ?
#define CEL_OPERATION_ACTION 2		// Args: PC, ID, S

#define CEL_TYPE_NONE 0
#define CEL_TYPE_UINT32 1
#define CEL_TYPE_INT32 2
#define CEL_TYPE_FLOAT 3
#define CEL_TYPE_STRING 4
#define CEL_TYPE_PC 5
#define CEL_TYPE_ID 6
#define CEL_TYPE_ARGLIST 7

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
    int pc;
    csStringID id;
    celXmlArgList* a;
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

public:
  celXmlScriptEventHandler ();
  ~celXmlScriptEventHandler ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }

  void ResolveParameters (iCelEntity* entity);
  int GetResolver (const char* pcname);

  void AddOperation (int op);

  // Add argument to last operation.
  celXmlArg& AddArgument ();

  void Execute (iCelEntity* entity);
};

/**
 * A script.
 */
class celXmlScript
{
private:
  char* name;
  csPDelArray<celXmlScriptEventHandler> event_handlers;
  csHash<celXmlScriptEventHandler*,const char*,
  	csConstCharHashKeyHandler> event_handlers_hash;

public:
  celXmlScript ();
  ~celXmlScript ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }

  celXmlScriptEventHandler* CreateEventHandler (const char* name);
  celXmlScriptEventHandler* GetEventHandler (const char* name);
};

#endif // __CEL_BLXML_SCRIPT__

