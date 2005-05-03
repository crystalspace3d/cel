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
  entity_name_par = 0;
  sector_name_par = 0;
}

celEnterSectorTriggerFactory::~celEnterSectorTriggerFactory ()
{
  delete[] entity_name_par;
  delete[] sector_name_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTrigger> celEnterSectorTriggerFactory::CreateTrigger (
    const celQuestParams& params)
{
  celEnterSectorTrigger* trig = new celEnterSectorTrigger (type,
  	params, entity_name_par, sector_name_par);
  return trig;
}

bool celEnterSectorTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_name_par; entity_name_par = 0;
  delete[] sector_name_par; sector_name_par = 0;
  entity_name_par = csStrNew (node->GetAttributeValue ("entity_name"));

  if (!entity_name_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.entersector",
      "'entity_name' attribute is missing for the entersector trigger!");
    return false;
  }
  sector_name_par = csStrNew (node->GetAttributeValue ("sector_name"));
  if (!sector_name_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.entersector",
      "'sector_name' attribute is missing for the entersector trigger!");
    return false;
  }
  return true;
}

void celEnterSectorTriggerFactory::SetEntityNameParameter (
	const char* entity_name)
{
  if (entity_name_par == entity_name) 
    return;

  delete[] entity_name_par;
  entity_name_par = csStrNew (entity_name);
}

void celEnterSectorTriggerFactory::SetSectorNameParameter (
	const char* sector_name)
{
  if (sector_name_par == sector_name) 
    return;

  delete[] sector_name_par;
  sector_name_par = csStrNew (sector_name);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celEnterSectorTrigger)
  SCF_IMPLEMENTS_INTERFACE (iQuestTrigger)
  SCF_IMPLEMENTS_INTERFACE (iCameraSectorListener)
SCF_IMPLEMENT_IBASE_END

celEnterSectorTrigger::celEnterSectorTrigger (
	celEnterSectorTriggerType* type,
  	const celQuestParams& params,
	const char* entity_name_par, const char* sector_name_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celEnterSectorTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity_name = csStrNew (qm->ResolveParameter (params, entity_name_par));
  sector_name = csStrNew (qm->ResolveParameter (params, sector_name_par));
}

celEnterSectorTrigger::~celEnterSectorTrigger ()
{
  DeactivateTrigger ();
  delete[] entity_name;
  delete[] sector_name;
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
  if (celEnterSectorTrigger::sector == sector)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
}

void celEnterSectorTrigger::FindSectorAndCamera ()
{
  if (camera && sector) return;
  sector = 0;
  camera = 0;
  csRef<iEngine> engine = CS_QUERY_REGISTRY (type->object_reg, iEngine);
  if (!engine) return;
  sector = engine->FindSector (sector_name);
  if (!sector) return;
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  iCelEntity* entity = pl->FindEntity (entity_name);
  if (!entity) return;
  csRef<iPcCamera> pccamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcCamera);
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

