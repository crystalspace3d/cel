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
#include "behaviourlayer/bl.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "csutil/debug.h"
#include "csutil/randomgen.h"
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
#include "csgeom/math3d.h"

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

csStringID celPcSpawn::action_addentitytype = csInvalidStringID;
csStringID celPcSpawn::action_addentitytpltype = csInvalidStringID;
csStringID celPcSpawn::action_settiming = csInvalidStringID;
csStringID celPcSpawn::action_resettiming = csInvalidStringID;
csStringID celPcSpawn::action_setenabled = csInvalidStringID;
csStringID celPcSpawn::action_clearentitylist = csInvalidStringID;
csStringID celPcSpawn::action_inhibit = csInvalidStringID;
csStringID celPcSpawn::action_spawn = csInvalidStringID;
csStringID celPcSpawn::id_repeat_param = csInvalidStringID;
csStringID celPcSpawn::id_random_param = csInvalidStringID;
csStringID celPcSpawn::id_mindelay_param = csInvalidStringID;
csStringID celPcSpawn::id_maxdelay_param = csInvalidStringID;
csStringID celPcSpawn::id_chance_param = csInvalidStringID;
csStringID celPcSpawn::id_entity_param = csInvalidStringID;
csStringID celPcSpawn::id_template_param = csInvalidStringID;
csStringID celPcSpawn::id_behaviour_param = csInvalidStringID;
csStringID celPcSpawn::id_layer_param = csInvalidStringID;
csStringID celPcSpawn::id_call_param = csInvalidStringID;
csStringID celPcSpawn::id_enabled_param = csInvalidStringID;
csStringID celPcSpawn::id_count_param = csInvalidStringID;
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
  sequence_cur = 0;
  count = 0;
  inhibit_count = 0;

  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);
  if (action_addentitytype == csInvalidStringID)
  {
    action_addentitytype = pl->FetchStringID ("cel.action.AddEntityType");
    action_addentitytpltype = pl->FetchStringID ("cel.action.AddEntityTemplateType");
    action_settiming = pl->FetchStringID ("cel.action.SetTiming");
    action_resettiming = pl->FetchStringID ("cel.action.ResetTiming");
    action_setenabled = pl->FetchStringID ("cel.action.SetEnabled");
    action_clearentitylist = pl->FetchStringID ("cel.action.ClearEntityList");
    action_inhibit = pl->FetchStringID ("cel.action.Inhibit");
    action_spawn = pl->FetchStringID ("cel.action.Spawn");
    id_repeat_param = pl->FetchStringID ("cel.parameter.repeat");
    id_random_param = pl->FetchStringID ("cel.parameter.random");
    id_mindelay_param = pl->FetchStringID ("cel.parameter.mindelay");
    id_maxdelay_param = pl->FetchStringID ("cel.parameter.maxdelay");
    id_chance_param = pl->FetchStringID ("cel.parameter.chance");
    id_entity_param = pl->FetchStringID ("cel.parameter.entity");
    id_template_param = pl->FetchStringID ("cel.parameter.template");
    id_behaviour_param = pl->FetchStringID ("cel.parameter.behaviour");
    id_layer_param = pl->FetchStringID ("cel.parameter.layer");
    id_call_param = pl->FetchStringID ("cel.parameter.call");
    id_enabled_param = pl->FetchStringID ("cel.parameter.enabled");
    id_count_param = pl->FetchStringID ("cel.parameter.count");
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

bool celPcSpawn::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_addentitytype)
  {
    CEL_FETCH_FLOAT_PAR (chance_param,params,id_chance_param);
    if (!p_chance_param) chance_param = 0.0f;
    CEL_FETCH_STRING_PAR (entity_param,params,id_entity_param);
    if (!p_entity_param)
      return Report (object_reg,
      	"Missing parameter 'entity' for action AddEntityType!");
    CEL_FETCH_STRING_PAR (behaviour_param,params,id_behaviour_param);
    if (!p_behaviour_param) behaviour_param = 0;
    CEL_FETCH_STRING_PAR (call_param,params,id_call_param);
    if (!p_call_param) call_param = 0;
    CEL_FETCH_STRING_PAR (layer_param,params,id_layer_param);
    if (p_layer_param)
    {
      iCelBlLayer* bl = pl->FindBehaviourLayer (layer_param);
      if (!bl)
        return Report (object_reg,
        	"Couldn't find '%s' behaviour layer in action AddEntityType!",
        	layer_param);
      scfiPcSpawn.AddEntityType (chance_param, entity_param, bl,
      	behaviour_param, call_param, params, 0);
    }
    else
    {
      csRef<iCelBlLayer> bl = CS_QUERY_REGISTRY (object_reg, iCelBlLayer);
      if (!bl)
        return Report (object_reg,
        	"Couldn't find behaviour layer in action AddEntityType!");
      scfiPcSpawn.AddEntityType (chance_param, entity_param, bl,
      	behaviour_param, call_param, params, 0);
    }
    return true;
  }
  else if (actionId == action_addentitytpltype)
  {
    CEL_FETCH_FLOAT_PAR (chance_param,params,id_chance_param);
    if (!p_chance_param) chance_param = 0.0f;
    CEL_FETCH_STRING_PAR (entity_param,params,id_entity_param);
    if (!p_entity_param) entity_param = 0;
    CEL_FETCH_STRING_PAR (template_param,params,id_template_param);
    if (!p_template_param)
      return Report (object_reg,
      	"Missing parameter 'template' for action AddEntityTemplateType!");
    CEL_FETCH_STRING_PAR (call_param,params,id_call_param);
    if (!p_call_param) call_param = 0;
    AddEntityTemplateType (chance_param, template_param, entity_param,
    	call_param, params);
    return true;
  }
  else if (actionId == action_settiming)
  {
    CEL_FETCH_BOOL_PAR (repeat_param,params,id_repeat_param);
    if (!p_repeat_param)
      return Report (object_reg,
      	"Missing parameter 'repeat' for action SetTiming!");
    CEL_FETCH_BOOL_PAR (random_param,params,id_random_param);
    if (!p_random_param)
      return Report (object_reg,
      	"Missing parameter 'random' for action SetTiming!");
    CEL_FETCH_LONG_PAR (mindelay_param,params,id_mindelay_param);
    if (!p_mindelay_param)
      return Report (object_reg,
      	"Missing parameter 'mindelay' for action SetTiming!");
    CEL_FETCH_LONG_PAR (maxdelay_param,params,id_maxdelay_param);
    if (!p_maxdelay_param)
      return Report (object_reg,
      	"Missing parameter 'maxdelay' for action SetTiming!");
    SetTiming (repeat_param, random_param, mindelay_param, maxdelay_param);
    return true;
  }
  else if (actionId == action_resettiming)
  {
    ResetTiming ();
    return true;
  }
  else if (actionId == action_setenabled)
  {
    CEL_FETCH_BOOL_PAR (enabled_param,params,id_enabled_param);
    if (!p_enabled_param)
      return Report (object_reg,
      	"Missing parameter 'enabled' for action SetEnabled!");
    SetEnabled (enabled_param);
    return true;
  }
  else if (actionId == action_clearentitylist)
  {
    ClearEntityList ();
    return true;
  }
  else if (actionId == action_inhibit)
  {
    bool ret = false;
    CEL_FETCH_LONG_PAR (count_param,params,id_count_param);
    if (p_count_param)
    {
      InhibitCount (count_param);
      ret = true;
    }
    return ret;
  }
  else if (actionId == action_spawn)
  {
    Spawn ();
    return true;
  }
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
  ResetTiming ();
}

