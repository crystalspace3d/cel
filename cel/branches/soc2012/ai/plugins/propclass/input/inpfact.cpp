/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein

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
#include "iutil/objreg.h"
#include "iutil/event.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/cfgmgr.h"
#include "ivideo/graph2d.h"
#include "csutil/event.h"
#include "csutil/inputdef.h"
#include "csutil/csevent.h"
#include "plugins/propclass/input/inpfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "ivaria/reporter.h"
#include "celtool/stdparams.h"

//---------------------------------------------------------------------------

CS_PLUGIN_NAMESPACE_BEGIN(pfInput)
{

CEL_IMPLEMENT_FACTORY_ALT (CommandInput, "pcinput.standard", "pccommandinput")

//---------------------------------------------------------------------------

csStringID celPcCommandInput::id_trigger = csInvalidStringID;
csStringID celPcCommandInput::id_state = csInvalidStringID;
csStringID celPcCommandInput::id_command = csInvalidStringID;
csStringID celPcCommandInput::id_x = csInvalidStringID;
csStringID celPcCommandInput::id_y = csInvalidStringID;
csStringID celPcCommandInput::id_prefix = csInvalidStringID;
csStringID celPcCommandInput::id_activate = csInvalidStringID;
csStringID celPcCommandInput::id_value = csInvalidStringID;

PropertyHolder celPcCommandInput::propinfo;

celPcCommandInput::celPcCommandInput (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  celPcCommandInput::object_reg = object_reg;

  scfiEventHandler = 0;
  screenspace = false;
  do_cooked = false;
  do_sendtrigger = false;
  handleMouse = true;
  handleKeyboard = true;
  handleJoystick = true;

  g2d = csQueryRegistry<iGraphics2D> (object_reg);
  if (!g2d)
  {
    Error ("Can't find the graphics2d plugin!");
    return;
  }
  name_reg = csEventNameRegistry::GetRegistry (object_reg);

  Activate ();

  if (id_trigger == csInvalidStringID)
  {
    id_trigger = pl->FetchStringID ("trigger");
    id_state = pl->FetchStringID ("state");
    id_command = pl->FetchStringID ("command");
    id_x = pl->FetchStringID ("x");
    id_y = pl->FetchStringID ("y");
    id_prefix = pl->FetchStringID ("prefix");
    id_activate = pl->FetchStringID ("activate");
    id_value = pl->FetchStringID ("value");
  }

  // For properties.
  propholder = &propinfo;

  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.commandinput.action.");
    AddAction (action_activate, "Activate");
    AddAction (action_bind, "Bind");
    AddAction (action_removebind, "RemoveBind");
    AddAction (action_removeallbinds, "RemoveAllBinds");
    AddAction (action_loadconfig, "LoadConfig");
    AddAction (action_saveconfig, "SaveConfig");
  }

  propinfo.SetCount (3);
  AddProperty (propid_cooked, "cooked",
  	CEL_DATA_BOOL, false, "Cooked mode.", &do_cooked);
  AddProperty (propid_screenspace, "screenspace",
  	CEL_DATA_BOOL, false, "Screenspace mode.", &screenspace);
  AddProperty (propid_sendtrigger, "sendtrigger",
  	CEL_DATA_BOOL, false, "Send trigger.", &do_sendtrigger);

  mouse_params.AttachNew (new celVariableParameterBlock (3));
  mouse_params->AddParameter (id_x);
  mouse_params->AddParameter (id_y);
  mouse_params->AddParameter (id_value);

  key_params.AttachNew (new celVariableParameterBlock (2));
  key_params->AddParameter (id_trigger);
  key_params->AddParameter (id_state);

  joy_params.AttachNew (new celOneParameterBlock ());
  joy_params->SetParameterDef (id_value);

  but_params.AttachNew (new celOneParameterBlock ());
  but_params->SetParameterDef (id_state);
}

celPcCommandInput::~celPcCommandInput ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
    if (q)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
}

