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
#include "csutil/hash.h"
#include "csutil/strhash.h"
#include "csutil/cscolor.h"
#include "csgeom/vector2.h"
#include "csgeom/vector3.h"

#include "physicallayer/datatype.h"

struct iCelEntity;
struct iCelPlLayer;
struct iCelPropertyClass;
struct iCelParameterBlock;
struct iPcInventory;
class celBehaviourXml;
class celXmlScriptEventHandler;
class celVariableParameterBlock;

// ?: various types possible
// -: nothing
// PC: pclass
// ID: ID
// S: string
// E: entity (or string for entity name)
// B: boolean
// F: float
// EH: event handler
// C: code location
// PC: property class
// I: integer
enum
{
  CEL_OPERATION_END = 0,	// A:-		S:-		OS:-
  CEL_OPERATION_CALLSTACK,	// A:-		S:-		OS:-
  CEL_OPERATION_VARIABLES,	// A:-		S:-		OS:-
  CEL_OPERATION_TRACEON,	// A:-		S:-		OS:-
  CEL_OPERATION_TRACEOFF,	// A:-		S:-		OS:-
  CEL_OPERATION_PRINT,		// A:-		S:?		OS:-

  CEL_OPERATION_INHERITPARAMS,	// A:-		S:-		OS:-
  CEL_OPERATION_ACTIONPARAMS,	// A:-		S:-		OS:-
  CEL_OPERATION_ACTIONPARAM,	// A:UL		S:ID,?		OS:-
  CEL_OPERATION_ACTION,		// A:-		S:PC,ID		OS:-
  CEL_OPERATION_PROPERTY,	// A:-		S:PC,ID,?	OS:-
  CEL_OPERATION_GETPROPERTY,	// A:-		S:PC,ID		OS:?
  CEL_OPERATION_GETPROPERTY1,	// A:-		S:ID		OS:?

  CEL_OPERATION_IF,		// A:EH,EH	S:B		OS:-
  CEL_OPERATION_IFGOTO,		// A:C		S:B		OS:-
  CEL_OPERATION_IFFUN,		// A:-		S:B,?,?		OS:?
  CEL_OPERATION_GOTO,		// A:C		S:-		OS:-
  CEL_OPERATION_FOR,		// A:EH		S:S,?,?		OS:-
  CEL_OPERATION_FORI,		// A:C		S:S,?,?		OS:-

  CEL_OPERATION_RETURN,		// A:-		S:?		OS:-
  CEL_OPERATION_CALL_I,		// A:EH		S:-		OS:-
  CEL_OPERATION_CALL,		// A:-		S:S		OS:-
  CEL_OPERATION_CALL_R,		// A:-		S:S,S		OS:-
  CEL_OPERATION_CALL_RS,	// A:S		S:-		OS:?
  CEL_OPERATION_CALL_E,		// A:-		S:E,S		OS:-
  CEL_OPERATION_CALL_ER,	// A:-		S:E,S,S		OS:-
  CEL_OPERATION_CALL_ERS,	// A:S		S:E		OS:?
  CEL_OPERATION_CALL_ERS2,	// A:S		S:E		OS:?
  CEL_OPERATION_QUIT,	// A:-		S:-		OS:-

  CEL_OPERATION_BB_TESTCOLLIDE,	// A:-		S:PC		OS:-
  CEL_OPERATION_BB_MOVE,	// A:-		S:I,I		OS:-
  CEL_OPERATION_BB_MOVE_E,	// A:-		S:E,I,I		OS:-
  CEL_OPERATION_BB_MOVEDELTA,	// A:-		S:I,I,I		OS:-
  CEL_OPERATION_BB_MOVEDELTA_E,	// A:-		S:E,I,I,I	OS:-
  CEL_OPERATION_BB_MOVELAYER,	// A:-		S:S,I,I		OS:-
  CEL_OPERATION_BB_TOFRONT,	// A:-		S:-		OS:-
  CEL_OPERATION_BB_TOBACK,	// A:-		S:-		OS:-
  CEL_OPERATION_BB_TOFRONT_E,	// A:-		S:PC		OS:-
  CEL_OPERATION_BB_TOBACK_E,	// A:-		S:PC		OS:-
  CEL_OPERATION_BB_UP,		// A:-		S:-		OS:-
  CEL_OPERATION_BB_DOWN,	// A:-		S:-		OS:-
  CEL_OPERATION_BB_UP_E,	// A:-		S:PC		OS:-
  CEL_OPERATION_BB_DOWN_E,	// A:-		S:PC		OS:-

