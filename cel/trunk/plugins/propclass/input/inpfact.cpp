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
#include "csutil/debug.h"
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

CS_IMPLEMENT_PLUGIN

CS_PLUGIN_NAMESPACE_BEGIN(pfInput)
{

CEL_IMPLEMENT_FACTORY (CommandInput, "pccommandinput")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistence",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

csStringID celPcCommandInput::id_trigger = csInvalidStringID;
csStringID celPcCommandInput::id_state = csInvalidStringID;
csStringID celPcCommandInput::id_command = csInvalidStringID;
csStringID celPcCommandInput::id_x = csInvalidStringID;
csStringID celPcCommandInput::id_y = csInvalidStringID;
csStringID celPcCommandInput::id_prefix = csInvalidStringID;
csStringID celPcCommandInput::id_activate = csInvalidStringID;

PropertyHolder celPcCommandInput::propinfo;

SCF_IMPLEMENT_IBASE (celPcCommandInput::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcCommandInput::celPcCommandInput (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  celPcCommandInput::object_reg = object_reg;

  keylist = 0;
  axislist = 0;
  buttonlist = 0;
  scfiEventHandler = 0;
  screenspace = false;
  do_cooked = false;
  do_sendtrigger = false;

  g2d = csQueryRegistry<iGraphics2D> (object_reg);
  if (!g2d)
  {
    Report (object_reg, "Can't find the graphics2d plugin!");
    return;
  }
  name_reg = csEventNameRegistry::GetRegistry (object_reg);

  Activate ();

  if (id_trigger == csInvalidStringID)
  {
    id_trigger = pl->FetchStringID ("cel.parameter.trigger");
    id_state = pl->FetchStringID ("cel.parameter.state");
    id_command = pl->FetchStringID ("cel.parameter.command");
    id_x = pl->FetchStringID ("cel.parameter.x");
    id_y = pl->FetchStringID ("cel.parameter.y");
    id_prefix = pl->FetchStringID ("cel.parameter.prefix");
    id_activate = pl->FetchStringID ("cel.parameter.activate");
  }

  // For properties.
  propholder = &propinfo;

  if (!propinfo.actions_done)
  {
    AddAction (action_activate, "cel.action.Activate");
    AddAction (action_bind, "cel.action.Bind");
    AddAction (action_removebind, "cel.action.RemoveBind");
    AddAction (action_removeallbinds, "cel.action.RemoveAllBinds");
    AddAction (action_loadconfig, "cel.action.LoadConfig");
    AddAction (action_saveconfig, "cel.action.SaveConfig");
  }

  propinfo.SetCount (3);
  AddProperty (propid_cooked, "cel.property.cooked",
  	CEL_DATA_BOOL, false, "Cooked mode.", &do_cooked);
  AddProperty (propid_screenspace, "cel.property.screenspace",
  	CEL_DATA_BOOL, false, "Screenspace mode.", &screenspace);
  AddProperty (propid_sendtrigger, "cel.property.sendtrigger",
  	CEL_DATA_BOOL, false, "Send trigger.", &do_sendtrigger);

  mouse_params = new celGenericParameterBlock (2);
  mouse_params->SetParameterDef (0, id_x, "x");
  mouse_params->SetParameterDef (1, id_y, "y");

  key_params = new celGenericParameterBlock (2);
  key_params->SetParameterDef (0,id_trigger, "trigger");
  key_params->SetParameterDef (1,id_state, "state");

  but_params = new celOneParameterBlock ();
  but_params->SetParameterDef (id_state, "state");
}

celPcCommandInput::~celPcCommandInput ()
{
  mouse_params->DecRef ();
  key_params->DecRef ();

  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
    if (q)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }

  //delete list of key mappings
  celKeyMap *kn, *k=keylist;
  while (k)
  {
    kn = k->next;
    delete [] k->command;
    delete k;
    k=kn;
  }

  //delete list of axis mappings
  celAxisMap *an, *a=axislist;
  while (a)
  {
    an = a->next;
    delete [] a->command;
    delete a;
    a=an;
  }

  //delete list of button mappings
  celButtonMap *bn, *b=buttonlist;
  while (b)
  {
    bn = b->next;
    if (b->command)
      delete [] b->command;
    delete b;
    b=bn;
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
        CEL_FETCH_BOOL_PAR (activate,params,id_activate);
        if (!activate) activate = true;
        Activate (activate);
        return true;
      }
    case action_bind:
      {
        CEL_FETCH_STRING_PAR (trigger,params,id_trigger);
        if (!trigger) return false;
        CEL_FETCH_STRING_PAR (command,params,id_command);
        if (!command) return false;
        Bind (trigger, command);
        return true;
      }
    case action_removebind:
      {
        CEL_FETCH_STRING_PAR (trigger,params,id_trigger);
        if (!trigger) return false;
        CEL_FETCH_STRING_PAR (command,params,id_command);
        if (!command) return false;
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
        CEL_FETCH_STRING_PAR (prefix,params,id_prefix);
        if (!prefix) return false;
        LoadConfig (prefix);
        return true;
      }
    case action_saveconfig:
      {
        CEL_FETCH_STRING_PAR (prefix,params,id_prefix);
        if (!prefix) return false;
        SaveConfig (prefix);
        return true;
      }
    default:
      return false;
  }
}

