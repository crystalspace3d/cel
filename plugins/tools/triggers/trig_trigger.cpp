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

#include "celtool/stdparams.h"
#include "plugins/tools/triggers/trig_trigger.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celTriggerTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(Trigger)

//---------------------------------------------------------------------------

celTriggerTriggerFactory::celTriggerTriggerFactory (
	celTriggerTriggerType* type) : scfImplementationType (this)
{
  celTriggerTriggerFactory::type = type;
  do_leave = false;
}

celTriggerTriggerFactory::~celTriggerTriggerFactory ()
{
}

csPtr<iTrigger> celTriggerTriggerFactory::CreateTrigger (
    iQuest* q, iCelParameterBlock* params)
{
  celTriggerTrigger* trig = new celTriggerTrigger (type, params,
  	entity_par, tag_par, do_leave);
  return trig;
}

bool celTriggerTriggerFactory::Save (iDocumentNode* node)
{
  if (!entity_par.IsEmpty ()) node->SetAttribute ("entity", entity_par);
  if (!tag_par.IsEmpty ()) node->SetAttribute ("entity_tag", tag_par);
  if (do_leave) node->SetAttribute ("leave", "true");
  return true;
}

bool celTriggerTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");

  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.triggers.trigger",
      "'entity' attribute is missing for the trigger trigger!");
    return false;
  }

  if (node->GetAttributeValue ("leave"))
    do_leave = true;

  return true;
}

void celTriggerTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

//---------------------------------------------------------------------------

celTriggerTrigger::celTriggerTrigger (
	celTriggerTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par,
	bool do_leave) : scfImplementationType (this)
{
  celTriggerTrigger::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->ResolveEntityParameter (params, entity_par, entityID);
  tag = pm->ResolveParameter (params, tag_par);
  celTriggerTrigger::do_leave = do_leave;
  params_entity.AttachNew (new celOneParameterBlock ());
  params_entity->SetParameterDef (type->pl->FetchStringID ("entity"));
}

celTriggerTrigger::~celTriggerTrigger ()
{
  DeactivateTrigger ();
}

void celTriggerTrigger::RegisterCallback (iTriggerCallback* callback)
{
  celTriggerTrigger::callback = callback;
}

void celTriggerTrigger::ClearCallback ()
{
  callback = 0;
}

void celTriggerTrigger::FireTrigger (const char* name)
{
  DeactivateTrigger ();
  params_entity->GetParameter (0).Set (name);
  callback->TriggerFired ((iTrigger*)this, params_entity);
}

void celTriggerTrigger::EntityEnters (iPcTrigger* trigger, iCelEntity* entity)
{
  if (!do_leave) FireTrigger (entity->GetName ());
}

void celTriggerTrigger::EntityLeaves (iPcTrigger* trigger, iCelEntity* entity)
{
  if (do_leave) FireTrigger (entity->GetName ());
}

void celTriggerTrigger::EnterTrigger (iPcTrigger*, iCelEntity*)
{
}

void celTriggerTrigger::LeaveTrigger (iPcTrigger*, iCelEntity*)
{
}

void celTriggerTrigger::FindEntities ()
{
  if (!pctrigger)
  {
    iCelPlLayer* pl = type->pl;
    iCelEntity* ent;
    if (!entity.IsEmpty ())
      ent = pl->FindEntity (entity);
    else
      ent = pl->GetEntity (entityID);
    if (!ent) return;
    pctrigger = celQueryPropertyClassTagEntity<iPcTrigger> (ent, tag);
  }
}

void celTriggerTrigger::ActivateTrigger ()
{
  FindEntities ();
  if (pctrigger)
    pctrigger->AddTriggerListener ((iPcTriggerListener*)this);
}

bool celTriggerTrigger::Check ()
{
  if (!pctrigger) return false;
  bool rc = pctrigger->Check ();
  if (rc) FireTrigger (pctrigger->GetMonitorEntity ());
  return rc;
}

void celTriggerTrigger::DeactivateTrigger ()
{
  if (pctrigger)
    pctrigger->RemoveTriggerListener ((iPcTriggerListener*)this);
}

//---------------------------------------------------------------------------