bool celPcCommandInput::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_activate:
      {
	bool activate;
	if (!Fetch (activate, params, id_activate, true, true)) return false;
	if (activate) Activate ();
	else Deactivate ();
        return true;
      }
    case action_bind:
      {
	csString trigger, command;
	if (!Fetch (trigger, params, id_trigger)) return false;
	if (!Fetch (command, params, id_command)) return false;
        Bind (trigger, command);
        return true;
      }
    case action_removebind:
      {
	csString trigger, command;
	if (!Fetch (trigger, params, id_trigger)) return false;
	if (!Fetch (command, params, id_command)) return false;
        RemoveBind (trigger, command);
        return true;
      }
    case action_removeallbinds:
      {
        RemoveAllBinds ();
        return true;
      }
    case action_loadconfig:
      {
	csString prefix;
	if (!Fetch (prefix, params, id_prefix)) return false;
        LoadConfig (prefix);
        return true;
      }
    case action_saveconfig:
      {
	csString prefix;
	if (!Fetch (prefix, params, id_prefix)) return false;
        SaveConfig (prefix);
        return true;
      }
    default:
      return false;
  }
}

void celPcCommandInput::Activate ()
{
  if (scfiEventHandler)
    return;

  csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
  CS_ASSERT (q);
  scfiEventHandler = new EventHandler (this);
  csEventID esub[] = {
    	csevKeyboardEvent (object_reg),
    	csevMouseEvent (object_reg),
    	csevJoystickEvent (object_reg),
    	CS_EVENTLIST_END
    	};
  q->RegisterListener (scfiEventHandler, esub);
}

void celPcCommandInput::Deactivate ()
{
  if (!scfiEventHandler)
    return;

  csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
  CS_ASSERT (q);
  q->RemoveListener (scfiEventHandler);
  scfiEventHandler->DecRef ();
  scfiEventHandler = 0;
}

bool celPcCommandInput::LoadConfig (const char* prefix)
{
  csRef<iConfigManager> cfg = csQueryRegistry<iConfigManager> (object_reg);
  if (!cfg) return false;

  csString strbind = csString (prefix);
  strbind += ".CommandInput.Bind.";
  csRef<iConfigIterator> cfgit (cfg->Enumerate (strbind.GetData ()));
  if (!cfgit) return false;
  while (cfgit->Next ())
  {
    Warning (
    	"*.CommandInput.Bind.key = action is deprecated. Use *.Input.Bind.action = key instead.");
    Bind (cfgit->GetKey (true), cfgit->GetStr ());
  }

  csString strfind = csString (prefix);
  strfind += ".Input.Bind.";
  csRef<iConfigIterator> it (cfg->Enumerate (strfind.GetData ()));
  if (!it) return false;
  while (it->Next ())
  {
    csString keystr = it->GetStr ();
    if (keystr.Find (" ") == (size_t)-1)
    {
      Bind (it->GetStr (), it->GetKey (true));
    }
    else
    {
      size_t start = 0;
      size_t pos = 0;
      do
      {
        pos = keystr.Find (" ", pos + 1);
        csString keyslice = keystr.Slice (start, pos - start);
        Bind (keyslice.GetData (), it->GetKey (true));
        start = pos + 1;
      }
      while (pos != (size_t)-1);
    }
  }

  return true;
}

