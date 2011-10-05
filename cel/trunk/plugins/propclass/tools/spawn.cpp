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
#include "propclass/mesh.h"
#include "propclass/light.h"
#include "propclass/linmove.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/virtclk.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"
#include "ivaria/reporter.h"
#include "ivaria/mapnode.h"
#include "csgeom/math3d.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/light.h"
#include "iengine/movable.h"
#include "iengine/sector.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (Spawn, "pclogic.spawn", "pcspawn")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
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
csStringID celPcSpawn::id_sector_param = csInvalidStringID;
csStringID celPcSpawn::id_position_param = csInvalidStringID;
csStringID celPcSpawn::id_yrot_param = csInvalidStringID;
csStringID celPcSpawn::id_entity = csInvalidStringID;
csStringID celPcSpawn::id_behaviour = csInvalidStringID;

PropertyHolder celPcSpawn::propinfo;

celPcSpawn::celPcSpawn (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  enabled = true;
  repeat = false;
  random = true;
  position_index = 0;
  mindelay = 1000;
  maxdelay = 1000;
  delay_todo = 0;
  total_chance = 0;
  sequence_cur = 0;
  count = 0;
  inhibit_count = 0;
  serialnr = 1;
  do_name_counter = true;
  do_spawn_unique = false;

  vc = csQueryRegistry<iVirtualClock> (object_reg);
  CS_ASSERT (vc != 0);
  engine = csQueryRegistry<iEngine> (object_reg);
  CS_ASSERT (engine != 0);

  if (id_repeat_param == csInvalidStringID)
  {
    id_repeat_param = pl->FetchStringID ("repeat");
    id_random_param = pl->FetchStringID ("random");
    id_mindelay_param = pl->FetchStringID ("mindelay");
    id_maxdelay_param = pl->FetchStringID ("maxdelay");
    id_chance_param = pl->FetchStringID ("chance");
    id_entity_param = pl->FetchStringID ("entity");
    id_template_param = pl->FetchStringID ("template");
    id_behaviour_param = pl->FetchStringID ("behaviour");
    id_layer_param = pl->FetchStringID ("layer");
    id_call_param = pl->FetchStringID ("call");
    id_enabled_param = pl->FetchStringID ("enabled");
    id_count_param = pl->FetchStringID ("count");
    id_sector_param = pl->FetchStringID ("sector");
    id_position_param = pl->FetchStringID ("position");
    id_yrot_param = pl->FetchStringID ("yrot");
    id_entity = pl->FetchStringID ("entity");
    id_behaviour = pl->FetchStringID ("behaviour");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.spawn.action.");
    AddAction (action_addentitytpltype, "AddEntityTemplateType");
    AddAction (action_settiming, "SetTiming");
    AddAction (action_resettiming, "ResetTiming");
    AddAction (action_setenabled, "SetEnabled");
    AddAction (action_clearentitylist, "ClearEntityList");
    AddAction (action_inhibit, "Inhibit");
    AddAction (action_spawn, "Spawn");
    AddAction (action_addspawnposition, "AddSpawnPosition");
  }

  // For properties.
  propinfo.SetCount (2);
  AddProperty (propid_namecounter, "namecounter",
  	CEL_DATA_BOOL, false, "Enable name counter.", &do_name_counter);
  AddProperty (propid_spawnunique, "spawnunique",
  	CEL_DATA_BOOL, false, "Enable unique spawning.", 0);

  params.AttachNew (new celVariableParameterBlock (2));
  params->AddParameter (id_entity);
  params->AddParameter (id_behaviour);

  atBaseline = false;
}

celPcSpawn::~celPcSpawn ()
{
}

void celPcSpawn::SetEntity (iCelEntity* entity)
{
  celPcCommon::SetEntity (entity);
  if (entity)
    rng.Initialize (entity->GetID ());
}

