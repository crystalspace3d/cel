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
#include "csutil/objreg.h"
#include "csutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "ivideo/graph3d.h"
#include "ivaria/reporter.h"
#include "ivaria/conin.h"

#include "plugins/tools/celconsole/celconsole.h"

CS_IMPLEMENT_PLUGIN

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
  {
  }
  virtual ~cmdHelp () { }
  virtual const char* GetCommand () { return "help"; }
  virtual const char* GetDescription () { return "Get help."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: help [ <command> ]\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    if (args.Length () <= 1)
      parent->ListCommands ();
    else
      parent->HelpCommand (args[1]);
  }
};

class cmdListEnt : public scfImplementation1<cmdListEnt, iCelConsoleCommand>
{
private:
  celConsole* parent;

public:
  cmdListEnt (celConsole* parent) : scfImplementationType (this),
				    parent (parent)
  {
  }
  virtual ~cmdListEnt () { }
  virtual const char* GetCommand () { return "listent"; }
  virtual const char* GetDescription () { return "List entities."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: listent\n");
    parent->GetOutputConsole ()->PutText ("  List all entities and the name of the associated behaviour.\n");
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
				    parent (parent)
  {
  }
  virtual ~cmdListTpl () { }
  virtual const char* GetCommand () { return "listtpl"; }
  virtual const char* GetDescription () { return "List entity templates."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: listtpl\n");
    parent->GetOutputConsole ()->PutText ("  List all entity templates and the name of the associated behaviour.\n");
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
				    parent (parent)
  {
  }
  virtual ~cmdCreateEntTpl () { }
  virtual const char* GetCommand () { return "createenttpl"; }
  virtual const char* GetDescription () { return "Create an entity from a template."; }
  virtual void Help ()
  {
    parent->GetOutputConsole ()->PutText ("Usage: createenttpl <tpl> <entname> { <parname> <value> ... } \n");
    parent->GetOutputConsole ()->PutText ("  Create an entity from a template with the given parameters\n");
  }
  virtual void Execute (const csStringArray& args)
  {
    parent->CreateEntityFromTemplate (args);
  }
};


//--------------------------------------------------------------------------

celConsole::celConsole (iBase* parent)
  : scfImplementationType (this, parent)
{
  scfiEventHandler = 0;
}

celConsole::~celConsole ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    if (q != 0)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
}

bool celConsole::Initialize (iObjectRegistry* object_reg)
{
  celConsole::object_reg = object_reg;

  csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
      object_reg);

  conout = csQueryRegistry<iConsoleOutput> (object_reg);
  if (!conout)
  {
    conout = CS_LOAD_PLUGIN (plugmgr,
	"crystalspace.console.output.simple", iConsoleOutput);
    if (!conout)
    {
      csReport (object_reg,
	    	CS_REPORTER_SEVERITY_ERROR, "cel.console",
		"Can't load the output console!");
      return false;
    }
  }
  conin = csQueryRegistry<iConsoleInput> (object_reg);
  if (!conin)
  {
    conin = CS_LOAD_PLUGIN (plugmgr,
	"crystalspace.console.input.standard", iConsoleInput);
    if (!conin)
    {
      csReport (object_reg,
	    	CS_REPORTER_SEVERITY_ERROR, "cel.console",
		"Can't load the input console!");
      return false;
    }
  }
  conin->Bind (conout);
  conin->SetPrompt ("cel> ");
  ConsoleExecCallback* cb = new ConsoleExecCallback (this);
  conin->SetExecuteCallback (cb);
  cb->DecRef ();
  conout->SetVisible (false);

  scfiEventHandler = new EventHandler (this);
  csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
  CS_ASSERT (q != 0);
  q->RemoveListener (scfiEventHandler);
  csEventID esub[] = { 
    csevKeyboardEvent (object_reg),
    csevFrame (object_reg),
    csevPreProcess (object_reg),
    csevPostProcess (object_reg),
    csevProcess (object_reg),
    CS_EVENTLIST_END 
  };
  q->RegisterListener (scfiEventHandler, esub);

  name_reg = csEventNameRegistry::GetRegistry (object_reg);
  g3d = csQueryRegistry<iGraphics3D> (object_reg);

  csRef<iCelConsoleCommand> cmd;
  cmd.AttachNew (new cmdHelp (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdListEnt (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdListTpl (this)); RegisterCommand (cmd);
  cmd.AttachNew (new cmdCreateEntTpl (this)); RegisterCommand (cmd);

  return true;
}

bool celConsole::HandleEvent (iEvent& ev)
{
  if (CS_IS_KEYBOARD_EVENT(name_reg,ev))
  {
    utf32_char key = csKeyEventHelper::GetCookedCode (&ev);
    if (key == '~')
    {
      uint32 type = csKeyEventHelper::GetEventType (&ev);
      if (type == csKeyEventTypeDown)
      {
        if (conout->GetVisible ())
	  conout->SetVisible (false);
        else
	  conout->SetVisible (true);
      }
    }
    else
    {
      if (conout->GetVisible ())
        conin->HandleEvent (ev);
    }
  }
  if (ev.Name == csevPostProcess (name_reg))
  {
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

iCelPlLayer* celConsole::GetPL ()
{
  if (!pl)
  {
    pl = csQueryRegistry<iCelPlLayer> (object_reg);
    if (!pl)
      conout->PutText ("Can't find physical layer!\n");
  }
  return pl;
}

void celConsole::ListEntities ()
{
  if (!GetPL ()) return;
  size_t cnt = pl->GetEntityCount ();
  size_t i;
  for (i = 0 ; i < cnt ; i++)
  {
    iCelEntity* ent = pl->GetEntityByIndex (i);
    iCelBehaviour* bh = ent->GetBehaviour ();
    conout->PutText ("Entity %u: %s (%s)\n", (unsigned int)i,
	ent->GetName (), bh ? bh->GetName () : "<no behaviour>");
  }
}

void celConsole::ListTemplates ()
{
  if (!GetPL ()) return;
  size_t cnt = pl->GetEntityTemplateCount ();
  size_t i;
  for (i = 0 ; i < cnt ; i++)
  {
    iCelEntityTemplate* tpl = pl->GetEntityTemplate (i);
    const char* name = tpl->GetName ();
    const char* layer = tpl->GetBehaviourLayer ();
    const char* bh = tpl->GetBehaviour ();
    conout->PutText ("Template %u: %s (%s/%s)\n", (unsigned int)i,
	name, layer, bh);
  }
}

void celConsole::CreateEntityFromTemplate (const csStringArray& args)
{
  if (args.Length () < 3)
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
  celEntityTemplateParams params;
  size_t i;
  for (i = 3 ; i < args.Length ()-1 ; i += 2)
  {
    params.Put (args[i], args[i+1]);
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
  csHash<csRef<iCelConsoleCommand>, csStrKey>::GlobalIterator it =
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

void celConsole::Execute (const char* cmd)
{
  csStringArray args;
  args.SplitString (cmd, " \t", csStringArray::delimIgnore);
  if (args.Length () <= 0) return;
  if (!args[0] || !*args[0]) return;
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