  CEL_OPERATION_CREATEPC,	// A:-		S:S		OS:-
  CEL_OPERATION_CREATEPCTAG,	// A:-		S:S,S		OS:-
  CEL_OPERATION_DEFAULTPC,	// A:-		S:PC		OS:-
  CEL_OPERATION_PC,		// A:-		S:E,S		OS:PC
  CEL_OPERATION_PCTHIS,		// A:-		S:PC		OS:PC
  CEL_OPERATION_PCTAG,		// A:-		S:E,S,S		OS:PC
  CEL_OPERATION_PCTAGTHIS,	// A:-		S:PC,S		OS:PC
  CEL_OPERATION_CALCID,		// A:-		S:S		OS:ID
  CEL_OPERATION_CALCACTID,	// A:-		S:S		OS:ID
  CEL_OPERATION_CALCPROPID,	// A:-		S:S		OS:ID
  CEL_OPERATION_CALCPARID,	// A:-		S:S		OS:ID

  CEL_OPERATION_DESTROYENTITY,	// A:-		S:E		OS:-
  CEL_OPERATION_CREATEENTITY,	// A:-		S:S,S		OS:-
  CEL_OPERATION_CREATEENTITYL,	// A:-		S:S,S,S		OS:-
  CEL_OPERATION_CREATEENTITYTPL,// A:-		S:S,S		OS:-
  CEL_OPERATION_ENTNAME,	// A:-		S:-		OS:S
  CEL_OPERATION_ENT,		// A:-		S:S		OS:E
  CEL_OPERATION_ENTTHIS,	// A:-		S:-		OS:E

  CEL_OPERATION_DEFAULTINV,	// A:-		S:PC		OS:-
  CEL_OPERATION_INVENTORY_ADD,	// A:-		S:E		OS:-
  CEL_OPERATION_INVENTORY_REM,	// A:-		S:E		OS:-
  CEL_OPERATION_INVENTORY_COUNT,// A:-		S:-		OS:I
  CEL_OPERATION_INVENTORY_GET,	// A:-		S:I		OS:E

  CEL_OPERATION_PUSH,		// A:?		S:-		OS:?
  CEL_OPERATION_PUSHSTR,	// A:S		S:-		OS:S
  CEL_OPERATION_DUP,		// A:-		S:?		OS:?,?
  CEL_OPERATION_DROP,		// A:-		S:?		OS:-
  CEL_OPERATION_CLEARSTACK,	// A:-		S:...		OS:<nothing>

  CEL_OPERATION_DEREFLVAR,	// A:I		S:-		OS:?
  CEL_OPERATION_DEREFVAR,	// A:-		S:S		OS:?
  CEL_OPERATION_DEREFVARENT,	// A:-		S:E,S		OS:?
  CEL_OPERATION_DEREFVAR_STR,	// A:S		S:-		OS:?
  CEL_OPERATION_DEREFVARENT_STR,// A:S		S:E		OS:?
  CEL_OPERATION_LVAR,		// A:I		S:?		OS:-
  CEL_OPERATION_VAR,		// A:-		S:S,?		OS:-
  CEL_OPERATION_VARENT,		// A:-		S:E,S,?		OS:-
  CEL_OPERATION_VAR_STR,	// A:S		S:?		OS:-
  CEL_OPERATION_VARENT_STR,	// A:S		S:E,?		OS:-
  CEL_OPERATION_PARAM,		// A:-		S:ID		OS:?
  CEL_OPERATION_DEREFARR,	// A:-		S:S,?		OS:?
  CEL_OPERATION_DEREFARR_STR,	// A:S		S:?		OS:?
  CEL_OPERATION_DEREFARRENT,	// A:-		S:E,S,?		OS:?
  CEL_OPERATION_DEREFARRENT_STR,// A:S		S:E,?		OS:?
  CEL_OPERATION_DEREFARR2,	// A:-		S:S,?,?		OS:?
  CEL_OPERATION_DEREFARR2_STR,	// A:S		S:?,?		OS:?
  CEL_OPERATION_DEREFARRENT2,	// A:-		S:E,S,?,?	OS:?
  CEL_OPERATION_DEREFARRENT2_STR,// A:S		S:E,?,?		OS:?

  CEL_OPERATION_COLOR,		// A:-		S:F,F,F		OS:C
  CEL_OPERATION_VECTOR2,	// A:-		S:F,F		OS:V2
  CEL_OPERATION_VECTOR3,	// A:-		S:F,F,F		OS:V3

