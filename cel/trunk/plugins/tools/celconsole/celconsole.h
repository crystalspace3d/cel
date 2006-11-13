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
#define CELPROMPT "cel> "
#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/weakrefarr.h"
#include "csutil/stringarray.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "ivaria/conin.h"
#include "ivaria/conout.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "physicallayer/propclas.h"
#include "behaviourlayer/behave.h"
#include "behaviourlayer/bl.h"
#include "tools/celconsole.h"
#include "propclass/prop.h"
#include "propclass/quest.h"

struct iCelExpressionParser;

/**
 * A snapshot.
 */
class celSnapshot
{
public:
  csWeakRefArray<iCelEntity> entities;
  csArray<int> entities_refcount;
  csStringArray entities_names;
  csWeakRefArray<iCelPropertyClass> pcs;
  csArray<int> pcs_refcount;
};

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
  csRef<iCelEntity> console_entity;

  csHash<csRef<iCelConsoleCommand>, csStrKey> commands;
  csRef<iCelConsoleCommand> override;
  celSnapshot* snapshot;

  iCelEntity* GetConsoleEntity ();
  iCelExpressionParser* GetParser ();

  utf32_char console_key;
  utf32_char entlist_key;
  uint32 console_modifiers;
  uint32 entlist_modifiers;

  void AssignVar (iCelEntity* ent, iCelExpression* exprvar,
    iCelExpression* expr);

  // Array of entities we are monitoring.
  csWeakRefArray<iCelEntity> monitor_entities;
  csArray<bool> monitor_wasremoved;
  bool do_monitor;

public:
  celConsole (iBase* parent);
  virtual ~celConsole ();
  virtual bool Initialize (iObjectRegistry* object_reg);
  bool HandleEvent (iEvent& ev);
  iCelPlLayer* GetPL ();
  void RegisterNewEntity (iCelEntity* entity);
  void RegisterRemoveEntity (iCelEntity* entity);
  void SetOverrideCommand (iCelConsoleCommand *override_cmd,
		  	   const char*prompt=CELPROMPT);
  void Execute (const char* cmd);
  void ListCommands ();
  void HelpCommand (const char* cmd);
  void ListQuests ();
  void ListEntities ();
  void ListTemplates ();
  void ListInfoEntity (const csStringArray& args);
  void CreateEntityFromTemplate (const csStringArray& args);
  void Snapshot ();
  void SnapshotDiffPC (iCelEntity* ent);
  void SnapshotDiff ();
  void EvalulateExpression (const csStringArray& args);
  void AssignVar (const csStringArray& args);
  void AssignVarEntity (const csStringArray& args);

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