void celPcSpawn::ResetTiming ()
{
  sequence_cur = 0;
  Reset ();
}

void celPcSpawn::Reset ()
{
  if (maxdelay > mindelay)
    pl->CallbackOnce ((iCelTimerListener*)this,
    	rand () % (maxdelay-mindelay) + mindelay, CEL_EVENT_PRE);
  else
    pl->CallbackOnce ((iCelTimerListener*)this, mindelay, CEL_EVENT_PRE);
}

void celPcSpawn::Spawn ()
{
  csRandomGen rng;
  SpawnEntityNr (rng.Get(spawninfo.Length ()));
}

void celPcSpawn::SpawnEntityNr (size_t idx)
{
  if (inhibit_count != 0 && inhibit_count == count) return;
  // To prevent the entity from being deleted during
  // the call of pcspawn_newentity we keep a temporary reference
  // here.
  csRef<iCelEntity> ref;
  if (spawninfo[idx].templ)
  {
    iCelEntityTemplate* entpl = pl->FindEntityTemplate (
    	spawninfo[idx].templ);
    celEntityTemplateParams entpl_params;
    spawninfo[idx].newent = pl->CreateEntity (entpl, spawninfo[idx].name,
    	entpl_params);
  }
  else
  {
    spawninfo[idx].newent = pl->CreateEntity (spawninfo[idx].name,
    	spawninfo[idx].bl, spawninfo[idx].behaviour, CEL_PROPCLASS_END);
  }

  size_t i;
  csStringArray& pcs = spawninfo[idx].pcs;
  for (i = 0 ; i < pcs.Length () ; i++)
  {
    iCelPropertyClass* pc = pl->CreatePropertyClass (spawninfo[idx].newent,
    	pcs[i]);
    if (!pc)
    {
      Report (object_reg, "Error creating property class '%s' for entity '%s'!",
      	(const char*)pcs[i], spawninfo[idx].newent->GetName ());
    }
  }

  // First send a message to our new entity if needed.
  celData ret;
  if (spawninfo[idx].behaviour && !spawninfo[idx].newent->GetBehaviour ())
    Report (object_reg, "Error creating behaviour for entity '%s'!",
    	spawninfo[idx].newent->GetName ());
  if (spawninfo[idx].msg_id && spawninfo[idx].newent->GetBehaviour ())
  {
    spawninfo[idx].newent->GetBehaviour ()->SendMessage (
    	spawninfo[idx].msg_id, this, ret, spawninfo[idx].params);
  }

  // Then send a message to our own entity.
  ref = entity;
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set (spawninfo[idx].newent);
    params->GetParameter (1).Set (spawninfo[idx].behaviour);
    bh->SendMessage ("pcspawn_newentity", this, ret, params);
  }
  count ++;
}

