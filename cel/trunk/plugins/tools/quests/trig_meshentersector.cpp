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

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celMeshEnterSectorTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iEnterSectorQuestTriggerFactory)
SCF_IMPLEMENT_IBASE_END

celMeshEnterSectorTriggerFactory::celMeshEnterSectorTriggerFactory (
	celMeshEnterSectorTriggerType* type)
{
  SCF_CONSTRUCT_IBASE (0);
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

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTrigger> celMeshEnterSectorTriggerFactory::CreateTrigger (
    const celQuestParams& params)
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
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.meshentersector",
      "'entity' attribute is missing for the meshentersector trigger!");
    return false;
  }
  sector_par = csStrNew (node->GetAttributeValue ("sector"));
  if (!sector_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.meshentersector",
      "'sector' attribute is missing for the meshentersector trigger!");
    return false;
  }
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

SCF_IMPLEMENT_IBASE (celMeshEnterSectorTrigger)
  SCF_IMPLEMENTS_INTERFACE (iQuestTrigger)
  SCF_IMPLEMENTS_INTERFACE (iMovableListener)
SCF_IMPLEMENT_IBASE_END

celMeshEnterSectorTrigger::celMeshEnterSectorTrigger (
	celMeshEnterSectorTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* sector_par)
{
  SCF_CONSTRUCT_IBASE (0);
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
  SCF_DESTRUCT_IBASE ();
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
  if (mesh && sector) return;
  sect = 0;
  mesh = 0;
  csRef<iEngine> engine = CS_QUERY_REGISTRY (type->object_reg, iEngine);
  if (!engine) return;
  sect = engine->FindSector (sector);
  if (!sect) return;	// @@@ Error report!!!
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return;
  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcMesh, tag);
  if (!pcmesh) return;
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