void celPcCommandInput::SaveConfig (const char* prefix)
{
  csRef<iConfigManager> cfg = csQueryRegistry<iConfigManager> (object_reg);
  if (!cfg) return;
  for (size_t i = 0 ; i < keylist.GetSize () ; i++)
  {
    celKeyMap* km = keylist[i];
    csString cmdbind = km->commandStr;
    if (km->packedargs)
      cmdbind += ".args";
    csString strbind = csString (prefix);
    strbind += ".Input.Bind.";
    if (km->key == CS_UC_INVALID)
    {
      strbind += "key";
    }
    else
    {
      csKeyModifiers modifiers;
      csKeyEventHelper::GetModifiers (km->modifiers, modifiers);
      strbind += csInputDefinition::GetKeyString (
      	  name_reg, km->key, &modifiers).GetData ();
    }
    cfg->SetStr (cmdbind.GetData (), strbind.GetData ());
  }
  for (size_t i = 0 ; i < axislist.GetSize () ; i++)
  {
    celAxisMap* am = axislist[i];
    csString strbind = csString (prefix);
    strbind += ".Input.Bind.";
    strbind += am->device;
    if (am->type == csevMouseMove (name_reg, am->device))
      strbind += "MouseAxis";
    if (am->type == csevJoystickMove (name_reg, am->device))
      strbind += "JoystickAxis";
    strbind += am->numeric;
    cfg->SetStr (am->commandStr, strbind.GetData ());
  }
  for (size_t i = 0 ; i < buttonlist.GetSize () ; i++)
  {
    celButtonMap* bm = buttonlist[i];
    csString cmdbind = bm->commandStr;
    if (bm->packedargs)
      cmdbind += ".args";
    csString strbind = csString (prefix);
    strbind += ".Input.Bind.";
    strbind += bm->device;
    if (bm->type == csevMouseButton (name_reg, bm->device))
      strbind += "MouseButton";
    if (bm->type == csevJoystickButton (name_reg, bm->device))
      strbind += "JoystickButton";
    strbind += bm->numeric;
    cfg->SetStr (cmdbind.GetData (), strbind.GetData ());
  }
  cfg->Save ();
}

bool celPcCommandInput::Bind (const char* triggername, const char* rawcommand)
{
  // find if we want packed args for this mapping
  csString strcommand = csString (rawcommand);
  size_t packedpos = strcommand.FindStr (".args");
  bool packedargs = false;
  const char *command = rawcommand;
  if (packedpos != (size_t)-1)
  {
    packedargs = true;
    strcommand.Truncate (packedpos);
    command = strcommand.GetData ();
  }
  // Catch a special case that catches all keys.
  if (!strcasecmp ("key", triggername))
  {
    celKeyMap* newkmap;
    if (!(newkmap = GetMap (CS_UC_INVALID, 0)))
    {
      newkmap = new celKeyMap;
      // Add a new entry to key mapping list
      newkmap->key = CS_UC_INVALID;
      newkmap->modifiers = 0;
      newkmap->packedargs = packedargs;

      keylist.Push (newkmap);
    }
    newkmap->commandStr = command;
    return true;
  }
  // parse and handle event type
  csEventID type;
  uint device;
  int numeric;
  csString strtrigger = csString (triggername);
  bool centered = false;
  size_t centerpos = strtrigger.FindStr ("_centered");
  if (centerpos != (size_t)-1)
  {
    centered = true;
    strtrigger.Truncate (centerpos);
  }
  const char* trig = strtrigger.GetData ();
  csKeyModifiers modifiers;
  if (!csInputDefinition::ParseOther (name_reg, trig, &type, &device,
  	&numeric, &modifiers))
  {
    return Error ("Bad input specification '%s'!", trig);
  }
  // Key binding
  if (type == csevKeyboardEvent (object_reg))
  {
    utf32_char key, cooked;
    csInputDefinition::ParseKey (name_reg, trig, &key, &cooked, &modifiers);
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);

    // only bind single keys
    //  if (shift != 0)
    //      return false;

    celKeyMap* newkmap;
    if (!(newkmap = GetMap (key, mods)))
    {
      newkmap = new celKeyMap;
      // Add a new entry to key mapping list
      newkmap->key = key;
      newkmap->modifiers = mods;        // Only used in cooked mode.

      keylist.Push (newkmap);
    }
    newkmap->packedargs = packedargs;
    // fill command in mapping structure
    newkmap->commandStr = command;
    return true;
  }
  else
  {
    // Joystick/Mouse move binding
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    if (type == csevMouseMove (name_reg, device) ||
    	type == csevJoystickMove (name_reg, device))
    {
      celAxisMap* newamap;
      if (!(newamap = GetAxisMap (type, device, numeric, mods)))
      {
        newamap = new celAxisMap;
        // Add a new entry to axis mapping list
        newamap->type = type;
        newamap->device = device;
        newamap->numeric = numeric;
        newamap->modifiers = mods;
	axislist.Push (newamap);
      }
      newamap->recenter = centered;
      newamap->commandStr = command;
    }
    // Joystick/Mouse button binding
    else
    {
      celButtonMap* newbmap;
      if (!(newbmap = GetButtonMap (type, device, numeric, mods)))
      {
        newbmap = new celButtonMap;
        // Add a new entry to button mapping list
        newbmap->type = type;
        newbmap->device = device;
        newbmap->numeric = numeric;
        newbmap->modifiers = mods;
	buttonlist.Push (newbmap);
      }
      newbmap->packedargs = packedargs;
      newbmap->commandStr = command;
    }
    return true;
  }
  return false;
}

