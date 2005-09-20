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

CEL_IMPLEMENT_FACTORY (CommandInput, "pccommandinput")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
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
csStringID celPcCommandInput::id_command = csInvalidStringID;
csStringID celPcCommandInput::action_bind = csInvalidStringID;

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

  g2d = CS_QUERY_REGISTRY (object_reg, iGraphics2D);
  if (!g2d)
  {
    Report (object_reg, "Can't find the graphics2d plugin!");
    return;
  }

  Activate ();

  if (action_bind == csInvalidStringID)
  {
    action_bind = pl->FetchStringID ("cel.action.Bind");
    id_trigger = pl->FetchStringID ("cel.parameter.trigger");
    id_command = pl->FetchStringID ("cel.parameter.command");
  }
}

celPcCommandInput::~celPcCommandInput ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    if (q)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }

  //delete list of key mappings
  celKeyMap *kn, *k=keylist;
  while (k)
  {
    kn = k->next;
    if (k->command)
      delete [] k->command;
    delete k;
    k=kn;
  }

  //delete list of axis mappings
  celAxisMap *an, *a=axislist;
  while (a)
  {
    an = a->next;
    if (a->command)
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

bool celPcCommandInput::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_bind)
  {
    CEL_FETCH_STRING_PAR (trigger,params,id_trigger);
    if (!trigger) return false;
    CEL_FETCH_STRING_PAR (command,params,id_command);
    if (!command) return false;
    Bind (trigger, command);
    return true;
  }
  return false;
}

#define COMMANDINPUT_SERIAL 1

csPtr<iCelDataBuffer> celPcCommandInput::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (COMMANDINPUT_SERIAL);
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
    *(newmap->command_end+1) = 0;	// Make sure there is an end there too.

    newmap->is_on = false;
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

    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    CS_ASSERT (q);
    scfiEventHandler = new EventHandler (this);
    q->RegisterListener (scfiEventHandler, CSMASK_Input);
  }
  else
  {
    if (!scfiEventHandler)
      return;

    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    CS_ASSERT (q);
    q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
    scfiEventHandler = 0;
  }
}

bool celPcCommandInput::LoadConfig (const char* /*fname*/)
{
  //TODO: not implemented
  return false;
}

