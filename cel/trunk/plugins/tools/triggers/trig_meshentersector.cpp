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
#include "propclass/mesh.h"

#include "plugins/tools/triggers/trig_meshentersector.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celMeshEnterSectorTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(MeshEnterSector)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.triggers.meshentersector", msg, arg);
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
}

celMeshEnterSectorTriggerFactory::~celMeshEnterSectorTriggerFactory ()
{
}

csPtr<iTrigger> celMeshEnterSectorTriggerFactory::CreateTrigger (
    iQuest* q, iCelParameterBlock* params)
{
  celMeshEnterSectorTrigger* trig = new celMeshEnterSectorTrigger (type,
  	params, entity_par, tag_par, sector_par);
  return trig;
}

bool celMeshEnterSectorTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");

  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the meshentersector trigger!");
  sector_par = node->GetAttributeValue ("sector");
  if (!sector_par)
    return Report (type->object_reg,
      "'sector' attribute is missing for the meshentersector trigger!");
  return true;
}

void celMeshEnterSectorTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celMeshEnterSectorTriggerFactory::SetSectorParameter (
	const char* sector)
{
  sector_par = sector;
}

//---------------------------------------------------------------------------

celMeshEnterSectorTrigger::celMeshEnterSectorTrigger (
	celMeshEnterSectorTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	const char* sector_par) : scfImplementationType (this)
{
  celMeshEnterSectorTrigger::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->ResolveEntityParameter (params, entity_par, entityID);
  tag = pm->ResolveParameter (params, tag_par);
  sector = pm->ResolveParameter (params, sector_par);
}

celMeshEnterSectorTrigger::~celMeshEnterSectorTrigger ()
{
  DeactivateTrigger ();
}

void celMeshEnterSectorTrigger::RegisterCallback (
	iTriggerCallback* callback)
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
    callback->TriggerFired ((iTrigger*)this, 0);
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
  csRef<iEngine> engine = csQueryRegistry<iEngine> (type->object_reg);
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
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent;
  if (!entity.IsEmpty ())
    ent = pl->FindEntity (entity);
  else
    ent = pl->GetEntity (entityID);
  if (!ent)
  {
    Report (type->object_reg,
    	"Can't find entity '%s' in meshentersector trigger!",
    	(const char*)entity);
    return;
  }
  csRef<iPcMesh> pcmesh = celQueryPropertyClassTagEntity<iPcMesh> (ent, tag);
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
  if (sect == sl->Get (0))
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iTrigger*)this, 0);
    return true;
  }
  return false;
}

void celMeshEnterSectorTrigger::DeactivateTrigger ()
{
  if (!mesh) return;
  mesh->GetMovable ()->RemoveListener ((iMovableListener*)this);
}

//---------------------------------------------------------------------------

