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
#include "iutil/document.h"
#include "ivaria/reporter.h"

#include "behaviourlayer/behave.h"
#include "physicallayer/pl.h"

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/reward_destroyentity.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(DestroyEntity)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.quests.reward.destroyentity",
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

celDestroyEntityRewardFactory::celDestroyEntityRewardFactory (
	celDestroyEntityRewardType* type) : scfImplementationType (this)
{
  celDestroyEntityRewardFactory::type = type;
}

celDestroyEntityRewardFactory::~celDestroyEntityRewardFactory ()
{
}

csPtr<iQuestReward> celDestroyEntityRewardFactory::CreateReward (
    iQuest*, const celQuestParams& params)
{
  iQuestReward* newquest;
  if (entity_par)
    newquest = new celDestroyEntityReward (type,
  	params, entity_par);
  else
    newquest = new celDestroyClassReward (type,
  	params, entity_par);
  return newquest;
}

bool celDestroyEntityRewardFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  class_par = node->GetAttributeValue ("class");
  if ((!entity_par) && (!class_par))
    return Report (type->object_reg,
      "one of 'entity' or 'class' attributes must be used for the destroyentity reward!");
  return true;
}

void celDestroyEntityRewardFactory::SetEntityParameter (
	const char* entity)
{
  entity_par = entity;
}

void celDestroyEntityRewardFactory::SetClassParameter (
	const char* ent_class)
{
  class_par = ent_class;
}

//---------------------------------------------------------------------------

celDestroyEntityReward::celDestroyEntityReward (
	celDestroyEntityRewardType* type,
  	const celQuestParams& params,
	const char* entity_par) : scfImplementationType (this)
{
  celDestroyEntityReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  entity = qm->ResolveParameter (params, entity_par, entity_dynamic);
}

celDestroyEntityReward::~celDestroyEntityReward ()
{
}

void celDestroyEntityReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;
  const char* e = GetDynamicParValue (type->object_reg, params, entity_dynamic, entity);
  if (!e) return;
  iCelEntity* ent = pl->FindEntity (e);
  if (!ent)
  {
    Report (type->object_reg,
      		"entity %s not found for destroyentity reward!",e);
    return;
  }

  pl->RemoveEntity(ent);
}

//---------------------------------------------------------------------------

celDestroyClassReward::celDestroyClassReward (
	celDestroyEntityRewardType* type,
  	const celQuestParams& params,
	const char* class_par) : scfImplementationType (this)
{
  celDestroyClassReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  ent_class = qm->ResolveParameter (params, class_par, ent_class_dynamic);
}

celDestroyClassReward::~celDestroyClassReward ()
{
}

void celDestroyClassReward::Reward (iCelParameterBlock* params)
{
  const char* cl = GetDynamicParValue (type->object_reg, params, ent_class_dynamic, ent_class);
  if (!cl) return;
  csStringID id = type->pl->FetchStringID (cl);
  iCelPlLayer* pl = type->pl;
  csRef<iCelEntityList> entlist = pl->GetClassEntitiesList (id);
  for (int i = entlist->GetCount()-1; i>=0; i--)
  {
    pl->RemoveEntity (entlist->Get (i));
  }
}
