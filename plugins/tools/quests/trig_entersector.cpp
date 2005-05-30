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
#include "propclass/camera.h"

#include "plugins/tools/quests/trig_entersector.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(EnterSector)

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEnterSectorTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iEnterSectorQuestTriggerFactory)
SCF_IMPLEMENT_IBASE_END

celEnterSectorTriggerFactory::celEnterSectorTriggerFactory (
	celEnterSectorTriggerType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celEnterSectorTriggerFactory::type = type;
  entity_par = 0;
  tag_par = 0;
  sector_par = 0;
}

celEnterSectorTriggerFactory::~celEnterSectorTriggerFactory ()
{
  delete[] entity_par;
  delete[] tag_par;
  delete[] sector_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTrigger> celEnterSectorTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celEnterSectorTrigger* trig = new celEnterSectorTrigger (type,
  	params, entity_par, tag_par, sector_par);
  return trig;
}

bool celEnterSectorTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] sector_par; sector_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_par"));

  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.entersector",
      "'entity' attribute is missing for the entersector trigger!");
    return false;
  }
  sector_par = csStrNew (node->GetAttributeValue ("sector"));
  if (!sector_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.entersector",
      "'sector' attribute is missing for the entersector trigger!");
    return false;
  }
  return true;
}

void celEnterSectorTriggerFactory::SetEntityParameter (
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

void celEnterSectorTriggerFactory::SetSectorParameter (
	const char* sector)
{
  if (sector_par == sector) 
    return;

  delete[] sector_par;
  sector_par = csStrNew (sector);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEnterSectorTrigger)
  SCF_IMPLEMENTS_INTERFACE (iQuestTrigger)
  SCF_IMPLEMENTS_INTERFACE (iCameraSectorListener)
SCF_IMPLEMENT_IBASE_END

celEnterSectorTrigger::celEnterSectorTrigger (
	celEnterSectorTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* sector_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celEnterSectorTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  sector = csStrNew (qm->ResolveParameter (params, sector_par));
}

celEnterSectorTrigger::~celEnterSectorTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  delete[] tag;
  delete[] sector;
  SCF_DESTRUCT_IBASE ();
}

void celEnterSectorTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celEnterSectorTrigger::callback = callback;
}

void celEnterSectorTrigger::ClearCallback ()
{
  callback = 0;
}

void celEnterSectorTrigger::NewSector (iCamera* camera, iSector* sector)
{
  if (celEnterSectorTrigger::sect == sector)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
}

void celEnterSectorTrigger::FindSectorAndCamera ()
{
  if (camera && sect) return;
  sect = 0;
  camera = 0;
  csRef<iEngine> engine = CS_QUERY_REGISTRY (type->object_reg, iEngine);
  if (!engine) return;
  sect = engine->FindSector (sector);
  if (!sect) return;
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return;
  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcCamera, tag);
  if (!pccamera) return;
  camera = pccamera->GetCamera ();
}

void celEnterSectorTrigger::ActivateTrigger ()
{
  FindSectorAndCamera ();
  // First remove to make sure we don't register ourselves multiple
  // times.
  camera->RemoveCameraSectorListener ((iCameraSectorListener*)this);
  camera->AddCameraSectorListener ((iCameraSectorListener*)this);
}

void celEnterSectorTrigger::DeactivateTrigger ()
{
  if (!camera) return;
  camera->RemoveCameraSectorListener ((iCameraSectorListener*)this);
}

bool celEnterSectorTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celEnterSectorTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------

