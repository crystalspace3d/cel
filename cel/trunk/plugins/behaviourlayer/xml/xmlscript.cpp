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
#include "plugins/behaviourlayer/xml/xmlscript.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/pl.h"
#include "propclass/prop.h"
#include "propclass/billboard.h"
#include "tools/billboard.h"

inline void DUMP_EXEC (const char* msg, ...)
{
#if 0
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
    resolvers[i].pc = pclist->FindByName (resolvers[i].pcname);
}

void celXmlScriptEventHandler::Execute (iCelEntity* entity,
	celBehaviourXml* behave)
{
  int i = 0;
  for (;;)
  {
    celXmlOperation& op = operations[i];
    i++;
    csArray<celXmlArg>& args = op.arg.arg.a->args;
    switch (op.op)
    {
      case CEL_OPERATION_END:
        return;
      case CEL_OPERATION_TESTCOLLIDE:
        {
	  iPcProperties* props = behave->GetProperties ();
	  if (!props) break;	// @@@ Report error!
	  int idx = props->GetPropertyIndex (args[0].arg.s);
	  if (idx == -1) break;	// @@@ Report error!
	  iCelPropertyClass* pc = props->GetPropertyPClass (idx);
	  if (!pc) break;	// @@@ Report error!
	  // @@@ Efficiency?
	  csRef<iPcBillboard> other_bb = SCF_QUERY_INTERFACE (pc, iPcBillboard);
	  if (!other_bb) break;	// @@@ Report error!
	  csRef<iPcBillboard> bb = CEL_QUERY_PROPCLASS (
	  	entity->GetPropertyClassList (), iPcBillboard);
	  if (!bb) break;	// @@@ Report error!
	  csRef<iBillboardManager> bbmgr = CS_QUERY_REGISTRY (
	  	behave->GetObjectRegistry (), iBillboardManager);
	  if (!bbmgr) break;	// @@@ Report error!
	  if (bbmgr->TestCollision (bb->GetBillboard (),
	  	other_bb->GetBillboard ()))
	  {
	    celXmlScriptEventHandler* truebranch = args[1].arg.h;
	    if (truebranch)
	    {
	      truebranch->ResolveParameters (entity);
	      truebranch->Execute (entity, behave);
	    }
	  }
	  else
	  {
	    celXmlScriptEventHandler* falsebranch = args[2].arg.h;
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
	  iPcProperties* props = behave->GetProperties ();
	  if (!props) break;	// @@@ Report error!
	  int idx = props->GetPropertyIndex (args[0].arg.s);
	  bool rc = false;
	  if (idx != -1)
	    switch (props->GetPropertyType (idx))
	    {
	      case CEL_DATA_LONG:
	        {
		  long l = props->GetPropertyLong (idx);
		  rc = (l != 0);
		  DUMP_EXEC (": if var=%s long=%ld rc=%d\n",
		  	args[0].arg.s, l, rc);
		}
		break;
	      case CEL_DATA_FLOAT:
	        {
		  float l = props->GetPropertyFloat (idx);
		  rc = (ABS (l) < SMALL_EPSILON);
		  DUMP_EXEC (": if var=%s flt=%g rc=%d\n", args[0].arg.s,
		  	l, rc);
		}
		break;
	      case CEL_DATA_BOOL:
	        {
		  bool l = props->GetPropertyBool (idx);
		  rc = l;
		  DUMP_EXEC (": if var=%s bool=%d rc=%d\n", args[0].arg.s,
		  	l, rc);
		}
		break;
	      case CEL_DATA_STRING:
	        {
		  const char* l = props->GetPropertyString (idx);
		  rc = l && strlen (l) > 0;
		  DUMP_EXEC (": if var=%s str=%s rc=%d\n", args[0].arg.s,
		  	l, rc);
		}
		break;
	      case CEL_DATA_PCLASS:
	        {
		  iCelPropertyClass* l = props->GetPropertyPClass (idx);
		  rc = (l != 0);
		  DUMP_EXEC (": if var=%s pc=%p rc=%d\n", args[0].arg.s,
		  	l, rc);
		}
		break;
	      case CEL_DATA_ENTITY:
	        {
		  iCelEntity* l = props->GetPropertyEntity (idx);
		  rc = (l != 0);
		  DUMP_EXEC (": if var=%s ent=%p rc=%d\n", args[0].arg.s,
		  	l, rc);
		}
		break;
	      default:
		DUMP_EXEC (": if var=%s undefined? rc=0\n", args[0].arg.s);
		rc = false;
	        break;
	    }
	  else
	  {
	    DUMP_EXEC (": if var=%s value=undefined rc=0\n", args[0].arg.s);
	    rc = false;
	  }
	  if (rc)
	  {
	    celXmlScriptEventHandler* truebranch = args[1].arg.h;
	    if (truebranch)
	    {
	      truebranch->ResolveParameters (entity);
	      truebranch->Execute (entity, behave);
	    }
	  }
	  else
	  {
	    celXmlScriptEventHandler* falsebranch = args[2].arg.h;
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
	  DUMP_EXEC (": print %s\n", op.arg.arg.s);
	  printf ("%s\n", op.arg.arg.s);
	  fflush (stdout);
	}
	break;
      case CEL_OPERATION_ACTION:
        {
          DUMP_EXEC (": action pc=%d id=%d s=%s\n", args[0].arg.pc,
	  	args[1].arg.id, args[2].arg.s);
	  resolvers[args[0].arg.pc].pc->PerformAction (
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
	  iPcProperties* props = behave->GetProperties ();
	  if (!props) break;	// @@@ Report error!
	  const char* pn = args[0].arg.s;
	  switch (args[1].type)
	  {
	    case CEL_TYPE_VAR:
	      {
                DUMP_EXEC (": var %s=var %s\n", pn, args[1].arg.s);
		int idx = props->GetPropertyIndex (args[1].arg.s);
		if (idx == -1) break;	// @@@ Report error!
		switch (props->GetPropertyType (idx))
		{
		  case CEL_DATA_LONG:
		    props->SetProperty (pn, props->GetPropertyLong (idx));
		    break;
		  case CEL_DATA_FLOAT:
		    props->SetProperty (pn, props->GetPropertyFloat (idx));
		    break;
		  case CEL_DATA_BOOL:
		    props->SetProperty (pn, props->GetPropertyBool (idx));
		    break;
		  case CEL_DATA_STRING:
		    props->SetProperty (pn, props->GetPropertyString (idx));
		    break;
		  case CEL_DATA_VECTOR2:
		    {
		      csVector2 v;
		      props->GetPropertyVector (idx, v);
		      props->SetProperty (pn, v);
		    }
		    break;
		  case CEL_DATA_VECTOR3:
		    {
		      csVector3 v;
		      props->GetPropertyVector (idx, v);
		      props->SetProperty (pn, v);
		    }
		    break;
		  case CEL_DATA_COLOR:
		    {
		      csColor v;
		      props->GetPropertyColor (idx, v);
		      props->SetProperty (pn, v);
		    }
		    break;
		  default:
		    CS_ASSERT (false);
		    break;
		}
	      }
	      break;
	    case CEL_TYPE_BOOL:
              DUMP_EXEC (": var %s=%d\n", pn, args[1].arg.b);
	      props->SetProperty (pn, args[1].arg.b);
	      break;
	    case CEL_TYPE_FLOAT:
              DUMP_EXEC (": var %s=%g\n", pn, args[1].arg.f);
	      props->SetProperty (pn, args[1].arg.f);
	      break;
	    case CEL_TYPE_STRING:
              DUMP_EXEC (": var %s=%s\n", pn, args[1].arg.s);
	      props->SetProperty (pn, args[1].arg.s);
	      break;
	    case CEL_TYPE_INT32:
              DUMP_EXEC (": var %s=%d\n", pn, args[1].arg.i);
	      props->SetProperty (pn, (long)args[1].arg.i);
	      break;
	    case CEL_TYPE_COLOR:
	      {
	        csColor col;
		col.red = args[1].arg.col.red;
		col.green = args[1].arg.col.green;
		col.blue = args[1].arg.col.blue;
                DUMP_EXEC (": var %s=%g,%g,%g\n", pn, col.red,
			col.green, col.blue);
	        props->SetProperty (pn, col);
	      }
	      break;
	    case CEL_TYPE_VECTOR2:
	      {
	        csVector2 vec;
		vec.x = args[1].arg.vec.x;
		vec.y = args[1].arg.vec.y;
                DUMP_EXEC (": var %s=%g,%g\n", pn, vec.x, vec.y);
	        props->SetProperty (pn, vec);
	      }
	      break;
	    case CEL_TYPE_VECTOR3:
	      {
	        csVector3 vec;
		vec.x = args[1].arg.vec.x;
		vec.y = args[1].arg.vec.y;
		vec.z = args[1].arg.vec.z;
                DUMP_EXEC (": var %s=%g,%g,%g\n", pn, vec.x,
			vec.y, vec.z);
	        props->SetProperty (pn, vec);
	      }
	      break;
	    default:
	      CS_ASSERT (false);
	  }
	}
	break;
      case CEL_OPERATION_GETPROPERTY:
        {
	  iPcProperties* props = behave->GetProperties ();
	  if (!props) break;	// @@@ Report error!
	  iCelPropertyClass* pc = resolvers[args[1].arg.pc].pc;
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
          DUMP_EXEC (": property pc=%d id=%d\n", args[0].arg.pc,
	  	args[1].arg.id);
	  iCelPropertyClass* pc = resolvers[args[0].arg.pc].pc;
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

int celXmlScriptEventHandler::GetResolver (const char* pcname)
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