const char* celPcCommandInput::GetBind (const char* triggername) const
{
  utf32_char key, cooked;
  csEventID type;
  uint device;
  int numeric;
  csKeyModifiers modifiers;
  if (csInputDefinition::ParseKey (name_reg, triggername, &key,
  	&cooked, &modifiers))
  {
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    celKeyMap* map;
    if (!(map = GetMap (key, mods)))
      return 0;
    return map->commandStr;
  }
  else if (csInputDefinition::ParseOther (name_reg, triggername, &type,
  	&device, &numeric, &modifiers))
  {
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    if (type == csevMouseMove (name_reg, device) ||
    	type == csevJoystickMove (name_reg, device))
    {
      celAxisMap* amap;
      if (!(amap = GetAxisMap (type, device, numeric, mods)))
        return 0;
      return amap->commandStr;
    }
    else
    {
      celButtonMap* bmap;
      if (!(bmap = GetButtonMap (type, device, numeric, mods)))
        return 0;
      return bmap->commandStr;
    }
  }
  return 0;
}

bool celPcCommandInput::RemoveBind (const char* triggername,
	const char* command)
{
  utf32_char key, cooked;
  csEventID type;
  uint device;
  int numeric;
  csKeyModifiers modifiers;
  if (csInputDefinition::ParseKey (name_reg, triggername, &key,
  	&cooked, &modifiers))
  {
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    for (size_t i = 0 ; i < keylist.GetSize () ; i++)
    {
      celKeyMap* map = keylist[i];
      if (map->key == key && map->modifiers == mods)
      {
	keylist.DeleteIndex (i);
        return true;
      }
    }
    return false;
  }
  else if (csInputDefinition::ParseOther (name_reg, triggername, &type,
  	&device, &numeric, &modifiers))
  {
    bool empty = true;
    bool done = false;
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    if (type == csevMouseMove (name_reg, device) ||
    	type == csevJoystickMove (name_reg, device))
    {
      size_t i = 0;
      while ((i < axislist.GetSize ()) && (!done || empty))
      {
        celAxisMap *amap = axislist[i++];
        if (amap->type == type && amap->device == device)
        {
          if (amap->numeric == numeric && amap->modifiers == mods)
          {
	    --i;
	    axislist.DeleteIndex (i);
	    done = true;
          }
          else
          {
            empty = false;
          }
        }
      }

      // If no other bind device axis, show cursor
      if (empty)
        g2d->SetMouseCursor (csmcArrow);

      return done;
    }
    else
    {
      for (size_t i = 0 ; i < buttonlist.GetSize () ; i++)
      {
        celButtonMap *bmap = buttonlist[i];
        if (bmap->type == type && bmap->device == device &&
        	bmap->numeric == numeric && bmap->modifiers == mods)
        {
	  buttonlist.DeleteIndex (i);
          return true;
        }
      }
      return false;
    }
  }
  return false;
}

void celPcCommandInput::RemoveAllBinds ()
{
  keylist.DeleteAll ();
  axislist.DeleteAll ();
  buttonlist.DeleteAll ();
}

void celPcCommandInput::EnableMouseEvents ()
{
  handleMouse = true;
}

void celPcCommandInput::DisableMouseEvents ()
{
  handleMouse = false;
}

void celPcCommandInput::EnableKeyboardEvents ()
{
  handleKeyboard = true;
}