bool celPcSpawn::SetPropertyIndexed (int idx, bool b)
{
  if (idx == propid_spawnunique)
  {
    EnableSpawnUnique (b);
    return true;
  }
  return false;
}

bool celPcSpawn::GetPropertyIndexed (int idx, bool& l)
{
  if (idx == propid_spawnunique)
  {
    l = do_spawn_unique;
    return true;
  }
  return false;
}

bool celPcSpawn::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_addentitytpltype:
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
    case action_settiming:
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
    case action_resettiming:
      ResetTiming ();
      return true;
    case action_setenabled:
      {
        CEL_FETCH_BOOL_PAR (enabled_param,params,id_enabled_param);
        if (!p_enabled_param)
          return Report (object_reg,
      	    "Missing parameter 'enabled' for action SetEnabled!");
        SetEnabled (enabled_param);
        return true;
      }
    case action_clearentitylist:
      ClearEntityList ();
      return true;
    case action_inhibit:
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
    case action_spawn:
      Spawn ();
      return true;
    case action_addspawnposition:
      {
        CEL_FETCH_STRING_PAR (sector_param,params,id_sector_param);
        if (!p_sector_param)
          return Report (object_reg,
      	    "Missing parameter 'sector' for action AddSpawnPosition!");
        CEL_FETCH_FLOAT_PAR (yrot_param,params,id_yrot_param);
        if (!p_yrot_param) yrot_param = 0.0f;
        CEL_FETCH_STRING_PAR (position_param,params,id_position_param);
        if (p_position_param)
        {
          AddSpawnPosition (position_param, yrot_param, sector_param);
        }
        else
        {
          CEL_FETCH_VECTOR3_PAR (position_param,params,id_position_param);
          if (!p_position_param)
            return Report (object_reg,
        	    "Missing parameter 'sector' for action AddSpawnPosition!");
          AddSpawnPosition (position_param, yrot_param, sector_param);
        }
        return true;
      }
    default:
      return false;
  }
}

void celPcSpawn::SetEnabled (bool e)
{
  if (enabled == e) return;
  enabled = e;
  if (enabled) Reset ();
}

void celPcSpawn::Activate ()
{
  if (enabled)
  {
    pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
    Reset ();
  }
}

void celPcSpawn::Deactivate ()
{
  pl->RemoveCallbackOnce ((iCelTimerListener*)this, CEL_EVENT_PRE);
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
  position_index = 0;
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
  SpawnEntityNr (rng.Get((uint32)spawninfo.GetSize ()));
}

void celPcSpawn::CountUniqueEntities ()
{
  if (do_spawn_unique)
  {
    count = 0;
    for (size_t i = 0 ; i < uniqueEntities.GetSize () ; i++)
      if (uniqueEntities[i]) count++;
  }
}

void celPcSpawn::UpdateFreeUniqueEntity (iCelEntity* entity)
{
  if (do_spawn_unique)
  {
    for (size_t i = 0 ; i < uniqueEntities.GetSize () ; i++)
      if (!uniqueEntities[i])
      {
        uniqueEntities[i] = entity;
        atBaseline = false;
        return;
      }
    printf ("No free slot for an entity? Impossible!!!\n");
    fflush (stdout);
    CS_ASSERT (false);
  }
}