bool celPcCommandInput::Bind (const char* triggername, const char* command)
{
  int type, numeric;
  csString strtrigger = csString (triggername);
  bool centered = false;
  size_t centerpos = strtrigger.FindStr ("_centered");
  if (centerpos < (size_t)-1)
  {
    centered = true;
    strtrigger.Truncate (centerpos);
  }
  const char* trig = strtrigger.GetData ();
  csKeyModifiers modifiers;
  csInputDefinition::ParseOther (trig, &type, &numeric, &modifiers);
  if (type == csevKeyboard)
  {
    utf32_char key, cooked;
    csInputDefinition::ParseKey (trig, &key, &cooked, &modifiers);
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);

    // only bind single keys
    //  if (shift != 0)
    //      return false;

    celKeyMap* newkmap;
    if (!(newkmap = GetMap (key, mods)))
    {
      newkmap = new celKeyMap;
      // Add a new entry to key mapping list
      newkmap->next=keylist;
      newkmap->prev=0;
      newkmap->key=key;
      newkmap->modifiers=mods;
      newkmap->command = new char[strlen ("pccommandinput_")+strlen(command)+2];
      strcpy (newkmap->command, "pccommandinput_");
      strcat (newkmap->command, command);
      newkmap->command_end = strchr (newkmap->command, 0);
      *(newkmap->command_end+1) = 0;	// Make sure there is an end there too.

      newkmap->is_on=false;
      if (keylist)
        keylist->prev = newkmap;
      keylist = newkmap;
    }
    else
    {
      delete [] newkmap->command;
      newkmap->command = new char[strlen ("pccommandinput_")+strlen(command)+2];
      strcpy (newkmap->command, "pccommandinput_");
      strcat (newkmap->command, command);
      newkmap->command_end = strchr (newkmap->command, 0);
      *(newkmap->command_end+1) = 0;	// Make sure there is an end there too.
    }

    return true;
  }
  else
  {
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    if (type == csevMouseMove || type == csevJoystickMove)
    {
      celAxisMap* newamap;
      if (!(newamap = GetAxisMap (type, numeric, mods)))
      {
        newamap = new celAxisMap;
        // Add a new entry to axis mapping list
        newamap->next = axislist;
        newamap->prev = 0;
        newamap->type = type;
	newamap->numeric = numeric;
        newamap->modifiers = mods;
	newamap->recenter = centered;
        newamap->command = new char[strlen ("pccommandinput_")+strlen(command)+1];
        strcpy (newamap->command, "pccommandinput_");
        strcat (newamap->command, command);

        if (axislist)
          axislist->prev = newamap;
        axislist = newamap;
      }
      else
      {
        delete [] newamap->command;
	newamap->recenter = centered;
        newamap->command = new char[strlen ("pccommandinput_")+strlen(command)+1];
        strcpy (newamap->command, "pccommandinput_");
        strcat (newamap->command, command);
      }
    }
    else
    {
      celButtonMap* newbmap;
      if (!(newbmap = GetButtonMap (type, numeric, mods)))
      {
        newbmap = new celButtonMap;
        // Add a new entry to button mapping list
        newbmap->next=buttonlist;
        newbmap->prev=0;
        newbmap->type=type;
	newbmap->numeric=numeric;
        newbmap->modifiers=mods;
        newbmap->command = new char[strlen ("pccommandinput_")+strlen(command)+2];
        strcpy (newbmap->command, "pccommandinput_");
        strcat (newbmap->command, command);
        newbmap->command_end = strchr (newbmap->command, 0);
        *(newbmap->command_end+1) = 0;	// Make sure there is an end there too.

        newbmap->is_on=false;
        if (buttonlist)
          buttonlist->prev = newbmap;
        buttonlist = newbmap;
      }
      else
      {
        delete [] newbmap->command;
        newbmap->command = new char[strlen ("pccommandinput_")+strlen(command)+2];
        strcpy (newbmap->command, "pccommandinput_");
        strcat (newbmap->command, command);
        newbmap->command_end = strchr (newbmap->command, 0);
        *(newbmap->command_end+1) = 0;	// Make sure there is an end there too.
      }
    }
    return true;
  }
  return false;
}