void celPcCommandInput::DisableKeyboardEvents ()
{
  handleKeyboard = false;
}

void celPcCommandInput::EnableJoystickEvents ()
{
  handleJoystick = true;
}

void celPcCommandInput::DisableJoystickEvents ()
{
  handleJoystick = false;
}

celKeyMap* celPcCommandInput::GetMap (utf32_char key, uint32 mods) const
{
  for (size_t i = 0 ; i < keylist.GetSize () ; i++)
  {
    celKeyMap *p = keylist[i];
    if (p->key == key && p->modifiers == mods) return p;
  }
  return 0;
}

celAxisMap* celPcCommandInput::GetAxisMap (csEventID type, uint device,
	int numeric, uint32 mods) const
{
  for (size_t i = 0 ; i < axislist.GetSize () ; i++)
  {
    celAxisMap *p = axislist[i];
    if (p->type == type && p->device == device &&
    	p->numeric == numeric && p->modifiers == mods)
      return p;
  }
  return 0;
}

celButtonMap* celPcCommandInput::GetButtonMap (csEventID type, uint device,
	int numeric, uint32 mods) const
{
  for (size_t i = 0 ; i < buttonlist.GetSize () ; i++)
  {
    celButtonMap *p = buttonlist[i];
    if (p->type == type && p->device == device &&
    	p->numeric == numeric && p->modifiers == mods)
      return p;
  }

  return 0;
}

void celPcCommandInput::ScreenCoordinates (bool screen)
{
  screenspace = screen;
}

float celPcCommandInput::ScreenToCentered (float screencoord, float axis)
{
  if (axis == 0)
    return (screencoord * 2 / g2d->GetWidth ()) - 1;
  else
    return (screencoord * 2 / g2d->GetHeight ()) - 1;
}

float celPcCommandInput::CenteredToScreen (float centeredcoord, float axis)
{
  if (axis == 0)
    return (centeredcoord + 1) / 2 * g2d->GetWidth ();
  else
    return (centeredcoord + 1) / 2 * g2d->GetHeight ();
}

static bool KeyEqual (utf32_char key1, utf32_char key2)
{
  return (key1 == key2)
  	|| (CSKEY_IS_MODIFIER (key1) && CSKEY_IS_MODIFIER (key2)
  	&& CSKEY_MODIFIER_COMPARE_CODE (key1, key2));
}

void celPcCommandInput::SendMessage (const char* command, char updown,
    iCelParameterBlock* params)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    csString cmd = "pccommandinput_";
    cmd += command;
    if (updown != ' ') cmd += updown;
    celData ret;
    bh->SendMessage ((const char*)cmd, this, ret, params);
  }
  csString cmd = "cel.input.";
  cmd += command;
  if (updown == '1') cmd += ".down";
  else if (updown == '0') cmd += ".up";
  else if (updown == '_') cmd += ".repeat";
  csStringID id = pl->FetchStringID (cmd);
  entity->QueryMessageChannel ()->SendMessage (id, this, params);
}

void celPcCommandInput::SendKeyMessage (celKeyMap* p, utf32_char key,
    csKeyModifiers key_modifiers, celKeyState keystate)
{
  char updown = ' ';
  if (p->packedargs)
    key_params->GetParameter (1).Set (keystate);
  else
  {
    key_params->GetParameter (1).Set (CEL_KEY_STATE_UNUSED);
    switch (keystate)
    {
      case CEL_KEY_STATE_UP:
	updown = '0';
        break;
      case CEL_KEY_STATE_DOWN:
	updown = '1';
        break;
      case CEL_KEY_STATE_REPEAT:
	updown = '_';
        break;
      default:
        // cant happen really..
        break;
    }
  }
  if (do_sendtrigger)
  {
    const char* trigger = csInputDefinition::GetKeyString (
    	name_reg, key, &key_modifiers).GetData ();
    key_params->GetParameter (0).Set (trigger);
  }
  else
  {
    key_params->GetParameter (0).Clear ();
  }
  SendMessage (p->commandStr, updown, key_params);
}

