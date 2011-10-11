/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin

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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "cstool/enginetools.h"
#include "cstool/collider.h"
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/mesh.h"

#include "plugins/tools/triggers/trig_watch.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celWatchTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(Watch)

//---------------------------------------------------------------------------

celWatchTriggerFactory::celWatchTriggerFactory (
	celWatchTriggerType* type) : scfImplementationType (this)
{
  celWatchTriggerFactory::type = type;
}

celWatchTriggerFactory::~celWatchTriggerFactory ()
{
}

csPtr<iTrigger> celWatchTriggerFactory::CreateTrigger (
    iQuest* q, iCelParameterBlock* params)
{
  celWatchTrigger* trig = new celWatchTrigger (type, params,
  	entity_par, tag_par, target_entity_par, target_tag_par,
	time_par, radius_par, offsetx_par, offsety_par, offsetz_par);
  return trig;
}

bool celWatchTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");
  if (entity_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.watch",
      "'entity' attribute is missing for the watch trigger!");
    return false;
  }

  target_entity_par = node->GetAttributeValue ("target");
  target_tag_par = node->GetAttributeValue ("target_tag");
  if (target_entity_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.watch",
      "'target' attribute is missing for the watch trigger!");
    return false;
  }

  time_par = node->GetAttributeValue ("checktime");
  radius_par = node->GetAttributeValue ("radius");

  csRef<iDocumentNode> offset_node = node->GetNode ("offset");
  if (offset_node)
  {
    offsetx_par = offset_node->GetAttributeValue ("x");
    offsety_par = offset_node->GetAttributeValue ("y");
    offsetz_par = offset_node->GetAttributeValue ("z");
  }

  return true;
}

void celWatchTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celWatchTriggerFactory::SetTargetEntityParameter (
	const char* entity, const char* tag)
{
  target_entity_par = entity;
  target_tag_par = tag;
}

void celWatchTriggerFactory::SetChecktimeParameter (const char* time)
{
  time_par = time;
}

void celWatchTriggerFactory::SetRadiusParameter (const char* radius)
{
  radius_par = radius;
}

void celWatchTriggerFactory::SetOffsetParameter (const char* offsetx,
    const char* offsety, const char* offsetz)
{
  offsetx_par = offsetx;
  offsety_par = offsety;
  offsetz_par = offsetz;
}

//---------------------------------------------------------------------------

static float ToFloat (const char* s)
{
  if (!s) return 0.0f;
  float f;
  sscanf (s, "%f", &f);
  return f;
}

celWatchTrigger::celWatchTrigger (
	celWatchTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	const char* target_entity_par, const char* target_tag_par,
	const char* time_par, const char* radius_par,
	const char* offsetx_par, const char* offsety_par,
	const char* offsetz_par)
	: scfImplementationType (this)
{
  celWatchTrigger::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->ResolveEntityParameter (params, entity_par, entityID);
  tag = pm->ResolveParameter (params, tag_par);
  target_entity = pm->ResolveEntityParameter (params, target_entity_par, target_entityID);
  target_tag = pm->ResolveParameter (params, target_tag_par);
  const char* t = pm->ResolveParameter (params, time_par);
  if (t)
    sscanf (t, "%d", &time);
  else
    time = 1000;
  const char* r = pm->ResolveParameter (params, radius_par);
  if (r)
    sscanf (r, "%f", &radius);
  else
    radius = 10000000.0f;
  sqradius = radius * radius;

  offset.Set (0, 0, 0);
  if (offsetx_par && *offsetx_par)
    offset.x = ToFloat (pm->ResolveParameter (params, offsetx_par));
  if (offsety_par && *offsety_par)
    offset.y = ToFloat (pm->ResolveParameter (params, offsety_par));
  if (offsetz_par && *offsetz_par)
    offset.z = ToFloat (pm->ResolveParameter (params, offsetz_par));

  cdsys = csQueryRegistry<iCollideSystem> (type->object_reg);
}

