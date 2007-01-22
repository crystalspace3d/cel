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
#include "propclass/linmove.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "csutil/debug.h"
#include "csutil/randomgen.h"
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
#include "iengine/movable.h"
#include "iengine/sector.h"

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
  serialnr = 1;
  do_name_counter = true;
  do_spawn_unique = false;

  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  CS_ASSERT (engine != 0);

  if (id_repeat_param == csInvalidStringID)
  {
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
    id_sector_param = pl->FetchStringID ("cel.parameter.sector");
    id_position_param = pl->FetchStringID ("cel.parameter.position");
    id_yrot_param = pl->FetchStringID ("cel.parameter.yrot");
    id_entity = pl->FetchStringID ("cel.parameter.entity");
    id_behaviour = pl->FetchStringID ("cel.parameter.behaviour");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_addentitytype, "cel.action.AddEntityType");
    AddAction (action_addentitytpltype, "cel.action.AddEntityTemplateType");
    AddAction (action_settiming, "cel.action.SetTiming");
    AddAction (action_resettiming, "cel.action.ResetTiming");
    AddAction (action_setenabled, "cel.action.SetEnabled");
    AddAction (action_clearentitylist, "cel.action.ClearEntityList");
    AddAction (action_inhibit, "cel.action.Inhibit");
    AddAction (action_spawn, "cel.action.Spawn");
    AddAction (action_addspawnposition, "cel.action.AddSpawnPosition");
  }

  // For properties.
  propinfo.SetCount (2);
  AddProperty (propid_namecounter, "cel.property.namecounter",
  	CEL_DATA_BOOL, false, "Enable name counter.", &do_name_counter);
  AddProperty (propid_spawnunique, "cel.property.spawnunique",
  	CEL_DATA_BOOL, false, "Enable unique spawning.", &do_spawn_unique);

  params = new celGenericParameterBlock (2);
  params->SetParameterDef (0, id_entity, "entity");
  params->SetParameterDef (1, id_behaviour, "behaviour");
}

celPcSpawn::~celPcSpawn ()
{
  delete params;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcSpawn);
}

bool celPcSpawn::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_addentitytype:
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
  SpawnEntityNr (rng.Get((uint32)spawninfo.Length ()));
}

void celPcSpawn::SpawnEntityNr (size_t idx)
{
  if (inhibit_count != 0 && inhibit_count == count) return;
  // To prevent the entity from being deleted during
  // the call of pcspawn_newentity we keep a temporary reference
  // here.
  csRef<iCelEntity> ref;
  csRandomGen rng (csGetTicks ());
  if (spawninfo[idx].templ)
  {
    csString entity_name = spawninfo[idx].name;
    if (do_spawn_unique && pl->FindEntity (entity_name))
      return;
    if (do_name_counter)
      entity_name += serialnr;
    iCelEntityTemplate* entpl = pl->FindEntityTemplate (
    	spawninfo[idx].templ);
    if (entpl)
    {
      celEntityTemplateParams entpl_params;
      spawninfo[idx].newent = pl->CreateEntity (entpl,
    	  entity_name, entpl_params);
      serialnr ++;
    }
    else
    {
      printf ("Warning: couldn't find template '%s'!\n",
	  spawninfo[idx].templ.GetData ());
      fflush (stdout);
    }
  }
  else
  {
    csString entity_name = spawninfo[idx].name;
    if (do_spawn_unique && pl->FindEntity (entity_name))
      return;
    spawninfo[idx].newent = pl->CreateEntity (entity_name,
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
      Report (object_reg,
      	"Error creating property class '%s' for entity '%s'!",
      	(const char*)pcs[i], spawninfo[idx].newent->GetName ());
    }
  }

  // Set position
  size_t len = spawnposition.Length ();
  if (len > 0)
  {
    size_t number = 0;
    if (random)
    {
      for (size_t i = 0; i != len; i ++)
      {
        number = rng.Get ((int)len);
        if (spawnposition[number].reserved == false)
          break;
        else if (spawnposition[i].reserved == false)
        {
          number = i;
          break;
        }
      }
    }
    else
    {
      for (number = 0; number != len; number ++)
        if (spawnposition[number].reserved == false)
          break;
    }
    if (number < len)
    {
      iSector* sect = engine->FindSector (spawnposition[number].sector);
      if (!sect)
      {
        Report (object_reg,
          "Can't find sector '%s' for action SetPosition!",
          (const char*)(spawnposition[number].sector));
      }
      else
      {
        csRef<iPcLinearMovement> linmove = CEL_QUERY_PROPCLASS_ENT (
          spawninfo[idx].newent, iPcLinearMovement);
        if (linmove)
        {
          spawnposition[number].reserved = true;
          if (!spawnposition[number].node.IsEmpty ())
            linmove->SetFullPosition (spawnposition[number].node,
              spawnposition[number].yrot, sect);
          else
            linmove->SetFullPosition (spawnposition[number].pos,
              spawnposition[number].yrot, sect);
        }
        else
        {
          csVector3 pos;
          if (!spawnposition[number].node.IsEmpty ())
          {
            csRef<iMapNode> mapnode = CS_GET_NAMED_CHILD_OBJECT (
              sect->QueryObject (), iMapNode,
              spawnposition[number].node);
            if (mapnode)
              pos = mapnode->GetPosition ();
            else
              Report (object_reg, "Can't find node '%s' for trigger!",
                (const char*)spawnposition[number].node);
          }
          else
          {
            pos = spawnposition[number].pos;
          }
          csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (
            spawninfo[idx].newent, iPcMesh);
          if (pcmesh)
          {
            iMovable* movable = pcmesh->GetMesh ()->GetMovable ();
            if (movable)
            {
              spawnposition[number].reserved = true;
              movable->SetPosition (sect, pos);
              movable->GetTransform ().SetO2T (
                (csMatrix3) csYRotMatrix3 (spawnposition[number].yrot));
              movable->UpdateMove ();
            }
            else
            {
              Report (object_reg, "Error: entity '%s' is not movable!",
                spawninfo[idx].newent->GetName ());
            }
          }
          else
          {
            Report (object_reg, "Error: entity '%s' is not a mesh!",
              spawninfo[idx].newent->GetName ());
          }
        }
      }
    }
  }

  // First send a message to our new entity if needed.
  celData ret;
  if (spawninfo[idx].behaviour && !spawninfo[idx].newent->GetBehaviour ())
    Report (object_reg, "Error creating behaviour for entity '%s'!",
    	spawninfo[idx].newent->GetName ());
  if ((!spawninfo[idx].msg_id.IsEmpty ()) &&
  	spawninfo[idx].newent->GetBehaviour ())
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
  si.name = name;
  si.bl = bl;
  si.behaviour = behaviour;
  si.msg_id = msg_id;
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
  si.templ = templ;
  if (name != 0)
    si.name = name;
  else
    si.name = templ;
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

//---------------------------------------------------------------------------
