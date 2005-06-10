/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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
#include "plugins/propclass/tools/spawn.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "csutil/debug.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (Spawn, "pcspawn")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.propclass.spawn",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcSpawn)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcSpawn)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcSpawn::PcSpawn)
  SCF_IMPLEMENTS_INTERFACE (iPcSpawn)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

csStringID celPcSpawn::id_entity = csInvalidStringID;
csStringID celPcSpawn::id_behaviour = csInvalidStringID;

celPcSpawn::celPcSpawn (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcSpawn);
  enabled = true;
  repeat = false;
  random = true;
  mindelay = 1000;
  maxdelay = 1000;
  delay_todo = 0;
  total_chance = 0;

  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);
  if (id_entity == csInvalidStringID)
  {
    id_entity = pl->FetchStringID ("cel.parameter.entity");
    id_behaviour = pl->FetchStringID ("cel.parameter.behaviour");
  }
  params = new celGenericParameterBlock (2);
  params->SetParameterDef (0, id_entity, "entity");
  params->SetParameterDef (1, id_behaviour, "behaviour");
}

celPcSpawn::~celPcSpawn ()
{
  delete params;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcSpawn);
}

bool celPcSpawn::PerformAction (csStringID /*actionId*/,
	iCelParameterBlock* /*params*/)
{
  return false;
}

#define SPAWN_SERIAL 1

csPtr<iCelDataBuffer> celPcSpawn::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (SPAWN_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcSpawn::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != SPAWN_SERIAL)
    return Report (object_reg, "Couldn't load spawn property class!");

  return true;
}

void celPcSpawn::SetTiming (bool repeat, bool random,
		  csTicks mindelay, csTicks maxdelay)
{
  celPcSpawn::repeat = repeat;
  celPcSpawn::random = random;
  celPcSpawn::mindelay = mindelay;
  celPcSpawn::maxdelay = maxdelay;
  Reset ();
}

void celPcSpawn::TickOnce ()
{
  if (!enabled) return;

  // To prevent the entity from being deleted during
  // the call of pcspawn_newentity we keep a temporary reference
  // here.
  csRef<iCelEntity> ref;
  if (repeat)
    Reset ();

  // First create our entity.
  size_t idx;
  if (random)
  {
    float c = float (rand () % 10000) * total_chance / 10000.0f;
    idx = 0;
    c -= spawninfo[idx].chance;
    while (c > 0)
    {
      idx++;
      if (idx >= spawninfo.Length ()-1) break;
      c -= spawninfo[idx].chance;
    }
  }
  else
  {
    idx = sequence_cur;
    sequence_cur++;
    if (sequence_cur >= spawninfo.Length ()) sequence_cur = 0;
  }

  csRef<iCelEntity> newent = pl->CreateEntity (spawninfo[idx].name,
      	spawninfo[idx].bl, spawninfo[idx].behaviour, (void*)0);
  size_t i;
  csStringArray& pcs = spawninfo[idx].pcs;
  for (i = 0 ; i < pcs.Length () ; i++)
  {
    iCelPropertyClass* pc = pl->CreatePropertyClass (newent, pcs[i]);
    if (!pc)
    {
      Report (object_reg, "Error creating property class '%s' for entity '%s'!",
      	(const char*)pcs[i], newent->GetName ());
    }
  }

  // First send a message to our new entity if needed.
  celData ret;
  if (spawninfo[idx].msg_id && newent->GetBehaviour ())
  {
    newent->GetBehaviour ()->SendMessage (spawninfo[idx].msg_id, this, ret,
		spawninfo[idx].params);
  }

  // Then send a message to our own entity.
  ref = entity;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set (newent);
    params->GetParameter (1).Set (spawninfo[idx].behaviour);
    bh->SendMessage ("pcspawn_newentity", this, ret, params);
  }
}

void celPcSpawn::Reset ()
{
  sequence_cur = 0;
  if (maxdelay > mindelay)
    pl->CallbackOnce ((iCelTimerListener*)this,
    	rand () % (maxdelay-mindelay) + mindelay, cscmdPreProcess);
  else
    pl->CallbackOnce ((iCelTimerListener*)this, mindelay, cscmdPreProcess);
}

void celPcSpawn::AddEntityType (float chance, const char* name, iCelBlLayer* bl,
		  const char* behaviour, const char* msg_id,
		  iCelParameterBlock* params, va_list behaviours)
{
  size_t idx = spawninfo.Push (SpawnInfo ());
  SpawnInfo& si = spawninfo[idx];
  si.chance = chance;
  si.name = csStrNew (name);
  si.bl = bl;
  si.behaviour = csStrNew (behaviour);
  si.msg_id = csStrNew (msg_id);
  si.params = params;
  char const* pcname = va_arg (behaviours, char*);
  while (pcname != 0)
  {
    si.pcs.Push (pcname);
    pcname = va_arg (behaviours, char*);
  }
  total_chance += chance;
}

void celPcSpawn::ClearEntityList ()
{
  spawninfo.DeleteAll ();
  total_chance = 0;
}

//---------------------------------------------------------------------------