celWatchTrigger::~celWatchTrigger ()
{
  DeactivateTrigger ();
}

void celWatchTrigger::RegisterCallback (iTriggerCallback* callback)
{
  celWatchTrigger::callback = callback;
}

void celWatchTrigger::ClearCallback ()
{
  callback = 0;
}

bool celWatchTrigger::FindEntities ()
{
  if (!source_mesh)
  {
    iCelPlLayer* pl = type->pl;
    iCelEntity* ent;
    if (!entity.IsEmpty ())
      ent = pl->FindEntity (entity);
    else
      ent = pl->GetEntity (entityID);
    if (!ent) return false;
    source_mesh = celQueryPropertyClassTagEntity<iPcMesh> (ent, tag);
    if (!source_mesh) return false;
  }
  if (!target_mesh)
  {
    iCelPlLayer* pl = type->pl;
    iCelEntity* ent;
    if (!target_entity.IsEmpty ())
      ent = pl->FindEntity (target_entity);
    else
      ent = pl->GetEntity (target_entityID);
    if (!ent) return false;
    target_mesh = celQueryPropertyClassTagEntity<iPcMesh> (ent, target_tag);
    if (!target_mesh) return false;
  }
  return true;
}

void celWatchTrigger::TickOnce ()
{
  if (!Check ())
  {
    iCelPlLayer* pl = type->pl;
    pl->CallbackOnce (static_cast<iCelTimerListener*> (this), time, 
  	CEL_EVENT_PRE);
  }
}

void celWatchTrigger::ActivateTrigger ()
{
  if (!FindEntities ()) return;
  iCelPlLayer* pl = type->pl;
  pl->CallbackOnce (static_cast<iCelTimerListener*> (this), time, 
  	CEL_EVENT_PRE);
}

bool celWatchTrigger::Check ()
{
  if (!source_mesh || !target_mesh) return false;

  iMeshWrapper* source_wrap = source_mesh->GetMesh ();
  if (!source_wrap) return false;
  iMovable* source_movable = source_wrap->GetMovable ();
  if (source_movable->GetSectors ()->GetCount () == 0) return false;
  iSector* source_sector = source_movable->GetSectors ()->Get (0);
  csVector3 source_pos = source_movable->GetFullPosition ();
  source_pos += offset;

  iMeshWrapper* target_wrap = target_mesh->GetMesh ();
  if (!target_wrap) return false;
  iMovable* target_movable = target_wrap->GetMovable ();
  if (target_movable->GetSectors ()->GetCount () == 0) return false;
  iSector* target_sector = target_movable->GetSectors ()->Get (0);
  csVector3 target_pos = target_movable->GetFullPosition ();
  target_pos += offset;

  csShortestDistanceResult rc = csEngineTools::FindShortestDistance (
  	source_pos, source_sector,
	target_pos, target_sector,
	radius);
  if (rc.sqdistance < 0.0f) return false;
  if (rc.sqdistance > sqradius) return false;
  csTraceBeamResult tbrc = csColliderHelper::TraceBeam (
  	cdsys, source_sector, source_pos, source_pos + rc.direction,
	true);
printf ("check sqdistance=%g sqradius=%g closest_mesh=%s\n", rc.sqdistance, sqradius, tbrc.closest_mesh ? (tbrc.closest_mesh->QueryObject ()->GetName ()) : "<null>"); fflush (stdout);
  // If we hit no mesh then we assume we reached the target (target
  // can be invisible in first player mode for example).
  if (tbrc.closest_mesh == 0 || tbrc.closest_mesh == target_wrap)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iTrigger*)this, 0);
    return true;
  }
  return false;
}

void celWatchTrigger::DeactivateTrigger ()
{
  if (type->pl) type->pl->RemoveCallbackOnce (
      static_cast<iCelTimerListener*> (this), CEL_EVENT_PRE);
}

//---------------------------------------------------------------------------