void celPcSpawn::TickOnce ()
{
  if (!enabled) return;
  if (inhibit_count != 0 && inhibit_count == count) return;
  if (repeat) Reset ();

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
  SpawnEntityNr (idx);
}

void celPcSpawn::AddEntityType (float chance, const char* name, iCelBlLayer* bl,
	const char* behaviour, const char* msg_id,
	iCelParameterBlock* params, va_list pcclasses)
{
  size_t idx = spawninfo.Push (SpawnInfo ());
  SpawnInfo& si = spawninfo[idx];
  si.chance = chance;
  si.name = csStrNew (name);
  si.bl = bl;
  si.behaviour = csStrNew (behaviour);
  si.msg_id = csStrNew (msg_id);
  si.params = params;
  char const* pcname = va_arg (pcclasses, char*);
  while (pcname != 0)
  {
    si.pcs.Push (pcname);
    pcname = va_arg (pcclasses, char*);
  }
  total_chance += chance;
}

void celPcSpawn::AddEntityTemplateType (float chance, const char* templ,
	const char* name, const char* msg_id, iCelParameterBlock* params)
{
  size_t idx = spawninfo.Push (SpawnInfo ());
  SpawnInfo& si = spawninfo[idx];
  si.chance = chance;
  si.templ = csStrNew (templ);
  if (name != 0)
    si.name = csStrNew (name);
  else
    si.name = csStrNew (templ);
  si.msg_id = csStrNew (msg_id);
  si.params = params;
  total_chance += chance;
}

void celPcSpawn::ClearEntityList ()
{
  spawninfo.DeleteAll ();
  total_chance = 0;
  count = 0;
}

void celPcSpawn::InhibitCount (int number)
{
  inhibit_count = number;
}

//---------------------------------------------------------------------------