  CEL_OPERATION_UNARYMINUS,	// A:-		S:?		OS:?
  CEL_OPERATION_MINUS,		// A:-		S:?,?		OS:?
  CEL_OPERATION_MINUS_I,	// A:?		S:?		OS:?
  CEL_OPERATION_ADD,		// A:-		S:?,?		OS:?
  CEL_OPERATION_ADD_I,		// A:?		S:?		OS:?
  CEL_OPERATION_MULT,		// A:-		S:?,?		OS:?
  CEL_OPERATION_DIV,		// A:-		S:?,?		OS:?
  CEL_OPERATION_MODULO,		// A:-		S:?,?		OS:?
  CEL_OPERATION_EQ,		// A:-		S:?,?		OS:B
  CEL_OPERATION_NE,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LT,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LE,		// A:-		S:?,?		OS:B
  CEL_OPERATION_GT,		// A:-		S:?,?		OS:B
  CEL_OPERATION_GE,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LOGAND,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LOGOR,		// A:-		S:?,?		OS:B
  CEL_OPERATION_LOGNOT,		// A:-		S:?,?		OS:?
  CEL_OPERATION_BITAND,		// A:-		S:?,?		OS:?
  CEL_OPERATION_BITOR,		// A:-		S:?,?		OS:?
  CEL_OPERATION_BITXOR,		// A:-		S:?,?		OS:?
  CEL_OPERATION_BITNOT,		// A:-		S:?,?		OS:?
  CEL_OPERATION_ABS,		// A:-		S:?		OS:?
  CEL_OPERATION_MIN,		// A:-		S:?,?		OS:?
  CEL_OPERATION_MAX,		// A:-		S:?,?		OS:?
  CEL_OPERATION_SIGN,		// A:-		S:?		OS:I
  CEL_OPERATION_INTPOL,		// A:-		S:F,?,?		OS:?
  CEL_OPERATION_INT,		// A:-		S:?		OS:I
  CEL_OPERATION_FLOAT,		// A:-		S:?		OS:F
  CEL_OPERATION_RANDOMIZE,	// A:-		S:-		OS:-
  CEL_OPERATION_RAND,		// A:-		S:?		OS:F
  CEL_OPERATION_MOUSEX,		// A:-		S:-		OS:I
  CEL_OPERATION_MOUSEY,		// A:-		S:-		OS:I
  CEL_OPERATION_BB_MOUSEX,	// A:-		S:-		OS:I
  CEL_OPERATION_BB_MOUSEY,	// A:-		S:-		OS:I
  CEL_OPERATION_SCR_WIDTH,	// A:-		S:-		OS:I
  CEL_OPERATION_SCR_HEIGHT,	// A:-		S:-		OS:I

  CEL_OPERATION_SOUND,		// A:-		S:S,B		OS:-
  CEL_OPERATION_READFILE,	// A:-		S:S,S		OS:I
  CEL_OPERATION_WRITEFILE,	// A:-		S:S,S,I,I	OS:B
  CEL_OPERATION_REPORTERROR,	// A:-		S:S		OS:-

  CEL_OPERATION_STRSUB,		// A:-		S:S,I,I		OS:S
  CEL_OPERATION_STRIDX,		// A:-		S:S,S		OS:I
  CEL_OPERATION_STRLEN,		// A:-		S:S		OS:I
  CEL_OPERATION_STRSPLIT,	// A:-		S:S,S,S,S	OS:-

  CEL_OPERATION_FINALOP
};

