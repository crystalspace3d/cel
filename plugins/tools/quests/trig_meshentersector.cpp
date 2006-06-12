/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/mesh.h"

#include "plugins/tools/quests/trig_meshentersector.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(MeshEnterSector)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.quests.trigger.meshentersector", msg, arg);
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

celMeshEnterSectorTriggerFactory::celMeshEnterSectorTriggerFactory (
	celMeshEnterSectorTriggerType* type) : scfImplementationType (this)
{
  celMeshEnterSectorTriggerFactory::type = type;
  entity_par = 0;
  tag_par = 0;
  sector_par = 0;
}

celMeshEnterSectorTriggerFactory::~celMeshEnterSectorTriggerFactory ()
{
  delete[] entity_par;
  delete[] tag_par;
  delete[] sector_par;
}

csPtr<iQuestTrigger> celMeshEnterSectorTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celMeshEnterSectorTrigger* trig = new celMeshEnterSectorTrigger (type,
  	params, entity_par, tag_par, sector_par);
  return trig;
}

bool celMeshEnterSectorTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] sector_par; sector_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));

  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the meshentersector trigger!");
  sector_par = csStrNew (node->GetAttributeValue ("sector"));
  if (!sector_par)
    return Report (type->object_reg,
      "'sector' attribute is missing for the meshentersector trigger!");
  return true;
}

void celMeshEnterSectorTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
  if (tag_par != tag)
  {
    delete[] tag_par;
    tag_par = csStrNew (tag);
  }
}

void celMeshEnterSectorTriggerFactory::SetSectorParameter (
	const char* sector)
{
  if (sector_par == sector) 
    return;

  delete[] sector_par;
  sector_par = csStrNew (sector);
}

//---------------------------------------------------------------------------

celMeshEnterSectorTrigger::celMeshEnterSectorTrigger (
	celMeshEnterSectorTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* sector_par) : scfImplementationType (this)
{
  celMeshEnterSectorTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  sector = csStrNew (qm->ResolveParameter (params, sector_par));
}

celMeshEnterSectorTrigger::~celMeshEnterSectorTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  delete[] tag;
  delete[] sector;
}

void celMeshEnterSectorTrigger::RegisterCallback (
	iQuestTriggerCallback* callback)
{
  celMeshEnterSectorTrigger::callback = callback;
}

void celMeshEnterSectorTrigger::ClearCallback ()
{
  callback = 0;
}

void celMeshEnterSectorTrigger::MovableChanged (iMovable* movable)
{
  iSectorList* sl = movable->GetSectors ();
  if (sl->GetCount () < 1) return;
  if (celMeshEnterSectorTrigger::sect == sl->Get (0))
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
}

void celMeshEnterSectorTrigger::MovableDestroyed (iMovable*)
{
}

void celMeshEnterSectorTrigger::FindSectorAndMesh ()
{
  // @@@ This routine should return bool for error?
  if (mesh && sector) return;
  sect = 0;
  mesh = 0;
  csRef<iEngine> engine = CS_QUERY_REGISTRY (type->object_reg, iEngine);
  if (!engine)
  {
    Report (type->object_reg, "Missing engine in meshentersector trigger!");
    return;
  }
  sect = engine->FindSector (sector);
  if (!sect)
  {
    Report (type->object_reg,
    	"Can't find sector '%s' in meshentersector trigger!",
    	(const char*)sector);
    return;
  }
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent)
  {
    Report (type->object_reg,
    	"Can't find entity '%s' in meshentersector trigger!",
    	(const char*)entity);
    return;
  }
  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcMesh, tag);
  if (!pcmesh)
  {
    Report (type->object_reg,
    	"Entity '%s' doesn't have a pcmesh (meshentersector trigger)!",
    	(const char*)entity);
    return;
  }
  mesh = pcmesh->GetMesh ();
}

void celMeshEnterSectorTrigger::ActivateTrigger ()
{
  FindSectorAndMesh ();
  if (!mesh) return;
  // First remove to make sure we don't register ourselves multiple
  // times.
  mesh->GetMovable ()->RemoveListener ((iMovableListener*)this);
  mesh->GetMovable ()->AddListener ((iMovableListener*)this);
}

bool celMeshEnterSectorTrigger::Check ()
{
  if (!mesh) return false;
  iMovable* movable = mesh->GetMovable ();
  iSectorList* sl = movable->GetSectors ();
  if (sl->GetCount () < 1) return false;
  return sect == sl->Get (0);
}

void celMeshEnterSectorTrigger::DeactivateTrigger ()
{
  if (!mesh) return;
  mesh->GetMovable ()->RemoveListener ((iMovableListener*)this);
}

bool celMeshEnterSectorTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celMeshEnterSectorTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------

