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
#include "csutil/debug.h"
#include "csutil/inpnames.h"
#include "csutil/csevent.h"
#include "pf/input/inpfact.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/datatype.h"
#include "pl/persist.h"
#include "bl/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (CommandInput, "pckeyinput")

SCF_EXPORT_CLASS_TABLE (pfinput)
  SCF_EXPORT_CLASS (celPfCommandInput, "cel.pcfactory.pckeyinput",
  	"CEL Key Input Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

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

  maplist = NULL;
  scfiEventHandler = NULL;

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

  databuf->IncRef ();	// Avoid smart pointer release.
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCommandInput::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != COMMANDINPUT_SERIAL) return false;
  int cnt = databuf->GetDataCount ();
  cnt /= 2;
  int i, j = 0;
  celData* cd;
  for (i = 0 ; i < cnt ; i++)
  {
    cd = databuf->GetData (j++); if (!cd) return false;
    int key = cd->value.ul;
    cd = databuf->GetData (j++); if (!cd) return false;
    celKeyMap* newmap = new celKeyMap ();
    // Add a new entry to key mapping list
    newmap->next = maplist;
    newmap->prev = NULL;
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
    q->RegisterListener (scfiEventHandler, CSMASK_KeyUp | CSMASK_KeyDown);
  }
  else
  {
    if (!scfiEventHandler)
      return;

    csRef<iEventQueue> q (CS_QUERY_REGISTRY (object_reg, iEventQueue));
    CS_ASSERT (q);
    q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
    scfiEventHandler = NULL;
  }
}

bool celPcCommandInput::LoadConfig (const char* /*fname*/)
{
  // not implemented
  return false;
}

bool celPcCommandInput::Bind (const char* triggername, const char* command)
{
  int key, shift;
  if (!csParseKeyDef (triggername, key, shift, false))
    return false;

  // only bind single keys
  if (shift != 0)
      return false;

  celKeyMap* newmap;
  if (!(newmap = GetMap (key)))
  {
    newmap = new celKeyMap;
    // Add a new entry to key mapping list
    newmap->next=maplist;
    newmap->prev=NULL;
    newmap->key=key;
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
  int key, shift;
  if (!csParseKeyDef (triggername, key, shift, false))
    return NULL;
  
  if (shift != 0)
    return NULL;

  celKeyMap* map;
  if (!(map = GetMap (key)))
    return NULL;

  return map->command+11;
}

bool celPcCommandInput::RemoveBind (const char* /*triggername*/,
    const char* /*command*/)
{
  //not implemented yet
  return false;
}

celKeyMap* celPcCommandInput::GetMap (int key) const
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
  CS_ASSERT(ev.Type==csevKeyUp || ev.Type==csevKeyDown);
  int key = ev.Key.Code;

  //find mapping
  celKeyMap *p = maplist;
  while (p)
  {
    if (p->key == key)
    {
      break;
    }
    p=p->next;
  }
  if (!p)
    return false;

  if (ev.Type == csevKeyUp)
  {
    if (p->is_on)
    {
      p->is_on=false;
      iCelBehaviour* bh = entity->GetBehaviour();
      CS_ASSERT(bh != NULL);
      *(p->command_end) = '0';
      bh->SendMessage (p->command, NULL);
      *(p->command_end) = 0;
    }
  }
  else
  {
    if (!p->is_on)
    {
      p->is_on=true;
      iCelBehaviour* bh = entity->GetBehaviour();
      CS_ASSERT(bh != NULL);
      *(p->command_end) = '1';
      bh->SendMessage (p->command, NULL);
      *(p->command_end) = 0;
    }
  }

  return true;
}

//---------------------------------------------------------------------------

