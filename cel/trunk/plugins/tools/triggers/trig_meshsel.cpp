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

#include "celtool/stdparams.h"
#include "plugins/tools/triggers/trig_meshsel.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celMeshSelectTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(MeshSelect)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.triggers.meshselect", msg, arg);
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

celMeshSelectTriggerFactory::celMeshSelectTriggerFactory (
	celMeshSelectTriggerType* type) : scfImplementationType (this)
{
  celMeshSelectTriggerFactory::type = type;
  entity_par = 0;
  tag_par = 0;
}

celMeshSelectTriggerFactory::~celMeshSelectTriggerFactory ()
{
}

csPtr<iTrigger> celMeshSelectTriggerFactory::CreateTrigger (
    iQuest* q, iCelParameterBlock* params)
{
  celMeshSelectTrigger* trig = new celMeshSelectTrigger (type,
  	params, entity_par, tag_par);
  return trig;
}

bool celMeshSelectTriggerFactory::Save (iDocumentNode* node)
{
  if (!entity_par.IsEmpty ()) node->SetAttribute ("entity", entity_par);
  if (!tag_par.IsEmpty ()) node->SetAttribute ("entity_tag", tag_par);
  return true;
}

bool celMeshSelectTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");
  if (entity_par.IsEmpty ())
    return Report (type->object_reg,
      "'entity' attribute is missing for the inventory trigger!");
  return true;
}

void celMeshSelectTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

//---------------------------------------------------------------------------

celMeshSelectTrigger::celMeshSelectTrigger (
	celMeshSelectTriggerType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* tag_par) :
	scfImplementationType (this)
{
  celMeshSelectTrigger::type = type;

  csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->ResolveEntityParameter (params, entity_par, entityID);
  tag = pm->ResolveParameter (params, tag_par);
  params_entity.AttachNew (new celOneParameterBlock ());
  params_entity->SetParameterDef (type->pl->FetchStringID ("entity"));
}

celMeshSelectTrigger::~celMeshSelectTrigger ()
{
  DeactivateTrigger ();
}

void celMeshSelectTrigger::RegisterCallback (iTriggerCallback* callback)
{
  celMeshSelectTrigger::callback = callback;
}

void celMeshSelectTrigger::ClearCallback ()
{
  callback = 0;
}

void celMeshSelectTrigger::FindMeshSelect ()
{
  if (meshselect) return;
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent;
  if (!entity.IsEmpty ())
    ent = pl->FindEntity (entity);
  else
    ent = pl->GetEntity (entityID);
  if (!ent) return;
  meshselect = celQueryPropertyClassTagEntity<iPcMeshSelect> (ent, tag);
}

void celMeshSelectTrigger::ActivateTrigger ()
{
  FindMeshSelect ();
  if (!meshselect)
  {
    Report (type->object_reg, "Can't find pcmeshselect for meshsel trigger!");
    return;
  }
  // First remove to make sure we don't register ourselves multiple
  // times.
  meshselect->RemoveMeshSelectListener ((iPcMeshSelectListener*)this);
  meshselect->AddMeshSelectListener ((iPcMeshSelectListener*)this);
}

bool celMeshSelectTrigger::Check ()
{
  return false;
}

void celMeshSelectTrigger::DeactivateTrigger ()
{
  if (!meshselect) return;
  meshselect->RemoveMeshSelectListener ((iPcMeshSelectListener*)this);
}

void celMeshSelectTrigger::MouseDown (iPcMeshSelect*,
  	int, int, int, iCelEntity* ent)
{
  DeactivateTrigger ();
  if (ent)
    params_entity->GetParameter (0).Set (ent->GetName ());
  else
    params_entity->GetParameter (0).Set ("");
  callback->TriggerFired ((iTrigger*)this, params_entity);
}

void celMeshSelectTrigger::MouseUp (iPcMeshSelect*,
  	int, int, int, iCelEntity*)
{
}

void celMeshSelectTrigger::MouseMove (iPcMeshSelect*,
  	int, int, int, iCelEntity*)
{
}

//---------------------------------------------------------------------------

