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
#include "csutil/event.h"
#include "csutil/debug.h"
#include "csutil/inpnames.h"
#include "csutil/csevent.h"
#include "plugins/propclass/input/inpfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (CommandInput, "pckeyinput")

void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistance",
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

SCF_IMPLEMENT_IBASE_EXT (celPcCommandInput)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCommandInput)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCommandInput::PcCommandInput)
  SCF_IMPLEMENTS_INTERFACE (iPcCommandInput)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_IBASE (celPcCommandInput::EventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_IBASE_END

celPcCommandInput::celPcCommandInput (iObjectRegistry* object_reg)
    : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCommandInput);
  celPcCommandInput::object_reg = object_reg;
  DG_TYPE (this, "celPcCommandInput()");

  maplist = 0;
  scfiEventHandler = 0;

  Activate ();
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

  //delete list of mappings
  celKeyMap* o,* p=maplist;
  while (p)
  {
    o = p->next;
    if (p->command)
      delete [] p->command;
    delete p;
    p=o;
  }
}

#define COMMANDINPUT_SERIAL 1

csPtr<iCelDataBuffer> celPcCommandInput::Save ()
{
  csRef<iCelPlLayer> pl (CS_QUERY_REGISTRY (object_reg, iCelPlLayer));
  csRef<iCelDataBuffer> databuf (pl->CreateDataBuffer (COMMANDINPUT_SERIAL));
  int cnt = 0;
  celKeyMap* m = maplist;
  while (m)
  {
    cnt++;
    m = m->next;
  }
  databuf->SetDataCount (cnt * 2);

  int j = 0;
  m = maplist;
  while (m)
  {
    databuf->GetData (j++)->Set ((uint32)m->key);
    databuf->GetData (j++)->Set (m->command);
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
  int cnt = databuf->GetDataCount ();
  cnt /= 2;
  int i, j = 0;
  celData* cd;
  for (i = 0 ; i < cnt ; i++)
  {
    cd = databuf->GetData (j++);
    if (!cd) 
    {
      Report (object_reg,"Key for map[%d] not specified.  Cannot load.",i);
      return false;
    }
    int key = cd->value.ul;
    cd = databuf->GetData (j++); 
    if (!cd) 
    {
      Report (object_reg,"Command for map[%d] not specified.  Cannot load.",i);
      return false;
    }
    celKeyMap* newmap = new celKeyMap ();
    // Add a new entry to key mapping list
    newmap->next = maplist;
    newmap->prev = 0;
    newmap->key = key;
    newmap->command = new char[strlen (*cd->value.s)+2];
    strcpy (newmap->command, *cd->value.s);
    newmap->command_end = strchr (newmap->command, 0);
    *(newmap->command_end+1) = 0;	// Make sure there is an end there too.

    newmap->is_on = false;
    if (maplist)
      maplist->prev = newmap;
    maplist = newmap;
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
    q->RegisterListener (scfiEventHandler, CSMASK_Keyboard);
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
  // not implemented
  return false;
}

bool celPcCommandInput::Bind (const char* triggername, const char* command)
{
  utf32_char key; 
  utf32_char cooked;
  csKeyModifiers modifiers; 
  if (!csParseKeyDef (triggername, &key, &cooked, &modifiers))
    return false;

  // only bind single keys
  //  if (shift != 0)
  //      return false;

  celKeyMap* newmap;
  if (!(newmap = GetMap (key)))
  {
    newmap = new celKeyMap;
    // Add a new entry to key mapping list
    newmap->next=maplist;
    newmap->prev=0;
    newmap->key=key;
    newmap->modifiers=csKeyEventHelper::GetModifiersBits (modifiers);
    newmap->command = new char[strlen ("pckeyinput_")+strlen(command)+2];
    strcpy (newmap->command, "pckeyinput_");
    strcat (newmap->command, command);
    newmap->command_end = strchr (newmap->command, 0);
    *(newmap->command_end+1) = 0;	// Make sure there is an end there too.

    newmap->is_on=false;
    if (maplist)
      maplist->prev = newmap;
    maplist = newmap;
  }
  else
  {
    delete [] newmap->command;
    newmap->command = new char[strlen ("pckeyinput_")+strlen(command)+2];
    strcpy (newmap->command, "pckeyinput_");
    strcat (newmap->command, command);
    newmap->command_end = strchr (newmap->command, 0);
    *(newmap->command_end+1) = 0;	// Make sure there is an end there too.
  }

  return true;
}

const char* celPcCommandInput::GetBind (const char* triggername) const
{
  utf32_char key, cooked;
  csKeyModifiers modifiers;
  if (!csParseKeyDef (triggername, &key, &cooked, &modifiers))
    return false;
  
  if (  csKeyEventHelper::GetModifiersBits (modifiers) != 0 )
    return 0;

  celKeyMap* map;
  if (!(map = GetMap (key)))
    return 0;

  return map->command+11;
}

bool celPcCommandInput::RemoveBind (const char* /*triggername*/,
    const char* /*command*/)
{
  //not implemented yet
  return false;
}

void celPcCommandInput::RemoveAllBinds ()
{
  celKeyMap *key, *next;
  
  key = maplist;
  while (key)
  {
    next = key->next;
    delete key;
    key = next;
  }
  maplist = 0;
}

celKeyMap* celPcCommandInput::GetMap (utf32_char key) const
{
  celKeyMap *p=maplist;
  while (p)
  {
    if (p->key==key)
      break;
    p=p->next;
  }

  return p;
}

bool celPcCommandInput::HandleEvent (iEvent &ev)
{
  CS_ASSERT(ev.Type==csevKeyboard);
  utf32_char key = csKeyEventHelper::GetCookedCode (&ev);
  csKeyModifiers key_modifiers;
  csKeyEventHelper::GetModifiers (&ev, key_modifiers);
  uint32 modifiers = csKeyEventHelper::GetModifiersBits (key_modifiers);
  uint32 type = csKeyEventHelper::GetEventType (&ev);

  //find mapping
  celKeyMap *p = maplist;
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
      p->is_on=false;
      iCelBehaviour* bh = entity->GetBehaviour();
      CS_ASSERT(bh != 0);
      *(p->command_end) = '0';
      bh->SendMessage (p->command, 0);
      *(p->command_end) = 0;
    }
  }
  else
  {
    if (!p->is_on)
    {
      p->is_on=true;
      iCelBehaviour* bh = entity->GetBehaviour();
      CS_ASSERT(bh != 0);
      *(p->command_end) = '1';
      bh->SendMessage (p->command, 0);
      *(p->command_end) = 0;
    }
  }

  return true;
}

//---------------------------------------------------------------------------

