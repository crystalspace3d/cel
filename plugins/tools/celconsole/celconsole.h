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

#ifndef __CEL_TOOLS_CELCONSOLE__
#define __CEL_TOOLS_CELCONSOLE__

#include "csutil/util.h"
#include "csutil/hash.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "ivaria/conin.h"
#include "ivaria/conout.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "behaviourlayer/behave.h"
#include "tools/celconsole.h"

/**
 * This is a input/output console for CEL.
 */
class celConsole : public scfImplementation2<
		   celConsole, iCelConsole, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iConsoleInput> conin;
  csRef<iConsoleOutput> conout;
  csRef<iEventNameRegistry> name_reg;
  csRef<iGraphics3D> g3d;
  csRef<iCelPlLayer> pl;

  csHash<csRef<iCelConsoleCommand>, csStrKey> commands;

  iCelPlLayer* GetPL ();

public:
  celConsole (iBase* parent);
  virtual ~celConsole ();
  virtual bool Initialize (iObjectRegistry* object_reg);
  bool HandleEvent (iEvent& ev);

  void Execute (const char* cmd);
  void ListCommands ();
  void HelpCommand (const char* cmd);
  void ListEntities ();
  void ListTemplates ();
  void CreateEntityFromTemplate (const csStringArray& args);

  virtual iConsoleInput* GetInputConsole () { return conin; }
  virtual iConsoleOutput* GetOutputConsole () { return conout; }
  virtual void RegisterCommand (iCelConsoleCommand* command);

  // Not an embedded interface to avoid circular references!!!
  class EventHandler : public scfImplementation1<EventHandler,
		       iEventHandler>
  {
  private:
    celConsole* parent;

  public:
    EventHandler (celConsole* parent)
      : scfImplementationType (this), parent (parent)
    {
    }
    virtual ~EventHandler ()
    {
    }

    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
    CS_EVENTHANDLER_NAMES("cel.tools.celconsole")
    CS_EVENTHANDLER_NIL_CONSTRAINTS
  } *scfiEventHandler;
};

#endif // __CEL_TOOLS_CELCONSOLE__

