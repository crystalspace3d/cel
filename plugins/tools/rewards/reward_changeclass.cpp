/*
    Crystal Space Entity Layer
    Copyright (C) 2013 by Jorrit Tyberghein

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
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "behaviourlayer/behave.h"
#include "physicallayer/pl.h"

#include "plugins/tools/rewards/reward_changeclass.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celChangeClassRewardType)
CEL_IMPLEMENT_REWARDTYPE(ChangeClass)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.rewards.changeclass",
    	msg, arg);
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

celChangeClassRewardFactory::celChangeClassRewardFactory (
	celChangeClassRewardType* type) : scfImplementationType (this)
{
  celChangeClassRewardFactory::type = type;
  remove = false;
}

celChangeClassRewardFactory::~celChangeClassRewardFactory ()
{
}

csPtr<iReward> celChangeClassRewardFactory::CreateReward (
    iQuest* q, iCelParameterBlock* params)
{
  iReward* newquest;
  if (!entity_par.IsEmpty ())
    newquest = new celChangeClassReward (type, params, entity_par, class_par, remove);
  else
    newquest = new celListChangeClassReward (type, params, entities_par, class_par, remove);
  return newquest;
}

bool celChangeClassRewardFactory::Save (iDocumentNode* node)
{
  if (!entity_par.IsEmpty ()) node->SetAttribute ("entity", entity_par);
  if (!entities_par.IsEmpty ()) node->SetAttribute ("entities", entities_par);
  if (!class_par.IsEmpty ()) node->SetAttribute ("class", class_par);
  if (remove) node->SetAttribute ("remove", "true");
  return true;
}

bool celChangeClassRewardFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  entities_par = node->GetAttributeValue ("entities");
  class_par = node->GetAttributeValue ("class");
  if ((!entity_par) && (!entities_par))
    return Report (type->object_reg,
      "Either 'entity' or 'entities' attributes must be used for the changeclass reward!");
  if (!class_par)
    return Report (type->object_reg,
      "'class' attributes must be used for the changeclass reward!");
  remove = node->GetAttributeValueAsBool ("remove", false);
  return true;
}

void celChangeClassRewardFactory::SetEntitiesParameter (
	const char* entities)
{
  entities_par = entities;
}

void celChangeClassRewardFactory::SetEntityParameter (
	const char* entity)
{
  entity_par = entity;
}

void celChangeClassRewardFactory::SetClassParameter (
	const char* ent_class)
{
  class_par = ent_class;
}

//---------------------------------------------------------------------------

celChangeClassReward::celChangeClassReward (
	celChangeClassRewardType* type,
  	iCelParameterBlock* params,
	const char* entity_par,
	const char* class_par,
	bool remove) : scfImplementationType (this)
{
  celChangeClassReward::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->GetParameter (params, entity_par);
  clazz = pm->GetParameter (params, class_par);
  celChangeClassReward::remove = remove;
}

celChangeClassReward::~celChangeClassReward ()
{
}

void celChangeClassReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;
  iCelEntity *ent = pm->ResolveEntityParameter (pl, params, entity);
  if (!ent) return;
  {
    Report (type->object_reg,
      		"entity %s not found for changeclass reward!",
		entity->Get (params));
    return;
  }
  const char* cl = clazz->Get (params);
  csStringID id = type->pl->FetchStringID (cl);
  if (remove)
    ent->RemoveClass (id);
  else
    ent->AddClass (id);
}

//---------------------------------------------------------------------------

celListChangeClassReward::celListChangeClassReward (
	celChangeClassRewardType* type,
  	iCelParameterBlock* params,
	const char* entities_par,
	const char* class_par,
	bool remove) : scfImplementationType (this)
{
  celListChangeClassReward::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entities = pm->GetParameter (params, entities_par);
  clazz = pm->GetParameter (params, class_par);
  celListChangeClassReward::remove = remove;
}

celListChangeClassReward::~celListChangeClassReward ()
{
}

void celListChangeClassReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;

  csRef<iCelEntityList> list = pm->ResolveEntityListParameter (pl, params, entities);

  const char* cl = clazz->Get (params);
  csStringID id = pl->FetchStringID (cl);
  for (size_t i = 0 ; i < list->GetCount () ; i++)
  {
    iCelEntity* ent = list->Get (i);
    if (remove)
      ent->RemoveClass (id);
    else
      ent->AddClass (id);
  }
}

//---------------------------------------------------------------------------