#define CEL_DATA_EVENTHANDLER CEL_DATA_LAST
#define CEL_DATA_ID (CEL_DATA_LAST+1)
#define CEL_DATA_CODELOCATION (CEL_DATA_LAST+2)

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
      char* s;
      bool cleanup;	// If true string must be cleaned up here.
    } str;
    bool b;
    iCelPropertyClass* pc;
    iCelEntity* entity;
    csStringID id;
    struct { celXmlScriptEventHandler* h_true, * h_false; } h;
    struct { float x, y, z; } vec;
    struct { float red, green, blue; } col;
    size_t codelocation;
    int localvar;
  } arg;
  celXmlArg () : type (CEL_DATA_NONE) { }
  celXmlArg (const celXmlArg& other);

  ~celXmlArg ()
  {
    Cleanup ();
  }
  void Cleanup ();
  void SetUInt32 (uint32 i)
  {
    Cleanup ();
    type = CEL_DATA_ULONG;
    arg.ui = i;
  }
  void SetInt32 (int32 i)
  {
    Cleanup ();
    type = CEL_DATA_LONG;
    arg.i = i;
  }
  void SetFloat (float f)
  {
    Cleanup ();
    type = CEL_DATA_FLOAT;
    arg.f = f;
  }
  void Set (bool b)
  {
    Cleanup ();
    type = CEL_DATA_BOOL;
    arg.b = b;
  }
  // Set a preallocated string.
  void SetStringPrealloc (const char* s)
  {
    Cleanup ();
    type = CEL_DATA_STRING;
    arg.str.s = (char*) s;
    arg.str.cleanup = true;
  }
  void SetString (const char* s, bool copy)
  {
    Cleanup ();
    type = CEL_DATA_STRING;
    if (copy)
    {
      arg.str.s = csStrNew (s);
      arg.str.cleanup = true;
    }
    else
    {
      arg.str.s = (char*) s;
      arg.str.cleanup = false;
    }
  }
  void SetPC (iCelPropertyClass* pc)
  {
    Cleanup ();
    type = CEL_DATA_PCLASS;
    arg.pc = pc;
  }
  void SetEntity (iCelEntity* ent)
  {
    Cleanup ();
    type = CEL_DATA_ENTITY;
    arg.entity = ent;
  }
  void SetID (csStringID id)
  {
    Cleanup ();
    type = CEL_DATA_ID;
    arg.id = id;
  }
  void SetCodeLocation (size_t location)
  {
    Cleanup ();
    type = CEL_DATA_CODELOCATION;
    arg.codelocation = location;
  }
  void SetEventHandlers (celXmlScriptEventHandler* h_true,
  	celXmlScriptEventHandler* h_false)
  {
    Cleanup ();
    type = CEL_DATA_EVENTHANDLER;
    arg.h.h_true = h_true;
    arg.h.h_false = h_false;
  }
  void SetVector (const csVector2& v)
  {
    Cleanup ();
    type = CEL_DATA_VECTOR2;
    arg.vec.x = v.x;
    arg.vec.y = v.y;
  }
  void SetVector (const csVector3& v)
  {
    Cleanup ();
    type = CEL_DATA_VECTOR3;
    arg.vec.x = v.x;
    arg.vec.y = v.y;
    arg.vec.z = v.z;
  }
  void SetColor (const csColor& v)
  {
    Cleanup ();
    type = CEL_DATA_COLOR;
    arg.col.red = v.red;
    arg.col.green = v.green;
    arg.col.blue = v.blue;
  }
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
  char* name;
  csArray<celXmlOperation> operations;
  iCelPlLayer* pl;
  csArray<celXmlArg> stack;
  csArray<celXmlArg> local_vars;
  csWeakRef<iCelPropertyClass> default_pc;
  csWeakRef<iPcInventory> default_inv;

  // Temporary variable to keep parameters for actions.
  csRef<iCelParameterBlock> action_params;

  bool ReportError (celBehaviourXml* behave, const char* msg, ...);
  bool EvaluateTrue (const celXmlArg& eval, celBehaviourXml* behave, bool& rc);
  void DumpCallStack (celBehaviourXml* behave);
  void DumpVariables (celBehaviourXml* behave);

public:
  celXmlScriptEventHandler (iCelPlLayer* pl);
  ~celXmlScriptEventHandler ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }

  void AddOperation (int op);
  int GetLastOperation () { return operations.Top ().op; }
  int GetOperation (size_t idx) { return operations[idx].op; }
  void ChangeOperation (size_t idx, int newop) { operations[idx].op = newop; }
  void ReplaceLastOperation (int op) { operations.Top ().op = op; }

  // Get argument for last operation.
  celXmlArg& GetArgument ();
  // Get argument index for last operation.
  size_t GetLastArgumentIndex () { return operations.Length ()-1; }
  // Get argument with index.
  celXmlArg& GetArgument (size_t idx);

  // Add a local variable. Return index.
  size_t AddLocalVariable ();
  // Get a local variable with index.
  celXmlArg& GetLocalVariable (size_t idx) { return local_vars[idx]; }

  bool Execute (iCelEntity* entity, celBehaviourXml* behave,
  	celData& ret, iCelParameterBlock* params, size_t startop = 0,
	bool newscope = true);
};

/**
 * A script.
 */
class celXmlScript
{
private:
  char* name;
  csPDelArray<celXmlScriptEventHandler> event_handlers;
  csHash<celXmlScriptEventHandler*,csStrKey> event_handlers_hash;
  iCelPlLayer* pl;
  celXmlScript* superscript;

public:
  celXmlScript (iCelPlLayer* pl);
  ~celXmlScript ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }

  void SetSuperScript (celXmlScript* su) { superscript = su; }
  celXmlScript* GetSuperScript () { return superscript; }

  celXmlScriptEventHandler* FindOrCreateEventHandler (const char* name);
  celXmlScriptEventHandler* CreateEventHandler (const char* name);
  celXmlScriptEventHandler* GetEventHandler (const char* name);
};

#endif // __CEL_BLXML_SCRIPT__