iCelEntity* celPcSpawn::CreateNewEntity (iCelEntityTemplate* tpl,
      const char* entityName, iCelParameterBlock* params,
      const csVector3& p, float yrot, const char* sector,
      const char* node)
{
  csVector3 pos = p;
  csRef<iCelEntity> newent = pl->CreateEntity (tpl, entityName, params);

  iSector* sect = engine->FindSector (sector);
  if (!sect)
  {
    Report (object_reg,
          "Can't find sector '%s' for action SetPosition!",
          sector);
    return 0;
  }

  if (node && *node)
  {
    csRef<iMapNode> mapnode = CS::GetNamedChildObject<iMapNode> (
              sect->QueryObject (), node);
    if (mapnode)
      pos = mapnode->GetPosition ();
    else
      Report (object_reg, "Can't find node '%s' for trigger!", node);
  }

  csRef<iPcLinearMovement> linmove = celQueryPropertyClassEntity<iPcLinearMovement> (newent);
  if (linmove)
  {
    if (node && *node)
      linmove->SetFullPosition (node, yrot, sect);
    else
      linmove->SetFullPosition (pos, yrot, sect);
  }
  else
  {
    csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (newent);
    if (pcmesh && pcmesh->GetMesh ())
    {
      iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
      movable->SetPosition (sect, pos);
      movable->GetTransform ().SetO2T (csYRotMatrix3 (yrot));
      movable->UpdateMove ();
    }
  }
  return newent;
}

size_t celPcSpawn::GetPositionIndex ()
{
  size_t len = spawnposition.GetSize ();
  if (len <= 0) return csArrayItemNotFound;

  size_t number = 0;
  if (random)
  {
    number = rng.Get ((int)len);
  }
  else
  {
    number = position_index;
    position_index++;
    if (position_index >= len) position_index = 0;
  }
  return number;
}

void celPcSpawn::SpawnEntityNr (size_t idx)
{
  CountUniqueEntities ();
  if (inhibit_count != 0 && count >= inhibit_count) return;

  // To prevent the entity from being deleted during
  // the call of pcspawn_newentity we keep a temporary reference
  // here.
  csRef<iCelEntity> ref = entity;
  csRef<iCelEntity> newent;

  csString entity_name = spawninfo[idx].name;
  if (do_name_counter)
  {
    entity_name += serialnr;
    atBaseline = false;
  }
  iCelEntityTemplate* entpl = pl->FindEntityTemplate (
      spawninfo[idx].templ);
  if (!entpl)
  {
    printf ("Warning: couldn't find template '%s'!\n",
        spawninfo[idx].templ.GetData ());
    fflush (stdout);
    return;
  }

  size_t number = GetPositionIndex ();
  csVector3 pos (0);
  float yrot = 0.0f;
  csString sector, node;
  if (number != csArrayItemNotFound)
  {
    pos = spawnposition[number].pos;
    yrot = spawnposition[number].yrot;
    sector = spawnposition[number].sector;
    node = spawnposition[number].node;
  }

  csRef<iCelParameterBlock> entpl_params;
  entpl_params.AttachNew (new celVariableParameterBlock ());
  if (!spawner)
    spawner = csQueryRegistryTagInterface<iCelSpawner> (object_reg, "cel.spawner");
  if (spawner)
    newent = spawner->CreateEntity (entpl, entity_name, entpl_params,
      pos, yrot, sector, node);
  else
    newent = CreateNewEntity (entpl, entity_name, entpl_params,
      pos, yrot, sector, node);
  if (!newent) return;
  serialnr++;

  UpdateFreeUniqueEntity (newent);

  // First send a message to our new entity if needed.
  celData ret;
  if (spawninfo[idx].behaviour && !newent->GetBehaviour ())
    Report (object_reg, "Error creating behaviour for entity '%s'!",
    	newent->GetName ());
  if ((!spawninfo[idx].msg_id.IsEmpty ()) &&
  	newent->GetBehaviour ())
  {
    newent->GetBehaviour ()->SendMessage (
    	spawninfo[idx].msg_id, this, ret, spawninfo[idx].params);
    // We use a direct SendMessage here because this is a one-time
    // only event.
    newent->QueryMessageChannel ()->SendMessage (
	spawninfo[idx].msg_id, this, spawninfo[idx].params);
  }

  // Then send a message to our own entity.
  params->GetParameter (0).Set (newent);
  params->GetParameter (1).Set (spawninfo[idx].behaviour);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
    bh->SendMessage ("pcspawn_newentity", this, ret, params);
  if (!dispatcher_new)
  {
    dispatcher_new = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, "cel.entity.new");
    if (!dispatcher_new) return;
  }
  dispatcher_new->SendMessage (params);
  count ++;
}

