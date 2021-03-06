/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein

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
#include "csutil/objreg.h"
#include "csutil/event.h"
#include "csutil/cfgacc.h"
#include "csutil/inputdef.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "ivideo/graph3d.h"
#include "ivaria/reporter.h"
#include "ivaria/conin.h"
#include "ivaria/script.h"

#include "tools/expression.h"
#include "celtool/stdparams.h"
#include "plugins/tools/celconsole/celconsole.h"

SCF_IMPLEMENT_FACTORY (celConsole)

//--------------------------------------------------------------------------

class ConsoleExecCallback : public scfImplementation1<ConsoleExecCallback,
			    iConsoleExecCallback>
{
private:
  celConsole* parent;

public:
  ConsoleExecCallback (celConsole* parent)
    : scfImplementationType (this), parent (parent)
  {
  }
  virtual ~ConsoleExecCallback () { }
  virtual void Execute (const char* cmd)
  {
    parent->Execute (cmd);
  }
};

//--------------------------------------------------------------------------

class cmdHelp : public scfImplementation1<cmdHelp, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdHelp (celConsole* parent) : scfImplementationType (this), parent (parent)
  { }
  virtual ~cmdHelp () { }
  virtual const char* GetCommand () { return "help"; }
  virtual const char* GetDescription () { return "Get help."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: help [ <command> ]\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    if (args.GetSize () <= 1)
      parent->ListCommands ();
    else
      parent->HelpCommand (args[1]);
  }
};

//--------------------------------------------------------------------------

class cmdPython : public scfImplementation1<cmdPython, iCelConsoleCommand>
{
private:
  celConsole* parent;
  csRef<iScript> blpython;
public:
  cmdPython (celConsole* parent) : scfImplementationType (this), parent (parent)
  {
  }
  virtual ~cmdPython () { }
  virtual const char* GetCommand () { return "python"; }
  virtual const char* GetDescription () { return "Set console in python mode."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: python\n");
  }
  void PyOverride()
  {
    iCelBlLayer *pybl = parent->GetPL()->FindBehaviourLayer("blpython");
    if (pybl) {
      blpython = scfQueryInterface<iScript> (pybl);
      parent->GetOutputConsole ()->PutText(
	    "Console set in python mode. Write 'exit' to resume normal mode.\n");
      parent->SetOverrideCommand(this,"pycel> ");
    }
    else
      parent->GetOutputConsole ()->PutText(
	    "Python bl is not present (can't go into python mode)\n");
  }
  void PyCommand(const csStringArray& args)
  {
    unsigned int i;
    csString cmd = args[0];
    for (i=1;i<args.GetSize();i++)
    {
      cmd+=" ";
      cmd+=args[i];
    }
    blpython->RunText(
	"__ovr__ = blcelc.CelConsoleOutOverride(blcelc.object_reg_ptr)");
    blpython->RunText(cmd);
    blpython->RunText("del __ovr__");
  }
  virtual void Execute (const csStringArray& args)
  {
    if (strcmp(args[0],"python")==0)
      PyOverride();
    else if(strcmp(args[0],"importall")==0)
    {
      blpython->RunText("from cspace import *");
      blpython->RunText("from blcelc import *");
      blpython->RunText("pl = physicallayer_ptr");
      blpython->RunText("oreg = object_reg_ptr");
      parent->GetOutputConsole ()->PutText( 
	      "Imported all from cspace and blcelc\n");
    }
    else if(strcmp(args[0],"exit")==0)
    {
      parent->GetOutputConsole ()->PutText ("Back to normal console mode\n");
      parent->SetOverrideCommand(0);
    }
    else
      PyCommand (args);
  }
};

//--------------------------------------------------------------------------