#define COMMANDINPUT_SERIAL 3

csPtr<iCelDataBuffer> celPcCommandInput::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (COMMANDINPUT_SERIAL);
  databuf->Add (do_cooked);
  databuf->Add (screenspace);
  int cnt = 0;
  celKeyMap* m = keylist;
  while (m)
  {
    cnt++;
    m = m->next;
  }
  databuf->Add ((int32)cnt);

  //TODO: Implement this for axes and buttons!

  m = keylist;
  while (m)
  {
    databuf->Add ((uint32)m->key);
    databuf->Add (m->command);
    m = m->next;
  }

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCommandInput::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != COMMANDINPUT_SERIAL)
  {
    Report (object_reg,"serialnr != COMMANDINPUT_SERIAL.  Cannot load.");
    return false;
  }
  do_cooked = databuf->GetBool ();
  screenspace = databuf->GetBool ();
  int cnt = databuf->GetInt32 ();
  int i;

  //TODO: Implement this for axes and buttons!

  for (i = 0 ; i < cnt ; i++)
  {
    int key = databuf->GetUInt32 ();
    const char* cmd = databuf->GetString ()->GetData ();
    celKeyMap* newmap = new celKeyMap ();
    // Add a new entry to key mapping list
    newmap->next = keylist;
    newmap->prev = 0;
    newmap->key = key;
    newmap->command = new char[strlen (cmd)+2];
    strcpy (newmap->command, cmd);
    newmap->command_end = strchr (newmap->command, 0);
    *(newmap->command_end+1) = 0; // Make sure there is an end there too.

    if (keylist)
      keylist->prev = newmap;
    keylist = newmap;
  }
  return true;
}

void celPcCommandInput::Activate (bool activate)
{
  if (activate)
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
  else
  {
    if (!scfiEventHandler)
      return;

    csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
    CS_ASSERT (q);
    q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
    scfiEventHandler = 0;
  }
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
    Bind (cfgit->GetKey (true), cfgit->GetStr ());

  return true;
}