bool celPcCommandInput::HandleEvent (iEvent &ev)
{
  if (CS_IS_KEYBOARD_EVENT(name_reg,ev) && handleKeyboard)
  {
    utf32_char key = csKeyEventHelper::GetRawCode (&ev);
    csKeyModifiers key_modifiers;
    csKeyEventHelper::GetModifiers (&ev, key_modifiers);
    uint32 modifiers = csKeyEventHelper::GetModifiersBits (key_modifiers);
    uint32 type = csKeyEventHelper::GetEventType (&ev);
    bool autorep = csKeyEventHelper::GetAutoRepeat (&ev);

    // Find mapping
    celKeyMap* found = 0;
    for (size_t i = 0 ; i < keylist.GetSize () ; i++)
    {
      celKeyMap* p = keylist[i];
      if (p->key == CS_UC_INVALID)
        break;
      if (KeyEqual (p->key, key)
        && (!do_cooked || ((modifiers & p->modifiers) == p->modifiers)))
      {
	found = p;
        break;
      }
    }
    if (!found)
      return false;

    // Keep a reference to the entity to prevent deletion
    // during behaviour message handling.
    csRef<iCelEntity> keepref = entity;
    if (type == csKeyEventTypeUp)
    {
      SendKeyMessage (found, key, key_modifiers, CEL_KEY_STATE_UP);
    }
    else
    {
      if (autorep)
      {
        // Send auto-repeat message.
        SendKeyMessage (found, key, key_modifiers, CEL_KEY_STATE_REPEAT);
      }
      else
      {
        SendKeyMessage (found, key, key_modifiers, CEL_KEY_STATE_DOWN);
      }
    }
  }
  else if (CS_IS_MOUSE_EVENT(name_reg, ev) && handleMouse)
  {
    uint device = csMouseEventHelper::GetNumber (&ev);
    if (ev.Name == csevMouseMove (name_reg, device))
    {
      // Keep a reference to the entity to prevent deletion
      // during behaviour message handling.
      csRef<iCelEntity> keepref = entity;

      //mouse move event
      int modifiers = csMouseEventHelper::GetModifiers (&ev);
      //find mapping
      csEventID mouse_id = csevMouseMove (object_reg, device);
      for (size_t i = 0 ; i < axislist.GetSize () ; i++)
      {
        celAxisMap *p = axislist[i];
        if ((p->device == device) &&
        	((modifiers & p->modifiers) == p->modifiers) &&
        	(csEventNameRegistry::IsKindOf (name_reg, p->type, mouse_id)))
        {
          float x, y;
          x = (float) csMouseEventHelper::GetX (&ev);
          y = (float) csMouseEventHelper::GetY (&ev);
          if (screenspace)
          {
            mouse_params->GetParameter (0).Set (x);
            mouse_params->GetParameter (1).Set (y);
            if (p->numeric == 0)
              mouse_params->GetParameter (2).Set (x);
            else if (p->numeric == 1)
              mouse_params->GetParameter (2).Set (y);
          }
          else
          {
            mouse_params->GetParameter (0).Set (ScreenToCentered (x, 0));
            mouse_params->GetParameter (1).Set (ScreenToCentered (y, 1));
            if (p->numeric == 0)
              mouse_params->GetParameter (2).Set (ScreenToCentered (x, 0));
            else if (p->numeric == 1)
              mouse_params->GetParameter (2).Set (ScreenToCentered (y, 1));
          }
          SendMessage (p->commandStr, ' ', mouse_params);
        }
        if (p->recenter)
        {
          // Recenter mouse so we don't lose focus
          int width = g2d->GetWidth ();
          int height = g2d->GetHeight ();
          g2d->SetMousePosition (width / 2, height / 2);
          g2d->SetMouseCursor (csmcNone);
        }
      }
    }
    else
    {
      //mouse button event
      int button = csMouseEventHelper::GetButton (&ev);
      uint32 modifiers = csMouseEventHelper::GetModifiers (&ev);
      //find mapping
      csEventID mouse_id = csevMouseButton (object_reg, device);
      celButtonMap* found = 0;
      for (size_t i = 0 ; i < buttonlist.GetSize () ; i++)
      {
        celButtonMap* p = buttonlist[i];
        if ((p->numeric == button) && (p->device == device) &&
        	((modifiers & p->modifiers) == p->modifiers) &&
        	csEventNameRegistry::IsKindOf (name_reg, p->type, mouse_id))
        {
	  found = p;
          break;
        }
      }
      if (!found)
        return false;

      // Keep a reference to the entity to prevent deletion
      // during behaviour message handling.
      csRef<iCelEntity> keepref = entity;

      if (ev.Name == csevMouseUp (object_reg, device))
        SendMessage (found->commandStr, '0', 0);
      else if (ev.Name == csevMouseDown (object_reg, device))
        SendMessage (found->commandStr, '1', 0);
    }
  }
  else if (CS_IS_JOYSTICK_EVENT(name_reg,ev) && handleJoystick)
  {
    uint device = csJoystickEventHelper::GetNumber(&ev);
    if (ev.Name == csevJoystickMove (name_reg, device))
    {
      // Keep a reference to the entity to prevent deletion
      // during behaviour message handling.
      csRef<iCelEntity> keepref = entity;

      //joystick move event
      uint32 modifiers = csJoystickEventHelper::GetModifiers (&ev);
      csJoystickEventData data;
      csJoystickEventHelper::GetEventData (&ev, data);
      //find mapping
      csEventID joy_id = csevJoystickMove (object_reg, device);
      bool found = false;
      for (size_t i = 0 ; i < axislist.GetSize () ; i++)
      {
        celAxisMap* p = axislist[i];
        if ((device == p->device) &&
        	((modifiers & p->modifiers) == p->modifiers) &&
        	(csEventNameRegistry::IsKindOf (name_reg, p->type, joy_id)) &&
        	((data.axesChanged & (1 << p->numeric)) != 0))
        {
          //The joystick driver returns values between -32767 and +32767.
          //Get the value in the range -1 to 1.
          float val = (float) csJoystickEventHelper::GetAxis(
            	&ev, p->numeric) / 32767;

          if (screenspace)
            joy_params->GetParameter (0).Set (
              	CenteredToScreen (val, p->numeric));
          else
            joy_params->GetParameter (0).Set (val);
          SendMessage (p->commandStr, ' ', joy_params);
	  found = true;
	  break;
        }
      }
      if (!found)
        return false;
    }
    else
    {
      //joystick button event
      int button = csJoystickEventHelper::GetButton (&ev);
      uint device = csJoystickEventHelper::GetNumber (&ev);
      uint32 modifiers = csJoystickEventHelper::GetModifiers (&ev);

      //find mapping
      csEventID joy_id = csevJoystickButton (object_reg, device);
      celButtonMap* found = 0;
      for (size_t i = 0 ; i < buttonlist.GetSize () ; i++)
      {
        celButtonMap* p = buttonlist[i];
        if ((p->numeric == button) && (p->device == device) &&
        	((modifiers & p->modifiers) == p->modifiers) &&
        	csEventNameRegistry::IsKindOf (name_reg, p->type, joy_id))
        {
	  found = p;
          break;
        }
      }
      if (!found)
        return false;

      // Keep a reference to the entity to prevent deletion
      // during behaviour message handling.
      csRef<iCelEntity> keepref = entity;

      celOneParameterBlock* celparms = 0;
      if (ev.Name == csevJoystickUp (name_reg, device))
      {
        if (found->packedargs)
        {
          but_params->GetParameter(0).Set (CEL_KEY_STATE_UP);
          celparms = but_params;
        }
        SendMessage (found->commandStr, '0', celparms);
      }
      else
      {
        if (found->packedargs)
        {
          but_params->GetParameter(0).Set (CEL_KEY_STATE_DOWN);
          celparms = but_params;
        }
        SendMessage (found->commandStr, '1', celparms);
      }
    }
  }

  // Allow other entities to process this event
  return false;
}

//---------------------------------------------------------------------------

}
CS_PLUGIN_NAMESPACE_END(pfInput)
