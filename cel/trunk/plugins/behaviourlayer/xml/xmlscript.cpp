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
#include "plugins/behaviourlayer/xml/xmlscript.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

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
  }
}

void celXmlArg::Cleanup ()
{
  switch (type)
  {
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

celXmlScriptEventHandler::celXmlScriptEventHandler ()
{
  name = 0;
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
    if (!resolvers[i].pc)
    {
      resolvers[i].pc = pclist->FindByName (resolvers[i].pcname);
      printf ("Resolve '%s'->%p\n", resolvers[i].pcname, resolvers[i].pc);
      fflush (stdout);
    }
}

void celXmlScriptEventHandler::Execute (iCelEntity* entity)
{
  int i;
  for (i = 0 ; i < operations.Length () ; i++)
  {
    celXmlOperation& op = operations[i];
    switch (op.op)
    {
      case CEL_OPERATION_ACTION:
        {
	  csArray<celXmlArg>& args = op.arg.arg.a->args;
          printf ("action pc=%d id=%d s=%s\n", args[0].arg.pc, args[1].arg.id,
	  	args[2].arg.s);
	  fflush (stdout);
	  resolvers[args[0].arg.pc].pc->PerformAction (
	  	args[1].arg.id, args[2].arg.s);
	}
        break;
      case CEL_OPERATION_PROPERTY:
        {
	  csArray<celXmlArg>& args = op.arg.arg.a->args;
          printf ("property pc=%d id=%d\n", args[0].arg.pc, args[1].arg.id);
	  fflush (stdout);
	  switch (args[2].type)
	  {
	    case CEL_TYPE_BOOL:
	      resolvers[args[0].arg.pc].pc->SetProperty (
	  	    args[1].arg.id, args[2].arg.b);
	      break;
	    case CEL_TYPE_FLOAT:
	      resolvers[args[0].arg.pc].pc->SetProperty (
	  	    args[1].arg.id, args[2].arg.f);
	      break;
	    case CEL_TYPE_STRING:
	      resolvers[args[0].arg.pc].pc->SetProperty (
	  	    args[1].arg.id, args[2].arg.s);
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

celXmlScript::celXmlScript ()
{
  name = 0;
}

celXmlScript::~celXmlScript ()
{
  delete[] name;
}

celXmlScriptEventHandler* celXmlScript::CreateEventHandler (const char* name)
{
  celXmlScriptEventHandler* h = new celXmlScriptEventHandler ();
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

