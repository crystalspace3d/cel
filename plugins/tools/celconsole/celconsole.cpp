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

  return true;
}

bool celConsole::HandleEvent (iEvent& ev)
{
  if (CS_IS_KEYBOARD_EVENT(name_reg,ev))
  {
    utf32_char key = csKeyEventHelper::GetCookedCode (&ev);
    if (key == CSKEY_TAB)
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
    g3d->BeginDraw (CSDRAW_2DGRAPHICS);
    if (conout->GetVisible ())
      conout->Draw2D (0);
    g3d->BeginDraw (CSDRAW_3DGRAPHICS);
    if (conout->GetVisible ())
      conout->Draw3D (0);
  }
  return false;
}

void celConsole::Execute (const char* cmd)
{
  printf ("Exec %s\n", cmd);
  fflush (stdout);
}

//---------------------------------------------------------------------------