void celPcSpawn::TickOnce ()
{
  if (!enabled) return;

  if (repeat) Reset ();
  CountUniqueEntities ();
  if (inhibit_count != 0 && count >= inhibit_count) return;

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
      if (idx >= spawninfo.GetSize ()-1) break;
      c -= spawninfo[idx].chance;
    }
  }
  else
  {
    idx = sequence_cur;
    sequence_cur++;
    if (sequence_cur >= spawninfo.GetSize ()) sequence_cur = 0;
  }
  SpawnEntityNr (idx);
}

void celPcSpawn::AddEntityTemplateType (float chance, const char* templ,
	const char* name, const char* msg_id, iCelParameterBlock* params)
{
  size_t idx = spawninfo.Push (SpawnInfo ());
  SpawnInfo& si = spawninfo[idx];
  si.chance = chance;
  si.templ = templ;
  si.name = name;
  si.msg_id = msg_id;
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
  if (do_spawn_unique)
  {
    size_t size;
    if (inhibit_count == 0) size = 1;
    else size = inhibit_count;
    uniqueEntities.SetSize (size, 0);
  }
}

void celPcSpawn::EnableSpawnUnique (bool en)
{
  if (en == do_spawn_unique) return;
  do_spawn_unique = en;
  if (do_spawn_unique)
  {
    size_t size;
    if (inhibit_count == 0) size = 1;
    else size = inhibit_count;
    uniqueEntities.SetSize (size, 0);
  }
}

void celPcSpawn::AddSpawnPosition (const char* node, float yrot,
	const char* sector)
{
  size_t idx = spawnposition.Push (SpawnPosition ());
  SpawnPosition& ni = spawnposition[idx];
  ni.node = node;
  ni.yrot = yrot;
  ni.sector = sector;
}

void celPcSpawn::AddSpawnPosition (const csVector3& pos, float yrot,
	const char* sector)
{
  size_t idx = spawnposition.Push (SpawnPosition ());
  SpawnPosition& pi = spawnposition[idx];
  pi.pos = pos;
  pi.yrot = yrot;
  pi.sector = sector;
}

void celPcSpawn::SaveModifications (iCelCompactDataBufferWriter* buf,
    iStringSet* strings)
{
  if (do_spawn_unique)
  {
    buf->AddUInt32 (uniqueEntities.GetSize ());
    for (size_t i = 0 ; i < uniqueEntities.GetSize () ; i++)
    {
      if (uniqueEntities[i])
        buf->AddUInt32 (uniqueEntities[i]->GetID ());
      else
        buf->AddUInt32 (csArrayItemNotFound);
    }
  }
  buf->AddUInt32 (serialnr);
}

void celPcSpawn::RestoreModifications (iCelCompactDataBufferReader* buf,
    const csHash<csString,csStringID>& strings)
{
  if (do_spawn_unique)
  {
    size_t size = buf->GetUInt32 ();
    uniqueEntities.SetSize (size, 0);
    for (size_t i = 0 ; i < uniqueEntities.GetSize () ; i++)
    {
      uint entid = (uint) buf->GetUInt32 ();
      if (entid == csArrayItemNotFound)
        uniqueEntities[i] = 0;
      else
      {
        csRef<iCelEntity> ent = pl->GetEntity (entid);
        if (!ent)
        {
          // If we can't find the entity we will create it here. We will then
          // assume that later code will complete the loading of this entity.
          printf ("Spawn: Couldn't find entity '%d', creating dummy!\n", entid);
          ent = pl->CreateEntity (entid);
        }
        uniqueEntities[i] = ent;
      }
    }
  }
  serialnr = buf->GetUInt32 ();

  atBaseline = false;
}


//---------------------------------------------------------------------------
