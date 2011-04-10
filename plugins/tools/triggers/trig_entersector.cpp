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
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/camera.h"

#include "plugins/tools/triggers/trig_entersector.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celEnterSectorTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(EnterSector)

//---------------------------------------------------------------------------

celEnterSectorTriggerFactory::celEnterSectorTriggerFactory (
	celEnterSectorTriggerType* type) : scfImplementationType (this)
{
  celEnterSectorTriggerFactory::type = type;
}

celEnterSectorTriggerFactory::~celEnterSectorTriggerFactory ()
{
}

csPtr<iTrigger> celEnterSectorTriggerFactory::CreateTrigger (
    const celParams& params)
{
  celEnterSectorTrigger* trig = new celEnterSectorTrigger (type,
  	params, entity_par, tag_par, sector_par);
  return trig;
}

bool celEnterSectorTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_par");

  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.entersector",
      "'entity' attribute is missing for the entersector trigger!");
    return false;
  }
  sector_par = node->GetAttributeValue ("sector");
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
  entity_par = entity;
  tag_par = tag;
}

void celEnterSectorTriggerFactory::SetSectorParameter (
	const char* sector)
{
  sector_par = sector;
}

//---------------------------------------------------------------------------

celEnterSectorTrigger::celEnterSectorTrigger (
	celEnterSectorTriggerType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* sector_par) : scfImplementationType (this)
{
  celEnterSectorTrigger::type = type;

  csRef<iPluginManager> plugin_mgr = 
    csQueryRegistry<iPluginManager> (type->object_reg);

  csRef<iParameterManager> pm = csLoadPlugin<iParameterManager> 
    (plugin_mgr, "cel.parameters.manager");

  entity = pm->ResolveParameter (params, entity_par);
  tag = pm->ResolveParameter (params, tag_par);
  sector = pm->ResolveParameter (params, sector_par);
}

celEnterSectorTrigger::~celEnterSectorTrigger ()
{
  DeactivateTrigger ();
}

void celEnterSectorTrigger::RegisterCallback (iTriggerCallback* callback)
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
    callback->TriggerFired ((iTrigger*)this, 0);
  }
}

void celEnterSectorTrigger::FindSectorAndCamera ()
{
  if (camera && sect) return;
  sect = 0;
  camera = 0;
  csRef<iEngine> engine = csQueryRegistry<iEngine> (type->object_reg);
  if (!engine) return;
  sect = engine->FindSector (sector);
  if (!sect) return;
  iCelPlLayer* pl = type->pl;
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
  camera->RemoveCameraListener (this);
  camera->AddCameraListener (this);
}

bool celEnterSectorTrigger::Check ()
{
  if (!camera) return false;
  if (camera->GetSector () == sect)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iTrigger*)this, 0);
    return true;
  }
  return false;
}

void celEnterSectorTrigger::DeactivateTrigger ()
{
  if (!camera) return;
  camera->RemoveCameraListener (this);
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

