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
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "behaviourlayer/behave.h"
#include "physicallayer/pl.h"

#include "plugins/tools/rewards/reward_destroyentity.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celDestroyEntityRewardType)
CEL_IMPLEMENT_REWARDTYPE(DestroyEntity)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.rewards.destroyentity",
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

csPtr<iReward> celDestroyEntityRewardFactory::CreateReward (
    const celParams& params)
{
  iReward* newquest;
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
  	const celParams& params,
	const char* entity_par) : scfImplementationType (this)
{
  celDestroyEntityReward::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity = pm->GetParameter (params, entity_par);
}

celDestroyEntityReward::~celDestroyEntityReward ()
{
}

void celDestroyEntityReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;
  const char* e = entity->Get (params);
  if (!e) return;
  iCelEntity* ent = pl->FindEntity (e);
  if (!ent)
  {
    Report (type->object_reg,
      		"entity %s not found for destroyentity reward!", e);
    return;
  }

  pl->RemoveEntity(ent);
}

//---------------------------------------------------------------------------

celDestroyClassReward::celDestroyClassReward (
	celDestroyEntityRewardType* type,
  	const celParams& params,
	const char* class_par) : scfImplementationType (this)
{
  celDestroyClassReward::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  ent_class = pm->GetParameter (params, class_par);
}

celDestroyClassReward::~celDestroyClassReward ()
{
}

void celDestroyClassReward::Reward (iCelParameterBlock* params)
{
  const char* cl = ent_class->Get (params);
  if (!cl) return;
  csStringID id = type->pl->FetchStringID (cl);
  iCelPlLayer* pl = type->pl;
  csRef<iCelEntityList> entlist = pl->GetClassEntitiesList (id);
  size_t i = entlist->GetCount();
  while (i>0)
  {
    i--;
    pl->RemoveEntity (entlist->Get (i));
  }
}