void celPcCommandInput::SaveConfig (const char* prefix)
{
  csRef<iConfigManager> cfg = csQueryRegistry<iConfigManager> (object_reg);
  if (!cfg) return;
  celKeyMap* km = keylist;
  while (km)
  {
    csString cmdbind = csString(km->command+15);
    if (km->packedargs)
      cmdbind += ".args";
    csString strbind = csString (prefix);
    strbind += ".CommandInput.Bind.";
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
    cfg->SetStr (strbind.GetData (), cmdbind.GetData());
    km = km->next;
  }
  celAxisMap* am = axislist;
  while (am)
  {
    csString strbind = csString (prefix);
    strbind += ".CommandInput.Bind.";
    strbind += am->device;
    if (am->type == csevMouseMove (name_reg, am->device))
      strbind += "MouseAxis";
    if (am->type == csevJoystickMove (name_reg, am->device))
      strbind += "JoystickAxis";
    strbind += am->numeric;
    cfg->SetStr (strbind.GetData (), am->command+15);
    am = am->next;
  }
  celButtonMap* bm = buttonlist;
  while (bm)
  {
    csString cmdbind = csString(km->command+15);
    if (km->packedargs)
      cmdbind += ".args";
    csString strbind = csString (prefix);
    strbind += ".CommandInput.Bind.";
    strbind += bm->device;
    if (bm->type == csevMouseButton (name_reg, bm->device))
      strbind += "MouseButton";
    if (bm->type == csevJoystickButton (name_reg, bm->device))
      strbind += "JoystickButton";
    strbind += bm->numeric;
    cfg->SetStr (strbind.GetData (), cmdbind.GetData());
    bm = bm->next;
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
    command = strcommand.GetData();  
  }
  // Catch a special case that catches all keys.
  if (!strcasecmp ("key", triggername))
  {
    celKeyMap* newkmap;
    if (!(newkmap = GetMap (CS_UC_INVALID, 0)))
    {
      newkmap = new celKeyMap;
      // Add a new entry to key mapping list
      newkmap->next = keylist;
      newkmap->prev = 0;
      newkmap->key = CS_UC_INVALID;
      newkmap->modifiers = 0;
      newkmap->packedargs = packedargs;

      if (keylist)
        keylist->prev = newkmap;
      keylist = newkmap;
    }
    delete [] newkmap->command;
    newkmap->command = new char[
      	strlen ("pccommandinput_")+strlen (command)+2];
    strcpy (newkmap->command, "pccommandinput_");
    strcat (newkmap->command, command);
    newkmap->command_end = strchr (newkmap->command, 0);
    *(newkmap->command_end+1) = 0; // Make sure there is an end there too.
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
    Report (object_reg, "Bad input specification '%s'!", trig);
    return false;
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
      newkmap->next = keylist;
      newkmap->prev = 0;
      newkmap->key = key;
      newkmap->modifiers = mods;        // Only used in cooked mode.

      if (keylist)
        keylist->prev = newkmap;
      keylist = newkmap;
    }
    newkmap->packedargs = packedargs;
    // fill command in mapping structure
    delete [] newkmap->command;
    newkmap->command = new char[
      	strlen ("pccommandinput_")+strlen (command)+2];
    strcpy (newkmap->command, "pccommandinput_");
    strcat (newkmap->command, command);
    newkmap->command_end = strchr (newkmap->command, 0);
    *(newkmap->command_end+1) = 0; // Make sure there is an end there too.
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
        newamap->next = axislist;
        newamap->prev = 0;
        newamap->type = type;
        newamap->device = device;
        newamap->numeric = numeric;
        newamap->modifiers = mods;

        if (axislist)
          axislist->prev = newamap;
        axislist = newamap;
      }
      delete [] newamap->command;
      newamap->recenter = centered;
      newamap->command = new char[
        	strlen ("pccommandinput_")+strlen (command)+1];
      strcpy (newamap->command, "pccommandinput_");
      strcat (newamap->command, command);
    }
    // Joystick/Mouse button binding
    else
    {
      celButtonMap* newbmap;
      if (!(newbmap = GetButtonMap (type, device, numeric, mods)))
      {
        newbmap = new celButtonMap;
        // Add a new entry to button mapping list
        newbmap->next = buttonlist;
        newbmap->prev = 0;
        newbmap->type = type;
        newbmap->device = device;
        newbmap->numeric = numeric;
        newbmap->modifiers = mods;

        if (buttonlist)
          buttonlist->prev = newbmap;
        buttonlist = newbmap;
      }
      newbmap->packedargs = packedargs;
      delete [] newbmap->command;
      newbmap->command = new char[
        	strlen ("pccommandinput_")+strlen (command)+2];
      strcpy (newbmap->command, "pccommandinput_");
      strcat (newbmap->command, command);
      newbmap->command_end = strchr (newbmap->command, 0);
      *(newbmap->command_end+1) = 0; // Make sure there is an end there too.
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
    return map->command+15;
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
      return amap->command+15;
    }
    else
    {
      celButtonMap* bmap;
      if (!(bmap = GetButtonMap (type, device, numeric, mods)))
        return 0;
      return bmap->command+15;
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
    celKeyMap *pmap = 0, *map = keylist;
    while (map)
    {
      if (map->key == key && map->modifiers == mods)
      {
        pmap->next = map->next;
        delete map;
        return true;
      }
      pmap = map;
      map = map->next;
    }
    return false;
  }
  else if (csInputDefinition::ParseOther (name_reg, triggername, &type,
  	&device, &numeric, &modifiers))
  {
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    if (type == csevMouseMove (name_reg, device) ||
        type == csevJoystickMove (name_reg, device))
    {
      celAxisMap *pamap = 0, *amap = axislist;
      while (amap)
      {
        if (amap->type == type && amap->device == device &&
        	amap->numeric == numeric && amap->modifiers == mods)
        {
          pamap->next = amap->next;
          delete amap;
          return true;
        }
        pamap = amap;
        amap = amap->next;
      }
      return false;
    }
    else
    {
      celButtonMap *pbmap = 0, *bmap = buttonlist;
      while (bmap)
      {
        if (bmap->type == type && bmap->device == device &&
        	bmap->numeric == numeric && bmap->modifiers == mods)
        {
          pbmap->next = bmap->next;
          delete bmap;
          return true;
        }
        pbmap = bmap;
        bmap = bmap->next;
      }
      return false;
    }
  }
  return false;
}

