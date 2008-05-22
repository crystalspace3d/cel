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

#include "plugins/tools/quests/trig_meshsel.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(MeshSelect)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.quests.trigger.meshsel", msg, arg);
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
  delete[] entity_par;
  delete[] tag_par;
}

csPtr<iQuestTrigger> celMeshSelectTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celMeshSelectTrigger* trig = new celMeshSelectTrigger (type,
  	params, entity_par, tag_par);
  return trig;
}

bool celMeshSelectTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;

  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));
  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the inventory trigger!");
  return true;
}

void celMeshSelectTriggerFactory::SetEntityParameter (
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

//---------------------------------------------------------------------------

celMeshSelectTrigger::celMeshSelectTrigger (
	celMeshSelectTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par) :
	scfImplementationType (this)
{
  celMeshSelectTrigger::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
}

celMeshSelectTrigger::~celMeshSelectTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  delete[] tag;
}

void celMeshSelectTrigger::RegisterCallback (iQuestTriggerCallback* callback)
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
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return;
  meshselect = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcMeshSelect, tag);
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

bool celMeshSelectTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celMeshSelectTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

void celMeshSelectTrigger::MouseDown (iPcMeshSelect*,
  	int, int, int, iCelEntity*)
{
  DeactivateTrigger ();
  callback->TriggerFired ((iQuestTrigger*)this);
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

