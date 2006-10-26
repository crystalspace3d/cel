/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein

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
#include "ivaria/reporter.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/mesh.h"

#include "plugins/tools/quests/trig_watch.h"

//---------------------------------------------------------------------------

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

csPtr<iQuestTrigger> celWatchTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
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
      "cel.questtrigger.watch",
      "'entity' attribute is missing for the watch trigger!");
    return false;
  }

  target_entity_par = node->GetAttributeValue ("target");
  target_tag_par = node->GetAttributeValue ("target_tag");
  if (target_entity_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.watch",
      "'target' attribute is missing for the watch trigger!");
    return false;
  }

  time_par = node->GetAttributeValue ("checktime");
  radius_par = node->GetAttributeValue ("radius");

  csRef<iDocumentNode> offset_node = node->GetNode ("offset");
  if (offset_node)
  {
    offsetx_par = csStrNew (offset_node->GetAttributeValue ("x"));
    offsety_par = csStrNew (offset_node->GetAttributeValue ("y"));
    offsetz_par = csStrNew (offset_node->GetAttributeValue ("z"));
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
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* target_entity_par, const char* target_tag_par,
	const char* time_par, const char* radius_par,
	const char* offsetx_par, const char* offsety_par,
	const char* offsetz_par)
	: scfImplementationType (this)
{
  celWatchTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = qm->ResolveParameter (params, entity_par);
  tag = qm->ResolveParameter (params, tag_par);
  target_entity = qm->ResolveParameter (params, target_entity_par);
  target_tag = qm->ResolveParameter (params, target_tag_par);
  const char* t = qm->ResolveParameter (params, time_par);
  if (t)
    sscanf (t, "%d", &time);
  else
    time = 1000;
  const char* r = qm->ResolveParameter (params, radius_par);
  if (r)
    sscanf (r, "%f", &radius);
  else
    radius = 10000000.0f;
  sqradius = radius * radius;

  offset.Set (0, 0, 0);
  if (offsetx_par && *offsetx_par)
    offset.x = ToFloat (qm->ResolveParameter (params, offsetx_par));
  if (offsety_par && *offsety_par)
    offset.y = ToFloat (qm->ResolveParameter (params, offsety_par));
  if (offsetz_par && *offsetz_par)
    offset.z = ToFloat (qm->ResolveParameter (params, offsetz_par));

  pl = csQueryRegistry<iCelPlLayer> (type->object_reg);
  cdsys = csQueryRegistry<iCollideSystem> (type->object_reg);
}

celWatchTrigger::~celWatchTrigger ()
{
  DeactivateTrigger ();
}

void celWatchTrigger::RegisterCallback (iQuestTriggerCallback* callback)
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
    iCelEntity* ent = pl->FindEntity (entity);
    if (!ent) return false;
    source_mesh = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcMesh, tag);
    if (!source_mesh) return false;
  }
  if (!target_mesh)
  {
    iCelEntity* ent = pl->FindEntity (target_entity);
    if (!ent) return false;
    target_mesh = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcMesh, target_tag);
    if (!target_mesh) return false;
  }
  return true;
}

void celWatchTrigger::TickOnce ()
{
  if (Check ())
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
  else
  {
    pl->CallbackOnce (static_cast<iCelTimerListener*> (this), time, 
  	CEL_EVENT_PRE);
  }
}

void celWatchTrigger::ActivateTrigger ()
{
  if (!FindEntities ()) return;
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
  if (tbrc.closest_mesh == 0) return true;
  if (tbrc.closest_mesh == target_wrap) return true;
  return false;
}

void celWatchTrigger::DeactivateTrigger ()
{
  pl->RemoveCallbackOnce (static_cast<iCelTimerListener*> (this),
  	CEL_EVENT_PRE);
}

bool celWatchTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celWatchTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------