void celPcCommandInput::RemoveAllBinds ()
{
  celKeyMap *key, *next;

  key = keylist;
  while (key)
  {
    next = key->next;
    delete key;
    key = next;
  }
  keylist = 0;

  celAxisMap *axis, *nexta;

  axis = axislist;
  while (axis)
  {
    nexta = axis->next;
    delete axis;
    axis = nexta;
  }
  axislist = 0;

  celButtonMap *button, *nextb;

  button = buttonlist;
  while (button)
  {
    nextb = button->next;
    delete button;
    button = nextb;
  }
  buttonlist = 0;
}

celKeyMap* celPcCommandInput::GetMap (utf32_char key, uint32 mods) const
{
  celKeyMap *p = keylist;
  while (p)
  {
    if (p->key == key && p->modifiers == mods)
      break;
    p = p->next;
  }

  return p;
}

celAxisMap* celPcCommandInput::GetAxisMap (csEventID type, uint device,
	int numeric, uint32 mods) const
{
  celAxisMap *p = axislist;
  while (p)
  {
    if (p->type == type && p->device == device &&
    	p->numeric == numeric && p->modifiers == mods)
      break;
    p = p->next;
  }

  return p;
}

celButtonMap* celPcCommandInput::GetButtonMap (csEventID type, uint device,
	int numeric, uint32 mods) const
{
  celButtonMap *p = buttonlist;
  while (p)
  {
    if (p->type == type && p->device == device &&
    	p->numeric == numeric && p->modifiers == mods)
      break;
    p = p->next;
  }

  return p;
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

void celPcCommandInput::SendKeyMessage (celKeyMap* p, utf32_char key,
    csKeyModifiers key_modifiers, celKeyState keystate)
{
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (!bh) return;
  celData ret;
  if (p->packedargs)
    key_params->GetParameter(1).Set(keystate);
  else
  {
    key_params->GetParameter(1).Set(CEL_KEY_STATE_UNUSED);
    switch(keystate)
    {
      case CEL_KEY_STATE_UP:
        *(p->command_end) = '0';
	break;
      case CEL_KEY_STATE_DOWN:
        *(p->command_end) = '1';
	break;
      case CEL_KEY_STATE_REPEAT:
        *(p->command_end) = '_';
	break;
      default:
	// cant happen really..
        *(p->command_end) = '0';
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
  bh->SendMessage (p->command, this, ret, key_params);
  if (!p->packedargs)
    *(p->command_end) = 0;
}

bool celPcCommandInput::HandleEvent (iEvent &ev)
{
  if (CS_IS_KEYBOARD_EVENT(name_reg,ev))
  {
    utf32_char key = csKeyEventHelper::GetRawCode (&ev);
    csKeyModifiers key_modifiers;
    csKeyEventHelper::GetModifiers (&ev, key_modifiers);
    uint32 modifiers = csKeyEventHelper::GetModifiersBits (key_modifiers);
    uint32 type = csKeyEventHelper::GetEventType (&ev);
    bool autorep = csKeyEventHelper::GetAutoRepeat (&ev);

    //find mapping
    celKeyMap *p = keylist;
    while (p)
    {
      if (p->key == CS_UC_INVALID)
	break;
      if (KeyEqual (p->key, key)
        && (!do_cooked || ((modifiers & p->modifiers) == p->modifiers)))
      {
        break;
      }
      p = p->next;
    }
    if (!p)
      return false;

    // Keep a reference to the entity to prevent deletion
    // during behaviour message handling.
    csRef<iCelEntity> keepref = entity;
    if (type == csKeyEventTypeUp)
    {
      SendKeyMessage (p, key, key_modifiers, CEL_KEY_STATE_UP);
    }
    else
    {
      if (autorep)
      {
        // Send auto-repeat message.
        SendKeyMessage (p, key, key_modifiers, CEL_KEY_STATE_REPEAT);
      }
      else
      {
        SendKeyMessage (p, key, key_modifiers, CEL_KEY_STATE_DOWN);
      }
    }
  }
  else if (CS_IS_MOUSE_EVENT(name_reg, ev))
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
      celAxisMap *p = axislist;
      csEventID mouse_id = csevMouseMove (object_reg, device);
      while (p)
      {
        if ((p->device == device) &&
        	((modifiers & p->modifiers) == p->modifiers) &&
        	(csEventNameRegistry::IsKindOf (name_reg, p->type, mouse_id)))
        {
          iCelBehaviour* bh = entity->GetBehaviour ();
          if (bh)
          {
            float x, y;
            x = (float) csMouseEventHelper::GetX (&ev);
            y = (float) csMouseEventHelper::GetY (&ev);
            celData ret;
            if (screenspace)
            {
              mouse_params->GetParameter (0).Set (x);
              mouse_params->GetParameter (1).Set (y);
            }
            else
            {
              mouse_params->GetParameter (0).Set (ScreenToCentered (x, 0));
              mouse_params->GetParameter (1).Set (ScreenToCentered (y, 1));
            }
            bh->SendMessage (p->command, this, ret, mouse_params);
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
        p = p->next;
      }
    }
    else
    {
      //mouse button event
      int button = csMouseEventHelper::GetButton (&ev);
      uint32 modifiers = csMouseEventHelper::GetModifiers (&ev);
      //find mapping
      celButtonMap *p = buttonlist;
      csEventID mouse_id = csevMouseButton (object_reg, device);
      while (p)
      {
        if ((p->numeric == button) && (p->device == device) &&
        	((modifiers & p->modifiers) == p->modifiers) &&
        	csEventNameRegistry::IsKindOf (name_reg, p->type, mouse_id))
        {
          break;
        }
        p = p->next;
      }
      if (!p)
        return false;

      // Keep a reference to the entity to prevent deletion
      // during behaviour message handling.
      csRef<iCelEntity> keepref = entity;

      if (ev.Name == csevMouseUp (object_reg, device))
      {
        iCelBehaviour* bh = entity->GetBehaviour();
        if (bh)
        {
          *(p->command_end) = '0';
          celData ret;
          bh->SendMessage (p->command, this, ret, 0);
          *(p->command_end) = 0;
        }
      }
      else if (ev.Name == csevMouseDown (object_reg, device))
      {
        iCelBehaviour* bh = entity->GetBehaviour();
        if (bh)
        {
          *(p->command_end) = '1';
          celData ret;
          bh->SendMessage (p->command, this, ret, 0);
          *(p->command_end) = 0;
        }
      }
    }
  }
  else if (CS_IS_JOYSTICK_EVENT(name_reg,ev))
  {
    uint device = csJoystickEventHelper::GetNumber(&ev);
    if (ev.Name == csevJoystickMove (name_reg, device))
    {
      // Keep a reference to the entity to prevent deletion
      // during behaviour message handling.
      csRef<iCelEntity> keepref = entity;

      //joystick move event
      uint32 modifiers = csJoystickEventHelper::GetModifiers(&ev);
      csJoystickEventData data;
      csJoystickEventHelper::GetEventData (&ev, data);
      //find mapping
      celAxisMap *p = axislist;
      csEventID joy_id = csevJoystickMove (object_reg, device);
      while (p)
      {
        if ((device == p->device) &&
        	((modifiers & p->modifiers) == p->modifiers) &&
        	(csEventNameRegistry::IsKindOf (name_reg, p->type, joy_id)) &&
        	((data.axesChanged & (1 << p->numeric)) != 0))
        {
          iCelBehaviour* bh = entity->GetBehaviour ();
          if (bh)
          {
            //The joystick driver returns values between -32767 and +32767.
            //Get the value in the range -1 to 1.
            float val = (float) csJoystickEventHelper::GetAxis(
            	&ev, p->numeric) / 32767;

            celData ret;
            csStringID id_joyval = pl->FetchStringID ("cel.parameter.value");
            celGenericParameterBlock *params =
            	new celGenericParameterBlock (1);
            params->SetParameterDef (0, id_joyval, "value");
            if (screenspace)
              params->GetParameter (0).Set (
              	CenteredToScreen (val, p->numeric));
            else
              params->GetParameter (0).Set (val);
            bh->SendMessage (p->command, this, ret, params);
            params->DecRef ();
          }
        }
        p = p->next;
      }
      if (!p)
        return false;
    }
    else
    {
      //joystick button event
      int button = csJoystickEventHelper::GetButton (&ev);
      uint device = csJoystickEventHelper::GetNumber (&ev);
      uint32 modifiers = csJoystickEventHelper::GetModifiers (&ev);

      //find mapping
      celButtonMap *p = buttonlist;
      csEventID joy_id = csevJoystickButton (object_reg, device);
      while (p)
      {
        if ((p->numeric == button) && (p->device == device) &&
        	((modifiers & p->modifiers) == p->modifiers) &&
        	csEventNameRegistry::IsKindOf (name_reg, p->type, joy_id))
        {
          break;
        }
        p = p->next;
      }
      if (!p)
        return false;

      // Keep a reference to the entity to prevent deletion
      // during behaviour message handling.
      csRef<iCelEntity> keepref = entity;

      celOneParameterBlock* celparms = 0;
      if (ev.Name == csevJoystickUp (name_reg, device))
      {
        iCelBehaviour* bh = entity->GetBehaviour ();
        if (bh)
        {
	  if (p->packedargs)
	  {
	    but_params->GetParameter(0).Set(CEL_KEY_STATE_UP);
	    celparms = but_params;
	  }
	  else
            *(p->command_end) = '0';
          celData ret;
          bh->SendMessage (p->command, this, ret, celparms);
	  if (!p->packedargs)
            *(p->command_end) = 0;
        }
      }
      else
      {
        iCelBehaviour* bh = entity->GetBehaviour ();
        if (bh)
        {
	  if (p->packedargs)
	  {
            but_params->GetParameter(0).Set(CEL_KEY_STATE_DOWN);
	    celparms = but_params;
	  }
	  else
            *(p->command_end) = '1';
          celData ret;
          bh->SendMessage (p->command, this, ret, celparms);
	  if (!p->packedargs)
            *(p->command_end) = 0;
        }
      }
    }
  }

  // Allow other entities to process this event
  return false;
}

//---------------------------------------------------------------------------

}
CS_PLUGIN_NAMESPACE_END(pfInput)
