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

#include "plugins/tools/quests/reward_destroyentity.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(DestroyEntity)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
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
  entity_par = 0;
}

celDestroyEntityRewardFactory::~celDestroyEntityRewardFactory ()
{
  delete[] entity_par;
}

csPtr<iQuestReward> celDestroyEntityRewardFactory::CreateReward (
    iQuest*, const csHash<csStrKey,csStrKey>& params)
{
  celDestroyEntityReward* newquest = new celDestroyEntityReward (type,
  	params, entity_par);
  return newquest;
}

bool celDestroyEntityRewardFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the destroyentity reward!");
  return true;
}

void celDestroyEntityRewardFactory::SetEntityParameter (
	const char* entity)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
}

//---------------------------------------------------------------------------

celDestroyEntityReward::celDestroyEntityReward (
	celDestroyEntityRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par) : scfImplementationType (this)
{
  celDestroyEntityReward::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
}

celDestroyEntityReward::~celDestroyEntityReward ()
{
  delete[] entity;
}

void celDestroyEntityReward::Reward ()
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent)
  {
    Report (type->object_reg,
      		"entity %s not found for destroyentity reward!",entity);
    return;
  }

  pl->RemoveEntity(ent);
}

//---------------------------------------------------------------------------