const char* celPcCommandInput::GetBind (const char* triggername) const
{
  utf32_char key, cooked;
  int type, numeric;
  csKeyModifiers modifiers;
  if (csInputDefinition::ParseKey (triggername, &key, &cooked, &modifiers))
  {
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    celKeyMap* map;
    if (!(map = GetMap (key, mods)))
      return 0;
    return map->command+15;
  }
  else if (csInputDefinition::ParseOther (triggername, &type, &numeric, &modifiers))
  {
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    if (type == csevMouseMove || type == csevJoystickMove)
    {
      celAxisMap* amap;
      if (!(amap = GetAxisMap (type, numeric, mods)))
        return 0;
      return amap->command+15;
    }
    else
    {
      celButtonMap* bmap;
      if (!(bmap = GetButtonMap (type, numeric, mods)))
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
  int type, numeric;
  csKeyModifiers modifiers;
  if (csInputDefinition::ParseKey (triggername, &key, &cooked, &modifiers))
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
  else if (csInputDefinition::ParseOther (triggername, &type, &numeric,
  	&modifiers))
  {
    uint32 mods = csKeyEventHelper::GetModifiersBits (modifiers);
    if (type == csevMouseMove || type == csevJoystickMove)
    {
      celAxisMap *pamap = 0, *amap = axislist;
      while (amap)
      {
        if (amap->type == type && amap->numeric == numeric
		&& amap->modifiers == mods)
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
        if (bmap->type == type && bmap->numeric == numeric
		&& bmap->modifiers == mods)
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
  celKeyMap *p=keylist;
  while (p)
  {
    if (p->key==key && p->modifiers==mods)
      break;
    p=p->next;
  }

  return p;
}

celAxisMap* celPcCommandInput::GetAxisMap (int type, int numeric,
	uint32 mods) const
{
  celAxisMap *p=axislist;
  while (p)
  {
    if (p->type==type && p->numeric==numeric && p->modifiers==mods)
      break;
    p=p->next;
  }

  return p;
}

celButtonMap* celPcCommandInput::GetButtonMap (int type, int numeric,
	uint32 mods) const
{
  celButtonMap *p=buttonlist;
  while (p)
  {
    if (p->type==type && p->numeric==numeric && p->modifiers==mods)
      break;
    p=p->next;
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

bool celPcCommandInput::HandleEvent (iEvent &ev)
{
  if (CS_IS_KEYBOARD_EVENT(ev))
  {
    utf32_char key = csKeyEventHelper::GetCookedCode (&ev);
    csKeyModifiers key_modifiers;
    csKeyEventHelper::GetModifiers (&ev, key_modifiers);
    uint32 modifiers = csKeyEventHelper::GetModifiersBits (key_modifiers);
    uint32 type = csKeyEventHelper::GetEventType (&ev);

    //find mapping
    celKeyMap *p = keylist;
    while (p)
    {
      if ((p->key == key) && ((modifiers & p->modifiers) == p->modifiers))
      {
        break;
      }
      p=p->next;
    }
    if (!p)
      return false;

    if (type == csKeyEventTypeUp)
    {
      if (p->is_on)
      {
        p->is_on = false;
        iCelBehaviour* bh = entity->GetBehaviour();
	if (bh)
	{
          *(p->command_end) = '0';
          celData ret;
          bh->SendMessage (p->command, this, ret, 0);
          *(p->command_end) = 0;
        }
      }
    }
    else
    {
      if (p->is_on)
      {
        // Send auto-repeat message.
        iCelBehaviour* bh = entity->GetBehaviour();
	if (bh)
	{
          *(p->command_end) = '_';
          celData ret;
          bh->SendMessage (p->command, this, ret, 0);
          *(p->command_end) = 0;
        }
      }
      else
      {
        p->is_on = true;
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
  else if (CS_IS_MOUSE_EVENT(ev))
  {
    if (ev.Type == csevMouseMove)
    {
      //mouse move event
      int modifiers = csMouseEventHelper::GetModifiers(&ev);
      //find mapping
      celAxisMap *p = axislist;
      while (p)
      {
        if ((modifiers & p->modifiers) == p->modifiers)
        {
	  iCelBehaviour* bh = entity->GetBehaviour ();
	  if (bh)
	  {
	    float val;
	    if (p->numeric == 0)
              val = (float) csMouseEventHelper::GetX(&ev);
	    else
              val = (float) csMouseEventHelper::GetY(&ev);
            celData ret;
	    csStringID id_mouseval = pl->FetchStringID ("cel.parameter.value");
	    celGenericParameterBlock *params = new celGenericParameterBlock (1);
	    params->SetParameterDef (0, id_mouseval, "value");
	    if (screenspace)
	      params->GetParameter(0).Set(val);
	    else
	      params->GetParameter(0).Set(ScreenToCentered (val, p->numeric));
            bh->SendMessage (p->command, this, ret, params);
	  }
	  if (p->recenter)
	  {
            // Recenter mouse so we don't lose focus
            int width = g2d->GetWidth();
            int height = g2d->GetHeight();
            g2d->SetMousePosition (width / 2, height / 2);
            g2d->SetMouseCursor (csmcNone);
          }
        }
        p=p->next;
      }
    }
    else
    {
      //mouse button event
      int button = csMouseEventHelper::GetButton(&ev);
      int modifiers = csMouseEventHelper::GetModifiers(&ev);
      //find mapping
      celButtonMap *p = buttonlist;
      while (p)
      {
        if ((p->numeric == button) && ((modifiers & p->modifiers)
		== p->modifiers) && ((1 << p->type) & CSMASK_Mouse))
        {
          break;
        }
        p=p->next;
      }
      if (!p)
        return false;

      if (ev.Type == (uint8) csevMouseUp)
      {
        if (p->is_on)
        {
          p->is_on = false;
          iCelBehaviour* bh = entity->GetBehaviour();
	  if (bh)
	  {
            *(p->command_end) = '0';
            celData ret;
            bh->SendMessage (p->command, this, ret, 0);
            *(p->command_end) = 0;
	  }
        }
      }
      else
      {
        if (p->is_on)
        {
          // Send auto-repeat message.
          iCelBehaviour* bh = entity->GetBehaviour();
	  if (bh)
	  {
            *(p->command_end) = '_';
            celData ret;
            bh->SendMessage (p->command, this, ret, 0);
            *(p->command_end) = 0;
	  }
        }
        else
        {
          p->is_on = true;
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
  }
  else if (CS_IS_JOYSTICK_EVENT(ev))
  {
    if (ev.Type == csevJoystickMove)
    {
      //joystick move event
      int modifiers = csJoystickEventHelper::GetModifiers(&ev);
      //find mapping
      celAxisMap *p = axislist;
      while (p)
      {
        if ((modifiers & p->modifiers) == p->modifiers)
        {
	  iCelBehaviour* bh = entity->GetBehaviour ();
	  if (bh)
	  {
	    float val;
	    if (p->numeric == 0)
              val = (float) csJoystickEventHelper::GetAxis(&ev, 1);
	    else
              val = (float) csJoystickEventHelper::GetAxis(&ev, 2);
            //The joystick driver returns values between -32767 and +32767.
	    //Get the value in the range -1 to 1.
            val = val / 32767;
            celData ret;
	    csStringID id_mouseval = pl->FetchStringID ("cel.parameter.value");
	    celGenericParameterBlock *params = new celGenericParameterBlock (1);
	    params->SetParameterDef (0, id_mouseval, "value");
	    if (screenspace)
	      params->GetParameter(0).Set(CenteredToScreen (val, p->numeric));
	    else
	      params->GetParameter(0).Set(val);
            bh->SendMessage (p->command, this, ret, params);
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
      int button = csJoystickEventHelper::GetButton(&ev);
      int modifiers = csJoystickEventHelper::GetModifiers(&ev);
      //find mapping
      celButtonMap *p = buttonlist;
      while (p)
      {
        if ((p->numeric == button) && ((modifiers & p->modifiers)
		== p->modifiers) && ((1 << p->type) & CSMASK_Joystick))
        {
          break;
        }
        p=p->next;
      }
      if (!p)
        return false;

      if (ev.Type == csevJoystickUp)
      {
        if (p->is_on)
        {
          p->is_on = false;
          iCelBehaviour* bh = entity->GetBehaviour();
	  if (bh)
	  {
            *(p->command_end) = '0';
            celData ret;
            bh->SendMessage (p->command, this, ret, 0);
            *(p->command_end) = 0;
	  }
        }
      }
      else
      {
        if (p->is_on)
        {
          // Send auto-repeat message.
          iCelBehaviour* bh = entity->GetBehaviour();
	  if (bh)
	  {
            *(p->command_end) = '_';
            celData ret;
            bh->SendMessage (p->command, this, ret, 0);
            *(p->command_end) = 0;
	  }
        }
        else
        {
          p->is_on = true;
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
  }

  // Allow other entities to process this event
  return false;
}

//---------------------------------------------------------------------------