class cmdListQuest : public scfImplementation1<cmdListQuest, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdListQuest (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdListQuest () { }
  virtual const char* GetCommand () { return "listquest"; }
  virtual const char* GetDescription () { return "List running quests."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: listquest\n");
    parent->GetOutputConsole ()->PutText ("  List all running quests.\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->ListQuests ();
  }
};

class cmdListEnt : public scfImplementation1<cmdListEnt, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdListEnt (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdListEnt () { }
  virtual const char* GetCommand () { return "listent"; }
  virtual const char* GetDescription () { return "List entities."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: listent\n");
    parent->GetOutputConsole ()->PutText (
	"  List all entities and the name of the associated behaviour.\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->ListEntities ();
  }
};

class cmdListTpl : public scfImplementation1<cmdListTpl, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdListTpl (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdListTpl () { }
  virtual const char* GetCommand () { return "listtpl"; }
  virtual const char* GetDescription () { return "List entity templates."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: listtpl\n");
    parent->GetOutputConsole ()->PutText (
     "  List all entity templates and the name of the associated behaviour.\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->ListTemplates ();
  }
};

class cmdCreateEntTpl : public scfImplementation1<cmdCreateEntTpl, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdCreateEntTpl (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdCreateEntTpl () { }
  virtual const char* GetCommand () { return "createenttpl"; }
  virtual const char* GetDescription ()
  { return "Create an entity from a template."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText (
	"Usage: createenttpl <tpl> <entname> { <parname> <value> ... } \n");
    parent->GetOutputConsole ()->PutText (
	"  Create an entity from a template with the given parameters\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->CreateEntityFromTemplate (args);
  }
};

class cmdInfoEnt : public scfImplementation1<cmdInfoEnt, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdInfoEnt (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdInfoEnt () { }
  virtual const char* GetCommand () { return "infoent"; }
  virtual const char* GetDescription ()
  { return "List information about an entity."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: infoent <entname>\n");
    parent->GetOutputConsole ()->PutText (
	"  List information about an entity.\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->ListInfoEntity (args);
  }
};

class cmdSnapshot : public scfImplementation1<cmdSnapshot, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdSnapshot (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdSnapshot () { }
  virtual const char* GetCommand () { return "snapshot"; }
  virtual const char* GetDescription ()
  { return "Snapshot current entities."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: snapshot\n");
    parent->GetOutputConsole ()->PutText (
	"  Keeps track of all entities currently in memory.\n");
    parent->GetOutputConsole ()->PutText (
	"  Use 'snapdiff' to check if entities got deleted or added since the snapshot.\n");
  }
  virtual void Execute (const csStringArray&)
  {
    parent->Snapshot ();
  }
};

class cmdSnapDiff : public scfImplementation1<cmdSnapDiff, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdSnapDiff (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdSnapDiff () { }
  virtual const char* GetCommand () { return "snapdiff"; }
  virtual const char* GetDescription ()
  { return "Show difference since snapshot."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: snapdiff\n");
    parent->GetOutputConsole ()->PutText (
	"  Shows all entity operations since snapshot.\n");
  }
  virtual void Execute (const csStringArray&)
  {
    parent->SnapshotDiff ();
  }
};

class cmdExpr : public scfImplementation1<cmdExpr, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdExpr (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdExpr () { }
  virtual const char* GetCommand () { return "expr"; }
  virtual const char* GetDescription ()
  { return "Evaluate an expression and print value."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: expr <expression>\n");
    parent->GetOutputConsole ()->PutText (
	"  Evaluate the expression and print the result.\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->EvalulateExpression (args);
  }
};

class cmdVar : public scfImplementation1<cmdVar, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdVar (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdVar () { }
  virtual const char* GetCommand () { return "var"; }
  virtual const char* GetDescription ()
  { return "Evaluate an expression and assign to a variable."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText (
	"Usage: var <varname> <expression>\n");
    parent->GetOutputConsole ()->PutText (
	"  Evaluate the expression and assign to a variable.\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->AssignVar (args);
  }
};

class cmdVarEnt : public scfImplementation1<cmdVarEnt, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdVarEnt (celConsole* parent) : scfImplementationType (this),
				    parent (parent) { }
  virtual ~cmdVarEnt () { }
  virtual const char* GetCommand () { return "varent"; }
  virtual const char* GetDescription ()
  { return "Evaluate an expression and assign to variable from other entity."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText (
	"Usage: varent <entname> <varname> <expression>\n");
    parent->GetOutputConsole ()->PutText (
	"  Evaluate the expression and assign to a variable\n");
    parent->GetOutputConsole ()->PutText (
	"  from another entity.\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->AssignVarEntity (args);
  }
};



//--------------------------------------------------------------------------

class celConsoleNewEntityCallback : public scfImplementation1<celConsoleNewEntityCallback,
  iCelNewEntityCallback>
{
private:
  csWeakRef<celConsole> console;

public:
  celConsoleNewEntityCallback (celConsole* console) : scfImplementationType (this),
  	console (console)
  {
  }
  virtual ~celConsoleNewEntityCallback () { }
  virtual void NewEntity (iCelEntity* entity)
  {
    if (console)
      console->RegisterNewEntity (entity);
  }
};


class celConsoleEntityRemoveCallback : public scfImplementation1<celConsoleEntityRemoveCallback,
  iCelEntityRemoveCallback>
{
private:
  csWeakRef<celConsole> console;

public:
  celConsoleEntityRemoveCallback (celConsole* console) : scfImplementationType (this),
  	console (console)
  {
  }
  virtual ~celConsoleEntityRemoveCallback () { }
  virtual void RemoveEntity (iCelEntity* entity)
  {
    if (console)
      console->RegisterRemoveEntity (entity);
  }
};


//--------------------------------------------------------------------------

celConsole::celConsole (iBase* parent)
  : scfImplementationType (this, parent)
{
  scfiEventHandler = 0;
  scfiOutEventHandler = 0;
  snapshot = 0;
}

celConsole::~celConsole ()
{
  if (console_entity)
    pl->RemoveEntity (console_entity);
  delete snapshot;
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
    if (q != 0)
    {
      q->RemoveListener (scfiEventHandler);
      q->RemoveListener (scfiOutEventHandler);
    }
    scfiEventHandler->DecRef ();
    scfiOutEventHandler->DecRef ();
  }
}

bool celConsole::Initialize (iObjectRegistry* object_reg)
{
  celConsole::object_reg = object_reg;

  csRef<iEventNameRegistry> namereg = csEventNameRegistry::GetRegistry (
      object_reg);

  csConfigAccess config;
  config.AddConfig (object_reg, "/celconfig/celconsole.cfg");
  const char* bind = config->GetStr ("CelConsole.Bind.Open", "tab");
  utf32_char cooked;
  csKeyModifiers modifiers;
  csInputDefinition::ParseKey (namereg, bind, &console_key, &cooked, &modifiers);
  console_modifiers = csKeyEventHelper::GetModifiersBits (modifiers);

  bind = config->GetStr ("CelConsole.Bind.List", "ctrl-shift-e");
  csInputDefinition::ParseKey (namereg, bind, &entlist_key, &cooked, &modifiers);
  entlist_modifiers = csKeyEventHelper::GetModifiersBits (modifiers);

  do_monitor = config->GetBool ("CelConsole.Monitor", false);

  csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
      object_reg);

  conout = csQueryRegistryOrLoad<iConsoleOutput> (object_reg,
  	"crystalspace.console.output.standard");
  if (!conout)
  {
    csReport (object_reg,
	    	CS_REPORTER_SEVERITY_ERROR, "cel.console",
		"Can't load the output console!");
    return false;
  }
  conin = csQueryRegistryOrLoad<iConsoleInput> (object_reg,
  	"crystalspace.console.input.standard");
  if (!conin)
  {
    csReport (object_reg,
	    	CS_REPORTER_SEVERITY_ERROR, "cel.console",
		"Can't load the input console!");
    return false;
  }
  conin->Bind (conout);
  conin->SetPrompt (CELPROMPT);
  ConsoleExecCallback* cb = new ConsoleExecCallback (this);
  conin->SetExecuteCallback (cb);
  cb->DecRef ();
  conout->SetVisible (false);
  conout->AutoUpdate (false);

  scfiEventHandler = new EventHandler (this);
  scfiOutEventHandler = new OutEventHandler (this);
  csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  csEventID esub[] = { 
    csevKeyboardEvent (object_reg),
    csevMouseEvent (object_reg),
    csevFrame (object_reg),
    CS_EVENTLIST_END 
  };
  q->RegisterListener (scfiEventHandler, esub);
  q->RegisterListener (scfiOutEventHandler, esub);

  name_reg = csEventNameRegistry::GetRegistry (object_reg);
  g3d = csQueryRegistry<iGraphics3D> (object_reg);

  csRef<iCelConsoleCommand> cmd;
  cmd.AttachNew (new cmdHelp (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdListEnt (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdListTpl (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdListQuest (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdCreateEntTpl (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdInfoEnt (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdSnapshot (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdSnapDiff (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdExpr (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdVar (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdVarEnt (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdPython (this)); RegisterCommand (cmd);

  GetPL ();

  return true;
}

iCelEntity* celConsole::GetConsoleEntity ()
{
  if (!GetPL ()) return 0;
  if (!console_entity)
  {
    console_entity = pl->CreateEntity ("__celconsole__", 0, 0,
      "pcproperties", CEL_PROPCLASS_END);
  }
  return console_entity;
}

bool celConsole::HandleDrawEvent (iEvent& ev)
{
  if (ev.Name == csevFrame (name_reg))
  {
    GetPL ();
    if (conout->GetVisible ())
    {
      g3d->BeginDraw (CSDRAW_2DGRAPHICS);
      conout->Draw2D (0);
      g3d->BeginDraw (CSDRAW_3DGRAPHICS);
      conout->Draw3D (0);
    }
  }
  return false;
}
bool celConsole::HandleEvent (iEvent& ev)
{
  if (CS_IS_KEYBOARD_EVENT(name_reg,ev))
  {
    utf32_char key = csKeyEventHelper::GetRawCode (&ev);
    csKeyModifiers key_modifiers;
    csKeyEventHelper::GetModifiers (&ev, key_modifiers);
    uint32 modifiers = csKeyEventHelper::GetModifiersBits (key_modifiers);
    uint32 type = csKeyEventHelper::GetEventType (&ev);

    if (key == console_key && modifiers == console_modifiers)
    {
      if (type == csKeyEventTypeDown)
      {
        if (conout->GetVisible ())
	  conout->SetVisible (false);
        else
	  conout->SetVisible (true);
      }
    }
    else if (key == entlist_key && modifiers == entlist_modifiers)
    {
      if (type == csKeyEventTypeDown)
      {
	if (!do_monitor)
	{
	  csReport (object_reg,
	    	CS_REPORTER_SEVERITY_WARNING, "cel.console",
		"Monitor is not enabled. Enable in celconsole.cfg!");
	}
	else
	{
	  printf ("List all entities still in memory:\n");
	  size_t i = 0;
	  while (i < monitor_entities.GetSize ())
	  {
	    iCelEntity* ent = monitor_entities[i];
	    if (ent)
	    {
	      if (monitor_wasremoved[i])
	        printf ("  ###### Entity %p/'%s' removed but still in memory (ref=%d)!\n",
		    ent, ent->GetName (), ent->GetRefCount ());
	      else
	        printf ("  Entity %p/'%s' not removed yet (ref=%d).\n",
		    ent, ent->GetName (), ent->GetRefCount ());
	      i++;
	    }
	    else
	    {
	      monitor_entities.DeleteIndex (i);
	      monitor_wasremoved.DeleteIndex (i);
	    }
	  }
	  fflush (stdout);
	}
      }
    }
    else
    {
      if (conout->GetVisible ())
      {
        return conin->HandleEvent (ev);
      }
    }
  }
  else if (CS_IS_MOUSE_EVENT (name_reg,ev))
  {
    if (conout->GetVisible ())
    {
      // Eat mouse events while console is visible.
      return true;
    }
  }
  return false;
}

iCelPlLayer* celConsole::GetPL ()
{
  if (!pl)
  {
    pl = csQueryRegistry<iCelPlLayer> (object_reg);
    if (!pl) return 0;
    if (do_monitor)
    {
      csRef<celConsoleNewEntityCallback> new_cb;
      new_cb.AttachNew (new celConsoleNewEntityCallback (this));
      csRef<celConsoleEntityRemoveCallback> rem_cb;
      rem_cb.AttachNew (new celConsoleEntityRemoveCallback (this));
      pl->AddNewEntityCallback (new_cb);
      pl->AddEntityRemoveCallback (rem_cb);
    }
  }
  return pl;
}

iCelExpressionParser* celConsole::GetParser ()
{
  csRef<iObjectRegistryIterator> it = object_reg->Get (
      scfInterfaceTraits<iCelExpressionParser>::GetID (),
      scfInterfaceTraits<iCelExpressionParser>::GetVersion ());
  iBase* b = it->Next ();
  csRef<iCelExpressionParser> parser;
  if (b)
  {
    parser = scfQueryInterface<iCelExpressionParser> (b);
  }
  if (!parser)
  {
    csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
	object_reg);
    parser = csLoadPlugin<iCelExpressionParser> (plugmgr,
      "cel.behaviourlayer.xml");
    if (!parser)
    {
      conout->PutText ("Cannot find the expression parser!\n");
      return 0;
    }
    object_reg->Register (parser, "iCelExpressionParser");
  }
  return parser;
}

void celConsole::RegisterNewEntity (iCelEntity* entity)
{
  if (!do_monitor) return;
  monitor_entities.Push (entity);
  monitor_wasremoved.Push (false);
}

void celConsole::RegisterRemoveEntity (iCelEntity* entity)
{
  if (!do_monitor) return;
  size_t idx = monitor_entities.Find (entity);
  if (idx != csArrayItemNotFound)
    monitor_wasremoved[idx] = true;
}

void celConsole::AssignVar (iCelEntity* ent, iCelExpression* exprvar,
    iCelExpression* expr)
{
  if (!exprvar)
  {
    conout->PutText ("Error parsing expression for variable!\n");
    return;
  }
  if (!expr)
  {
    conout->PutText ("Error parsing expression!\n");
    return;
  }
  celData retvar;
  if (!exprvar->Execute (GetConsoleEntity (), retvar))
  {
    conout->PutText ("Error running expression!\n");
    return;
  }
  celData ret;
  if (!expr->Execute (GetConsoleEntity (), ret))
  {
    conout->PutText ("Error running expression!\n");
    return;
  }
  if (retvar.type != CEL_DATA_STRING)
  {
    conout->PutText ("Variable expression must be a string!\n");
    return;
  }
  csRef<iPcProperties> pcprop = celQueryPropertyClassEntity<iPcProperties> (ent);
  switch (ret.type)
  {
    case CEL_DATA_LONG:
      pcprop->SetProperty (retvar.value.s->GetData (), (long)ret.value.l);
      break;
    case CEL_DATA_ULONG:
      pcprop->SetProperty (retvar.value.s->GetData (), (long)ret.value.ul);
      break;
    case CEL_DATA_BOOL:
        pcprop->SetProperty (retvar.value.s->GetData (),
	    (ret.value.b ? true : false));
      break;
    case CEL_DATA_FLOAT:
      pcprop->SetProperty (retvar.value.s->GetData (), ret.value.f);
      break;
    case CEL_DATA_STRING:
      pcprop->SetProperty (retvar.value.s->GetData (), ret.value.s->GetData ());
      break;
    case CEL_DATA_PCLASS:
      pcprop->SetProperty (retvar.value.s->GetData (), ret.value.pc);
      break;
    case CEL_DATA_ENTITY:
      pcprop->SetProperty (retvar.value.s->GetData (), ret.value.ent);
      break;
    case CEL_DATA_COLOR:
      {
	csColor col (ret.value.col.red, ret.value.col.green,
	    ret.value.col.blue);
	pcprop->SetProperty (retvar.value.s->GetData (), col);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
	csVector3 v (ret.value.v.x, ret.value.v.y, ret.value.v.z);
        pcprop->SetProperty (retvar.value.s->GetData (), v);
      }
      break;
    case CEL_DATA_VECTOR2:
      {
	csVector2 v (ret.value.v.x, ret.value.v.y);
        pcprop->SetProperty (retvar.value.s->GetData (), v);
      }
      break;
    default:
      conout->PutText ("Warning! Unknown type!\n");
      break;
  }
}

void celConsole::AssignVarEntity (const csStringArray& args)
{
  if (args.GetSize () < 4)
  {
    conout->PutText ("Too few parameters for 'varent'!\n");
    return;
  }
  iCelExpressionParser* parser = GetParser ();
  if (!parser) return;
  csRef<iCelExpression> exprent = parser->Parse (args[1]);
  if (!exprent)
  {
    conout->PutText ("Error parsing expression for entity!\n");
    return;
  }
  csRef<iCelExpression> exprvar = parser->Parse (args[2]);
  csRef<iCelExpression> expr = parser->Parse (args[3]);
  celData retent;
  if (!exprent->Execute (GetConsoleEntity (), retent))
  {
    conout->PutText ("Error running expression for entity!\n");
    return;
  }
  if (retent.type != CEL_DATA_ENTITY && retent.type != CEL_DATA_STRING)
  {
    conout->PutText ("Entity expression must be an entity or a string!\n");
    return;
  }
  iCelEntity* ent;
  if (retent.type == CEL_DATA_ENTITY)
    ent = retent.value.ent;
  else
    ent = pl->FindEntity (retent.value.s->GetData ());
  AssignVar (ent, exprvar, expr);
}

void celConsole::AssignVar (const csStringArray& args)
{
  if (args.GetSize () < 3)
  {
    conout->PutText ("Too few parameters for 'var'!\n");
    return;
  }
  iCelExpressionParser* parser = GetParser ();
  if (!parser) return;
  csRef<iCelExpression> exprvar = parser->Parse (args[1]);
  csRef<iCelExpression> expr = parser->Parse (args[2]);
  if (!GetConsoleEntity ()) return;
  AssignVar (GetConsoleEntity (), exprvar, expr);
}

void celConsole::EvalulateExpression (const csStringArray& args)
{
  if (args.GetSize () < 2)
  {
    conout->PutText ("Too few parameters for 'expr'!\n");
    return;
  }
  iCelExpressionParser* parser = GetParser ();
  if (!parser) return;
  csRef<iCelExpression> expr = parser->Parse (args[1]);
  if (!expr)
  {
    conout->PutText ("Error parsing expression!\n");
    return;
  }
  celData ret;
  if (!expr->Execute (GetConsoleEntity (), ret))
  {
    conout->PutText ("Error running expression!\n");
    return;
  }
  switch (ret.type)
  {
    case CEL_DATA_LONG:
      conout->PutText ("LONG: %d\n", ret.value.l);
      break;
    case CEL_DATA_ULONG:
      conout->PutText ("ULONG: %u\n", ret.value.ul);
      break;
    case CEL_DATA_BOOL:
      conout->PutText ("BOOL: %d\n", ret.value.b);
      break;
    case CEL_DATA_FLOAT:
      conout->PutText ("FLOAT: %g\n", ret.value.f);
      break;
    case CEL_DATA_STRING:
      conout->PutText ("STRING: %s\n", ret.value.s->GetData ());
      break;
    case CEL_DATA_PCLASS:
      conout->PutText ("PC: %p/%s\n", ret.value.pc,
	  ret.value.pc ? ret.value.pc->GetName () : "<null>");
      break;
    case CEL_DATA_ENTITY:
      conout->PutText ("ENTITY: %p/%s\n", ret.value.ent,
	  ret.value.ent ? ret.value.ent->GetName () : "<null>");
      break;
    case CEL_DATA_COLOR:
      conout->PutText ("COLOR: %g,%g,%g\n", ret.value.col.red,
	  ret.value.col.green, ret.value.col.blue);
      break;
    case CEL_DATA_VECTOR3:
      conout->PutText ("VECTOR3: %g,%g,%g\n", ret.value.v.x,
	  ret.value.v.y, ret.value.v.z);
      break;
    case CEL_DATA_VECTOR2:
      conout->PutText ("VECTOR2: %g,%g\n", ret.value.v.x,
	  ret.value.v.y);
      break;
    default:
      conout->PutText ("UNKNOWN\n");
      break;
  }
}

void celConsole::Snapshot ()
{
  if (!GetPL ()) return;
  delete snapshot;
  snapshot = new celSnapshot ();
  size_t count = pl->GetEntityCount ();
  size_t i;
  for (i = 0 ; i < count ; i++)
  {
    iCelEntity* ent = pl->GetEntityByIndex (i);
    snapshot->entities.Push (ent);
    snapshot->entities_refcount.Push (ent->GetRefCount ());
    snapshot->entities_names.Push (ent->GetName ());
    iCelPropertyClassList* plist = ent->GetPropertyClassList ();
    size_t pccount = plist->GetCount ();
    size_t j;
    for (j = 0 ; j < pccount ; j++)
    {
      iCelPropertyClass* pc = plist->Get (j);
      snapshot->pcs.Push (pc);
      snapshot->pcs_refcount.Push (pc->GetRefCount ());
    }
  }
}

void celConsole::SnapshotDiffPC (iCelEntity* ent)
{
  iCelPropertyClassList* plist = ent->GetPropertyClassList ();
  size_t pccount = plist->GetCount ();
  size_t i, j;
  for (i = 0 ; i < pccount ; i++)
  {
    iCelPropertyClass* pc = plist->Get (i);
    size_t foundidx = csArrayItemNotFound;
    for (j = 0 ; j < snapshot->pcs.GetSize () ; j++)
    {
      if (snapshot->pcs[j] == pc) { foundidx = j; break; }
    }
    if (foundidx == csArrayItemNotFound)
    {
      conout->PutText ("  NEW PC %p/%s (ref=%d)\n", pc, pc->GetName (),
      	pc->GetRefCount ());
    }
    else if (snapshot->pcs_refcount[foundidx] != pc->GetRefCount ())
    {
      conout->PutText ("REF PC %p/%s (ref orig=%d now=%d)\n",
      	pc, pc->GetName (), snapshot->pcs_refcount[foundidx],
	pc->GetRefCount ());
    }
  }
#if 0
  // @@@ TODO
  for (i = 0 ; i < snapshot->pcs.GetSize () ; i++)
  {
    iCelPropertyClass* pc = snapshot->pcs[i];
    if (pc == 0)
    {
      conout->PutText ("DEL PC %s\n", snapshot->pcs_names[i]);
    }
    else
    {
      size_t foundidx = csArrayItemNotFound;
      for (j = 0 ; j < count ; j++)
      {
        if (ent == pl->GetEntityByIndex (j)) { foundidx = j; break; }
      }
      if (foundidx == csArrayItemNotFound)
      {
        conout->PutText ("LEAK Entity %p/%s (ref orig=%d now=%d)\n",
      	  ent, ent->GetName (), snapshot->entities_refcount[foundidx],
	  ent->GetRefCount ());
        SnapshotDiffPC (ent);
      }
    }
  }
#endif
}

void celConsole::SnapshotDiff ()
{
  if (!snapshot)
  {
    conout->PutText ("There is no snapshot!\n");
    return;
  }
  if (!GetPL ()) return;
  size_t count = pl->GetEntityCount ();
  size_t i, j;
  for (i = 0 ; i < count ; i++)
  {
    iCelEntity* ent = pl->GetEntityByIndex (i);
    size_t foundidx = csArrayItemNotFound;
    for (j = 0 ; j < snapshot->entities.GetSize () ; j++)
    {
      if (snapshot->entities[j] == ent) { foundidx = j; break; }
    }
    if (foundidx == csArrayItemNotFound)
    {
      conout->PutText ("NEW Entity %p/%s (ref=%d)\n", ent, ent->GetName (),
      	ent->GetRefCount ());
    }
    else if (snapshot->entities_refcount[foundidx] != ent->GetRefCount ())
    {
      conout->PutText ("REF Entity %p/%s (ref orig=%d now=%d)\n",
      	ent, ent->GetName (), snapshot->entities_refcount[foundidx],
	ent->GetRefCount ());
    }
    SnapshotDiffPC (ent);
  }
  for (i = 0 ; i < snapshot->entities.GetSize () ; i++)
  {
    iCelEntity* ent = snapshot->entities[i];
    if (ent == 0)
    {
      conout->PutText ("DEL Entity %s\n", snapshot->entities_names[i]);
    }
    else
    {
      size_t foundidx = csArrayItemNotFound;
      for (j = 0 ; j < count ; j++)
      {
        if (ent == pl->GetEntityByIndex (j)) { foundidx = j; break; }
      }
      if (foundidx == csArrayItemNotFound)
      {
        conout->PutText ("LEAK Entity %p/%s (ref orig=%d now=%d)\n",
      	  ent, ent->GetName (), snapshot->entities_refcount[foundidx],
	  ent->GetRefCount ());
        SnapshotDiffPC (ent);
      }
    }
  }
}

void celConsole::ListInfoEntity (const csStringArray& args)
{
  if (args.GetSize () < 2)
  {
    conout->PutText ("Too few parameters for 'infoent'!\n");
    return;
  }
  if (!GetPL ()) return;
  iCelEntity* entity = pl->FindEntity (args[1]);
  if (!entity)
  {
    conout->PutText ("Can't find entity '%s'!\n", args[1]);
    return;
  }
  conout->PutText ("Entity: %p/%s refcount=%d id=%u\n", entity,
      entity->GetName (), entity->GetRefCount (), entity->GetID ());
  iCelBehaviour* behave = entity->GetBehaviour ();
  if (behave)
  {
    conout->PutText ("Behaviour: %p/%s (layer %s)\n",
	behave, behave->GetName (),
	behave->GetBehaviourLayer ()->GetName ());
  }
  iCelPropertyClassList* plist = entity->GetPropertyClassList ();
  size_t count = plist->GetCount ();
  size_t i;
  for (i = 0 ; i < count ; i++)
  {
    iCelPropertyClass* pc = plist->Get (i);
    conout->PutText ("    PC: %p/%s (tag %s)\n", pc, pc->GetName (),
	pc->GetTag ());
    size_t propcount = pc->GetPropertyAndActionCount ();
    size_t j;
    for (j = 0 ; j < propcount ; j++)
    {
      csStringID id = pc->GetPropertyOrActionID (j);
      celDataType type = pc->GetPropertyOrActionType (id);
      const char* desc = pc->GetPropertyOrActionDescription (id);
      const char* idstr = pl->FetchString (id);
      int ro = int (pc->IsPropertyReadOnly (id));
      switch (type)
      {
	case CEL_DATA_LONG:
	  conout->PutText ("        prop: LONG id=%u/%s ro=%d v=%ld (%s)\n",
	      (unsigned int) id, idstr, ro, pc->GetPropertyLongByID (id), desc);
	  break;
	case CEL_DATA_FLOAT:
	  conout->PutText ("        prop: FLOAT id=%u/%s ro=%d v=%g (%s)\n",
	      (unsigned int) id, idstr, ro, pc->GetPropertyFloatByID (id),
              desc);
	  break;
	case CEL_DATA_BOOL:
	  conout->PutText ("        prop: BOOL id=%u/%s ro=%d v=%d (%s)\n",
	      (unsigned int) id, idstr, ro, pc->GetPropertyBoolByID (id), desc);
	  break;
	case CEL_DATA_STRING:
	  conout->PutText ("        prop: STRING id=%u/%s ro=%d v=%s (%s)\n",
	      (unsigned int) id, idstr, ro, pc->GetPropertyStringByID (id),
              desc);
	  break;
	case CEL_DATA_VECTOR2:
	  {
	    csVector2 v;
	    pc->GetPropertyVectorByID (id, v);
	    conout->PutText ("        prop: VECTOR2 id=%u/%s ro=%d v=%g,%g (%s)\n",
	      (unsigned int) id, idstr, ro, v.x, v.y, desc);
	  }
	  break;
	case CEL_DATA_VECTOR3:
	  {
	    csVector3 v;
	    pc->GetPropertyVectorByID (id, v);
	    conout->PutText ("        prop: VECTOR3 id=%u/%s ro=%d v=%g,%g,%g (%s)\n",
	      (unsigned int) id, idstr, ro, v.x, v.y, v.z, desc);
	  }
	  break;
	case CEL_DATA_COLOR:
	  {
	    csColor v;
	    pc->GetPropertyColorByID (id, v);
	    conout->PutText ("        prop: COLOR id=%u/%s ro=%d v=%g,%g,%g (%s)\n",
	      (unsigned int) id, idstr, ro, v.red, v.green, v.blue, desc);
	  }
	  break;
	case CEL_DATA_ACTION:
	  conout->PutText ("        prop: ACTION id=%u/%s ro=%d (%s)\n",
	      (unsigned int) id, idstr, ro, desc);
	  break;
	case CEL_DATA_PCLASS:
	  {
	    iCelPropertyClass* p = pc->GetPropertyPClassByID (id);
	    conout->PutText ("        prop: PC id=%u/%s ro=%d v=%p/%s (%s)\n",
	      (unsigned int) id, idstr, ro, p, p ? p->GetName () : "0", desc);
	  }
	  break;
	case CEL_DATA_ENTITY:
	  {
	    iCelEntity* p = pc->GetPropertyEntityByID (id);
	    conout->PutText ("        prop: ENTITY id=%u/%s ro=%d v=%p/%s (%s)\n",
	      (unsigned int) id, idstr, ro, p, p ? p->GetName () : "0", desc);
	  }
	  break;
	default:
	  conout->PutText ("        prop: ? id=%u/%s ro=%d (%s)\n",
	      (unsigned int) id, idstr, ro, desc);
	  break;
      }
    }
  }
}

void celConsole::ListQuests ()
{
  if (!GetPL ()) return;
  size_t cnt = pl->GetEntityCount ();
  size_t i, j;
  for (i = 0 ; i < cnt ; i++)
  {
    iCelEntity* ent = pl->GetEntityByIndex (i);
    if (ent == console_entity) continue;
    iCelPropertyClassList* plist = ent->GetPropertyClassList ();
    for (j = 0 ; j < plist->GetCount () ; j++)
    {
      iCelPropertyClass* pc = plist->Get (j);
      csRef<iPcQuest> pcquest = scfQueryInterface<iPcQuest> (pc);
      if (pcquest)
      {
	if (pc->GetTag ())
	{
	  conout->PutText (
	      "Quest '%s' (tag '%s') for entity %s -> state '%s'\n",
	      pcquest->GetQuestName (), pc->GetTag (),
	      ent->GetName (), pcquest->GetQuest ()->GetCurrentState ());
	}
	else
	{
	  conout->PutText (
	      "Quest '%s' for entity %s -> state '%s'\n",
	      pcquest->GetQuestName (),
	      ent->GetName (), pcquest->GetQuest ()->GetCurrentState ());
	}
      }
    }
  }
}

void celConsole::ListEntities ()
{
  if (!GetPL ()) return;
  size_t cnt = pl->GetEntityCount ();
  int counter = 0;
  size_t i;
  for (i = 0 ; i < cnt ; i++)
  {
    iCelEntity* ent = pl->GetEntityByIndex (i);
    if (ent == console_entity) continue;
    iCelBehaviour* bh = ent->GetBehaviour ();
    conout->PutText ("Entity %d: %s (%s)\n", counter,
	ent->GetName (), bh ? bh->GetName () : "<no behaviour>");
    counter++;
  }
}

void celConsole::ListTemplates ()
{
  if (!GetPL ()) return;
  csRef<iCelEntityTemplateIterator> it = pl->GetEntityTemplates ();
  while (it->HasNext ())
  {
    iCelEntityTemplate* tpl = it->Next ();
    const char* name = tpl->GetName ();
    const char* layer = tpl->GetBehaviourLayer ();
    const char* bh = tpl->GetBehaviour ();
    conout->PutText ("Template: %s (%s/%s)\n", name, layer, bh);
  }
}

void celConsole::CreateEntityFromTemplate (const csStringArray& args)
{
  if (args.GetSize () < 3)
  {
    conout->PutText ("Too few parameters for 'createenttpl'!\n");
    return;
  }
  if (!GetPL ()) return;
  const char* tplname = args[1];
  iCelEntityTemplate* tpl = pl->FindEntityTemplate (tplname);
  if (!tpl)
  {
    conout->PutText ("Can't find entity template '%s'!\n", tplname);
    return;
  }
  const char* entname = args[2];
  csRef<celVariableParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());
  for (size_t i = 3 ; i < args.GetSize ()-1 ; i += 2)
  {
    csStringID id = pl->FetchStringID (args[i]);
    params->AddParameter (id).Set (args[i+1]);
  }
  iCelEntity* ent = pl->CreateEntity (tpl, entname, params);
  if (!ent)
  {
    conout->PutText ("Can't create entity!\n");
    return;
  }
}

void celConsole::ListCommands ()
{
  csHash<csRef<iCelConsoleCommand>, csStringBase>::GlobalIterator it =
    commands.GetIterator ();
  while (it.HasNext ())
  {
    const csRef<iCelConsoleCommand>& cmd = it.Next ();
    conout->PutText ("%s: %s\n", cmd->GetCommand (), cmd->GetDescription ());
  }
}

void celConsole::HelpCommand (const char* cmd)
{
  csRef<iCelConsoleCommand> command = commands.Get (cmd, 0);
  if (!command)
  {
    conout->PutText ("Unknown command '%s'!\n", cmd);
  }
  else
  {
    command->Help ();
  }
}

void celConsole::SetOverrideCommand (iCelConsoleCommand *override_cmd,
				     const char*prompt)
{
  override = override_cmd;
  if (override)
    conin->SetPrompt(prompt);
  else
    conin->SetPrompt(CELPROMPT);
}

void celConsole::Execute (const char* cmd)
{
  csStringArray args;
  args.SplitString (cmd, " \t", csStringArray::delimIgnore);
  if (args.GetSize () <= 0) return;
  if (!args[0] || !*args[0]) return;
  if (override)
  {
	  override->Execute (args);
	  return;
  }
  csRef<iCelConsoleCommand> command = commands.Get (args[0], 0);
  if (!command)
  {
    conout->PutText ("Unknown command '%s'!\n", args[0]);
  }
  else
  {
    command->Execute (args);
  }
}

void celConsole::RegisterCommand (iCelConsoleCommand* command)
{
  commands.Put (command->GetCommand (), command);
}

//---------------------------------------------------------------------------
