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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "iutil/event.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "csutil/debug.h"
#include "csutil/cskeys.h"
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

  maplist=NULL;
  scfiEventHandler=NULL;

  Activate();
}

celPcCommandInput::~celPcCommandInput ()
{
  DG_REM (this);

  //delete list of mappings
  celKeyMap* o,* p=maplist;
  while (p)
  {
    o=p->next;
    if (p->command)
	delete [] p->command;
    delete p;
    p=o;
  }
}

#define COMMANDINPUT_SERIAL 23

iCelDataBuffer* celPcCommandInput::Save ()
{
  iCelPlLayer* pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  iCelDataBuffer* databuf = pl->CreateDataBuffer (COMMANDINPUT_SERIAL);
  pl->DecRef ();
  databuf->SetDataCount (0);
  return databuf;
}

bool celPcCommandInput::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != COMMANDINPUT_SERIAL) return false;
  if (databuf->GetDataCount () != 0) return false;
  return true;
}

void celPcCommandInput::Activate (bool activate)
{
  if (activate)
  {
    if (scfiEventHandler)
      return;

    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    CS_ASSERT(q);
    scfiEventHandler = new EventHandler(this);
    q->RegisterListener (scfiEventHandler, CSMASK_KeyUp | CSMASK_KeyDown);
    q->DecRef();
  }
  else
  {
    if (!scfiEventHandler)
      return;
    
    iEventQueue* q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
    CS_ASSERT(q);
    q->RemoveListener (scfiEventHandler);
    q->DecRef();
    scfiEventHandler->DecRef();
    scfiEventHandler=NULL;
  }
}

bool celPcCommandInput::LoadConfig (const char* /*fname*/)
{
    // not implemented
    return false;
}

bool celPcCommandInput::Bind (const char* triggername, const char* command)
{
  int key,shiftmask;
  if (!csParseKeyDef(triggername, key, shiftmask))
    return false;

  printf ("Bind: %s -> %s, %d %d\n",triggername, command, key, shiftmask);

  //Only bid keys - no key combinations
  if (shiftmask)
    return false;
  
  celKeyMap* newmap;
  if (! (newmap=GetMap(key)))
  {
    newmap = new celKeyMap;
    // Add a new entry to key mapping list
    newmap->next=maplist;
    newmap->prev=NULL;
    newmap->key=key;
    newmap->command=new char[strlen(command)+2];
    strcpy (newmap->command+1, command);
    newmap->is_on=false;
    if (maplist)
        maplist->prev=newmap;
    maplist=newmap;			     
  }
  else
  {
    if (newmap->command) 
      delete [] newmap->command;
    newmap->command=new char[strlen(command)+2];
    strcpy (newmap->command+1, command);
  }
  
  return true;
}

const char* celPcCommandInput::GetBind (const char* triggername) const
{
  int key,shiftmask;
  if (!csParseKeyDef(triggername, key, shiftmask))
    return NULL;

  if (shiftmask)
    return NULL;
  
  celKeyMap* map;
  if (! (map=GetMap(key)))
    return NULL;
  
  return map->command;
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
    if (p->is_on) {
      p->is_on=false;
      iCelBehaviour* bh = entity->GetBehaviour();
      CS_ASSERT(bh != NULL);
      p->command[0]='-';
      printf ("DeActivate: %s\n", p->command);
      bh->SendMessage (p->command, NULL);
    }
  }
  else
  {
    if (!p->is_on) {
      p->is_on=true;
      iCelBehaviour* bh = entity->GetBehaviour();
      CS_ASSERT(bh != NULL);
      p->command[0]='+';
      printf ("Activate: %s\n", p->command);
      bh->SendMessage (p->command, NULL);
    }
  }

  return true;
}

//---------------------------------------------------------------------------

